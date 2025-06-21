#pragma once

#include "Havok/Base/hkpConvexTransformShapeBase.h"

namespace nemesis
{
    struct hkpConvexTranslateShape : nemesis::hkpConvexTransformShapeBase
    {
        static constexpr nemesis::hkClass Class{0x5ba0a5f7,
                                                "hkpConvexTranslateShape",
                                                &nemesis::hkpConvexTransformShapeBase::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpConvexTranslateShape);

        nemesis::hkVector4 m_translation;

    public:
        hkpConvexTranslateShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexTranslateShape::Class;
}
