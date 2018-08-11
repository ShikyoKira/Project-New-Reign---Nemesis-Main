#include "SettingsSave.h"
#include "Global.h"
#include "alphanum.hpp"
#include <fstream>
#include <QMessageBox>

void createCache(std::string language, std::vector<std::string> chosenBehavior)
{
	if (CreateFolder("cache"))
	{
		std::ofstream cachefile("cache\\settings");

		if (cachefile.is_open())
		{
			std::vector<std::string> storeline;

			storeline.push_back("*LANGUAGE");
			storeline.push_back("\n");
			storeline.push_back(language);
			storeline.push_back("\n");
			storeline.push_back("\n");
			storeline.push_back("*MODS");
			storeline.push_back("\n");

			for (auto& line : chosenBehavior)
			{
				storeline.push_back(line);
				storeline.push_back("\n");
			}

			storeline.pop_back();

			for (auto& line : storeline)
			{
				cachefile << line;
			}

			cachefile.close();
		}
		else
		{
			QMessageBox* msg = new QMessageBox;
			msg->setWindowTitle("ERROR");
			msg->setText("Error: Failed to create cache file. Please ensure Nemesis has permission to create file in current folder");
			msg->setAttribute(Qt::WA_DeleteOnClose);
			msg->show();
		}
	}
}

bool getCache(std::string& language, std::unordered_map<std::string, bool>& chosenBehavior)
{
	if (!isFileExist("cache"))
	{
		return false;
	}

	std::string filename = "cache\\settings";

	if (!isFileExist(filename))
	{
		return false;
	}

	vecstr storeline;
	GetFunctionLines(filename, storeline, false);
	bool lang = false;
	bool behavior = false;
	int done = 0;

	for (auto& line : storeline)
	{
		if (line.length() != 0)
		{
			if (line == "*LANGUAGE")
			{
				lang = true;
				++done;
			}
			else if (line == "*MODS")
			{
				behavior = true;
				++done;
			}
			else if (behavior)
			{
				chosenBehavior[line] = true;
			}
			else if (lang)
			{
				language = line;
			}
		}
		else
		{
			lang = false;
			behavior = false;
		}
	}

	if (done == 2)
	{
		return true;
	}
	else
	{
		QMessageBox* msg = new QMessageBox;
		msg->setWindowTitle("WARNING");
		msg->setText("Warning: Failed to read cache file. Settings are set to default");
		msg->setAttribute(Qt::WA_DeleteOnClose);
		msg->setIcon(QMessageBox::Warning);
		msg->show();
	}

	return false;
}
