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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  //ui->setupUi(this);

  this -> setWindowTitle(tr("233 Editor"));
  this -> resize(QSize(800, 600));

  QMenuBar* menubar = menuBar();
  QMenu* file = menubar -> addMenu(tr("&File"));
  QMenu* tool = menubar -> addMenu(tr("&Tool"));
  QMenu* help = menubar -> addMenu(tr("&Help"));

  newAction = new QAction(QIcon(":/images/file-new"), tr("&New"), this);
  newAction -> setShortcut(QKeySequence::New);
  newAction -> setStatusTip(tr("Create a new file"));
  file -> addAction(newAction);
  openAction = new QAction(QIcon(":/images/file-open"), tr("&Open"), this);
  openAction -> setShortcut(QKeySequence::Open);
  openAction -> setStatusTip(tr("Open a existing file"));
  file -> addAction(openAction);
  saveAction = new QAction(QIcon(":/images/file-save"), tr("&Save"), this);
  saveAction -> setShortcut(QKeySequence::Save);
  saveAction -> setStatusTip(tr("Save changes"));
  file -> addAction(saveAction);
  saveAsAction = new QAction(QIcon(":/images/file-saveas"), tr("&Save As"), this);
  saveAsAction -> setShortcut(QKeySequence::SaveAs);
  saveAsAction -> setStatusTip(tr("Save as file..."));
  file -> addAction(saveAsAction);
  quitAction = new QAction(QIcon(":/images/file-quit"), tr("&Quit"), this);
  quitAction -> setShortcut(QKeySequence::Quit);
  quitAction -> setStatusTip(tr("Quit"));
  file -> addAction(quitAction);
  searchAction = new QAction(QIcon(":/images/file-search"), tr("&Search"), this);
  searchAction -> setStatusTip(tr("Search content"));
  tool -> addAction(searchAction);
  aboutMeAction = new QAction(QIcon(":/images/lenka"), tr("&About Me"), this);
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
  toolBar -> addAction(searchAction);
  toolBar -> addAction(quitAction);

  textEdit = new QTextEdit(this);
  setCentralWidget(textEdit);

  statusBar();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::newFile() {
  if (!filePath.isEmpty()) {
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
      case QMessageBox::Save : saveFile(); filePath = ""; textEdit -> setText(""); break;
      case QMessageBox::Discard : filePath = ""; textEdit -> setText(""); break;
      case QMessageBox::Cancel : break;
      }
  }
}

void
MainWindow::openFile() {
  if (!filePath.isEmpty()) {
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
      case QMessageBox::Save : saveFile(); filePath = ""; textEdit -> setText(""); break;
      case QMessageBox::Discard : filePath = ""; textEdit -> setText(""); break;
      case QMessageBox::Cancel : break;
      }
  }
  filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        ".",
        tr("All(*.*);;Text Files(*.txt);;C source(*.c);;CPP file(*.cpp);;C/CPP header(*.h);;C file(*.c")
        );
  if (!filePath.isEmpty()) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    QTextStream in(&file);
    textEdit -> setText(in.readAll());

    file.close();
  }
}

void
MainWindow::saveFile() {
  if (!filePath.isEmpty()) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QTextStream out(&file);
    out << textEdit -> toPlainText();

    file.close();
  } else {
    filePath = saveAsFile();
  }
}

QString
MainWindow::saveAsFile() {
  QString path = QFileDialog::getSaveFileName(
        this,
        tr("Save File"),
        ".",
        tr("All(*.*);;Text Files(*.txt);;C source(*.c);;CPP file(*.cpp);;C/CPP header(*.h);;C file(*.c")
        );
  if (!path.isEmpty()) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return path;

    QTextStream out(&file);
    out << textEdit -> toPlainText();

    file.close();
  }
  return path;
}

void
MainWindow::searchContent() {

}

void
MainWindow::quitProgram() {

}
