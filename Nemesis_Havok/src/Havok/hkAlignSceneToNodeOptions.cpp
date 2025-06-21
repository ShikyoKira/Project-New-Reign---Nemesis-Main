#include "Havok/hkAlignSceneToNodeOptions.h"

REGISTER_HAVOK_POINTER_SOURCE(hkAlignSceneToNodeOptions,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkAlignSceneToNodeOptions::Class; });

nemesis::hkAlignSceneToNodeOptions::hkAlignSceneToNodeOptions() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkAlignSceneToNodeOptions::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkAlignSceneToNodeOptions::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("invert", m_invert);                         // offset: 8/16 size: 1/1 align: 1/1
    serializer.WriteValue("transformPositionX", m_transformPositionX); // offset: 9/17 size: 1/1 align: 1/1
    serializer.WriteValue("transformPositionY", m_transformPositionY); // offset: 10/18 size: 1/1 align: 1/1
    serializer.WriteValue("transformPositionZ", m_transformPositionZ); // offset: 11/19 size: 1/1 align: 1/1
    serializer.WriteValue("transformRotation", m_transformRotation);   // offset: 12/20 size: 1/1 align: 1/1
    serializer.WriteValue("transformScale", m_transformScale);         // offset: 13/21 size: 1/1 align: 1/1
    serializer.WriteValue("transformSkew", m_transformSkew);           // offset: 14/22 size: 1/1 align: 1/1
    serializer.Skip(1);                                                // offset: 15/23 size: 1/1

    serializer.WriteValue("keyframe", m_keyframe); // offset: 16/24 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());   // offset: 20/28 size: 0/4

    if (cur_ver > HavokVersion::HK_660)
    {
        if (!m_nodeName_0.IsEmpty() && m_nodeName_1.GetValue().empty())
        {
            std::string str;

            for (size_t i = 0; i < m_nodeName_0.GetSize(); i++)
            {
                str.push_back(m_nodeName_0[i]);
            }

            m_nodeName_1.SetValue(str);
        }

        serializer.WriteValue("nodeName", m_nodeName_1); // offset: 20/32 size: 4/8 align: 4/8
    }
    else
    {
        if (!m_nodeName_1.GetValue().empty() && m_nodeName_0.IsEmpty())
        {
            auto& str = m_nodeName_1.GetValue();

            for (size_t i = 0; i < str.length(); i++)
            {
                m_nodeName_0.PushBack(new char(str[i]));
            }
        }

        serializer.WriteObject("nodeName", m_nodeName_0);
    }

    // class size: 24/40 align: 4/8
}

void nemesis::hkAlignSceneToNodeOptions::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("invert", m_invert);                         // offset: 8/16 size: 1/1 align: 1/1
    deserializer.ReadValue("transformPositionX", m_transformPositionX); // offset: 9/17 size: 1/1 align: 1/1
    deserializer.ReadValue("transformPositionY", m_transformPositionY); // offset: 10/18 size: 1/1 align: 1/1
    deserializer.ReadValue("transformPositionZ", m_transformPositionZ); // offset: 11/19 size: 1/1 align: 1/1
    deserializer.ReadValue("transformRotation", m_transformRotation);   // offset: 12/20 size: 1/1 align: 1/1
    deserializer.ReadValue("transformScale", m_transformScale);         // offset: 13/21 size: 1/1 align: 1/1
    deserializer.ReadValue("transformSkew", m_transformSkew);           // offset: 14/22 size: 1/1 align: 1/1
    deserializer.Skip(1);                                               // offset: 15/23 size: 1/1

    deserializer.ReadValue("keyframe", m_keyframe);  // offset: 16/24 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 20/28 size: 0/4

    if (cur_ver > HavokVersion::HK_660)
    {
        deserializer.ReadValue("nodeName", m_nodeName_1); // offset: 20/32 size: 4/8 align: 4/8
    }
    else
    {
        deserializer.ReadObject("nodeName", m_nodeName_0);
    }

    // class size: 24/40 align: 4/8
}
