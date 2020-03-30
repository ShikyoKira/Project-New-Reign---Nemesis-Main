#include "Global.h"

#include "ui/ErrorMsgBox.h"
#include "ui/MultiInstanceCheck.h"

#include <atlstr.h>

bool isProgramAlreadyRunning();

bool programInitiateCheck()
{
    if (isProgramAlreadyRunning())
    {
        CEMsgBox* errorMsg = new CEMsgBox;
        errorMsg->setWindowTitle("Nemesis Unlimited Behavior Engine");
        errorMsg->setText("An instance of Nemesis is already running");
        errorMsg->show();
        return false;
    }

    return true;
}

bool isProgramAlreadyRunning()
{
    CString appName = CString(_T("Nemesis-Ultimate=Behavior+Engine"));
    HANDLE m_mutex  = CreateMutexW(NULL, FALSE, appName);

    switch (GetLastError())
    {
        case ERROR_SUCCESS: break;

        case ERROR_ALREADY_EXISTS: return true;

        default: return true;
    }

    return false;
}
