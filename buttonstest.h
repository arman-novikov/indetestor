#ifndef BUTTONSTEST_H
#define BUTTONSTEST_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QTimer>

class ButtonsTest : public QWidget
{
    Q_OBJECT
public:
    ButtonsTest(const QString &workDir, QWidget *parent = nullptr);
    ~ButtonsTest();
private:
    int makeBuild();
    int openTTY();
    int set_interface_attribs(const int fd, const int speed);
    QPushButton *clearBtn;
    QPushButton *startBtn;
    QPushButton *stopBtn;
    QPlainTextEdit *textEdit;
    QHBoxLayout *buttonsLay;
    QVBoxLayout *mainLay;
    QTimer *_readTimer;
    QString _workDir;
    QString _portName;
    int _fd;
    bool _isTTYuploaded;
    bool _isTTYopened;
private slots:
    void startBtnPushed();
    void stopBtnPushed();
    void clearBtnPushed();
     void readTimerOut();
};

#endif // BUTTONSTEST_H
