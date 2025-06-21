#include "Havok/hkPackfileSectionHeader.h"

#include <iostream>

unsigned int PadOffset(unsigned int offset)
{
    return offset - (offset % 16) + (16 * (offset % 16 > 0));
}

nemesis::hkPackfileSectionHeader::hkPackfileSectionHeader()
    : nemesis::HavokObject(Class.GetSignature())
{
}

const std::string& nemesis::hkPackfileSectionHeader::GetSectionTag() const noexcept
{
    return m_sectionTag;
}

int nemesis::hkPackfileSectionHeader::GetAbsoluteDataStart() const noexcept
{
    return m_absoluteDataStart;
}

int nemesis::hkPackfileSectionHeader::GetLocalFixupsOffset() const noexcept
{
    return m_localFixupsOffset;
}

int nemesis::hkPackfileSectionHeader::GetGlobalFixupsOffset() const noexcept
{
    return m_globalFixupsOffset;
}

int nemesis::hkPackfileSectionHeader::GetVirtualFixupsOffset() const noexcept
{
    return m_virtualFixupsOffset;
}

int nemesis::hkPackfileSectionHeader::GetExportFixupsOffset() const noexcept
{
    return m_exportsOffset;
}

int nemesis::hkPackfileSectionHeader::GetImportFixupsOffset() const noexcept
{
    return m_importsOffset;
}

int nemesis::hkPackfileSectionHeader::GetEndOffset() const noexcept
{
    return m_endOffset;
}

void nemesis::hkPackfileSectionHeader::SetAbsoluteDataStart(int absoluteDataStart) noexcept
{
    m_absoluteDataStart = absoluteDataStart;
}

void nemesis::hkPackfileSectionHeader::SetLocalFixupsOffset(int fixupsOffset) noexcept
{
    m_localFixupsOffset = fixupsOffset;
}

void nemesis::hkPackfileSectionHeader::SetGlobalFixupsOffset(int fixupsOffset) noexcept
{
    m_globalFixupsOffset = fixupsOffset;
}

void nemesis::hkPackfileSectionHeader::SetVirtualFixupsOffset(int fixupsOffset) noexcept
{
    m_virtualFixupsOffset = fixupsOffset;
}

void nemesis::hkPackfileSectionHeader::SetExportFixupsOffset(int fixupsOffset) noexcept
{
    m_exportsOffset = fixupsOffset;
}

void nemesis::hkPackfileSectionHeader::SetImportFixupsOffset(int fixupsOffset) noexcept
{
    m_importsOffset = fixupsOffset;
}

void nemesis::hkPackfileSectionHeader::SetEndOffset(int offset) noexcept
{
    m_endOffset = offset;
}

const nemesis::hkClass* nemesis::hkPackfileSectionHeader::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkPackfileSectionHeader::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("", m_sectionTag, 19, '\0');
    serializer.WriteValue("", m_nullByte);
    serializer.WriteValue("", m_absoluteDataStart);
    serializer.WriteValue("", m_localFixupsOffset);
    serializer.WriteValue("", m_globalFixupsOffset);
    serializer.WriteValue("", m_virtualFixupsOffset);
    serializer.WriteValue("", m_exportsOffset);
    serializer.WriteValue("", m_importsOffset);
    serializer.WriteValue("", m_endOffset);

    if (serializer.GetContentsVersion() <= HavokVersion::HK_2013_1_0) return;
    
    serializer.WriteValue("", m_pad);
}

void nemesis::hkPackfileSectionHeader::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    m_sectionTag = deserializer.AssertString(19, {m_sectionTag});
    deserializer.AssertValue(m_nullByte, {static_cast<char>(0xFF)});
    deserializer.ReadValue("", m_absoluteDataStart);
    deserializer.ReadValue("", m_localFixupsOffset);
    deserializer.ReadValue("", m_globalFixupsOffset);
    deserializer.ReadValue("", m_virtualFixupsOffset);
    deserializer.ReadValue("", m_exportsOffset);
    deserializer.ReadValue("", m_importsOffset);
    deserializer.ReadValue("", m_endOffset);

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_2013_1_0) return;

    for (auto& each : m_pad)
    {
        each = deserializer.AssertValue({0xFFFFFFFF});
    }
}

unsigned int nemesis::hkPackfileSectionHeader::GetHeaderSize(nemesis::HavokVersion version)
{
    return version <= HavokVersion::HK_2013_1_0 ? 48 : 64;
}
