#include "Havok/hkUiAttribute.h"

REGISTER_HAVOK_POINTER_SOURCE(hkUiAttribute,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkUiAttribute::Class; });

nemesis::hkUiAttribute::hkUiAttribute() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkUiAttribute::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkUiAttribute::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("visible", m_visible); // offset: 0/0 size: 1/1 align: 1/1

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        serializer.WriteValue("editable", m_editable);
    }

    serializer.WriteValue(cur_ver > HavokVersion::HK_2013_3_0 ? "hideCriteria" : "hideInModeler",
                          m_hideInModeler);      // offset: 1/1 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 2/2 size: 2/6

    serializer.WriteValue("label", m_label); // offset: 4/8 size: 4/8 align: 4/8
    serializer.WriteValue("group", m_group); // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("hideBaseClassMembers",
                          m_hideBaseClassMembers);   // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("endGroup", m_endGroup);   // offset: 16/32 size: 1/1 align: 1/1
    serializer.WriteValue("endGroup2", m_endGroup2); // offset: 17/33 size: 1/1 align: 1/1
    serializer.WriteValue("advanced", m_advanced);   // offset: 18/34 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());     // offset: 19/35 size: 1/5
}

void nemesis::hkUiAttribute::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("visible", m_visible); // offset: 0/0 size: 1/1 align: 1/1

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        deserializer.ReadValue("editable", m_editable);
    }

    deserializer.ReadValue(cur_ver > HavokVersion::HK_2013_3_0 ? "hideCriteria" : "hideInModeler",
                           m_hideInModeler);         // offset: 1/1 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 2/2 size: 2/6

    deserializer.ReadValue("label", m_label); // offset: 4/8 size: 4/8 align: 4/8
    deserializer.ReadValue("group", m_group); // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("hideBaseClassMembers",
                           m_hideBaseClassMembers);   // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("endGroup", m_endGroup);   // offset: 16/32 size: 1/1 align: 1/1
    deserializer.ReadValue("endGroup2", m_endGroup2); // offset: 17/33 size: 1/1 align: 1/1
    deserializer.ReadValue("advanced", m_advanced);   // offset: 18/34 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());  // offset: 19/35 size: 1/5
    // class size: 20/40 align: 4/8
}
