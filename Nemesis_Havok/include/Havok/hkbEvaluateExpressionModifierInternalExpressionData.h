#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbEvaluateExpressionModifierInternalExpressionData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xb8686f6b,
                                                "hkbEvaluateExpressionModifierInternalExpressionData",
                                                nullptr,
                                                2,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbEvaluateExpressionModifierInternalExpressionData);

        bool m_raisedEvent;
        bool m_wasTrueInPreviousFrame;

    public:
        hkbEvaluateExpressionModifierInternalExpressionData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEvaluateExpressionModifierInternalExpressionData::Class;
}
