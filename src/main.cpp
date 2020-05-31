#include "Global.h"

#include <filesystem>

#include <QtGui/QIcon>
#include <QtWidgets/QApplication>

#include "Global.h"

#include "ui/CmdLaunch.h"
#include "ui/ErrorMsgBox.h"
#include "ui/MessageHandler.h"
#include "ui/MultiInstanceCheck.h"
#include "ui/NemesisEngine.h"

extern std::wstring stagePath;

int main(int argc, char* argv[])
{
    bool generate = false;
    bool update   = false;
    VecStr modlist;
    std::string logfile = "CriticalLog.txt";

    try
    {
        if (isFileExist(logfile) && !std::filesystem::is_directory(logfile)) std::filesystem::remove(logfile);
    }
    catch (const std::exception&)
    {
        // empty
    }

    QApplication a(argc, argv);

    try
    {
        if (argc > 1)
        {
            for (uint i = 1; i < argc; ++i)
            {
                if (nemesis::iequals(argv[i], "-update"))
                {
                    if (generate)
                    {
                        std::cout << "Invalid arguments. \"update\" argument and \"generate\" arugment "
                                     "cannot be used simultaneously";
                        std::cout << "Failed to generate behavior";
                        return 1;
                    }

                    update = true;
                }
                if (nemesis::iequals(argv[i], "-generate"))
                {
                    if (update)
                    {
                        std::cout << "Invalid arguments. \"update\" argument and \"generate\" arugment "
                                     "cannot be used simultaneously";
                        std::cout << "Failed to generate behavior";
                        return 1;
                    }

                    generate = true;
                }
                else if (std::string_view(argv[i]).find("-stage=") == 0)
                {
                    stagePath = nemesis::transform_to<std::wstring>(std::string(argv[i] + 7));

                    if (stagePath.size() > 0 && stagePath[0] == L'\"') stagePath = stagePath.substr(1);

                    if (stagePath.size() > 0 && stagePath.back() == L'\"') stagePath.pop_back();
                }
                else
                {
                    modlist.push_back(argv[i]);
                }
            }
        }

        if (generate)
        {
            if (!isFileExist("languages"))
            {
                CEMsgBox* msg = new CEMsgBox;
                msg->setWindowTitle("ERROR");
                msg->setText("Error: \"languages\" folder not found. Please reinstall Nemesis");
                msg->show();
            }
            else if (!isFileExist("languages\\english.txt"))
            {
                CEMsgBox* msg = new CEMsgBox;
                msg->setWindowTitle("ERROR");
                msg->setText(
                    "Error: \"english.txt\" file not found in language folder. Please reinstall Nemesis");
                msg->show();
            }
            else
            {
                NewDebugMessage(*new DebugMsg("english"));
                NemesisInfo* nemesisInfo = new NemesisInfo;
                CmdGenerateInitialize(modlist, nemesisInfo);
                delete nemesisInfo;
            }
        }
        else if (update)
        {
            if (!isFileExist("languages"))
            {
                CEMsgBox* msg = new CEMsgBox;
                msg->setWindowTitle("ERROR");
                msg->setText("Error: \"languages\" folder not found. Please reinstall Nemesis");
                msg->show();
            }
            else if (!isFileExist("languages\\english.txt"))
            {
                CEMsgBox* msg = new CEMsgBox;
                msg->setWindowTitle("ERROR");
                msg->setText(
                    "Error: \"english.txt\" file not found in language folder. Please reinstall Nemesis");
                msg->show();
            }
            else
            {
                NewDebugMessage(*new DebugMsg("english"));
                NemesisInfo* nemesisInfo = new NemesisInfo;
                CmdUpdateInitialize(nemesisInfo);
                delete nemesisInfo;
            }
        }
        else if (programInitiateCheck())
        {
            NemesisEngine w;
            w.setWindowIcon(QIcon(":/icon/title icon.png"));
            w.show();
            return a.exec();
        }

        return a.exec();
    }
    catch (const std::exception& ex)
    {
        QMessageBox::information(nullptr, "Exception Caught", ex.what(), QMessageBox::Ok);
    }

    return 0;
}
