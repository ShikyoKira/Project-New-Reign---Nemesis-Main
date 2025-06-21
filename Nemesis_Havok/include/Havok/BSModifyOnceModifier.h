#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSModifyOnceModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x1e20a97a,
                                                "BSModifyOnceModifier",
                                                &nemesis::hkbModifier::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSModifyOnceModifier);

        nemesis::hkRefPtr<nemesis::hkbModifier> m_pOnActivateModifier;   // align 16
        nemesis::hkRefPtr<nemesis::hkbModifier> m_pOnDeactivateModifier; // align 16

    public:
        BSModifyOnceModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSModifyOnceModifier::Class;
}
