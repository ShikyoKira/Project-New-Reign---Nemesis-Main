#include <memory>
#include <QDebug>
#include <QDir>
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

AppLauncher::~AppLauncher()
{
    quitRunningProcess();
}

void AppLauncher::launchProgram(const QString& program_path, const QStringList& args, bool is_preload)
{
    is_read_ready = !is_preload;
    has_error     = false;
    output_buffer.clear();

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

    process->start(QDir::toNativeSeparators(program_path), args);

    if (process->waitForStarted()) return;

    qWarning() << "Failed to start program: " << program_path;
    emit finishedReceived();
}

Q_INVOKABLE void AppLauncher::writeToProgram(const QStringList& args)
{
    if (!process->isWritable()) return;

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch());
    auto ms_str = std::to_string(duration.count());

    if (args.isEmpty())
    {
        input_buffer = QString::fromStdString(ms_str).toUtf8();
    }
    else
    {
        input_buffer = args.join(' ').append(" " + ms_str).toUtf8();
    }

    std::scoped_lock<std::mutex> lock(read_mutex);
    is_read_ready = true;
    QTextStream stream(&output_buffer, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        readFormattedOutput(stream.readLine() + "\n");
    }

    output_buffer.clear();
}

void AppLauncher::readFormattedOutput(const QString& output)
{
    static const QRegularExpression progress_rgx("\\x1b\\[999P(\\d+) / (\\d+)\\x1b\\[999E\n");
    int pos = 0;

    while (pos < output.length())
    {
        QRegularExpressionMatch match = progress_rgx.match(output, pos);

        if (match.hasMatch())
        {
            if (!has_error)
            {
                // Extract and process the progress number
                int step = match.captured(1).toInt();
                int max  = match.captured(2).toInt();
                qDebug() << step << " / " << max;
                emit progressUp(step, max);
            }

            // Advance position past the matched progress indicator
            pos = match.capturedEnd();
        }
        else
        {
            // No more progress indicators from pos
            // If there's a full line, print it as regular output
            int newlineIndex = output.indexOf('\n', pos);
            QString regularLine;

            if (newlineIndex != -1)
            {
                regularLine = output.mid(pos, newlineIndex - pos + 1);
            }
            else
            {
                regularLine = output.mid(pos);
                newlineIndex = output.length();
            }

            if (regularLine.startsWith("Mod Codes: "))
            {
                process->write(input_buffer + "\n");
                input_buffer.clear();
            }
            else
            {
                qDebug() << regularLine;

                if (regularLine.startsWith("[ERROR]"))
                {
                    has_error = true;
                    emit errorReceived(regularLine);
                }
                else
                {
                    emit outputReceived(regularLine);
                }
            }

            pos = newlineIndex + 1;
        }
    }
}

void AppLauncher::readOutput()
{
    QString output = process->readAllStandardOutput();

    std::scoped_lock<std::mutex> lock(read_mutex);

    if (!is_read_ready)
    {
        output_buffer += output;
        return;
    }

    qDebug() << "Console: " << output;
    readFormattedOutput(output);
}

void AppLauncher::readError()
{
    QString error = process->readAllStandardError();
    emit errorReceived(error);
}

void AppLauncher::runFinished(int exit_code, QProcess::ExitStatus exit_status)
{
    if (exit_code == 0) return;

    emit errorReceived("\n\n[ERROR] Core program crashed unexpectedly\n");
}

void AppLauncher::quitRunningProcess()
{
    if (process->state() != QProcess::NotRunning)
    {
        process->terminate();
        process->waitForFinished(3000);

        if (process->state() != QProcess::NotRunning) process->kill();
    }
}
