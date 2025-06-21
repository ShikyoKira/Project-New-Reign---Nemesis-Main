#include "Havok/hkbFootIkControlsModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkControlsModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkControlsModifier::Class; });

nemesis::hkbFootIkControlsModifier::hkbFootIkControlsModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbFootIkControlsModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkControlsModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteObject("controlData", m_controlData); // offset: 48/80 size: 48/48 align: 16/16

    if (serializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        serializer.WriteObject("legs", m_legs); // offset: 96/128 size: 12/16 align: 4/8
        serializer.Pad(16);                     // offset: 108/144 size: 4/0
    }

    serializer.WriteValue("errorOutTranslation",
                          m_errorOutTranslation); // offset: 112/144 size: 16/16 align: 16/16
    serializer.WriteValue("alignWithGroundRotation",
                          m_alignWithGroundRotation); // offset: 128/160 size: 16/16 align: 16/16
    // class size: 144/176 align: 16/16
}

void nemesis::hkbFootIkControlsModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadObject("controlData", m_controlData); // offset: 48/80 size: 48/48 align: 16/16

    if (deserializer.GetContentsVersion() > HavokVersion::HK_710)
    {
        deserializer.ReadObject("legs", m_legs); // offset: 96/128 size: 12/16 align: 4/8
        deserializer.Pad(16);                    // offset: 108/144 size: 4/0
    }

    deserializer.ReadValue("errorOutTranslation",
                           m_errorOutTranslation); // offset: 112/144 size: 16/16 align: 16/16
    deserializer.ReadValue("alignWithGroundRotation",
                           m_alignWithGroundRotation); // offset: 128/160 size: 16/16 align: 16/16
    // class size: 144/176 align: 16/16
}
