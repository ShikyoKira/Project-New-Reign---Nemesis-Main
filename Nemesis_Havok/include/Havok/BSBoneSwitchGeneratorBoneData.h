#pragma once

#include "Havok/Base/hkbBindable.h"
#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkbBoneWeightArray.h"

namespace nemesis
{
    struct BSBoneSwitchGeneratorBoneData : nemesis::hkbBindable
    {
        static constexpr nemesis::hkClass Class{0xc1215be6,
                                                "BSBoneSwitchGeneratorBoneData",
                                                &nemesis::hkbBindable::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(BSBoneSwitchGeneratorBoneData);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_pGenerator; // align 16
        nemesis::hkRefPtr<nemesis::hkbBoneWeightArray> m_spBoneWeight;

    public:
        BSBoneSwitchGeneratorBoneData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSBoneSwitchGeneratorBoneData::Class;
}
