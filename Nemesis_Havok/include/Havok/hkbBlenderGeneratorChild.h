#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkbBoneWeightArray.h"

namespace nemesis
{
    struct hkbBlenderGeneratorChild : nemesis::hkbBindable
    {
        static constexpr nemesis::hkClass Class{0xe2b384b0,
                                                "hkbBlenderGeneratorChild",
                                                &nemesis::hkbBindable::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbBlenderGeneratorChild);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_generator; // align 16
        nemesis::hkRefPtr<nemesis::hkbBoneWeightArray> m_boneWeights;
        float m_weight{};
        float m_worldFromModelWeight{};

    public:
        hkbBlenderGeneratorChild() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBlenderGeneratorChild::Class;
}
