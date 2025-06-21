#include "Havok/hkxLight.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxLight,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxLight::Class; });

nemesis::hkxLight::hkxLight() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxLight::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxLight::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("type", m_type); // offset: 8/16 size: 1/1 align: 1/1
    serializer.Pad(16);                    // offset: 9/17 size: 7/15

    serializer.WriteValue("position", m_position);   // offset: 16/32 size: 16/16 align: 16/16
    serializer.WriteValue("direction", m_direction); // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("color", m_color);         // offset: 48/64 size: 4/4 align: 4/4
    serializer.WriteValue("angle", m_angle);         // offset: 52/68 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("range", m_range);
        serializer.WriteValue("fadeStart", m_fadeStart);
        serializer.WriteValue("fadeEnd", m_fadeEnd);

        if (cur_ver > HavokVersion::HK_2012_1_0)
        {
            serializer.WriteValue("decayRate", m_decayRate);
            serializer.Skip(2);
        }

        serializer.WriteValue("intensity", m_intensity);
        serializer.WriteValue("shadowCaster", m_shadowCaster);
        serializer.Pad(16);
    }
    else
    {
        serializer.Skip(8); // offset: 56/72 size: 8/8
    }

    // class size: 64/80 align: 16/16
}

void nemesis::hkxLight::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("type", m_type); // offset: 8/16 size: 1/1 align: 1/1
    deserializer.Pad(16);                   // offset: 9/17 size: 7/15

    deserializer.ReadValue("position", m_position);   // offset: 16/32 size: 16/16 align: 16/16
    deserializer.ReadValue("direction", m_direction); // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("color", m_color);         // offset: 48/64 size: 4/4 align: 4/4
    deserializer.ReadValue("angle", m_angle);         // offset: 52/68 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("range", m_range);
        deserializer.ReadValue("fadeStart", m_fadeStart);
        deserializer.ReadValue("fadeEnd", m_fadeEnd);

        if (cur_ver > HavokVersion::HK_2012_1_0)
        {
            deserializer.ReadValue("decayRate", m_decayRate);
            deserializer.Skip(2);
        }

        deserializer.ReadValue("intensity", m_intensity);
        deserializer.ReadValue("shadowCaster", m_shadowCaster);
        deserializer.Pad(16);
    }
    else
    {
        deserializer.Skip(8); // offset: 56/72 size: 8/8
    }

    // class size: 64/80 align: 16/16
}
