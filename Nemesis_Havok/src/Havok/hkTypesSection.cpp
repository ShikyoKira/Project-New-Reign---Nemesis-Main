#include "Havok/hkTypesSection.h"
#include "Havok/hkTypesSectionHeader.h"

nemesis::hkPackfileSectionHeader& nemesis::hkTypesSection::GetHeader()
{
    return Header;
}

const nemesis::hkPackfileSectionHeader& nemesis::hkTypesSection::GetHeader() const
{
    return Header;
}

UPtr<nemesis::hkPackfileSectionHeader> nemesis::hkTypesSection::NewHeader() const
{
    return std::make_unique<nemesis::hkTypesSectionHeader>();
}

void nemesis::hkTypesSection::SerializeTo(nemesis::Serializer& serializer) const
{
}

void nemesis::hkTypesSection::DeserializeFrom(nemesis::Deserializer& deserializer)
{
}
