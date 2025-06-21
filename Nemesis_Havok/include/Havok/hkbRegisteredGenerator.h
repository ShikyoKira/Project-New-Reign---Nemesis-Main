#pragma once

#include "Havok/Base/hkbGenerator.h"

namespace nemesis
{
    struct hkbRegisteredGenerator : nemesis::hkbBindable
    {
        static constexpr nemesis::hkClass Class{0x58b1d082,
                                                "hkbRegisteredGenerator",
                                                &nemesis::hkbBindable::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbRegisteredGenerator);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_generator;
        nemesis::hkVector4 m_relativePosition;
        nemesis::hkVector4 m_relativeDirection;

    public:
        hkbRegisteredGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRegisteredGenerator::Class;
}
