#include "Havok/hkDataSectionHeader.h"

nemesis::hkDataSectionHeader::hkDataSectionHeader()
{
    m_sectionTag          = "__data__";
    m_nullByte            = 0xff;
    m_absoluteDataStart   = 0x00;
    m_localFixupsOffset   = 0x00;
    m_globalFixupsOffset  = 0x00;
    m_virtualFixupsOffset = 0x00;
    m_exportsOffset       = 0x00;
    m_importsOffset       = 0x00;
    m_endOffset           = 0x00;
    m_pad[0]              = 0xffffffff;
    m_pad[1]              = 0xffffffff;
    m_pad[2]              = 0xffffffff;
    m_pad[3]              = 0xffffffff;
}
