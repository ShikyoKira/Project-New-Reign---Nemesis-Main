#include "Havok/hkbFootIkModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkModifier::Class; });

nemesis::hkbFootIkModifier::hkbFootIkModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbFootIkModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                   // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("gains", m_gains);                        // offset: 44/80 size: 48/48 align: 4/4
    serializer.WriteObject("legs", m_legs);                          // offset: 92/128 size: 12/16 align: 4/8
    serializer.WriteValue("raycastDistanceUp", m_raycastDistanceUp); // offset: 104/144 size: 4/4 align: 4/4
    serializer.WriteValue("raycastDistanceDown",
                          m_raycastDistanceDown); // offset: 108/148 size: 4/4 align: 4/4
    serializer.WriteValue("originalGroundHeightMS",
                          m_originalGroundHeightMS); // offset: 112/152 size: 4/4 align: 4/4
    serializer.WriteValue("errorOut", m_errorOut);   // offset: 116/156 size: 4/4 align: 4/4
    serializer.Pad(16);                              // offset: 120/160 size: 8/0

    serializer.WriteValue("errorOutTranslation",
                          m_errorOutTranslation); // offset: 128/160 size: 16/16 align: 16/16
    serializer.WriteValue("alignWithGroundRotation",
                          m_alignWithGroundRotation);          // offset: 144/176 size: 16/16 align: 16/16
    serializer.WriteValue("verticalOffset", m_verticalOffset); // offset: 160/192 size: 4/4 align: 4/4
    serializer.WriteValue("collisionFilterInfo",
                          m_collisionFilterInfo); // offset: 164/196 size: 4/4 align: 4/4
    serializer.WriteValue("forwardAlignFraction",
                          m_forwardAlignFraction); // offset: 168/200 size: 4/4 align: 4/4
    serializer.WriteValue("sidewaysAlignFraction",
                          m_sidewaysAlignFraction); // offset: 172/204 size: 4/4 align: 4/4
    serializer.WriteValue("sidewaysSampleWidth",
                          m_sidewaysSampleWidth);          // offset: 176/208 size: 4/4 align: 4/4
    serializer.WriteValue("useTrackData", m_useTrackData); // offset: 180/212 size: 1/1 align: 1/1
    serializer.WriteValue("lockFeetWhenPlanted",
                          m_lockFeetWhenPlanted); // offset: 181/213 size: 1/1 align: 1/1
    serializer.WriteValue("useCharacterUpVector",
                          m_useCharacterUpVector); // offset: 182/214 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        serializer.WriteValue("keepSourceFootEndAboveGround", m_keepSourceFootEndAboveGround);
    }

    serializer.WriteValue("alignMode", m_alignMode); // offset: 183/215 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredObject("internalLegData",
                                           m_internalLegData); // offset: 184/216 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("prevIsFootIkEnabled",
                                          m_prevIsFootIkEnabled); // offset: 196/232 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("isSetUp", m_isSetUp);  // offset: 200/236 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("isGroundPositionValid",
                                          m_isGroundPositionValid); // offset: 201/237 size: 1/1 align: 1/1
    serializer.Pad(4);                                              // offset: 202/238 size: 2/2

    serializer.WriteSerializeIgnoredValue("timeStep", m_timeStep); // offset: 204/240 size: 4/4 align: 4/4
    serializer.Pad(16);                                            // offset: 208/244 size: 0/12
    // class size: 208/256 align: 16/16
}

void nemesis::hkbFootIkModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);              // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("gains", m_gains);                        // offset: 44/80 size: 48/48 align: 4/4
    deserializer.ReadObject("legs", m_legs);                          // offset: 92/128 size: 12/16 align: 4/8
    deserializer.ReadValue("raycastDistanceUp", m_raycastDistanceUp); // offset: 104/144 size: 4/4 align: 4/4
    deserializer.ReadValue("raycastDistanceDown",
                           m_raycastDistanceDown); // offset: 108/148 size: 4/4 align: 4/4
    deserializer.ReadValue("originalGroundHeightMS",
                           m_originalGroundHeightMS); // offset: 112/152 size: 4/4 align: 4/4
    deserializer.ReadValue("errorOut", m_errorOut);   // offset: 116/156 size: 4/4 align: 4/4
    deserializer.Pad(16);                             // offset: 120/160 size: 8/0

    deserializer.ReadValue("errorOutTranslation",
                           m_errorOutTranslation); // offset: 128/160 size: 16/16 align: 16/16
    deserializer.ReadValue("alignWithGroundRotation",
                           m_alignWithGroundRotation);          // offset: 144/176 size: 16/16 align: 16/16
    deserializer.ReadValue("verticalOffset", m_verticalOffset); // offset: 160/192 size: 4/4 align: 4/4
    deserializer.ReadValue("collisionFilterInfo",
                           m_collisionFilterInfo); // offset: 164/196 size: 4/4 align: 4/4
    deserializer.ReadValue("forwardAlignFraction",
                           m_forwardAlignFraction); // offset: 168/200 size: 4/4 align: 4/4
    deserializer.ReadValue("sidewaysAlignFraction",
                           m_sidewaysAlignFraction); // offset: 172/204 size: 4/4 align: 4/4
    deserializer.ReadValue("sidewaysSampleWidth",
                           m_sidewaysSampleWidth);          // offset: 176/208 size: 4/4 align: 4/4
    deserializer.ReadValue("useTrackData", m_useTrackData); // offset: 180/212 size: 1/1 align: 1/1
    deserializer.ReadValue("lockFeetWhenPlanted",
                           m_lockFeetWhenPlanted); // offset: 181/213 size: 1/1 align: 1/1
    deserializer.ReadValue("useCharacterUpVector",
                           m_useCharacterUpVector); // offset: 182/214 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        deserializer.ReadValue("keepSourceFootEndAboveGround", m_keepSourceFootEndAboveGround);
    }

    deserializer.ReadValue("alignMode", m_alignMode);              // offset: 183/215 size: 1/1 align: 1/1
    deserializer.ReadObject("internalLegData", m_internalLegData); // offset: 184/216 size: 12/16 align: 4/8
    deserializer.ReadValue("prevIsFootIkEnabled",
                           m_prevIsFootIkEnabled); // offset: 196/232 size: 4/4 align: 4/4
    deserializer.ReadValue("isSetUp", m_isSetUp);  // offset: 200/236 size: 1/1 align: 1/1
    deserializer.ReadValue("isGroundPositionValid",
                           m_isGroundPositionValid); // offset: 201/237 size: 1/1 align: 1/1
    deserializer.Pad(4);                             // offset: 202/238 size: 2/2

    deserializer.ReadValue("timeStep", m_timeStep); // offset: 204/240 size: 4/4 align: 4/4
    deserializer.Pad(16);                           // offset: 208/244 size: 0/12
    // class size: 208/256 align: 16/16
}
