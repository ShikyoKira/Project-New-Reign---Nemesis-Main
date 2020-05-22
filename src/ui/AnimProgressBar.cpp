#include <thread>
#include <cmath>

#include "Global.h"

#include <QtCore/qtimer.h>
#include <QtConcurrent/qtconcurrentrun.h>

#include "ui/AnimProgressBar.h"

#include "utilities/atomiclock.h"

bool AreDoubleSame(double dFirstVal, double dSecondVal)
{
    return std::abs(dFirstVal - dSecondVal) < std::numeric_limits<double>::epsilon();
}

AnimProgressBar::AnimProgressBar(QWidget* parent)
{
    QTimer* mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &AnimProgressBar::ForeverLoop);
    mTimer->start(50);
    
    std::thread t(std::bind(&AnimProgressBar::valueUpdate, this));
    t.detach();
}

void AnimProgressBar::valueUpdate() 
{
    try
    {
        int oldvalue ;
        int base;
        double root;
        const int max = 110;
        int counter;

        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //newValue(798);

        auto resetRaise = [&] {
            oldvalue = trueValue;
            base     = value();
            counter  = max;
            root     = std::log(max) / std::log(oldvalue - base);
        };

        while (true)
        {
            {
                std::unique_lock ulock(pbmtx);
                pbcv.wait(ulock, [&] { return trueValue != value(); });
            }

            resetRaise();

            while (counter > 0)
            {
                int v = oldvalue - static_cast<int>(std::pow(--counter, 1 / root));

                if (v >= 0) commitValue(v);

                std::this_thread::sleep_for(std::chrono::milliseconds(20));

                if (oldvalue != trueValue)
                {
                    resetRaise();
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
    }
    catch (...)
    {
    }

    std::thread t(std::bind(&AnimProgressBar::valueUpdate, this));
    t.detach();
}

void AnimProgressBar::ForeverLoop()
{
    QString style
        = font + "QProgressBar::chunk {background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, ";
    QString trueopq = QString::number(255 - opq);
    double bRunner = aRunner + gsize;
    double aGap    = aRunner - gap;
    double bGap    = bRunner + gap;

    const auto BigOrEqual = [](double a, double b) { return a > b || AreDoubleSame(a, b); };
    const auto SmallOrEqual = [](double a, double b) { return a < b || AreDoubleSame(a, b); };

    // Symbols below are used to illustrate the progress bar coordination as visual representation
    // [xx] : this is the opaque block
    // [    : left side of the opaque block (aRunner)
    // ]    : right side of the opaque block (bRunner)
    // xx   : size of the opaque block (gap)
    // ---- : this is the solid color section
    // |S|   : this is the start point
    // |E|   : this is the end point
    // <==  : this is the gradient gap

    // |S|----------<==[xx]==>--------|E|
    // Illustration above is visual for progress bar where the opaque block is flowing within the visible section

    // Note: Visible section only exists between start point and end point, meaning in between |S| and |E|

    if (SmallOrEqual(bGap, 0) || BigOrEqual(aGap, 1))
    {
        // <==[xx]==>--|S|---
        // --|E|---<==[xx]==>
        // block and gradient gap are outside of visible section

        style.append("stop:0 hsv(%1), stop:1 hsv(%1)");
    }
    else if (BigOrEqual(aRunner, 1))
    {
        if (SmallOrEqual(aGap, 0))
        {
            // <=|S|=|E|[xx]
            // whole chunk only contains the gradient gap area

            double num   = 255 - std::fmin(255, std::fabs(aGap) / gap * opq);
            style.append("stop:0 hsv(%1, " + QString::number(num) + ")");
        }
        else
        {
            // --|S|----------<=|E|=[xx]==>
            // small gradient gap still within visible section

            style.append("stop:" + QString::number(1 - gap) + " hsv(%1)");
        }

        double num = 255 - std::fmin(255, (1 + gap - aRunner) / gap * opq);
        style.append(", stop:1 hsv(%1, " + QString::number(num) + ")");
    }
    else if (SmallOrEqual(bRunner, 0))
    {
        // <==[xx]=|S|=>----------
        // only the gradient part of the opaque block is within the visible section at the start

        double num = 255 - std::fmax(0, bGap / gap * opq);
        style.append("stop:0 hsv(%1, " + QString::number(num) + "), stop:" + QString::number(bGap)
                     + " hsv(%1)");
    }
    else if (SmallOrEqual(aRunner, 0))
    {
        // <==[x|S|x]==>
        // only part of the opaque block is within the visible section at the start

        style.append("stop:0 hsv(%1, " + trueopq + ")");

        if (SmallOrEqual(bRunner, 1))
        {
            style.append(", stop:" + QString::number(bRunner) + " hsv(%1, " + trueopq + ")");

            if (BigOrEqual(bGap, 1))
            {
                // <==[x|S|x]==|E|>
                // the progress bar's chunk is small that right end of the gradient gap is hitting the end of the progress bar chunk

                double num   = 255 - std::fmin(255, (bGap - 1) / gap * opq);
                style.append(", stop:1 hsv(%1, " + QString::number(num) + ")");
            }
            else
            {
                // <==[x|S|x]==>---------|E|
                //          ^
                //       bRunner
                // the progress bar's chunk gradient gap not hitting the end

                style.append(", stop:" + QString::number(bGap) + " hsv(%1)");
            }
        }
        else
        {
            // <==[x|S|x|E|]==>
            // the progress bar's chunk is so small that fits within the whole block
        }
    }
    else if (SmallOrEqual(aGap, 0))
    {
        // <=|S|=[xx]
        // part of the left gradient is outside of the start point

        double anum = 255 - std::fmin(255, std::fabs(aGap) / gap * opq);
        style.append("stop:0 hsv(%1, " + QString::number(anum) + "), stop:" + QString::number(aRunner) + " hsv(%1, " + trueopq + ")");

        if (SmallOrEqual(bRunner, 1))
        {
            style.append(", stop:" + QString::number(bRunner) + " hsv(%1, " + trueopq + ")");

            if (BigOrEqual(bGap, 1))
            {
                // <=|S|=[xx]=|E|=>
                // the part of the gradient area at left and right are outside of the start point and end point respectively

                double bextra = 1 + gap - bGap;
                double num    = 255 - std::fmin(255, bextra / gap * opq);
                style.append(", stop:1 hsv(%1, " + QString::number(num) + ")");
            }
            else
            {
                // <=|S|=[xx]==>-------|E|
                // part of the left gradient is outside of the start point

                style.append(", stop:" + QString::number(bGap) + " hsv(%1)");
            }
        }
        else
        {
            // <=|S|=[x|E|x]==>
            // the part of the gradient area at left is outside of the start point while the block is outside of the end point
        }
    }
    else if (BigOrEqual(bRunner, 1))
    {
        // ------<==[x|E|x]==>
        // only part of the opaque block is within the end point

        style.append("stop:" + QString::number(aGap) + " hsv(%1), stop:" + QString::number(aRunner) + " hsv(%1, "
                     + trueopq + "), stop:0.99995 hsv(%1, " + trueopq + "), stop:1 hsv(%1)");
    }
    else if (BigOrEqual(bGap, 1))
    {
        // ------<==[xx]=|E|=>
        // part of the right gradient is outside of the end point

        double bextra = gap + bGap - 1;
        double bnum   = 255 - std::fmin(255, bextra / gap * opq);
        style.append("stop:" + QString::number(aGap) + " hsv(%1), stop:" + QString::number(aRunner)
                     + " hsv(%1, " + trueopq + "), stop:" + QString::number(bRunner) + " hsv(%1, " + trueopq
                     + "), stop:1 hsv(%1, " + QString::number(bnum) + ")");
    }
    else
    {
        // |S|----------<==[xx]==>--------|E|
        // the opaque block is flowing within the visible section

        style.append("stop:" + QString::number(aGap) + " hsv(%1), stop:" + QString::number(aRunner)
                     + " hsv(%1, " + trueopq + "), stop:" + QString::number(bRunner) + " hsv(%1, " + trueopq
                     + "), stop:" + QString::number(bGap) + " hsv(%1)");
    }

    QString color = hue + ", " + sat + ", " + val;
    style.append(");}");

    try
    {
        Lockless n(setFlag);
        setStyleSheet(style.arg(color));
    }
    catch (...)
    {
    }

    aRunner += speed;

    if (aRunner < 0 - overtime || aGap > 1 + overtime) aRunner = 0 - overtime;
}

void AnimProgressBar::newValue(int value)
{
    std::unique_lock ulock(pbmtx);
    trueValue = value;
    trueValue > 0 ? pbcv.notify_one() : commitValue(trueValue);
}

void AnimProgressBar::commitValue(int value)
{
    Lockless n(setFlag);
    int max_anim = maximum();

    if (0 < value && value <= max_anim)
    {
        double decivalue = value;
        double power     = decivalue / max_anim;
        double mult      = max_anim / std::min(max_anim, value);
        overtime         = c_overtime * mult;
        speed            = c_speed * mult;
        gsize            = c_gsize * mult;
        gap              = c_gap * mult;
        hue              = QString::number(static_cast<int>(120 - (decivalue / max_anim * 120)));
        val              = QString::number(static_cast<int>(170 + (decivalue / max_anim * 55)));
    }

    if (value == max_anim * 7 / 10) font = "QProgressBar { color: rgb(255, 255, 255); } ";

    try
    {
        setValue(value);
    }
    catch (...)
    {
    }

    try
    {
        setFormat(QString::number(value) + " animation(s)");
    }
    catch (const std::exception&)
    {
    }
}
