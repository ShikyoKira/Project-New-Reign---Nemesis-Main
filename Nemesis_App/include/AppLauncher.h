#pragma once

#include <QObject>
#include <QProcess>

class AppLauncher : public QObject
{
    Q_OBJECT
public:
    explicit AppLauncher(QObject* parent = nullptr);

    Q_INVOKABLE void launchProgram(const QString &program_path, const QStringList& args);

signals:
    void outputReceived(const QString &output);
    void errorReceived(const QString &error);
    void finishedReceived();
    void progressUp(int step, int max);

private slots:
    void readOutput();
    void readError();
    void runFinished();

private:
    QProcess* process;
};
