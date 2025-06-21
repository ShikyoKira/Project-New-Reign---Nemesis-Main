#include "Havok/hkpListShapeChildInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpListShapeChildInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpListShapeChildInfo::Class; });

nemesis::hkpListShapeChildInfo::hkpListShapeChildInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpListShapeChildInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpListShapeChildInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("shape", m_shape);                            // offset: 0/0 size: 4/8 align: 16/16
    serializer.WriteValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 4/8 size: 4/4 align: 4/4

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        serializer.WriteSerializeIgnoredValue("shapeInfo", m_shapeInfo);
        serializer.WriteSerializeIgnoredValue("shapeSize", m_shapeSize);
    }
    else
    {
        serializer.WriteSerializeIgnoredValue(
            "shapeSize", static_cast<int>(m_shapeSize)); // offset: 8/12 size: 4/4 align: 4/4
    }

    serializer.WriteSerializeIgnoredValue("numChildShapes",
                                          m_numChildShapes); // offset: 12/16 size: 4/4 align: 4/4
    serializer.Pad(16);                                      // offset: 16/20 size: 0/12
    // class size: 16/32 align: 16/16
}

void nemesis::hkpListShapeChildInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("shape", m_shape); // offset: 0/0 size: 4/8 align: 16/16
    deserializer.ReadValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 4/8 size: 4/4 align: 4/4

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadValue("shapeInfo", m_shapeInfo);
        deserializer.ReadValue("shapeSize", m_shapeSize);
    }
    else
    {
        short byte;
        m_shapeSize = deserializer.ReadValue("shapeSize", byte); // offset: 8/12 size: 4/4 align: 4/4
    }

    deserializer.ReadValue("numChildShapes", m_numChildShapes); // offset: 12/16 size: 4/4 align: 4/4
    deserializer.Pad(16);                                       // offset: 16/20 size: 0/12
    // class size: 16/32 align: 16/16
}
