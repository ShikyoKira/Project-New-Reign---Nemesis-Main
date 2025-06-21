#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/BSIStateManagerModifierBSIStateManagerStateListener.h"
#include "Havok/BSIStateManagerModifierBSiStateData.h"

namespace nemesis
{
    struct BSIStateManagerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x6cb24f2e,
                                                "BSIStateManagerModifier",
                                                &nemesis::hkbModifier::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSIStateManagerModifier);

        int m_iStateVar;
        nemesis::hkArray<nemesis::BSIStateManagerModifierBSiStateData> m_stateData;
        nemesis::BSIStateManagerModifierBSIStateManagerStateListener m_myStateListener;

    public:
        BSIStateManagerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSIStateManagerModifier::Class;
}
