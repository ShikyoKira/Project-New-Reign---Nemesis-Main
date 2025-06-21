#pragma once

#include "Havok/Types/hkRotation.h"

namespace nemesis
{
	struct hkTransform : nemesis::hkVariant
	{
    protected:
        nemesis::hkRotation m_rotation;
        nemesis::hkVector4 m_translation;

	public:
        nemesis::hkRotation& GetRotation();
        const nemesis::hkRotation& GetRotation() const;

        nemesis::hkVector4& GetTranslation();
        const nemesis::hkVector4& GetTranslation() const;
	};
}
