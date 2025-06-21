#pragma once

#include "Havok/hkPackfileSection.h"
#include "Havok/hkTypesSectionHeader.h"

namespace nemesis
{
	struct hkTypesSection : nemesis::hkPackfileSection
	{
    private:
        nemesis::hkTypesSectionHeader Header;

	public:
		nemesis::hkPackfileSectionHeader& GetHeader() override;
        const nemesis::hkPackfileSectionHeader& GetHeader() const override;
        UPtr<nemesis::hkPackfileSectionHeader> NewHeader() const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
	};
}
