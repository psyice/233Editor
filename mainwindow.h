#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTextEdit>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:

  bool firstSearch;
  bool findLast;

  QAction* newAction;
  QAction* openAction;
  QAction* saveAction;
  QAction* saveAsAction;
  QAction* quitAction;
  QAction* searchAction;
  QAction* aboutMeAction;
  QAction* undoAction;
  QAction* redoAction;

  QTextEdit* textEdit;
  QLineEdit* findTextLineEdit;
  QLineEdit* replaceTextLineEdit;

  QString filePath;

  void saveOrNot();

  void newFile();
  void openFile();
  void saveFile();
  QString saveAsFile();
  void searchContent();
  void quitProgram();

private slots:
  void showNextFindText();
  void showPreviousFindText();
  void replaceContent();
  void replaceAllContent();
  void changeWindowTitle();
};

#endif // MAINWINDOW_H
