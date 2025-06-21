#include "Havok/hkbFootIkDriverInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkDriverInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkDriverInfo::Class; });

nemesis::hkbFootIkDriverInfo::hkbFootIkDriverInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbFootIkDriverInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkDriverInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);            // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("legs", m_legs);                          // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteValue("raycastDistanceUp", m_raycastDistanceUp); // offset: 20/32 size: 4/4 align: 4/4
    serializer.WriteValue("raycastDistanceDown", m_raycastDistanceDown); // offset: 24/36 size: 4/4 align: 4/4
    serializer.WriteValue("originalGroundHeightMS",
                          m_originalGroundHeightMS);                     // offset: 28/40 size: 4/4 align: 4/4
    serializer.WriteValue("verticalOffset", m_verticalOffset);           // offset: 32/44 size: 4/4 align: 4/4
    serializer.WriteValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 36/48 size: 4/4 align: 4/4
    serializer.WriteValue("forwardAlignFraction",
                          m_forwardAlignFraction); // offset: 40/52 size: 4/4 align: 4/4
    serializer.WriteValue("sidewaysAlignFraction",
                          m_sidewaysAlignFraction);                      // offset: 44/56 size: 4/4 align: 4/4
    serializer.WriteValue("sidewaysSampleWidth", m_sidewaysSampleWidth); // offset: 48/60 size: 4/4 align: 4/4
    serializer.WriteValue("lockFeetWhenPlanted", m_lockFeetWhenPlanted); // offset: 52/64 size: 1/1 align: 1/1
    serializer.WriteValue("useCharacterUpVector",
                          m_useCharacterUpVector);                   // offset: 53/65 size: 1/1 align: 1/1
    serializer.WriteValue("isQuadrupedNarrow", m_isQuadrupedNarrow); // offset: 54/66 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        serializer.WriteValue("keepSourceFootEndAboveGround", m_keepSourceFootEndAboveGround);
    }

    serializer.Pad(serializer.GetPointerSize()); // offset: 55/67 size: 1/5
    // class size: 56/72 align: 4/8
}

void nemesis::hkbFootIkDriverInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);       // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("legs", m_legs);                          // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadValue("raycastDistanceUp", m_raycastDistanceUp); // offset: 20/32 size: 4/4 align: 4/4
    deserializer.ReadValue("raycastDistanceDown",
                           m_raycastDistanceDown); // offset: 24/36 size: 4/4 align: 4/4
    deserializer.ReadValue("originalGroundHeightMS",
                           m_originalGroundHeightMS);           // offset: 28/40 size: 4/4 align: 4/4
    deserializer.ReadValue("verticalOffset", m_verticalOffset); // offset: 32/44 size: 4/4 align: 4/4
    deserializer.ReadValue("collisionFilterInfo",
                           m_collisionFilterInfo); // offset: 36/48 size: 4/4 align: 4/4
    deserializer.ReadValue("forwardAlignFraction",
                           m_forwardAlignFraction); // offset: 40/52 size: 4/4 align: 4/4
    deserializer.ReadValue("sidewaysAlignFraction",
                           m_sidewaysAlignFraction); // offset: 44/56 size: 4/4 align: 4/4
    deserializer.ReadValue("sidewaysSampleWidth",
                           m_sidewaysSampleWidth); // offset: 48/60 size: 4/4 align: 4/4
    deserializer.ReadValue("lockFeetWhenPlanted",
                           m_lockFeetWhenPlanted); // offset: 52/64 size: 1/1 align: 1/1
    deserializer.ReadValue("useCharacterUpVector",
                           m_useCharacterUpVector);                   // offset: 53/65 size: 1/1 align: 1/1
    deserializer.ReadValue("isQuadrupedNarrow", m_isQuadrupedNarrow); // offset: 54/66 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        deserializer.ReadValue("keepSourceFootEndAboveGround", m_keepSourceFootEndAboveGround);
    }

    deserializer.Pad(deserializer.GetPointerSize()); // offset: 55/67 size: 1/5
    // class size: 56/72 align: 4/8
}
