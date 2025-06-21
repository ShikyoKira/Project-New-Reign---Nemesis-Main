#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkaBone.h"
#include "Havok/hkaSkeletonLocalFrameOnBone.h"
#include "Havok/hkaSkeletonPartition.h"

namespace nemesis
{
    struct hkaSkeleton : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x366e8220,
                                                "hkaSkeleton",
                                                &nemesis::hkReferencedObject::Class,
                                                120,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaSkeleton);

        nemesis::hkStringPtr m_name;
        nemesis::hkArray<short> m_parentIndices;
        nemesis::hkArray<nemesis::hkaBone> m_bones;
        nemesis::hkArray<nemesis::hkQsTransform> m_referencePose;
        nemesis::hkArray<float> m_referenceFloats;
        nemesis::hkArray<nemesis::hkCString> m_floatSlots;
        nemesis::hkArray<nemesis::hkaSkeletonLocalFrameOnBone> m_localFrames;
        nemesis::hkArray<nemesis::hkaSkeletonPartition> m_partitions;

    public:
        hkaSkeleton() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaSkeleton::Class;
}
