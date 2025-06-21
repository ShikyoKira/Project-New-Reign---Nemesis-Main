#include "Havok/hkpDisplayBindingDataRigidBody.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpDisplayBindingDataRigidBody,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpDisplayBindingDataRigidBody::Class; });

nemesis::hkpDisplayBindingDataRigidBody::hkpDisplayBindingDataRigidBody() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpDisplayBindingDataRigidBody::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpDisplayBindingDataRigidBody::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("rigidBody", m_rigidBody);     // offset: 8/16 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("displayObjectPtr", m_displayObjectPtr); // offset: 12/24 size: 4/8 align: 4/8
    }
    else
    {
        serializer.WriteObject("displayObject", m_displayObject);
    }

    serializer.WriteValue("rigidBodyFromDisplayObjectTransform",
                          m_rigidBodyFromDisplayObjectTransform); // offset: 16/32 size: 64/64 align: 16/16
    // class size: 80/96 align: 16/16
}

void nemesis::hkpDisplayBindingDataRigidBody::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("rigidBody", m_rigidBody);          // offset: 8/16 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("displayObjectPtr", m_displayObjectPtr); // offset: 12/24 size: 4/8 align: 4/8
    }
    else
    {
        deserializer.ReadObject("displayObject", m_displayObject);
    }

    deserializer.ReadValue("rigidBodyFromDisplayObjectTransform",
                           m_rigidBodyFromDisplayObjectTransform); // offset: 16/32 size: 64/64 align: 16/16
    // class size: 80/96 align: 16/16
}
