#pragma once

#include "Havok/hkPackfileSection.h"
#include "Havok/hkClassNamesSectionHeader.h"

namespace nemesis
{
    struct PackfileDeserializer;

	struct hkClassNamesSection : nemesis::hkPackfileSection
	{
    private:
        nemesis::hkClassNamesSectionHeader Header;
        Vec<const nemesis::hkClass*> Classes;
        USet<const nemesis::hkClass*> ClassSet;

	public:
        hkClassNamesSection();

        const nemesis::hkClass* TryAddClass(const nemesis::hkClass* cls);

        nemesis::hkPackfileSectionHeader& GetHeader() override;
        const nemesis::hkPackfileSectionHeader& GetHeader() const override;
        UPtr<nemesis::hkPackfileSectionHeader> NewHeader() const override;

        UMap<const nemesis::hkClass*, unsigned int> GetClassFixups() const;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
        void DeserializeFrom(nemesis::PackfileDeserializer& deserializer);
	};
}
