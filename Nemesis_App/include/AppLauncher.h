#pragma once

#include <mutex>
#include <QObject>
#include <QProcess>

class AppLauncher : public QObject
{
    Q_OBJECT
public:
    explicit AppLauncher(QObject* parent = nullptr);
    ~AppLauncher();

    Q_INVOKABLE void launchProgram(const QString &program_path, const QStringList& args, bool is_preload);
    Q_INVOKABLE void writeToProgram(const QStringList& args);

    void readFormattedOutput(const QString& output);

signals:
    void outputReceived(const QString &output);
    void errorReceived(const QString &error);
    void finishedReceived();
    void progressUp(int step, int max);

private slots:
    void readOutput();
    void readError();
    void runFinished(int exit_code, QProcess::ExitStatus exit_status);

public slots:
    void quitRunningProcess();

private:
    QProcess* process;
    QString output_buffer;
    QByteArray input_buffer;
    std::chrono::high_resolution_clock::time_point start_time;
    std::mutex read_mutex;
    bool is_read_ready;
    bool has_error;
};
