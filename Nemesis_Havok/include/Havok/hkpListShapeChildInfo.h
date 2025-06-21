#pragma once

#include "Havok/Base/hkpShape.h"

namespace nemesis
{
    struct hkpListShapeChildInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x80df0f90,
                                                "hkpListShapeChildInfo",
                                                nullptr,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpListShapeChildInfo);

        nemesis::hkRefPtr<nemesis::hkpShape> m_shape; // align 16
        unsigned int m_collisionFilterInfo;
        unsigned short m_shapeInfo;
        short m_shapeSize;
        int m_numChildShapes;

    public:
        hkpListShapeChildInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpListShapeChildInfo::Class;
}
