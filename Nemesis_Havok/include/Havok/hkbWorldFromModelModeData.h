#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbWorldFromModelModeData : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xa3af8783,
                                                "hkbWorldFromModelModeData",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbWorldFromModelModeData);

        using WorldFromModelMode = nemesis::hkInternalClassEnumItem<char>;

        short m_poseMatchingBone0{};
        short m_poseMatchingBone1{};
        short m_poseMatchingBone2{};
        nemesis::hkEnum<WorldFromModelMode, char> m_mode = {
            {"WORLD_FROM_MODEL_MODE_USE_OLD", 0},
            {"WORLD_FROM_MODEL_MODE_USE_INPUT", 1},
            {"WORLD_FROM_MODEL_MODE_COMPUTE", 2},
            {"WORLD_FROM_MODEL_MODE_NONE", 3},
            {"WORLD_FROM_MODEL_MODE_RAGDOLL", 4},
        };

    public:
        hkbWorldFromModelModeData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbWorldFromModelModeData::Class;
}
