#ifndef TERMINATOR_H_
#define TERMINATOR_H_

#include <QtCore\QObject>

class NemesisEngine;

class Terminator : public QObject
{
	Q_OBJECT

public:
	void exitSignal();

signals:
	void end();
};

bool isRunning(Terminator*& curEvent);


#endif