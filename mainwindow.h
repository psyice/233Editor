#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QCloseEvent>
#include <QLineEdit>
#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow {
	Q_OBJECT

 public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

 protected:
	void closeEvent(QCloseEvent* event);

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
    void aboutMe();

 private slots:
	void showNextFindText();
	void showPreviousFindText();
	void replaceContent();
	void replaceAllContent();
	void changeWindowTitle();
};

#endif	// MAINWINDOW_H
