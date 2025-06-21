#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkFourTransposedPoints : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xce97c03f,
                                                "hkFourTransposedPoints",
                                                nullptr,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkFourTransposedPoints);

        std::array<nemesis::hkVector4, 3> m_vertices;

    public:
        hkFourTransposedPoints() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkFourTransposedPoints::Class;
}
