#include "Havok/hkPackfile.h"

#include "Havok/CoreHavokObjects.h"

nemesis::hkPackfile::SectionArray::SectionArray(nemesis::hkPackfile& packfile)
    : SectionObjects({std::make_unique<nemesis::hkClassNamesSection>(),
                      std::make_unique<nemesis::hkTypesSection>(),
                      std::make_unique<nemesis::hkDataSection>()})
{
}

nemesis::HavokObject* nemesis::hkPackfile::CreateObjectImplt(const std::string& classname)
{
    auto& class_map = GetCreationMap();
    auto itr        = class_map.find(classname);

    if (itr == class_map.end()) return nullptr;

    return ObjectList.emplace_back(itr->second()).get();
}

nemesis::hkPackfile::hkPackfile()
    : nemesis::HavokObject(0x79f9ffda)
    , m_sections(*this)
{
    TryAddClass(nemesis::hkClass::GetCurrentClass());
}

const nemesis::hkClass* nemesis::hkPackfile::TryAddClass(const nemesis::hkClass* cls)
{
    if (!cls) return cls;

    return GetClassNamesSection().TryAddClass(cls);
}

const nemesis::hkClass* nemesis::hkPackfile::GetClassFromName(const std::string& classname) const
{
    auto& class_map = GetClassMap();
    auto itr        = class_map.find(classname);

    if (itr == class_map.end()) return nullptr;

    return itr->second(HavokVersion::HK_2010_2_0);
}

void nemesis::hkPackfile::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteFileHeader();

    for (auto& section : m_sections.SectionObjects)
    {
        serializer.Serialize(*section);
    }
}

void nemesis::hkPackfile::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadFileHeader(m_header);

    for (auto& section_obj : m_sections.SectionObjects)
    {
        deserializer.Deserialize(*section_obj);
    }
}

const nemesis::hkPackfileHeader& nemesis::hkPackfile::GetHeader() const noexcept
{
    return m_header;
}

nemesis::hkClassNamesSectionHeader& nemesis::hkPackfile::CreateClassNamesSectionHeader() noexcept
{
    return static_cast<nemesis::hkClassNamesSectionHeader&>(GetClassNamesSection().GetHeader());
}

const nemesis::hkClassNamesSectionHeader& nemesis::hkPackfile::CreateClassNamesSectionHeader() const noexcept
{
    return static_cast<const nemesis::hkClassNamesSectionHeader&>(GetClassNamesSection().GetHeader());
}

nemesis::hkClassNamesSection& nemesis::hkPackfile::GetClassNamesSection() noexcept
{
    return static_cast<nemesis::hkClassNamesSection&>(*m_sections.SectionObjects[0]);
}

const nemesis::hkClassNamesSection& nemesis::hkPackfile::GetClassNamesSection() const noexcept
{
    return static_cast<const nemesis::hkClassNamesSection&>(*m_sections.SectionObjects[0]);
}

nemesis::hkTypesSectionHeader& nemesis::hkPackfile::CreateTypesSectionHeader() noexcept
{
    return static_cast<nemesis::hkTypesSectionHeader&>(GetTypesSection().GetHeader());
}

const nemesis::hkTypesSectionHeader& nemesis::hkPackfile::CreateTypesSectionHeader() const noexcept
{
    return static_cast<const nemesis::hkTypesSectionHeader&>(GetTypesSection().GetHeader());
}

nemesis::hkTypesSection& nemesis::hkPackfile::GetTypesSection() noexcept
{
    return static_cast<nemesis::hkTypesSection&>(*m_sections.SectionObjects[1]);
}

const nemesis::hkTypesSection& nemesis::hkPackfile::GetTypesSection() const noexcept
{
    return static_cast<const nemesis::hkTypesSection&>(*m_sections.SectionObjects[1]);
}

nemesis::hkDataSectionHeader& nemesis::hkPackfile::CreateDataSectionHeader() noexcept
{
    return static_cast<nemesis::hkDataSectionHeader&>(GetDataSection().GetHeader());
}

const nemesis::hkDataSectionHeader& nemesis::hkPackfile::CreateDataSectionHeader() const noexcept
{
    return static_cast<const nemesis::hkDataSectionHeader&>(GetDataSection().GetHeader());
}

nemesis::hkDataSection& nemesis::hkPackfile::GetDataSection() noexcept
{
    return static_cast<nemesis::hkDataSection&>(*m_sections.SectionObjects[2]);
}

const nemesis::hkDataSection& nemesis::hkPackfile::GetDataSection() const noexcept
{
    return static_cast<const nemesis::hkDataSection&>(*m_sections.SectionObjects[2]);
}
