#include "filerelease.h"
#include "wstrconvert.h"
#include <Windows.h>
#include <RestartManager.h>
#pragma comment (lib, "Rstrtmgr.lib")

BOOL ReleaseLockedFile(LPCWSTR filepath);

bool ReleaseLockedFile(std::string filepath)
{
	return (ReleaseLockedFile(wstrConv.from_bytes(filepath).c_str()) != 0);
}

BOOL ReleaseLockedFile(LPCWSTR filepath)
{
	BOOL bResult = FALSE;
	DWORD dwSession;
	WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
	DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);

	if (dwError == ERROR_SUCCESS)
	{
		dwError = RmRegisterResources(dwSession, 1, &filepath, 0, NULL, 0, NULL);

		if (dwError == ERROR_SUCCESS)
		{
			UINT nProcInfoNeeded = 0;
			UINT nProcInfo = 0;
			RM_PROCESS_INFO rgpi[1];
			DWORD dwReason;
			dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi, &dwReason);

			if (dwError == ERROR_SUCCESS || dwError == ERROR_MORE_DATA)
			{
				if (nProcInfoNeeded > 0)
				{
					dwError = RmShutdown(dwSession, RmForceShutdown, NULL);

					if (dwError == ERROR_SUCCESS)
					{
						bResult = TRUE;
					}
				}
				else
				{
					bResult = TRUE;
				}
			}
		}
	}

	RmEndSession(dwSession);
	SetLastError(dwError);
	return bResult;
}
