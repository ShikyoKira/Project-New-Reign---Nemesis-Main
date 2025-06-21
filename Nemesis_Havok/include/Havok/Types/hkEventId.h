#pragma once

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
	struct hkEventId : nemesis::hkVariant
	{
    private:
        int Value;

	public:
        nemesis::hkEventId& operator=(const nemesis::hkEventId& event_id);
        nemesis::hkEventId& operator=(int event_id);

		operator int() const;
	};
}
