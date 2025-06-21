#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkArray.h"

#include "Havok/Types/hkCString.h"

namespace nemesis
{
    struct BGSGamebryoSequenceGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0xc8df2d77,
                                                "BGSGamebryoSequenceGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BGSGamebryoSequenceGenerator);

        using BlendModeFunction = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkCString m_pSequence;
        nemesis::hkEnum<BlendModeFunction, char> m_eBlendModeFunction = {
            {"BMF_NONE", 0},
            {"BMF_PERCENT", 1},
            {"BMF_ONE_MINUS_PERCENT", 2},
        };
        float m_fPercent;
        nemesis::hkRefPtr<nemesis::HavokObject> m_events;
        float m_fTime;
        bool m_bDelayedActivate;
        bool m_bLooping;

    public:
        BGSGamebryoSequenceGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BGSGamebryoSequenceGenerator::Class;
}
