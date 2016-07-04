#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  QAction* newAction;
  QAction* openAction;
  QAction* saveAction;
  QAction* saveAsAction;
  QAction* quitAction;
  QAction* searchAction;
  QAction* aboutMeAction;

  QTextEdit* textEdit;

  QString filePath;

  void saveOrNot();

  void newFile();
  void openFile();
  void saveFile();
  QString saveAsFile();
  void searchContent();
  void quitProgram();
};

#endif // MAINWINDOW_H
