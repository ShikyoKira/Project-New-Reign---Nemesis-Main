#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbEvaluateExpressionModifierInternalExpressionData.h"

namespace nemesis
{
    struct hkbEvaluateExpressionModifierInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xb414d58e,
                                                "hkbEvaluateExpressionModifierInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbEvaluateExpressionModifierInternalState);

        nemesis::hkArray<nemesis::hkbEvaluateExpressionModifierInternalExpressionData> m_internalExpressionsData;

    public:
        hkbEvaluateExpressionModifierInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEvaluateExpressionModifierInternalState::Class;
}
