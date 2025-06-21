#pragma once

#include "Havok/Base/hkpConvexShape.h"

#include "Havok/hkpSingleShapeContainer.h"

namespace nemesis
{
    struct hkpConvexTransformShapeBase : nemesis::hkpConvexShape
    {
        static constexpr nemesis::hkClass Class{0xfbd72f9,
                                                "hkpConvexTransformShapeBase",
                                                &nemesis::hkpConvexShape::Class,
                                                64,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        nemesis::hkpSingleShapeContainer m_childShape;
        int m_childShapeSize;

    public:
        hkpConvexTransformShapeBase(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConvexTransformShapeBase::Class;
}
