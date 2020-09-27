#include "mainwindow.h"
#include <QMessageBox>
#include <QMenuBar>

constexpr int WIDTH = 400;
constexpr int HEIGHT = 270;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _tests = {new IButtonTest(this), };
    setWindowTitle(QString("Indetestor"));
    setFixedSize(QSize(WIDTH, HEIGHT));

    createMenuBar();        ///> see createMenuBar() implementation
    totalConnect();         ///> see totalConnect() implementation
}

void MainWindow::createMenuBar()
{
    about = new QMenu(QString("About"),this);                   ///> about authors action button in about menu
        authors = new QAction(QString("&Authors"), this);
        authors->setShortcut(Qt::Key_F1);
        about->addAction(authors);

    selectTest = new QMenu(QString("&Select test"), this);
        _testsActions.reserve(_tests.size());
        for (auto iter : _tests) {
            const QString text("start " + QString(iter->getName()));
            QAction *newTestAction = new QAction(text, this);
            _testsActions.append(newTestAction);
            selectTest->addAction(newTestAction);
        }
    menuBar()->addMenu(selectTest);
    menuBar()->addMenu(about);
}

void MainWindow::totalConnect()
{
    for (int i = 0; i < _tests.size(); ++i) {
        connect(_testsActions[i], &QAction::triggered, _tests[i],
                [this, i](){_tests[i]->showTesting();});
    }
    connect(authors, SIGNAL(triggered(bool)), this, SLOT(authorsSlot()));
}

void MainWindow::authorsSlot()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->setWindowTitle(QString("Authors"));
    msg->setText(QString("designed and developed by armnov"));
    msg->show(); // or exec()?
}

MainWindow::~MainWindow()
{

}
