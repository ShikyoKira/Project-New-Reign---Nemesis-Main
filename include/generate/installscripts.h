#ifndef INSTALLSCRIPTS_H_
#define INSTALLSCRIPTS_H_

#include <QtCore\QObject>

class InstallScripts : public QObject
{
	Q_OBJECT

		public slots:
	void Run();

signals:
	void end();
};

#endif
