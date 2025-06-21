#pragma once

#include "Havok/Base/hkpShape.h"

namespace nemesis
{
    struct hkpCdBody : virtual nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x54a4b841,
                                                "hkpCdBody",
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
        nemesis::hkRefPtr<nemesis::hkpShape> m_shape;
        unsigned int m_shapeKey;
        nemesis::hkRefPtr<nemesis::HavokObject> m_motion;
        nemesis::hkRefPtr<nemesis::hkpCdBody> m_parent;

    public:
        hkpCdBody(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCdBody::Class;
}
