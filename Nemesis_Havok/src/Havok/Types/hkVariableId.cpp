#include "Havok/Types/hkVariableId.h"

nemesis::hkVariableId& nemesis::hkVariableId::operator=(const nemesis::hkVariableId& var_id)
{
    Value = var_id;
    return *this;
}

nemesis::hkVariableId& nemesis::hkVariableId::operator=(int var_id)
{
    Value = var_id;
    return *this;
}

nemesis::hkVariableId::operator int() const
{
    return Value;
}
