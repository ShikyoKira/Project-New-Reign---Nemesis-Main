#include "Havok/hkbFootIkControlsModifierLeg.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbFootIkControlsModifierLeg,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbFootIkControlsModifierLeg::Class; });

nemesis::hkbFootIkControlsModifierLeg::hkbFootIkControlsModifierLeg() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbFootIkControlsModifierLeg::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbFootIkControlsModifierLeg::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("groundPosition", m_groundPosition);    // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteObject("ungroundedEvent", m_ungroundedEvent); // offset: 16/16 size: 8/16 align: 4/8
    serializer.WriteValue("verticalError", m_verticalError);      // offset: 24/32 size: 4/4 align: 4/4
    serializer.WriteValue("hitSomething", m_hitSomething);        // offset: 28/36 size: 1/1 align: 1/1
    serializer.WriteValue("isPlantedMS", m_isPlantedMS);          // offset: 29/37 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        serializer.WriteValue("enabled", m_enabled);
    }

    serializer.Pad(16); // offset: 30/38 size: 2/10
    // class size: 32/48 align: 16/16
}

void nemesis::hkbFootIkControlsModifierLeg::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("groundPosition", m_groundPosition);    // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadObject("ungroundedEvent", m_ungroundedEvent); // offset: 16/16 size: 8/16 align: 4/8
    deserializer.ReadValue("verticalError", m_verticalError);      // offset: 24/32 size: 4/4 align: 4/4
    deserializer.ReadValue("hitSomething", m_hitSomething);        // offset: 28/36 size: 1/1 align: 1/1
    deserializer.ReadValue("isPlantedMS", m_isPlantedMS);          // offset: 29/37 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2013_3_0)
    {
        deserializer.ReadValue("enabled", m_enabled);
    }

    deserializer.Pad(16); // offset: 30/38 size: 2/10
    // class size: 32/48 align: 16/16
}
