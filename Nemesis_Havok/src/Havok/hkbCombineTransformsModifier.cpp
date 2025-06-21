#include "Havok/hkbCombineTransformsModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCombineTransformsModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCombineTransformsModifier::Class; });

nemesis::hkbCombineTransformsModifier::hkbCombineTransformsModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbCombineTransformsModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCombineTransformsModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("translationOut", m_translationOut);     // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("rotationOut", m_rotationOut);           // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("leftTranslation", m_leftTranslation);   // offset: 80/112 size: 16/16 align: 16/16
    serializer.WriteValue("leftRotation", m_leftRotation);         // offset: 96/128 size: 16/16 align: 16/16
    serializer.WriteValue("rightTranslation", m_rightTranslation); // offset: 112/144 size: 16/16 align: 16/16
    serializer.WriteValue("rightRotation", m_rightRotation);       // offset: 128/160 size: 16/16 align: 16/16
    serializer.WriteValue("invertLeftTransform",
                          m_invertLeftTransform); // offset: 144/176 size: 1/1 align: 1/1
    serializer.WriteValue("invertRightTransform",
                          m_invertRightTransform);         // offset: 145/177 size: 1/1 align: 1/1
    serializer.WriteValue("invertResult", m_invertResult); // offset: 146/178 size: 1/1 align: 1/1
    serializer.Skip(13);                                   // offset: 147/179 size: 13/13
    // class size: 160/192 align: 16/16
}

void nemesis::hkbCombineTransformsModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("translationOut", m_translationOut);   // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("rotationOut", m_rotationOut);         // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("leftTranslation", m_leftTranslation); // offset: 80/112 size: 16/16 align: 16/16
    deserializer.ReadValue("leftRotation", m_leftRotation);       // offset: 96/128 size: 16/16 align: 16/16
    deserializer.ReadValue("rightTranslation",
                           m_rightTranslation);               // offset: 112/144 size: 16/16 align: 16/16
    deserializer.ReadValue("rightRotation", m_rightRotation); // offset: 128/160 size: 16/16 align: 16/16
    deserializer.ReadValue("invertLeftTransform",
                           m_invertLeftTransform); // offset: 144/176 size: 1/1 align: 1/1
    deserializer.ReadValue("invertRightTransform",
                           m_invertRightTransform);         // offset: 145/177 size: 1/1 align: 1/1
    deserializer.ReadValue("invertResult", m_invertResult); // offset: 146/178 size: 1/1 align: 1/1
    deserializer.Skip(13);                                  // offset: 147/179 size: 13/13
    // class size: 160/192 align: 16/16
}
