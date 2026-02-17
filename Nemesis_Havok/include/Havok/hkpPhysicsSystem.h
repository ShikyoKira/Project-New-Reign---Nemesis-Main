#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkpConstraintInstance.h"
#include "Havok/hkpRigidBody.h"

#include "Havok/Base/hkpAction.h"
#include "Havok/Base/hkpPhantom.h"

namespace nemesis
{
    struct hkpPhysicsSystem : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xff724c17,
                                                "hkpPhysicsSystem",
                                                &nemesis::hkReferencedObject::Class,
                                                104,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPhysicsSystem);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpRigidBody>> m_rigidBodies;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpConstraintInstance>> m_constraints;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpAction>> m_actions;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpPhantom>> m_phantoms;
        nemesis::hkStringPtr m_name;
        nemesis::hkUlong m_userData;
        bool m_active{};

    public:
        hkpPhysicsSystem() noexcept;
        hkpPhysicsSystem(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPhysicsSystem::Class;
}
