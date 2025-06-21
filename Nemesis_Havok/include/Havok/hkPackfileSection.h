#pragma once

#include "Havok/Base/HavokObject.h"

namespace nemesis
{
    struct hkPackfile;
    struct hkPackfileSectionHeader;

	struct hkPackfileSection : nemesis::HavokObject
    {
	public:
        hkPackfileSection();

        virtual nemesis::hkPackfileSectionHeader& GetHeader()             = 0;
        virtual const nemesis::hkPackfileSectionHeader& GetHeader() const = 0;
        virtual UPtr<nemesis::hkPackfileSectionHeader> NewHeader() const  = 0;

        virtual void SerializeTo(nemesis::Serializer& serializer) const   = 0;
        virtual void DeserializeFrom(nemesis::Deserializer& deserializer) = 0;
	};
}
