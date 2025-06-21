#include "Havok/hkpSimpleContactConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSimpleContactConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSimpleContactConstraintAtom::Class; });

nemesis::hkpSimpleContactConstraintAtom::hkpSimpleContactConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpSimpleContactConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSimpleContactConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer);           // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("sizeOfAllAtoms", m_sizeOfAllAtoms);     // offset: 2/2 size: 2/2 align: 2/2
    serializer.WriteValue("numContactPoints", m_numContactPoints); // offset: 4/4 size: 2/2 align: 2/2
    serializer.WriteValue("numReservedContactPoints",
                          m_numReservedContactPoints);                     // offset: 6/6 size: 2/2 align: 2/2
    serializer.WriteValue("numUserDatasForBodyA", m_numUserDatasForBodyA); // offset: 8/8 size: 1/1 align: 1/1
    serializer.WriteValue("numUserDatasForBodyB", m_numUserDatasForBodyB); // offset: 9/9 size: 1/1 align: 1/1
    serializer.WriteValue("contactPointPropertiesStriding",
                          m_contactPointPropertiesStriding); // offset: 10/10 size: 1/1 align: 1/1
    serializer.Skip(1);                                      // offset: 11/11 size: 1/1

    serializer.WriteValue("maxNumContactPoints", m_maxNumContactPoints); // offset: 12/12 size: 2/2 align: 2/2
    serializer.Skip(2);                                                  // offset: 14/14 size: 2/2

    serializer.WriteObject("info", m_info); // offset: 16/16 size: 32/32 align: 16/16
    // class size: 48/48 align: 16/16
}

void nemesis::hkpSimpleContactConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer);      // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("sizeOfAllAtoms", m_sizeOfAllAtoms);     // offset: 2/2 size: 2/2 align: 2/2
    deserializer.ReadValue("numContactPoints", m_numContactPoints); // offset: 4/4 size: 2/2 align: 2/2
    deserializer.ReadValue("numReservedContactPoints",
                           m_numReservedContactPoints); // offset: 6/6 size: 2/2 align: 2/2
    deserializer.ReadValue("numUserDatasForBodyA",
                           m_numUserDatasForBodyA); // offset: 8/8 size: 1/1 align: 1/1
    deserializer.ReadValue("numUserDatasForBodyB",
                           m_numUserDatasForBodyB); // offset: 9/9 size: 1/1 align: 1/1
    deserializer.ReadValue("contactPointPropertiesStriding",
                           m_contactPointPropertiesStriding); // offset: 10/10 size: 1/1 align: 1/1
    deserializer.Skip(1);                                     // offset: 11/11 size: 1/1

    deserializer.ReadValue("maxNumContactPoints",
                           m_maxNumContactPoints); // offset: 12/12 size: 2/2 align: 2/2
    deserializer.Skip(2);                          // offset: 14/14 size: 2/2

    deserializer.ReadObject("info", m_info); // offset: 16/16 size: 32/32 align: 16/16
    // class size: 48/48 align: 16/16
}
