#include "Havok/hkpConvexTransformShape.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConvexTransformShape,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConvexTransformShape::Class; });

nemesis::hkpConvexTransformShape::hkpConvexTransformShape() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConvexTransformShapeBase(Class)
{
}

const nemesis::hkClass* nemesis::hkpConvexTransformShape::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConvexTransformShape::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConvexTransformShapeBase::SerializeTo(serializer); // offset: 0/0 size: 36/64 align: 4/8
    serializer.Pad(16);                                            // offset: 36/64 size: 12/0

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_1_0)
    {
        serializer.WriteValue("transform", m_transform_1);

        if (cur_ver > HavokVersion::HK_2011_2_0)
        {
            serializer.WriteValue("extraScale", m_extraScale);
        }
    }
    else
    {
        serializer.WriteValue("transform", m_transform_0); // offset: 48/64 size: 64/64 align: 16/16
    }

    // class size: 112/128 align: 16/16
}

void nemesis::hkpConvexTransformShape::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConvexTransformShapeBase::DeserializeFrom(deserializer); // offset: 0/0 size: 36/64 align: 4/8
    deserializer.Pad(16);                                                // offset: 36/64 size: 12/0

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadValue("transform", m_transform_1);

        if (cur_ver > HavokVersion::HK_2011_2_0)
        {
            deserializer.ReadValue("extraScale", m_extraScale);
        }
    }
    else
    {
        deserializer.ReadValue("transform", m_transform_0); // offset: 48/64 size: 64/64 align: 16/16
    }

    // class size: 112/128 align: 16/16
}
