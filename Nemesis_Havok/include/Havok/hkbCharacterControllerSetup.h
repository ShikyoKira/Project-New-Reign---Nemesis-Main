#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbRigidBodySetup.h"

namespace nemesis
{
    struct hkbCharacterControllerSetup : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xaf5f7339,
                                                "hkbCharacterControllerSetup",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterControllerSetup);

        nemesis::hkbRigidBodySetup m_rigidBodySetup;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_controllerCinfo;

    public:
        hkbCharacterControllerSetup() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterControllerSetup::Class;
}
