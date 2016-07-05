#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QIcon>
#include <QTextEdit>
#include <QFileDialog>
#include <QTextStream>
#include <QSize>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextDocument>
#include <QPalette>
#include <QDebug>

MainWindow::
MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  setWindowTitle(tr("untitled - 233 Editor"));
  resize(QSize(800, 600));

  QMenuBar* menubar = menuBar();
  QMenu* file = menubar -> addMenu(tr("&File"));
  QMenu* tool = menubar -> addMenu(tr("&Tool"));
  QMenu* help = menubar -> addMenu(tr("&Help"));

  newAction = new QAction(QIcon(":/images/new"), tr("&New"), this);
  newAction -> setShortcut(QKeySequence::New);
  newAction -> setStatusTip(tr("Create a new file"));
  file -> addAction(newAction);
  openAction = new QAction(QIcon(":/images/open"), tr("&Open"), this);
  openAction -> setShortcut(QKeySequence::Open);
  openAction -> setStatusTip(tr("Open a existing file"));
  file -> addAction(openAction);
  saveAction = new QAction(QIcon(":/images/save"), tr("&Save"), this);
  saveAction -> setShortcut(QKeySequence::Save);
  saveAction -> setStatusTip(tr("Save changes"));
  file -> addAction(saveAction);
  saveAsAction = new QAction(QIcon(":/images/saveas"), tr("&Save As"), this);
  saveAsAction -> setShortcut(QKeySequence::SaveAs);
  saveAsAction -> setStatusTip(tr("Save as file..."));
  file -> addAction(saveAsAction);
  quitAction = new QAction(QIcon(":/images/quit"), tr("&Quit"), this);
  quitAction -> setShortcut(QKeySequence::Quit);
  quitAction -> setStatusTip(tr("Quit"));
  file -> addAction(quitAction);
  searchAction = new QAction(QIcon(":/images/search"), tr("&Search and Replace"), this);
  searchAction -> setStatusTip(tr("Search and replace content"));
  tool -> addAction(searchAction);
  aboutMeAction = new QAction(QIcon(":/images/aboutme"), tr("&About Me"), this);
  aboutMeAction -> setStatusTip(tr("About the author"));
  help -> addAction(aboutMeAction);

  connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
  connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
  connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
  connect(searchAction, &QAction::triggered, this, &MainWindow::searchContent);
  connect(quitAction, &QAction::triggered, this, &MainWindow::quitProgram);

  QToolBar* toolBar = addToolBar(tr("Toolbar"));
  toolBar -> addAction(newAction);
  toolBar -> addAction(openAction);
  toolBar -> addAction(saveAction);
  toolBar -> addAction(saveAsAction);
  toolBar -> addAction(searchAction);
  toolBar -> addAction(quitAction);

  textEdit = new QTextEdit(this);
  setCentralWidget(textEdit);

  statusBar();
  connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::changeWindowTitle);
}

MainWindow::
~MainWindow() {
}

void
MainWindow::
saveOrNot() {
  QMessageBox msgBox;
  msgBox.setText(tr("The document has been modified."));
  msgBox.setInformativeText(tr("Do you want to save your changes?"));
  msgBox.setDetailedText(tr("Differences here..."));
  msgBox.setStandardButtons(QMessageBox::Save
                            | QMessageBox::Discard
                            | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  int ret = msgBox.exec();
  switch (ret) {
  case QMessageBox::Save : textEdit -> document() -> setModified(0); saveFile(); break;
  case QMessageBox::Discard : textEdit -> document() -> setModified(0); break;
  case QMessageBox::Cancel : break;
  }
}

void
MainWindow::
newFile() {
  if (textEdit -> document() -> isModified()) {
    saveOrNot();
  }
  filePath = "";
  textEdit -> setText("");
  this -> setWindowTitle(tr("untitled - 233 Editor"));
}

void
MainWindow::
openFile() {
  if (textEdit -> document() -> isModified()) {
    saveOrNot();
  }
  filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        ".",
        tr("All(*.*);;Text Files(*.txt);;C source(*.c);;CPP file(*.cpp);;C/CPP header(*.h);;C file(*.c"));
  if (!filePath.isEmpty()) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QMessageBox::warning(this, tr("Read File"),
                           tr("Cannot open file:\n%1").arg(filePath));
      return;
    }

    QTextStream in(&file);
    textEdit -> setText(in.readAll());

    file.close();
  }
  setWindowTitle(tr("%1 - 233 Editor").arg(filePath));
}

void
MainWindow::
saveFile() {
  if (!filePath.isEmpty()) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QMessageBox::warning(this, tr("Write File"),
                           tr("Cannot open file:\n%1").arg(filePath));
      return;
    }

    QTextStream out(&file);
    out << textEdit -> toPlainText();

    file.close();
  } else {
    filePath = saveAsFile();
  }
  textEdit -> document() -> setModified(0);
  setWindowTitle(tr("%1 - 233 Editor").arg(filePath));
}

QString
MainWindow::
saveAsFile() {
  QString path = QFileDialog::getSaveFileName(
        this,
        tr("Save File"),
        ".",
        tr("All(*.*);;Text Files(*.txt);;C source(*.c);;CPP file(*.cpp);;C/CPP header(*.h);;C file(*.c"));
  if (!path.isEmpty()) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QMessageBox::warning(this, tr("Write File"),
                           tr("Cannot open file:\n%1").arg(filePath));
      return path;
    }

    QTextStream out(&file);
    out << textEdit -> toPlainText();

    file.close();
  }
  textEdit -> document() -> setModified(0);
  setWindowTitle(tr("%1 - 233 Editor").arg(path));
  return path;
}

void
MainWindow::
searchContent() {
  QDialog* findDialog = new QDialog(this);
  findDialog -> setWindowTitle("Search and Replace");
  findTextLineEdit = new QLineEdit(findDialog);
  QPushButton* findNextButton = new QPushButton(tr("find next"), findDialog);
  QPushButton* findPreviousButton = new QPushButton(tr("find previous"), findDialog);
  QVBoxLayout* layout = new QVBoxLayout(findDialog);
  layout -> addWidget(findTextLineEdit);
  layout -> addWidget(findNextButton);
  layout -> addWidget(findPreviousButton);

  findDialog -> setLayout(layout);
  findDialog -> show();
  connect(findNextButton, &QPushButton::clicked, this, &MainWindow::showNextFindText);
  connect(findPreviousButton, &QPushButton::clicked, this, &MainWindow::showPreviousFindText);
}

void
MainWindow::
quitProgram() {
  if (textEdit -> document() -> isModified()) {
    saveOrNot();
  }
  this -> hide();
}

// 向前查找
void
MainWindow::
showNextFindText() {
  QString findText = findTextLineEdit -> text();
  if (textEdit -> find(findText)) {
      QPalette palette = textEdit -> palette();
      palette.setColor(QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
      textEdit -> setPalette(palette);
  } else {
      QMessageBox::warning(this, tr("Search and Replace"),
                           tr("Content not found:\n%1").arg(findText));
  }
}

// 向后查找
void
MainWindow::
showPreviousFindText() {
  QString findText = findTextLineEdit -> text();
  if (textEdit -> find(findText, QTextDocument::FindBackward)) {
      QPalette palette = textEdit -> palette();
      palette.setColor(QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
      textEdit -> setPalette(palette);
  } else {
      QMessageBox::warning(this, tr("Search and Replace"),
                           tr("Content not found:\n%1").arg(findText));
  }
}

// 当文档被modified时, 在窗口标题前添加*
void
MainWindow::
changeWindowTitle() {
    QString str = this -> windowTitle();
    if (str[0] != '*') {
      str = "*" + str;
    }
    this -> setWindowTitle(str);
}
