#pragma once

#include "Havok/hkpConstraintChainInstanceAction.h"
#include "Havok/hkpConstraintInstance.h"
#include "Havok/hkpEntity.h"

namespace nemesis
{
    struct hkpConstraintChainInstance : nemesis::hkpConstraintInstance
    {
        static constexpr nemesis::hkClass Class{0x7a490753,
                                                "hkpConstraintChainInstance",
                                                &nemesis::hkpConstraintInstance::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpConstraintChainInstance);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkpEntity>> m_chainedEntities;
        nemesis::hkRefPtr<nemesis::hkpConstraintChainInstanceAction> m_action;
        nemesis::hkUlong m_chainConnectedness;

    public:
        hkpConstraintChainInstance() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintChainInstance::Class;
}
