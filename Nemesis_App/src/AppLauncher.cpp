#include <QProcess>
#include <QDebug>
#include <iostream>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QTextStream>
#include <QRegularExpression>

#include "AppLauncher.h"

AppLauncher::AppLauncher(QObject* parent)
    : QObject{parent}
{
    process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardOutput, this, &AppLauncher::readOutput);
    connect(process, &QProcess::readyReadStandardError, this, &AppLauncher::readError);
    connect(process, &QProcess::finished, this, &AppLauncher::runFinished);
}

void AppLauncher::launchProgram(const QString& program_path, const QStringList& args)
{
    std::string args_str;

    for (int i = 0; i < args.count(); ++i)
    {
        if (args[i].contains(" "))
        {
            args_str.append("\"" + args[i].toStdString() + "\"");
        }
        else
        {
            args_str.append(args[i].toStdString());
        }

        if (i + 1 != args.count())
        {
            args_str.append(" ");
        }
    }

    qDebug() << "Starting program: \"" << program_path << "\" " << args_str;

    process->start(program_path, args);

    if (process->waitForStarted()) return;

    qWarning() << "Failed to start program: " << program_path;
    emit finishedReceived();
}

void AppLauncher::readOutput()
{
    static const QRegularExpression progress_rgx("\\x1b\\[999P(\\d+) / (\\d+)\\x1b\\[999E");
    QString output = process->readAllStandardOutput();
    int pos = 0;

    while (pos < output.length())
    {
        QRegularExpressionMatch match = progress_rgx.match(output, pos);

        if (match.hasMatch()) {
            // We found a progress indicator!

            // Extract and process the progress number
            int step = match.captured(1).toInt();
            int max = match.captured(2).toInt();
            qDebug() << step << " / " << max;
            emit progressUp(step, max);

            // Advance position past the matched progress indicator
            pos = match.capturedEnd();
        }
        else
        {
            // No more progress indicators from pos
            // If there's a full line, print it as regular output
            int newlineIndex = output.indexOf('\n', pos);

            if (newlineIndex == -1) break;

            QString regularLine = output.mid(pos, newlineIndex - pos + 1);
            emit outputReceived(regularLine);
            pos = newlineIndex + 1;
        }
    }
}

void AppLauncher::readError()
{
    QString error = process->readAllStandardError();
    emit errorReceived(error);
}

void AppLauncher::runFinished()
{
    emit finishedReceived();
}
