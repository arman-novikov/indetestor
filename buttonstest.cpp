#include "buttonstest.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <QDebug>

static void closeTTY(const int fd);

const QString DIRECTORY = ("~/sketchbook/");
const QString BTNS_TEST_DIR = "/btns_test_v2/";
const int TIMER_DELAY = 5;

ButtonsTest::ButtonsTest(const QString &workDir, QWidget *parent):
    QWidget (parent),
    _workDir(workDir), _portName("/dev/ttyACM0"), _fd(0),
    _isTTYuploaded(false), _isTTYopened(false)
{
    _readTimer = new QTimer(this);

    clearBtn = new QPushButton("Clear");
    startBtn = new QPushButton("Start");
    stopBtn  = new QPushButton("Stop");
    textEdit = new QPlainTextEdit();

    buttonsLay = new QHBoxLayout();
        buttonsLay->addWidget(startBtn);
        buttonsLay->addWidget(stopBtn);
        buttonsLay->addWidget(clearBtn);

    mainLay = new QVBoxLayout(this);
        mainLay->addLayout(buttonsLay);
        mainLay->addWidget(textEdit);

    connect(startBtn, SIGNAL(clicked(bool)), this, SLOT(startBtnPushed()));
    connect(stopBtn, SIGNAL(clicked(bool)), this, SLOT(stopBtnPushed()));
    connect(clearBtn, SIGNAL(clicked(bool)), this, SLOT(clearBtnPushed()));
    connect(_readTimer, SIGNAL(timeout()), this, SLOT(readTimerOut()));
}

void ButtonsTest::startBtnPushed()
{
    startBtn->setEnabled(false);

    if (!_isTTYuploaded) {
        if (makeBuild()) {
            qDebug() << "make build bleat\'";
        } else {
            qDebug() << "yeee rock";
            _isTTYuploaded = true;
        }
    }

    if (!_isTTYopened) {
        if (openTTY()) {
            qDebug() << "openTTY bleat\'";
        } else {
            qDebug() << "yeee rock";
            _isTTYopened = true;
        }
    }

    _readTimer->setInterval(TIMER_DELAY);
    _readTimer->start();
}

void ButtonsTest::stopBtnPushed()
{
    _readTimer->stop();
    closeTTY(_fd);
    _isTTYopened = false;
    startBtn->setEnabled(true);
}

void ButtonsTest::clearBtnPushed()
{
    textEdit->setPlainText(QString(""));
}

int ButtonsTest::makeBuild()
{
    const QString cmd = "make --directory " + _workDir + "Makefile";
    const QByteArray ba = cmd.toLocal8Bit();

    return system(ba.data());
}

void ButtonsTest::readTimerOut()
{
    const ssize_t readBufSize = 1024;
    char rdbuf[readBufSize] = {0};
    ssize_t rdlen = read(_fd, rdbuf, readBufSize - 1);

    rdbuf[rdlen] = '\0';
    QString res = rdbuf;
    if (res.size())
        textEdit->appendPlainText(res);
}

int ButtonsTest::openTTY()
{
    int res = 0;
    _fd = open(_portName.toLocal8Bit().data(), O_RDWR | O_NOCTTY | O_SYNC);
    if (_fd < 0) {
        qDebug() << "can't open" + _portName;
        return errno;
    }

    res = set_interface_attribs(_fd, B115200);
    if (res) {
        qDebug() << "can't set_interface_attribs" + _portName;
        return errno;
    }

    return res;
}

int ButtonsTest::set_interface_attribs(const int fd, const int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        qDebug() << "Error from tcgetattr " << errno;
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 0; // 1 !!!
    tty.c_cc[VTIME] = 0; // 1 !!!

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        qDebug() <<"Error from tcsetattr: " << errno;
        return -1;
    }
    return 0;
}

ButtonsTest::~ButtonsTest()
{
    closeTTY(_fd);
}

static void closeTTY(const int fd)
{
    if (fd)
        close(fd);
}
