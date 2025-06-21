#pragma once

#include "Havok/Base/hkbGenerator.h"

namespace nemesis
{
    struct BSIStateManagerModifierBSiStateData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x6b8a15fc,
                                                "BSIStateManagerModifierBSiStateData",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSIStateManagerModifierBSiStateData);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_pStateMachine;
        int m_StateID;
        int m_iStateToSetAs;

    public:
        BSIStateManagerModifierBSiStateData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSIStateManagerModifierBSiStateData::Class;
}
