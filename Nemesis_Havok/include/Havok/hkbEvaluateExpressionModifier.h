#pragma once

#include "Havok/Base/hkbModifier.h"

#include "Havok/hkbCompiledExpressionSet.h"
#include "Havok/hkbExpressionDataArray.h"

namespace nemesis
{
    struct hkbEvaluateExpressionModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xf900f6be,
                                                "hkbEvaluateExpressionModifier",
                                                &nemesis::hkbModifier::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                1};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbEvaluateExpressionModifier);

        nemesis::hkRefPtr<nemesis::hkbExpressionDataArray> m_expressions;
        nemesis::hkRefPtr<nemesis::hkbCompiledExpressionSet> m_compiledExpressionSet;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_internalExpressionsData;

    public:
        hkbEvaluateExpressionModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEvaluateExpressionModifier::Class;
}
