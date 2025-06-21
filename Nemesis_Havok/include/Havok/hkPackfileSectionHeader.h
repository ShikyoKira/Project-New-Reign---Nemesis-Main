#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkPackfile;
    struct PackfileSerializer;

    struct hkPackfileSectionHeader : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xf2a92154,
                                                "hkPackfileSectionHeader",
                                                nullptr,
                                                46,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    protected:
        std::string m_sectionTag;
        char m_nullByte;
        int m_absoluteDataStart;
        int m_localFixupsOffset;
        int m_globalFixupsOffset;
        int m_virtualFixupsOffset;
        int m_exportsOffset;
        int m_importsOffset;
        int m_endOffset;
        std::array<int, 4> m_pad;

    public:
        hkPackfileSectionHeader();

        const std::string& GetSectionTag() const noexcept;
        int GetAbsoluteDataStart() const noexcept;
        int GetLocalFixupsOffset() const noexcept;
        int GetGlobalFixupsOffset() const noexcept;
        int GetVirtualFixupsOffset() const noexcept;
        int GetExportFixupsOffset() const noexcept;
        int GetImportFixupsOffset() const noexcept;
        int GetEndOffset() const noexcept;

        void SetAbsoluteDataStart(int absoluteDataStart) noexcept;
        void SetLocalFixupsOffset(int fixupsOffset) noexcept;
        void SetGlobalFixupsOffset(int fixupsOffset) noexcept;
        void SetVirtualFixupsOffset(int fixupsOffset) noexcept;
        void SetExportFixupsOffset(int fixupsOffset) noexcept;
        void SetImportFixupsOffset(int fixupsOffset) noexcept;
        void SetEndOffset(int offset) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;

        static unsigned int GetHeaderSize(nemesis::HavokVersion version);
    };

    constexpr nemesis::hkClass nemesis::hkPackfileSectionHeader::Class;
}
