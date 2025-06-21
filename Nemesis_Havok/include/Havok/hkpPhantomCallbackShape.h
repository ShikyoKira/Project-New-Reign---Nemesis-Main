#pragma once

#include "Havok/Base/hkpShape.h"

namespace nemesis
{
    struct hkpPhantomCallbackShape : nemesis::hkpShape
    {
        static constexpr nemesis::hkClass Class{0xe7eca7eb,
                                                "hkpPhantomCallbackShape",
                                                &nemesis::hkpShape::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpPhantomCallbackShape);

    public:
        hkpPhantomCallbackShape() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPhantomCallbackShape::Class;
}
