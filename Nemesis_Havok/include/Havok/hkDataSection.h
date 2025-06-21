#pragma once

#include "Havok/hkDataSectionHeader.h"
#include "Havok/hkPackfileSection.h"
#include "Havok/hkRootLevelContainer.h"

namespace nemesis
{
	struct hkDataSection : nemesis::hkPackfileSection
	{
    private:
        nemesis::hkDataSectionHeader Header;
        nemesis::hkRootLevelContainer Root;

	public:
        nemesis::hkPackfileSectionHeader& GetHeader() override;
        const nemesis::hkPackfileSectionHeader& GetHeader() const override;
        UPtr<nemesis::hkPackfileSectionHeader> NewHeader() const override;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
	};
}
