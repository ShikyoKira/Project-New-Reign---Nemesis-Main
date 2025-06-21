#pragma once

#include "Havok/Base/hkpAction.h"

namespace nemesis
{
    struct hkpConstraintChainInstance;

    struct hkpConstraintChainInstanceAction : nemesis::hkpAction
    {
        static constexpr nemesis::hkClass Class{0xc3971189,
                                                "hkpConstraintChainInstanceAction",
                                                &nemesis::hkpAction::Class,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpConstraintChainInstanceAction);

        nemesis::hkpConstraintChainInstance* m_constraintInstance;

    public:
        hkpConstraintChainInstanceAction() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintChainInstanceAction::Class;
}
