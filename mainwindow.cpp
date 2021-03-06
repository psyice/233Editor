#include "mainwindow.h"
#include <QCursor>
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QSize>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextEdit>
#include <QTextStream>
#include <QToolBar>
#include <QVBoxLayout>
#include <QPixmap>
 MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(tr("untitled - 233 Editor"));
    setWindowIcon(QIcon(":/images/editor"));
    resize(QSize(800, 600));

    QMenuBar* menubar = menuBar();
    QMenu* file				= menubar->addMenu(tr("&File"));
    QMenu* edit				= menubar->addMenu(tr("&Edit"));
    QMenu* help				= menubar->addMenu(tr("&Help"));

    newAction = new QAction(QIcon(":/images/new"), tr("&New"), this);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new file"));
    file->addAction(newAction);
    openAction = new QAction(QIcon(":/images/open"), tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a existing file"));
    file->addAction(openAction);
    saveAction = new QAction(QIcon(":/images/save"), tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save changes"));
    file->addAction(saveAction);
    saveAsAction = new QAction(QIcon(":/images/saveas"), tr("&Save As"), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save as file..."));
    file->addAction(saveAsAction);
    quitAction = new QAction(QIcon(":/images/quit"), tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
    quitAction->setStatusTip(tr("Quit"));
    file->addAction(quitAction);
    undoAction = new QAction(QIcon(":/images/undo"), tr("&Undo"), this);
    quitAction->setShortcut(QKeySequence::Undo);
    undoAction->setStatusTip(tr("Undo Changes"));
    edit->addAction(undoAction);
    redoAction = new QAction(QIcon(":/images/redo"), tr("&Redo"), this);
    quitAction->setShortcut(QKeySequence::Redo);
    redoAction->setStatusTip(tr("Redo Changes"));
    edit->addAction(redoAction);
    searchAction =
            new QAction(QIcon(":/images/search"), tr("&Search and Replace"), this);
    searchAction->setStatusTip(tr("Search and replace content"));
    edit->addAction(searchAction);
    aboutMeAction = new QAction(QIcon(":/images/aboutme"), tr("&About Me"), this);
    aboutMeAction->setStatusTip(tr("About the author"));
    help->addAction(aboutMeAction);

    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    connect(searchAction, &QAction::triggered, this, &MainWindow::searchContent);
    connect(quitAction, &QAction::triggered, this, &MainWindow::quitProgram);
    connect(aboutMeAction, &QAction::triggered, this, &MainWindow::aboutMe);

    QToolBar* toolBar = addToolBar(tr("Toolbar"));
    toolBar->addAction(newAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(saveAsAction);
    toolBar->addSeparator();
    toolBar->addAction(undoAction);
    toolBar->addAction(redoAction);
    toolBar->addAction(searchAction);
    toolBar->addSeparator();
    toolBar->addAction(quitAction);

    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);

    statusBar();
    connect(textEdit, &QTextEdit::textChanged, this,
                    &MainWindow::changeWindowTitle);
    connect(undoAction, &QAction::triggered, textEdit, &QTextEdit::undo);
    connect(redoAction, &QAction::triggered, textEdit, &QTextEdit::redo);
}

MainWindow::~MainWindow() {}

void
MainWindow::closeEvent(QCloseEvent* event) {
    quitProgram();
}

//确认是否保存
void
MainWindow::saveOrNot() {
    QMessageBox msgBox;
    msgBox.setText(tr("The document has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                                        QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Save:
            textEdit->document()->setModified(0);
            saveFile();
            break;
        case QMessageBox::Discard:
            textEdit->document()->setModified(0);
            break;
        case QMessageBox::Cancel:
            break;
    }
}

void
MainWindow::newFile() {
    if (textEdit->document()->isModified()) {
        saveOrNot();
    }
    filePath = "";
    textEdit->setText("");
    this->setWindowTitle(tr("untitled - 233 Editor"));
}

void
MainWindow::openFile() {
    if (textEdit->document()->isModified()) {
        saveOrNot();
    }
    QString path = QFileDialog::getOpenFileName(
            this, tr("Open File"), ".",
            tr("All(*.*);;Text Files(*.txt);;C source(*.c);;CPP file(*.cpp);;C/CPP "
                 "header(*.h);;C file(*.c"));
    if (!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"),
                                                     tr("Cannot open file:\n%1").arg(filePath));
            return;
        }

        QTextStream in(&file);
        textEdit->setText(in.readAll());

        file.close();
        setWindowTitle(tr("%1 - 233 Editor").arg(path));
        textEdit->document()->setModified(0);
        filePath = path;
    }
}

void
MainWindow::saveFile() {
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"),
                                                     tr("Cannot open file:\n%1").arg(filePath));
            return;
        }

        QTextStream out(&file);
        out << textEdit->toPlainText();

        file.close();
        textEdit->document()->setModified(0);
        setWindowTitle(tr("%1 - 233 Editor").arg(filePath));
    } else {
        filePath = saveAsFile();
    }
}

QString
MainWindow::saveAsFile() {
    QString path = QFileDialog::getSaveFileName(
            this, tr("Save File"), ".",
            tr("All(*.*);;Text Files(*.txt);;C source(*.c);;CPP file(*.cpp);;C/CPP "
                 "header(*.h);;C file(*.c"));
    if (!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"),
                                                     tr("Cannot open file:\n%1").arg(filePath));
            return path;
        }

        QTextStream out(&file);
        out << textEdit->toPlainText();

        file.close();
        textEdit->document()->setModified(0);
        setWindowTitle(tr("%1 - 233 Editor").arg(path));
    }
    return path;
}

void
MainWindow::searchContent() {
    firstSearch					= 0;
    QDialog* findDialog = new QDialog(this);
    findDialog->setWindowTitle("Search and Replace");
    QLabel* label1 = new QLabel(findDialog);
    label1->setText(tr("Search for"));
    QLabel* label2 = new QLabel(findDialog);
    label2->setText(tr("Replace for"));
    findTextLineEdit								= new QLineEdit(findDialog);
    replaceTextLineEdit							= new QLineEdit(findDialog);
    QPushButton* findNextButton			= new QPushButton(tr("Next"), findDialog);
    QPushButton* findPreviousButton = new QPushButton(tr("Previous"), findDialog);
    QPushButton* replaceButton			= new QPushButton(tr("Replace"), findDialog);
    QPushButton* replaceAllButton =
            new QPushButton(tr("Replace All"), findDialog);
    QVBoxLayout* leftLayout	= new QVBoxLayout();
    QVBoxLayout* rightLayout = new QVBoxLayout();
    leftLayout->addSpacing(5);
    leftLayout->addWidget(label1);
    leftLayout->addWidget(findTextLineEdit);
    leftLayout->addStretch();
    leftLayout->addWidget(label2);
    leftLayout->addWidget(replaceTextLineEdit);
    leftLayout->addSpacing(5);
    rightLayout->addSpacing(5);
    rightLayout->addWidget(findNextButton);
    rightLayout->addWidget(findPreviousButton);
    rightLayout->addStretch();
    rightLayout->addWidget(replaceButton);
    rightLayout->addWidget(replaceAllButton);
    rightLayout->addSpacing(5);

    QHBoxLayout* mainLayout = new QHBoxLayout(findDialog);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    findDialog->setLayout(mainLayout);
    findDialog->show();
    connect(findNextButton, &QPushButton::clicked, this,
                    &MainWindow::showNextFindText);
    connect(findPreviousButton, &QPushButton::clicked, this,
                    &MainWindow::showPreviousFindText);
    connect(replaceButton, &QPushButton::clicked, this,
                    &MainWindow::replaceContent);
    connect(replaceAllButton, &QPushButton::clicked, this,
                    &MainWindow::replaceAllContent);
}

void
MainWindow::quitProgram() {
    if (textEdit->document()->isModified()) {
        saveOrNot();
    }
    this->hide();
}

// 向前查找
void
MainWindow::showNextFindText() {
    QString findText = findTextLineEdit->text();
    if (findText.trimmed().isEmpty()) {
        QMessageBox::information(this, tr("Empty search field"),
                                                         tr("The search field is empty!"));
    } else {
        if (textEdit->find(findText)) {
            QPalette palette = textEdit->palette();
            palette.setColor(QPalette::Highlight,
                                             palette.color(QPalette::Active, QPalette::Highlight));
            textEdit->setPalette(palette);
            findLast = false;
        } else {
            findLast = true;
            QMessageBox::warning(this, tr("Search and Replace"),
                                                     tr("Content not found:\n%1").arg(findText));
        }
    }
}

// 向后查找
void
MainWindow::showPreviousFindText() {
    QString findText = findTextLineEdit->text();
    if (findText.trimmed().isEmpty()) {
        QMessageBox::information(this, tr("Empty search field"),
                                                         tr("The search field is empty!"));
    } else {
        QString findText = findTextLineEdit->text();
        if (textEdit->find(findText, QTextDocument::FindBackward)) {
            QPalette palette = textEdit->palette();
            palette.setColor(QPalette::Highlight,
                                             palette.color(QPalette::Active, QPalette::Highlight));
            textEdit->setPalette(palette);
            findLast = false;
        } else {
            findLast = true;
            QMessageBox::warning(this, tr("Search and Replace"),
                                                     tr("Content not found:\n%1").arg(findText));
        }
    }
}

void
MainWindow::replaceContent() {
    QString findText		= findTextLineEdit->text();
    QString replaceText = replaceTextLineEdit->text();
    //若替换文本框为空(或全为空格), 则报错
    if (replaceText.trimmed().isEmpty()) {
        QMessageBox::information(this, tr("Empty replace field"),
                                                         tr("The replace field is empty!"));
    } else {
        //若搜索文本框为空(或全为空格), 则报错
        if (findText.trimmed().isEmpty()) {
            QMessageBox::information(this, tr("Empty search field"),
                                                             tr("The search field is empty!"));
        } else {
            if (!firstSearch) {
                this->showNextFindText();
                firstSearch = true;
            } else {
                if (!findLast) {
                    textEdit->textCursor().deleteChar();
                    textEdit->textCursor().insertText(replaceText);
                }
                this->showNextFindText();
            }
        }
    }
}

void
MainWindow::replaceAllContent() {
    QString findText		= findTextLineEdit->text();
    QString replaceText = replaceTextLineEdit->text();
    //若替换文本框为空(或全为空格), 则报错
    if (replaceText.trimmed().isEmpty()) {
        QMessageBox::information(this, tr("Empty replace field"),
                                                         tr("The replace field is empty!"));
    } else {
        //若搜索文本框为空(或全为空格), 则报错
        if (findText.trimmed().isEmpty()) {
            QMessageBox::information(this, tr("Empty search field"),
                                                             tr("The search field is empty!"));
        } else {
            QTextDocument* document = textEdit->document();
            bool found							= false;
            QTextCursor highlightCursor(document);
            QTextCursor cursor(document);

            cursor.beginEditBlock();

            QTextCharFormat colorFormat(highlightCursor.charFormat());
            colorFormat.setAnchor(true);
            while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
                highlightCursor = document->find(findText, highlightCursor);
                if (!highlightCursor.isNull()) {
                    found = true;
                    highlightCursor.deleteChar();
                    highlightCursor.insertText(replaceText);
                }
            }
            cursor.endEditBlock();

            if (found == false) {
                QMessageBox::warning(this, tr("Search and Replace"),
                                                         tr("Content not found:\n%1").arg(findText));
            }
        }
    }
}

// 当文档被modified时, 在窗口标题前添加*
void
MainWindow::changeWindowTitle() {
    QString str = this->windowTitle();
    if (str[0] != '*') {
        str = "*" + str;
    }
    this->setWindowTitle(str);
}

void
MainWindow::aboutMe() {
  QDialog* about = new QDialog(this);
  about -> setMaximumWidth(100);
  about -> setMaximumHeight(50);
  QPixmap pic(":/images/aboutme");
  QLabel* la = new QLabel(about);
  QLabel* author = new QLabel("Author : Psyice-Cust", about);
  QLabel* email = new QLabel("Email : synthetic_ice@yahoo.com", about);
  la -> setPixmap(pic);
  QHBoxLayout* hLayout = new QHBoxLayout(about);
  QVBoxLayout* vLayout = new QVBoxLayout(about);
  hLayout -> addWidget(la);
  vLayout -> addWidget(author);
  vLayout -> addWidget(email);
  hLayout -> addLayout(vLayout);
  about -> setLayout(hLayout);
  about -> show();
}
