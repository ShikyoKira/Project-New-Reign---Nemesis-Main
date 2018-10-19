#include "CmdLaunch.h"
#include "NemesisMainGUI.h"
#include "MultiInstanceCheck.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	bool generate = false;
	bool update = false;
	vecstr modlist;
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
					return 1;
				}

				update = true;
			}
			if (boost::iequals(argv[i], "-generate"))
			{
				if (update)
				{
					std::cout << "Invalid arguments. \"update\" argument and \"generate\" arugment cannot be used simultaneously";
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
			skyrimDataPath = new DataPath;
			CmdGenerateInitialize(modlist);
		}

		return 1;
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
			skyrimDataPath = new DataPath;
			CmdUpdateInitialize();
		}
	}
	else
	{
		if (programInitiateCheck())
		{
			NemesisMainGUI w;

			skyrimDataPath = new DataPath;
			bool preError = false;

			if (error)
			{
				preError = error;
				error = false;
			}

			if (error)
			{
				return 1;
			}

			if (preError)
			{
				w.ui.buttonCheck->setDisabled(true);
				w.ui.buttonLaunch->setDisabled(true);
				w.ui.buttonUpdate->setDisabled(true);
				w.ui.modView->setDisabled(true);
				w.ui.comboBox->setDisabled(true);
			}

			w.show();
			return a.exec();
		}
	}

	return a.exec();
}
