#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbExpressionData.h"

namespace nemesis
{
    struct hkbExpressionDataArray : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x4b9ee1a2,
                                                "hkbExpressionDataArray",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbExpressionDataArray);

        nemesis::hkArray<nemesis::hkbExpressionData> m_expressionsData;

    public:
        hkbExpressionDataArray() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbExpressionDataArray::Class;
}
