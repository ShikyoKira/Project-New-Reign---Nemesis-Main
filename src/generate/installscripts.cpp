#include "Global.h"

#include "Global.h"

#include "generate/playerexclusive.h"
#include "generate/installscripts.h"
#include "generate/alternateanimation.h"

using namespace std;

extern atomic<int> m_RunningThread;

void InstallScripts::Run()
{
	try
	{
		++m_RunningThread;

		try
		{
			if (alternateAnim.size() > 0)
			{
				if (!AAInstallation())
				{
					emit end();
					return;
				}
			}
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, "Alternate Animation", ex.what());
		}
	}
	catch (nemesis::exception)
	{
		emit end();
		return;
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "Alternate Animation", "AA animation: Unknown");
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}

	try
	{
		if (error) throw nemesis::exception();

		try
		{
			if (pcealist.size() > 0) PCEAInstallation();
		}
		catch (exception& ex)
		{
			ErrorMessage(6002, "PCEA Animation", ex.what());
		}
	}
	catch (nemesis::exception&)
	{
		// resolved exception
	}
	catch (...)
	{
		try
		{
			ErrorMessage(6002, "PCEA Animation", "PCEA animation: Unknown");
		}
		catch (nemesis::exception&)
		{
			// resolved exception
		}
	}

	emit end();
}
