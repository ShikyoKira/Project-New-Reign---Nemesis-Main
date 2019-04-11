#include "CmdLaunch.h"
#include "NemesisMainGUI.h"
#include "MultiInstanceCheck.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	bool generate = false;
	bool update = false;
	vecstr modlist;
	std::string logfile = "CriticalLog.txt";
	
	try
	{
		if (isFileExist(logfile) && !boost::filesystem::is_directory(logfile))
		{
			boost::filesystem::remove(logfile);
		}
	}
	catch (const std::exception&)
	{
		// empty
	}

	QApplication a(argc, argv);

	if (argc > 1)
	{
		for (unsigned int i = 1; i < argc; ++i)
		{
			if (boost::iequals(argv[i], "-update"))
			{
				if (generate)
				{
					std::cout << "Invalid arguments. \"update\" argument and \"generate\" arugment cannot be used simultaneously";
					std::cout << "Failed to generate behavior";
					return 1;
				}

				update = true;
			}
			if (boost::iequals(argv[i], "-generate"))
			{
				if (update)
				{
					std::cout << "Invalid arguments. \"update\" argument and \"generate\" arugment cannot be used simultaneously";
					std::cout << "Failed to generate behavior";
					return 1;
				}

				generate = true;
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
			msg->setText("Error: \"english.txt\" file not found in language folder. Please reinstall Nemesis");
			msg->show();
		}
		else
		{
			NewDebugMessage(*new DebugMsg("english"));

			try
			{
				nemesisInfo = new NemesisInfo;
			}
			catch (nemesis::exception) {}

			CmdGenerateInitialize(modlist);
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
			msg->setText("Error: \"english.txt\" file not found in language folder. Please reinstall Nemesis");
			msg->show();
		}
		else
		{
			NewDebugMessage(*new DebugMsg("english"));

			try
			{
				nemesisInfo = new NemesisInfo;
			}
			catch (nemesis::exception) {}

			CmdUpdateInitialize();
		}
	}
	else if (programInitiateCheck())
	{
		NemesisMainGUI w;

		if (!error)
		{
			w.setWindowIcon(QIcon(":/icon/title icon.png"));
			w.show();
			return a.exec();
		}
	}

	return a.exec();
}
