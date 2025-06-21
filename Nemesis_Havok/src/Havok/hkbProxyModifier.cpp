#include "Havok/hkbProxyModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbProxyModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbProxyModifier::Class; });

nemesis::hkbProxyModifier::hkbProxyModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbProxyModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbProxyModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16); // offset: 44/80 size: 4/0

    serializer.WriteObject("proxyInfo", m_proxyInfo);          // offset: 48/80 size: 80/80 align: 16/16
    serializer.WriteValue("linearVelocity", m_linearVelocity); // offset: 128/160 size: 16/16 align: 16/16
    serializer.WriteValue("horizontalGain", m_horizontalGain); // offset: 144/176 size: 4/4 align: 4/4
    serializer.WriteValue("verticalGain", m_verticalGain);     // offset: 148/180 size: 4/4 align: 4/4
    serializer.WriteValue("maxHorizontalSeparation",
                          m_maxHorizontalSeparation); // offset: 152/184 size: 4/4 align: 4/4
    serializer.WriteValue("maxVerticalSeparation",
                          m_maxVerticalSeparation); // offset: 156/188 size: 4/4 align: 4/4
    serializer.WriteValue("verticalDisplacementError",
                          m_verticalDisplacementError); // offset: 160/192 size: 4/4 align: 4/4
    serializer.WriteValue("verticalDisplacementErrorGain",
                          m_verticalDisplacementErrorGain); // offset: 164/196 size: 4/4 align: 4/4
    serializer.WriteValue("maxVerticalDisplacement",
                          m_maxVerticalDisplacement); // offset: 168/200 size: 4/4 align: 4/4
    serializer.WriteValue("minVerticalDisplacement",
                          m_minVerticalDisplacement);        // offset: 172/204 size: 4/4 align: 4/4
    serializer.WriteValue("capsuleHeight", m_capsuleHeight); // offset: 176/208 size: 4/4 align: 4/4
    serializer.WriteValue("capsuleRadius", m_capsuleRadius); // offset: 180/212 size: 4/4 align: 4/4
    serializer.WriteValue("maxSlopeForRotation",
                          m_maxSlopeForRotation); // offset: 184/216 size: 4/4 align: 4/4
    serializer.WriteValue("collisionFilterInfo",
                          m_collisionFilterInfo);        // offset: 188/220 size: 4/4 align: 4/4
    serializer.WriteValue("phantomType", m_phantomType); // offset: 192/224 size: 1/1 align: 1/1
    serializer.WriteValue("linearVelocityMode",
                          m_linearVelocityMode); // offset: 193/225 size: 1/1 align: 1/1
    serializer.WriteValue("ignoreIncomingRotation",
                          m_ignoreIncomingRotation); // offset: 194/226 size: 1/1 align: 1/1
    serializer.WriteValue("ignoreCollisionDuringRotation",
                          m_ignoreCollisionDuringRotation); // offset: 195/227 size: 1/1 align: 1/1
    serializer.WriteValue("ignoreIncomingTranslation",
                          m_ignoreIncomingTranslation); // offset: 196/228 size: 1/1 align: 1/1
    serializer.WriteValue("includeDownwardMomentum",
                          m_includeDownwardMomentum); // offset: 197/229 size: 1/1 align: 1/1
    serializer.WriteValue("followWorldFromModel",
                          m_followWorldFromModel);                 // offset: 198/230 size: 1/1 align: 1/1
    serializer.WriteValue("isTouchingGround", m_isTouchingGround); // offset: 199/231 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredObject("characterProxy",
                                           m_characterProxy);     // offset: 200/232 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("phantom", m_phantom); // offset: 204/240 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("phantomShape",
                                           m_phantomShape); // offset: 208/248 size: 4/8 align: 4/8
    serializer.Pad(16);                                     // offset: 212/256 size: 12/0

    serializer.WriteSerializeIgnoredValue(
        "horizontalDisplacement", m_horizontalDisplacement); // offset: 224/256 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredValue("verticalDisplacement",
                                          m_verticalDisplacement); // offset: 240/272 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("timestep", m_timestep); // offset: 244/276 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue(
        "previousFrameFollowWorldFromModel",
        m_previousFrameFollowWorldFromModel); // offset: 248/280 size: 1/1 align: 1/1
    serializer.Skip(7);                       // offset: 249/281 size: 7/7
    // class size: 256/288 align: 16/16
}

void nemesis::hkbProxyModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16); // offset: 44/80 size: 4/0

    deserializer.ReadObject("proxyInfo", m_proxyInfo); // offset: 48/80 size: 80/80 align: 16/16
    deserializer.ReadValue("linearVelocity",
                           m_linearVelocity);                   // offset: 128/160 size: 16/16 align: 16/16
    deserializer.ReadValue("horizontalGain", m_horizontalGain); // offset: 144/176 size: 4/4 align: 4/4
    deserializer.ReadValue("verticalGain", m_verticalGain);     // offset: 148/180 size: 4/4 align: 4/4
    deserializer.ReadValue("maxHorizontalSeparation",
                           m_maxHorizontalSeparation); // offset: 152/184 size: 4/4 align: 4/4
    deserializer.ReadValue("maxVerticalSeparation",
                           m_maxVerticalSeparation); // offset: 156/188 size: 4/4 align: 4/4
    deserializer.ReadValue("verticalDisplacementError",
                           m_verticalDisplacementError); // offset: 160/192 size: 4/4 align: 4/4
    deserializer.ReadValue("verticalDisplacementErrorGain",
                           m_verticalDisplacementErrorGain); // offset: 164/196 size: 4/4 align: 4/4
    deserializer.ReadValue("maxVerticalDisplacement",
                           m_maxVerticalDisplacement); // offset: 168/200 size: 4/4 align: 4/4
    deserializer.ReadValue("minVerticalDisplacement",
                           m_minVerticalDisplacement);        // offset: 172/204 size: 4/4 align: 4/4
    deserializer.ReadValue("capsuleHeight", m_capsuleHeight); // offset: 176/208 size: 4/4 align: 4/4
    deserializer.ReadValue("capsuleRadius", m_capsuleRadius); // offset: 180/212 size: 4/4 align: 4/4
    deserializer.ReadValue("maxSlopeForRotation",
                           m_maxSlopeForRotation); // offset: 184/216 size: 4/4 align: 4/4
    deserializer.ReadValue("collisionFilterInfo",
                           m_collisionFilterInfo);        // offset: 188/220 size: 4/4 align: 4/4
    deserializer.ReadValue("phantomType", m_phantomType); // offset: 192/224 size: 1/1 align: 1/1
    deserializer.ReadValue("linearVelocityMode",
                           m_linearVelocityMode); // offset: 193/225 size: 1/1 align: 1/1
    deserializer.ReadValue("ignoreIncomingRotation",
                           m_ignoreIncomingRotation); // offset: 194/226 size: 1/1 align: 1/1
    deserializer.ReadValue("ignoreCollisionDuringRotation",
                           m_ignoreCollisionDuringRotation); // offset: 195/227 size: 1/1 align: 1/1
    deserializer.ReadValue("ignoreIncomingTranslation",
                           m_ignoreIncomingTranslation); // offset: 196/228 size: 1/1 align: 1/1
    deserializer.ReadValue("includeDownwardMomentum",
                           m_includeDownwardMomentum); // offset: 197/229 size: 1/1 align: 1/1
    deserializer.ReadValue("followWorldFromModel",
                           m_followWorldFromModel); // offset: 198/230 size: 1/1 align: 1/1
    deserializer.ReadValue("isTouchingGround",
                           m_isTouchingGround);                  // offset: 199/231 size: 1/1 align: 1/1
    deserializer.ReadObject("characterProxy", m_characterProxy); // offset: 200/232 size: 4/8 align: 4/8
    deserializer.ReadObject("phantom", m_phantom);               // offset: 204/240 size: 4/8 align: 4/8
    deserializer.ReadObject("phantomShape", m_phantomShape);     // offset: 208/248 size: 4/8 align: 4/8
    deserializer.Pad(16);                                        // offset: 212/256 size: 12/0

    deserializer.ReadValue("horizontalDisplacement",
                           m_horizontalDisplacement); // offset: 224/256 size: 16/16 align: 16/16
    deserializer.ReadValue("verticalDisplacement",
                           m_verticalDisplacement); // offset: 240/272 size: 4/4 align: 4/4
    deserializer.ReadValue("timestep", m_timestep); // offset: 244/276 size: 4/4 align: 4/4
    deserializer.ReadValue("previousFrameFollowWorldFromModel",
                           m_previousFrameFollowWorldFromModel); // offset: 248/280 size: 1/1 align: 1/1
    deserializer.Skip(7);                                        // offset: 249/281 size: 7/7
    // class size: 256/288 align: 16/16
}
