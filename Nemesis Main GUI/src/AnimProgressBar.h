#ifndef ANIMPROGRESSBAR_H_
#define ANIMPROGRESSBAR_H_

#include <QProgressBar>
#include <algorithm>

class AnimProgressBar : public QProgressBar
{
	Q_OBJECT

public:
	AnimProgressBar(QWidget *parent = nullptr);

public slots:
	void newValue(int value);
};


#endif
