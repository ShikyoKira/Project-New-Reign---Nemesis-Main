#include "Havok/hkbCharacterSetup.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterSetup,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterSetup::Class; });

nemesis::hkbCharacterSetup::hkbCharacterSetup() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterSetup::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterSetup::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("retargetingSkeletonMappers",
                           m_retargetingSkeletonMappers);             // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("animationSkeleton", m_animationSkeleton); // offset: 20/32 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() <= HavokVersion::HK_710)
    {
        serializer.WriteObject("ragdollSkeleton", m_ragdollSkeleton);
    }

    serializer.WriteObject("ragdollToAnimationSkeletonMapper",
                           m_ragdollToAnimationSkeletonMapper); // offset: 24/40 size: 4/8 align: 4/8
    serializer.WriteObject("animationToRagdollSkeletonMapper",
                           m_animationToRagdollSkeletonMapper); // offset: 28/48 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("animationBindingSet",
                                           m_animationBindingSet); // offset: 32/56 size: 4/8 align: 4/8
    serializer.WriteObject("data", m_data);                        // offset: 36/64 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("mirroredSkeleton",
                                           m_mirroredSkeleton); // offset: 40/72 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("characterPropertyIdMap",
                                           m_characterPropertyIdMap); // offset: 44/80 size: 4/8 align: 4/8
    // class size: 48/88 align: 4/8
}

void nemesis::hkbCharacterSetup::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("retargetingSkeletonMappers",
                            m_retargetingSkeletonMappers);             // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("animationSkeleton", m_animationSkeleton); // offset: 20/32 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() <= HavokVersion::HK_710)
    {
        deserializer.ReadObject("ragdollSkeleton", m_ragdollSkeleton);
    }

    deserializer.ReadObject("ragdollToAnimationSkeletonMapper",
                            m_ragdollToAnimationSkeletonMapper); // offset: 24/40 size: 4/8 align: 4/8
    deserializer.ReadObject("animationToRagdollSkeletonMapper",
                            m_animationToRagdollSkeletonMapper); // offset: 28/48 size: 4/8 align: 4/8
    deserializer.ReadObject("animationBindingSet",
                            m_animationBindingSet);                  // offset: 32/56 size: 4/8 align: 4/8
    deserializer.ReadObject("data", m_data);                         // offset: 36/64 size: 4/8 align: 4/8
    deserializer.ReadObject("mirroredSkeleton", m_mirroredSkeleton); // offset: 40/72 size: 4/8 align: 4/8
    deserializer.ReadObject("characterPropertyIdMap",
                            m_characterPropertyIdMap); // offset: 44/80 size: 4/8 align: 4/8
    // class size: 48/88 align: 4/8
}
