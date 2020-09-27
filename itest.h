#ifndef ITEST_H
#define ITEST_H

#include <QWidget>
#include <QMainWindow>
#include "buttonstest.h"

const QString BTNS_TEST_WD = "~/sketchbook/test_btns_v2/";

class ITest: public QWidget
{
    Q_OBJECT
public:
    ITest(QMainWindow *parent): QWidget(parent), _parent(parent) {}
    virtual QString getName() const = 0;
    void showTesting()
    {
        QWidget* _testWidget = nullptr;
        createTesting(_testWidget);

        if (_testWidget) {
            _testWidget->setAttribute(Qt::WA_DeleteOnClose);
            _parent->setCentralWidget(_testWidget);
        } else {
            // todo: add faults handling
        }
    }
    virtual ~ITest(){}
protected:
    virtual int createTesting(QWidget* &testWidget) const = 0;
    QMainWindow *_parent;
};

class IButtonTest : public ITest
{
    Q_OBJECT
public:
    IButtonTest(QMainWindow *parent): ITest(parent) {}
    QString getName() const override { return QString("Button testing"); }
    ~IButtonTest() override {}
protected:
    int createTesting(QWidget* &testWidget) const override
    {
        testWidget = new ButtonsTest(BTNS_TEST_WD, _parent);
        return 0; // todo: add faults processing
    }
};

#endif // ITEST_H
