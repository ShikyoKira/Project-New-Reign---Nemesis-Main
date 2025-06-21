#pragma once

#include "Havok/hkbStateListener.h"

namespace nemesis
{
    struct BSIStateManagerModifierBSIStateManagerStateListener : nemesis::hkbStateListener
    {
        static constexpr nemesis::hkClass Class{0x99463586,
                                                "BSIStateManagerModifierBSIStateManagerStateListener",
                                                &nemesis::hkbStateListener::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSIStateManagerModifierBSIStateManagerStateListener);

        nemesis::hkRefPtr<nemesis::HavokObject> m_pStateManager;

    public:
        BSIStateManagerModifierBSIStateManagerStateListener() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSIStateManagerModifierBSIStateManagerStateListener::Class;
}
