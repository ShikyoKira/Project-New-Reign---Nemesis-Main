#pragma once

#include "Havok/Base/hkbGenerator.h"

namespace nemesis
{
    struct hkbReferencePoseGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0x26a5675a,
                                                "hkbReferencePoseGenerator",
                                                &nemesis::hkbGenerator::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbReferencePoseGenerator);

        nemesis::hkRefPtr<nemesis::HavokObject> m_skeleton;

    public:
        hkbReferencePoseGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbReferencePoseGenerator::Class;
}
