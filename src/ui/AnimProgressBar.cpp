#include "Global.h"

#include <QtCore/qtimer.h>

#include "ui/AnimProgressBar.h"

AnimProgressBar::AnimProgressBar(QWidget* parent)
{
    QTimer* mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(ForeverLoop()));
    mTimer->start(50);
}

void AnimProgressBar::ForeverLoop()
{
    QString color = hue + ", " + sat + ", " + val;

    if (mRunner >= 1)
    {
        if (mRunner <= 1 + gap)
        {
            double num = 255 - std::fmin(255, std::fmax(0, ((1 + gap - mRunner) / gap * opq)));
            QString style
                = font
                  + QString(
                      "QProgressBar::chunk {background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:"
                      + QString::number(0.99995 - gap) + " hsv(" + color + "), stop:0.99995 hsv(" + color
                      + ", " + QString::number(num) + "), stop:1 hsv(" + color + "));}");
            setStyleSheet(style);
        }
        else
        {
            QString style
                = font
                  + QString(
                      "QProgressBar::chunk {background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:"
                      + QString::number(0.99995 - gap) + " hsv(" + color + "), stop:1 hsv(" + color + "));}");
            setStyleSheet(style);
        }
    }
    else if (mRunner <= 0)
    {
        double num = 255 - std::fmax(0, ((gap + mRunner) / gap * opq));
        QString style
            = font
              + QString(
                  "QProgressBar::chunk {background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 hsv("
                  + color + "), stop:0.00005 hsv(" + color + ", " + QString::number(num)
                  + "), stop:" + QString::number(0.00005 + gap) + " hsv(" + color + "));}");
        setStyleSheet(style);
    }
    else
    {
        double lower  = std::fmax(0, mRunner - gap);
        double higher = std::fmin(1, mRunner + gap);
        QString style
            = font
              + QString("QProgressBar::chunk {background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:"
                        + QString::number(lower) + " hsv(" + color + "), stop:%1 hsv(" + color + ", "
                        + QString::number(255 - opq) + "), stop:" + QString::number(higher) + " hsv(" + color
                        + "));}")
                    .arg(mRunner);
        setStyleSheet(style);
    }

    mRunner += cTimer;

    if (mRunner > (1 + gap + 0.8)) { mRunner = 0 - gap; }
}

void AnimProgressBar::newValue(int value)
{
    if (value <= MAX_ANIM)
    {
        double decivalue = value;
        double power     = decivalue / MAX_ANIM;
        opq              = std::fmin(155, 55 + (decivalue / (MAX_ANIM * 3 / 10) * 100));
        gap              = std::fmax(0.2, 0.9 - (decivalue / (MAX_ANIM * 3 / 10) * 0.7));
        cTimer           = 0.01 * powf(0.1, power) / 0.1;
        hue              = QString::number(static_cast<int>(120 - (decivalue / MAX_ANIM * 120)));
        val              = QString::number(static_cast<int>(170 + (decivalue / MAX_ANIM * 55)));
    }

    if (value == MAX_ANIM * 7 / 10) { font = "QProgressBar { color: rgb(255, 255, 255); } "; }

    setValue(value);
    setFormat(QString::number(value) + " animation(s)");
}
