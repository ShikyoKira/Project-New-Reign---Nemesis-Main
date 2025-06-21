#pragma once

#include "Havok/hkClassNamesSection.h"
#include "Havok/hkDataSection.h"
#include "Havok/hkPackfileHeader.h"
#include "Havok/hkPackfileSection.h"
#include "Havok/hkTypesSection.h"

namespace nemesis
{
    struct hkPackfile : nemesis::HavokObject
    {
    private:
        struct SectionArray
        {
            std::array<UPtr<nemesis::hkPackfileSection>, 3> SectionObjects;

            SectionArray(nemesis::hkPackfile& packfile);
        };

        nemesis::hkPackfileHeader m_header;
        SectionArray m_sections;

        Vec<UPtr<nemesis::HavokObject>> ObjectList;

        nemesis::HavokObject* CreateObjectImplt(const std::string& classname);

    public:
        hkPackfile();

        const nemesis::hkClass* TryAddClass(const nemesis::hkClass* cls);
        const nemesis::hkClass* GetClassFromName(const std::string& classname) const;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        const nemesis::hkPackfileHeader& GetHeader() const noexcept;

        nemesis::hkClassNamesSectionHeader& CreateClassNamesSectionHeader() noexcept;
        const nemesis::hkClassNamesSectionHeader& CreateClassNamesSectionHeader() const noexcept;

        nemesis::hkClassNamesSection& GetClassNamesSection() noexcept;
        const nemesis::hkClassNamesSection& GetClassNamesSection() const noexcept;

        nemesis::hkTypesSectionHeader& CreateTypesSectionHeader() noexcept;
        const nemesis::hkTypesSectionHeader& CreateTypesSectionHeader() const noexcept;

        nemesis::hkTypesSection& GetTypesSection() noexcept;
        const nemesis::hkTypesSection& GetTypesSection() const noexcept;

        nemesis::hkDataSectionHeader& CreateDataSectionHeader() noexcept;
        const nemesis::hkDataSectionHeader& CreateDataSectionHeader() const noexcept;

        nemesis::hkDataSection& GetDataSection() noexcept;
        const nemesis::hkDataSection& GetDataSection() const noexcept;

        template <typename T, std::enable_if_t<std::is_base_of_v<nemesis::HavokObject, T>, int> = 0>
        T* CreateObject(const std::string& classname)
        {
            return static_cast<T*>(CreateObjectImplt(classname));
        }

        nemesis::HavokObject* CreateObject(const std::string& classname)
        {
            return CreateObjectImplt(classname);
        }

        friend nemesis::Serializer;
        friend nemesis::Deserializer;
    };
}
