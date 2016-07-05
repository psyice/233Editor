#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTextEdit>
#include <QLineEdit>

namespace Ui {
class MainWindow;
}

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
  QLineEdit* findTextLineEdit;

  QString filePath;

  void saveOrNot();

  void newFile();
  void openFile();
  void saveFile();
  QString saveAsFile();
  void searchContent();
  void quitProgram();

  Ui::MainWindow *ui;

private slots:
  void showNextFindText();
  void showPreviousFindText();
  void changeWindowTitle();
};

#endif // MAINWINDOW_H
