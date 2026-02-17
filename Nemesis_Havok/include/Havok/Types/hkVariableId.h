#pragma once

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
	struct hkVariableId : nemesis::hkVariant
	{
    private:
        int Value{};

	public:
        nemesis::hkVariableId& operator=(const nemesis::hkVariableId& var_id);
        nemesis::hkVariableId& operator=(int var_id);

        operator int() const;
	};
}
