#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkaSkeleton.h"
#include "Havok/hkpConstraintInstance.h"
#include "Havok/hkpRigidBody.h"

namespace nemesis
{
    struct hkaRagdollInstance : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x154948e8,
                                                "hkaRagdollInstance",
                                                &nemesis::hkReferencedObject::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaRagdollInstance);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpRigidBody>> m_rigidBodies;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpConstraintInstance>> m_constraints;
        nemesis::hkArray<int> m_boneToRigidBodyMap;
        nemesis::hkRefPtr<nemesis::hkaSkeleton> m_skeleton;

    public:
        hkaRagdollInstance() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaRagdollInstance::Class;
}
