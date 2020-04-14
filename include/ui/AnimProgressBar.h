#ifndef ANIMPROGRESSBAR_H_
#define ANIMPROGRESSBAR_H_

#include <algorithm>
#include <QProgressBar>

#include "nemesisinfo.h"

class AnimProgressBar : public QProgressBar
{
    Q_OBJECT

private:
    double cTimer     = 0.9;
    double mRunner    = -0.2;
    double gap        = 0.2;
    double opq        = 155;
    QString hue       = "120";
    const QString sat = "255";
    QString val       = "170";

public:
    QString font = "";
    AnimProgressBar(QWidget* parent = nullptr);

public slots:
    void ForeverLoop();
    void newValue(int value);
};

#endif
