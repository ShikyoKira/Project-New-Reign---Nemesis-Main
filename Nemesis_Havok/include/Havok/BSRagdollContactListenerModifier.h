#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbBoneIndexArray.h"
#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSRagdollContactListenerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x8003d8ce,
                                                "BSRagdollContactListenerModifier",
                                                &nemesis::hkbModifier::Class,
                                                136,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSRagdollContactListenerModifier);

        nemesis::hkbEventProperty m_contactEvent;
        nemesis::hkRefPtr<nemesis::hkbBoneIndexArray> m_bones;
        bool m_throwEvent{};
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkReferencedObject>> m_ragdollRigidBodies;

    public:
        BSRagdollContactListenerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSRagdollContactListenerModifier::Class;
}
