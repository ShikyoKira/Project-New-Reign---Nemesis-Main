#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkpDisplayBindingDataRigidBody.h"
#include "Havok/hkpPhysicsSystem.h"

namespace nemesis
{
    struct hkpDisplayBindingDataPhysicsSystem : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc8ae86a7,
                                                "hkpDisplayBindingDataPhysicsSystem",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpDisplayBindingDataPhysicsSystem);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpDisplayBindingDataRigidBody>> m_bindings;
        nemesis::hkRefPtr<nemesis::hkpPhysicsSystem> m_system;

    public:
        hkpDisplayBindingDataPhysicsSystem() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpDisplayBindingDataPhysicsSystem::Class;
}
