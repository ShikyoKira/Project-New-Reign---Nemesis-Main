#ifndef ANIMPROGRESSBAR_H_
#define ANIMPROGRESSBAR_H_

#include <atomic>
#include <algorithm>
#include <QProgressBar>

#include "nemesisinfo.h"

class AnimProgressBar : public QProgressBar
{
    Q_OBJECT

private:
    const double c_overtime = 0.25;
    const double c_speed    = 0.035;
    const double c_gsize    = 0.02;
    const double c_gap      = 0.075;
    const double opq        = 150;
    double overtime         = c_overtime;
    double speed            = c_speed;
    double gsize            = c_gsize;
    double gap              = c_gap;
    double aRunner          = -0.2;
    QString hue             = "120";
    QString val             = "170";
    const QString sat       = "255";

    std::mutex pbmtx;
    std::condition_variable pbcv;

    std::atomic<int> trueValue = 0;

    std::atomic_flag setFlag{};

public:
    QString font = "";
    AnimProgressBar(QWidget* parent = nullptr);

private:
    void valueUpdate();
    void commitValue(int value);

public slots:
    void ForeverLoop();
    void newValue(int value);
};

#endif
