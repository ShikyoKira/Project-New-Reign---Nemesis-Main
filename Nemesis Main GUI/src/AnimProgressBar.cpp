#include "AnimProgressBar.h"

AnimProgressBar::AnimProgressBar(QWidget * parent)
{
}

void AnimProgressBar::newValue(int value)
{
	if (value <= 12000)
	{
		setValue(value);
		int red = std::min(value / 5000 * 150, 150);
		int val = std::max(150 + (value / 10000 * 105), 255);
		int green = std::min(std::max(val - (value - 5000) / 5000 * val, 0), val);
		const int blue = 0;
		QString color = "QProgressBar::chunk {background-color: rgb(" + QString::number(red) + ", " + QString::number(green) + ", " + QString::number(blue) + ";}";
		setStyleSheet(color);
	}

	setFormat(QString::number(value) + " animation(s)");
}
