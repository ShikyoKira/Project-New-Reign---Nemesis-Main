#include "Havok/hkbAssetBundleStringData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbAssetBundleStringData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbAssetBundleStringData::Class; });

nemesis::hkbAssetBundleStringData::hkbAssetBundleStringData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbAssetBundleStringData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbAssetBundleStringData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("bundleName", m_bundleName);  // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("assetNames", m_assetNames); // offset: 4/8 size: 12/16 align: 4/8
    // class size: 16/24 align: 4/8
}

void nemesis::hkbAssetBundleStringData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("bundleName", m_bundleName);  // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("assetNames", m_assetNames); // offset: 4/8 size: 12/16 align: 4/8
    // class size: 16/24 align: 4/8
}
