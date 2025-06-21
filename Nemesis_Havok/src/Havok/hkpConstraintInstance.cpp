#include "Havok/hkpConstraintInstance.h"
#include "Havok/hkpEntity.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpConstraintInstance,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpConstraintInstance::Class; });

nemesis::hkpConstraintInstance::hkpConstraintInstance() noexcept
    : nemesis::hkpConstraintInstance(Class)
{
}

nemesis::hkpConstraintInstance::hkpConstraintInstance(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkpConstraintInstance::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpConstraintInstance::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkReferencedObject::SerializeTo(serializer);     // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("owner", m_owner); // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("data", m_data);                   // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteObject("constraintModifiers",
                           m_constraintModifiers);       // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteObject("entities", m_entities);      // offset: 20/40 size: 8/16 align: 4/8
    serializer.WriteValue("priority", m_priority);       // offset: 28/56 size: 1/1 align: 1/1
    serializer.WriteValue("wantRuntime", m_wantRuntime); // offset: 29/57 size: 1/1 align: 1/1
    serializer.WriteValue("destructionRemapInfo",
                          m_destructionRemapInfo); // offset: 30/58 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                  // offset: 31/59 size: 1/5

    serializer.WriteSerializeIgnoredObject("listeners", m_listeners); // offset: 32/64 size: 8/16 align: 4/8
    serializer.WriteValue("name", m_name);                            // offset: 40/80 size: 4/8 align: 4/8
    serializer.WriteValue("userData", m_userData);                    // offset: 44/88 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("internal", m_internal);   // offset: 48/96 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue("uid", m_uid);              // offset: 52/104 size: 4/4 align: 4/4
    serializer.Pad(padding_size);                                     // offset: 56/108 size: 0/4
    // class size: 56/112 align: 4/8
}

void nemesis::hkpConstraintInstance::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("owner", m_owner);                  // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("data", m_data);                    // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadObject("constraintModifiers",
                            m_constraintModifiers);       // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadObject("entities", m_entities);      // offset: 20/40 size: 8/16 align: 4/8
    deserializer.ReadValue("priority", m_priority);       // offset: 28/56 size: 1/1 align: 1/1
    deserializer.ReadValue("wantRuntime", m_wantRuntime); // offset: 29/57 size: 1/1 align: 1/1
    deserializer.ReadValue("destructionRemapInfo",
                           m_destructionRemapInfo); // offset: 30/58 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                 // offset: 31/59 size: 1/5

    deserializer.ReadObject("listeners", m_listeners); // offset: 32/64 size: 8/16 align: 4/8
    deserializer.ReadValue("name", m_name);            // offset: 40/80 size: 4/8 align: 4/8
    deserializer.ReadValue("userData", m_userData);    // offset: 44/88 size: 4/8 align: 4/8
    deserializer.ReadObject("internal", m_internal);   // offset: 48/96 size: 4/8 align: 4/8
    deserializer.ReadValue("uid", m_uid);              // offset: 52/104 size: 4/4 align: 4/4
    deserializer.Pad(padding_size);                    // offset: 56/108 size: 0/4
    // class size: 56/112 align: 4/8
}
