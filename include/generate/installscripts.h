#ifndef INSTALLSCRIPTS_H_
#define INSTALLSCRIPTS_H_

#include <QtCore\QObject>

class NemesisInfo;

class InstallScripts : public QObject
{
    Q_OBJECT

public:
    InstallScripts(const NemesisInfo* _ini);

public slots:
    void Run();

signals:
    void end();

private:
    const NemesisInfo* nemesisInfo;
};

#endif
