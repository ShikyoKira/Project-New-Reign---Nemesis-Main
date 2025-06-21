#pragma once

#include "Havok/hkpRigidBody.h"
#include "Havok/hkxMesh.h"

namespace nemesis
{
    struct hkpDisplayBindingDataRigidBody : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xfe16e2a3,
                                                "hkpDisplayBindingDataRigidBody",
                                                &nemesis::hkReferencedObject::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpDisplayBindingDataRigidBody);

        nemesis::hkRefPtr<nemesis::hkpRigidBody> m_rigidBody;
        nemesis::hkRefPtr<nemesis::hkxMesh> m_displayObject;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_displayObjectPtr;
        nemesis::hkMatrix4 m_rigidBodyFromDisplayObjectTransform;

    public:
        hkpDisplayBindingDataRigidBody() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpDisplayBindingDataRigidBody::Class;
}
