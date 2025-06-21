#include "Havok/hkClassNamesSection.h"
#include "Havok/hkPackfile.h"

#include "Serialize/PackfileDeserializer.h"

nemesis::hkClassNamesSection::hkClassNamesSection()
    : Classes({nemesis::hkClass::GetCurrentClass()})
{
}

const nemesis::hkClass* nemesis::hkClassNamesSection::TryAddClass(const nemesis::hkClass* cls)
{
    auto itr = ClassSet.find(cls);

    if (itr != ClassSet.end()) return cls;

    ClassSet.insert(cls);
    Classes.emplace_back(cls);
    return cls;
}

nemesis::hkPackfileSectionHeader& nemesis::hkClassNamesSection::GetHeader()
{
    return Header;
}

const nemesis::hkPackfileSectionHeader& nemesis::hkClassNamesSection::GetHeader() const
{
    return Header;
}

UPtr<nemesis::hkPackfileSectionHeader> nemesis::hkClassNamesSection::NewHeader() const
{
    return std::make_unique<nemesis::hkClassNamesSectionHeader>();
}

UMap<const nemesis::hkClass*, unsigned int> nemesis::hkClassNamesSection::GetClassFixups() const
{
    unsigned int pos = 0;
    UMap<const nemesis::hkClass*, unsigned int> map;

    for (auto& cls : Classes)
    {
        pos += 5;
        map[cls] = pos;
        pos += std::string_view(cls->GetName()).length() + 1;
    }

    return map;
}

void nemesis::hkClassNamesSection::SerializeTo(nemesis::Serializer& serializer) const
{
    for (auto& cls : Classes)
    {
        serializer.WriteValue("" , cls->GetSignature());
        serializer.WriteValue("", static_cast<char>(0x09));
        serializer.WriteValue("", cls->GetName(), true);
    }

    serializer.Pad(16, static_cast<unsigned char>(0xff));
}

void nemesis::hkClassNamesSection::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::PackfileDeserializer* pf_deser_ptr = dynamic_cast<nemesis::PackfileDeserializer*>(&deserializer);

    if (!pf_deser_ptr) return;

    DeserializeFrom(*pf_deser_ptr);
}

void nemesis::hkClassNamesSection::DeserializeFrom(nemesis::PackfileDeserializer& deserializer)
{
    size_t end_pos = Header.GetAbsoluteDataStart() + Header.GetLocalFixupsOffset();
    Classes.clear();
    ClassSet.clear();

    while (static_cast<size_t>(deserializer.GetCurrentPosition()) + 5 < end_pos)
    {
        const nemesis::hkClass* cls = deserializer.DeserializeClass();

        if (!cls) continue;

        TryAddClass(cls);
    }
}
