#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbCompiledExpressionSetToken.h"

namespace nemesis
{
    struct hkbCompiledExpressionSet : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x3a7d76cc,
                                                "hkbCompiledExpressionSet",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCompiledExpressionSet);

        nemesis::hkArray<nemesis::hkbCompiledExpressionSetToken> m_rpn;
        nemesis::hkArray<int> m_expressionToRpnIndex;
        char m_numExpressions{};

    public:
        hkbCompiledExpressionSet() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCompiledExpressionSet::Class;
}
