#pragma once

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
    struct hkCharacterPropertyId : nemesis::hkVariant
    {
    private:
        int Value{};

    public:
        nemesis::hkCharacterPropertyId& operator=(const nemesis::hkCharacterPropertyId& prop_id);
        nemesis::hkCharacterPropertyId& operator=(int prop_id);

        operator int() const;
    };
}
