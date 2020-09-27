#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include "itest.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenuBar();
    void totalConnect();
    QMenu *selectTest;
        QVector<QAction*> _testsActions;
        QVector<ITest*> _tests;
    QMenu *about;
        QAction *authors;
private slots:
    void authorsSlot();
};

#endif // MAINWINDOW_H
