#include "Havok/Types/hkEventId.h"

nemesis::hkEventId& nemesis::hkEventId::operator=(const nemesis::hkEventId& event_id)
{
    Value = event_id;
    return *this;
}

nemesis::hkEventId& nemesis::hkEventId::operator=(int event_id)
{
    Value = event_id;
    return *this;
}

nemesis::hkEventId::operator int() const
{
    return Value;
}
