#pragma once

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
    struct hkAttributeId : nemesis::hkVariant
    {
    private:
        int Value;

    public:
        nemesis::hkAttributeId& operator=(const nemesis::hkAttributeId& attr_id);
        nemesis::hkAttributeId& operator=(int var_id);

        operator int() const;
    };
}
