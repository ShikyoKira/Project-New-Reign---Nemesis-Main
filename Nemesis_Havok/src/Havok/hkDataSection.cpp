#include "Havok/hkDataSection.h"

nemesis::hkPackfileSectionHeader& nemesis::hkDataSection::GetHeader()
{
    return Header;
}

const nemesis::hkPackfileSectionHeader& nemesis::hkDataSection::GetHeader() const
{
    return Header;
}

UPtr<nemesis::hkPackfileSectionHeader> nemesis::hkDataSection::NewHeader() const
{
    return std::make_unique<nemesis::hkDataSectionHeader>();
}

const nemesis::hkClass* nemesis::hkDataSection::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkDataSection::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.Serialize(Root);
}

void nemesis::hkDataSection::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.Deserialize(Root);
}
