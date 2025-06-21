#pragma once

#include "Enums/PlatformType.h"

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkPackfileHeader : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x79f9ffda,
                                                "hkPackfileHeader",
                                                nullptr,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        std::array<unsigned int, 2> m_magic;
        int m_userTag;
        int m_fileVersion;
        std::array<unsigned char, 4> m_layoutRules;
        int m_numSections;
        int m_contentsSectionIndex;
        int m_contentsSectionOffset;
        int m_contentsClassNameSectionIndex;
        int m_contentsClassNameSectionOffset;
        std::string m_contentsVersion;
        int m_flags;
        unsigned short m_maxPredicate;
        unsigned short m_predicateArraySizePlusPadding;

        short m_unknown40;
        short m_unknown42;
        unsigned int m_unknown44;
        unsigned int m_unknown48;
        unsigned int m_unknown4C;

    public:
        hkPackfileHeader();
        hkPackfileHeader(nemesis::PlatformType platform, nemesis::HavokVersion version);

        void Reset(nemesis::PlatformType platform, nemesis::HavokVersion version);

        unsigned int GetMagic0() const noexcept;
        unsigned int GetMagic1() const noexcept;
        int GetUserTag() const noexcept;
        int GetFileVersion() const noexcept;
        char GetPointerSize() const noexcept;
        char GetEndian() const noexcept;
        char GetPaddingOption() const noexcept;
        char GetBaseClass() const noexcept;
        int GetSectionCount() const noexcept;
        int GetContentsSectionIndex() const noexcept;
        int GetContentsSectionOffset() const noexcept;
        int GetContentsClassNameSectionIndex() const noexcept;
        int GetContentsClassNameSectionOffset() const noexcept;
        const std::string& GetContentsVersionString() const noexcept;
        int GetFlags() const noexcept;
        unsigned short GetMaxPredicate() const noexcept;
        unsigned short GetPredicateArraySizePlusPadding() const noexcept;
        short GetUnknown40() const noexcept;
        short GetUnknown42() const noexcept;
        unsigned int GetUnknown44() const noexcept;
        unsigned int GetUnknown48() const noexcept;
        unsigned int GetUnknown4C() const noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };
}
