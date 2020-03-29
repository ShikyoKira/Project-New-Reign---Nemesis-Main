#include "Global.h"

#include "ui/Terminator.h"

void Terminator::exitSignal()
{
    emit end();
}
