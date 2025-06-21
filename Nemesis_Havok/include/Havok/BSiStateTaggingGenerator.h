#pragma once

#include "Havok/Base/hkbGenerator.h"

namespace nemesis
{
    struct BSiStateTaggingGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0xf0826fc1,
                                                "BSiStateTaggingGenerator",
                                                &nemesis::hkbGenerator::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(BSiStateTaggingGenerator);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_pDefaultGenerator; // align 16
        int m_iStateToSetAs;
        int m_iPriority;

    public:
        BSiStateTaggingGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSiStateTaggingGenerator::Class;
}
