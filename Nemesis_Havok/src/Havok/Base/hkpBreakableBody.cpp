#include "Havok/Base/hkpBreakableBody.h"

nemesis::hkpBreakableBody::hkpBreakableBody(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

void nemesis::hkpBreakableBody::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        serializer.WriteObject("controller", m_controller);
        serializer.WriteObject("breakableShape", m_breakableShape);
        serializer.WriteValue("bodyTypeAndFlags", m_bodyTypeAndFlags);
        serializer.Skip(1);

        serializer.WriteValue("constraintStrength", m_constraintStrength);
        serializer.Pad(serializer.GetPointerSize());
    }

    // class size: 8/16 align: 4/8
}

void nemesis::hkpBreakableBody::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadObject("controller", m_controller);
        deserializer.ReadObject("breakableShape", m_breakableShape);
        deserializer.ReadValue("bodyTypeAndFlags", m_bodyTypeAndFlags);
        deserializer.Skip(1);

        deserializer.ReadValue("constraintStrength", m_constraintStrength);
        deserializer.Pad(deserializer.GetPointerSize());
    }

    // class size: 8/16 align: 4/8
}
