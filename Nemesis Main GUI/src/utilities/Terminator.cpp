#include "Terminator.h"
#include "NemesisMainGUI.h"

void Terminator::exitSignal()
{
	emit end();
}
