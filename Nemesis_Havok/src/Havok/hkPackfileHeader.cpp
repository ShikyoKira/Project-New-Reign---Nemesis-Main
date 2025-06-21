#include "Havok/hkPackfileHeader.h"

nemesis::hkPackfileHeader::hkPackfileHeader()
    : nemesis::HavokObject(0x79f9ffda)
{
    Reset(nemesis::PlatformType::WIN32, nemesis::HavokVersion::HK_2010_2_0);
}

nemesis::hkPackfileHeader::hkPackfileHeader(nemesis::PlatformType platform, nemesis::HavokVersion version)
    : nemesis::HavokObject(0x79f9ffda)
{
    Reset(platform, version);
}

void nemesis::hkPackfileHeader::Reset(nemesis::PlatformType platform, nemesis::HavokVersion version)
{
    m_magic[0]                       = 0x57E0E057;
    m_magic[1]                       = 0x10C0C010;
    m_userTag                        = 0;
    m_fileVersion                    = 8;

    switch (platform)
    {
        case nemesis::PlatformType::WIN32:
        case nemesis::PlatformType::PS3:
        case nemesis::PlatformType::XB360:
            m_layoutRules[0] = 0x04;
            break;
        default:
            m_layoutRules[0] = 0x08;
            break;
    }

    m_layoutRules[1]
        = platform == nemesis::PlatformType::PS3 || platform == nemesis::PlatformType::XB360 ? 0x00 : 0x01;
    m_layoutRules[2]
        = platform != nemesis::PlatformType::PS3 && platform != nemesis::PlatformType::PS4 ? 0x00 : 0x01;
    m_layoutRules[3]                 = 0x01;
    m_numSections                    = 3;
    m_contentsSectionIndex           = 2;
    m_contentsSectionOffset          = 0;
    m_contentsClassNameSectionIndex  = 0;
    m_contentsClassNameSectionOffset = 0x4B;
    m_contentsVersion                = nemesis::GetStringVersion(version);
    m_flags                          = 0;
    m_maxPredicate                   = 65535;
    m_predicateArraySizePlusPadding  = 65535;
    m_unknown40                      = 0;
    m_unknown42                      = 0;
    m_unknown44                      = 0;
    m_unknown48                      = 0;
    m_unknown4C                      = 0;
}

unsigned int nemesis::hkPackfileHeader::GetMagic0() const noexcept
{
    return m_magic[0];
}

unsigned int nemesis::hkPackfileHeader::GetMagic1() const noexcept
{
    return m_magic[1];
}

int nemesis::hkPackfileHeader::GetUserTag() const noexcept
{
    return m_userTag;
}

int nemesis::hkPackfileHeader::GetFileVersion() const noexcept
{
    return m_fileVersion;
}

char nemesis::hkPackfileHeader::GetPointerSize() const noexcept
{
    return m_layoutRules[0];
}

char nemesis::hkPackfileHeader::GetEndian() const noexcept
{
    return m_layoutRules[1];
}

char nemesis::hkPackfileHeader::GetPaddingOption() const noexcept
{
    return m_layoutRules[2];
}

char nemesis::hkPackfileHeader::GetBaseClass() const noexcept
{
    return m_layoutRules[3];
}

int nemesis::hkPackfileHeader::GetSectionCount() const noexcept
{
    return m_numSections;
}

int nemesis::hkPackfileHeader::GetContentsSectionIndex() const noexcept
{
    return m_contentsSectionOffset;
}

int nemesis::hkPackfileHeader::GetContentsSectionOffset() const noexcept
{
    return m_contentsClassNameSectionIndex;
}

int nemesis::hkPackfileHeader::GetContentsClassNameSectionIndex() const noexcept
{
    return m_contentsClassNameSectionOffset;
}

int nemesis::hkPackfileHeader::GetContentsClassNameSectionOffset() const noexcept
{
    return m_contentsClassNameSectionOffset;
}

const std::string& nemesis::hkPackfileHeader::GetContentsVersionString() const noexcept
{
    return m_contentsVersion;
}

int nemesis::hkPackfileHeader::GetFlags() const noexcept
{
    return m_flags;
}

unsigned short nemesis::hkPackfileHeader::GetMaxPredicate() const noexcept
{
    return m_maxPredicate;
}

unsigned short nemesis::hkPackfileHeader::GetPredicateArraySizePlusPadding() const noexcept
{
    return m_predicateArraySizePlusPadding;
}

short nemesis::hkPackfileHeader::GetUnknown40() const noexcept
{
    return m_unknown40;
}

short nemesis::hkPackfileHeader::GetUnknown42() const noexcept
{
    return m_unknown42;
}

unsigned int nemesis::hkPackfileHeader::GetUnknown44() const noexcept
{
    return m_unknown44;
}

unsigned int nemesis::hkPackfileHeader::GetUnknown48() const noexcept
{
    return m_unknown48;
}

unsigned int nemesis::hkPackfileHeader::GetUnknown4C() const noexcept
{
    return m_unknown4C;
}

const nemesis::hkClass* nemesis::hkPackfileHeader::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkPackfileHeader::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", m_magic[0]);
    serializer.WriteValue("", m_magic[1]);

    serializer.WriteValue("", m_userTag);
    serializer.WriteValue("", m_fileVersion);
    serializer.WritePointerSize(m_layoutRules[0]);
    serializer.WriteEndian(m_layoutRules[1]);
    serializer.WritePaddingOption(m_layoutRules[2]);
    serializer.WriteValue("", m_layoutRules[3]);
    serializer.WriteValue("", m_numSections);
    serializer.WriteValue("", m_contentsSectionIndex);
    serializer.WriteValue("", m_contentsSectionOffset);
    serializer.WriteValue("", m_contentsClassNameSectionIndex);
    serializer.WriteValue("", m_contentsClassNameSectionOffset);
    serializer.WriteContentsVersionString(m_contentsVersion);
    serializer.WriteValue("", m_flags);
    serializer.WriteValue("", m_maxPredicate);
    serializer.WriteValue("", m_predicateArraySizePlusPadding);

    if (m_predicateArraySizePlusPadding != 16) return;

    serializer.WriteValue("", m_unknown40);
    serializer.WriteValue("", m_unknown42);
    serializer.WriteValue("", m_unknown44);
    serializer.WriteValue("", m_unknown48);
    serializer.WriteValue("", m_unknown4C);
}

void nemesis::hkPackfileHeader::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.AssertValue<unsigned int>(m_magic[0], {0x57E0E057});
    deserializer.AssertValue<unsigned int>(m_magic[1], {0x10C0C010});

    deserializer.ReadValue("", m_userTag);
    deserializer.AssertValue<int>(m_fileVersion, {0x08});

    deserializer.AssertValue<unsigned char>(m_layoutRules[0], {0x04, 0x08});
    deserializer.AssertValue<unsigned char>(m_layoutRules[1], {0x00, 0x01});
    deserializer.AssertValue<unsigned char>(m_layoutRules[2], {0x00, 0x01});
    deserializer.AssertValue<unsigned char>(m_layoutRules[3], {0x01});

    deserializer.AssertValue<int>(m_numSections, {0x03});
    deserializer.ReadValue("", m_contentsSectionIndex);
    deserializer.ReadValue("", m_contentsSectionOffset);
    deserializer.ReadValue("", m_contentsClassNameSectionIndex);
    deserializer.ReadValue("", m_contentsClassNameSectionOffset);
    deserializer.ReadValue("", m_contentsVersion, 16);
    deserializer.ReadValue("", m_flags);
    deserializer.ReadValue("", m_maxPredicate);
    deserializer.ReadValue("", m_predicateArraySizePlusPadding);

    if (m_predicateArraySizePlusPadding != 16) return;

    deserializer.ReadValue("", m_unknown40);
    deserializer.ReadValue("", m_unknown42);
    deserializer.ReadValue("", m_unknown44);
    deserializer.ReadValue("", m_unknown48);
    deserializer.ReadValue("", m_unknown4C);
}
