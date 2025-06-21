#include "Havok/hkbHandIkControlData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbHandIkControlData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbHandIkControlData::Class; });

nemesis::hkbHandIkControlData::hkbHandIkControlData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbHandIkControlData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbHandIkControlData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("targetPosition", m_targetPosition); // offset: 0/0 size: 16/16 align: 16/16
    serializer.WriteValue("targetRotation", m_targetRotation); // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("targetNormal", m_targetNormal);     // offset: 32/32 size: 16/16 align: 16/16

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("targetHandle", m_targetHandle); // offset: 48/48 size: 4/8 align: 4/8
        serializer.WriteValue("transformOnFraction",
                              m_transformOnFraction); // offset: 52/56 size: 4/4 align: 4/4
    }
    else
    {
        serializer.WriteValue("positionOnFraction", m_transformOnFraction);
    }

    serializer.WriteValue("normalOnFraction", m_normalOnFraction); // offset: 56/60 size: 4/4 align: 4/4
    serializer.WriteValue("fadeInDuration", m_fadeInDuration);     // offset: 60/64 size: 4/4 align: 4/4
    serializer.WriteValue("fadeOutDuration", m_fadeOutDuration);   // offset: 64/68 size: 4/4 align: 4/4

    if (cur_ver >= HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("extrapolationTimeStep",
                              m_extrapolationTimeStep);                  // offset: 68/72 size: 4/4 align: 4/4
        serializer.WriteValue("handleChangeSpeed", m_handleChangeSpeed); // offset: 72/76 size: 4/4 align: 4/4
        serializer.WriteValue("handleChangeMode", m_handleChangeMode);   // offset: 76/80 size: 1/1 align: 1/1
        serializer.WriteValue("fixUp", m_fixUp);                         // offset: 77/81 size: 1/1 align: 1/1
    }

    serializer.Pad(16); // offset: 78/82 size: 2/14
    // class size: 80/96 align: 16/16
}

void nemesis::hkbHandIkControlData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("targetPosition", m_targetPosition); // offset: 0/0 size: 16/16 align: 16/16
    deserializer.ReadValue("targetRotation", m_targetRotation); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("targetNormal", m_targetNormal);     // offset: 32/32 size: 16/16 align: 16/16

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("targetHandle", m_targetHandle); // offset: 48/48 size: 4/8 align: 4/8
        deserializer.ReadValue("transformOnFraction",
                               m_transformOnFraction); // offset: 52/56 size: 4/4 align: 4/4
    }
    else
    {
        deserializer.ReadValue("positionOnFraction", m_transformOnFraction);
    }

    deserializer.ReadValue("normalOnFraction", m_normalOnFraction); // offset: 56/60 size: 4/4 align: 4/4
    deserializer.ReadValue("fadeInDuration", m_fadeInDuration);     // offset: 60/64 size: 4/4 align: 4/4
    deserializer.ReadValue("fadeOutDuration", m_fadeOutDuration);   // offset: 64/68 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("extrapolationTimeStep",
                               m_extrapolationTimeStep); // offset: 68/72 size: 4/4 align: 4/4
        deserializer.ReadValue("handleChangeSpeed",
                               m_handleChangeSpeed);                    // offset: 72/76 size: 4/4 align: 4/4
        deserializer.ReadValue("handleChangeMode", m_handleChangeMode); // offset: 76/80 size: 1/1 align: 1/1
        deserializer.ReadValue("fixUp", m_fixUp);                       // offset: 77/81 size: 1/1 align: 1/1
    }

    deserializer.Pad(16); // offset: 78/82 size: 2/14
    // class size: 80/96 align: 16/16
}
