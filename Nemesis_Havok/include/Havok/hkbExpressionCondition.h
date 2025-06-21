#pragma once

#include "Havok/Base/hkbCondition.h"

namespace nemesis
{
    struct hkbExpressionCondition : nemesis::hkbCondition
    {
        static constexpr nemesis::hkClass Class{0x1c3c1045,
                                                "hkbExpressionCondition",
                                                &nemesis::hkbCondition::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbExpressionCondition);

        nemesis::hkStringPtr m_expression;
        nemesis::hkRefPtr<nemesis::HavokObject> m_compiledExpressionSet;

    public:
        hkbExpressionCondition() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbExpressionCondition::Class;
}
