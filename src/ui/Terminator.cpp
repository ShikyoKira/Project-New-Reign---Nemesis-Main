#include "header/ui/Terminator.h"

void Terminator::exitSignal()
{
	emit end();
}
