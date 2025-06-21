#include "Havok/hkxNode.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxNode,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxNode::Class; });

nemesis::hkxNode::hkxNode() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkxAttributeHolder(Class)
{
}

const nemesis::hkClass* nemesis::hkxNode::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxNode::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkxAttributeHolder::SerializeTo(serializer); // offset: 0/0 size: 20/32 align: 4/8
    serializer.WriteValue("name", m_name);                // offset: 20/32 size: 4/8 align: 4/8
    serializer.WriteObject("object", m_object);           // offset: 24/40 size: 4/8 align: 4/8
    serializer.WriteObject("keyFrames", m_keyFrames);     // offset: 28/48 size: 12/16 align: 4/8
    serializer.WriteObject("children", m_children);       // offset: 40/64 size: 12/16 align: 4/8
    serializer.WriteObject("annotations", m_annotations); // offset: 52/80 size: 12/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("linearKeyFrameHints", m_linearKeyFrameHints);
    }

    serializer.WriteValue("userProperties", m_userProperties); // offset: 64/96 size: 4/8 align: 4/8
    serializer.WriteValue("selected", m_selected);             // offset: 68/104 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("bone", m_bone);
    }

    serializer.Pad(serializer.GetPointerSize()); // offset: 69/105 size: 3/7
    // class size: 72/112 align: 4/8
}

void nemesis::hkxNode::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkxAttributeHolder::DeserializeFrom(deserializer); // offset: 0/0 size: 20/32 align: 4/8
    deserializer.ReadValue("name", m_name);                     // offset: 20/32 size: 4/8 align: 4/8
    deserializer.ReadObject("object", m_object);                // offset: 24/40 size: 4/8 align: 4/8
    deserializer.ReadObject("keyFrames", m_keyFrames);          // offset: 28/48 size: 12/16 align: 4/8
    deserializer.ReadObject("children", m_children);            // offset: 40/64 size: 12/16 align: 4/8
    deserializer.ReadObject("annotations", m_annotations);      // offset: 52/80 size: 12/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("linearKeyFrameHints", m_linearKeyFrameHints);
    }

    deserializer.ReadValue("userProperties", m_userProperties); // offset: 64/96 size: 4/8 align: 4/8
    deserializer.ReadValue("selected", m_selected);             // offset: 68/104 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("bone", m_bone);
    }

    deserializer.Pad(deserializer.GetPointerSize()); // offset: 69/105 size: 3/7
    // class size: 72/112 align: 4/8
}
