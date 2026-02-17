#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbBehaviorInfoIdToNamePair : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x35a0439a,
                                                "hkbBehaviorInfoIdToNamePair",
                                                nullptr,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBehaviorInfoIdToNamePair);

        using ToolNodeType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkStringPtr m_behaviorName;
        nemesis::hkStringPtr m_nodeName;
        nemesis::hkEnum<ToolNodeType, unsigned char> m_toolType = {
            {"NODE_TYPE_UNKNOWN", 0},
            {"NODE_TYPE_STATE_MACHINE", 1},
            {"NODE_TYPE_CLIP", 2},
            {"NODE_TYPE_BLEND", 3},
            {"NODE_TYPE_MODIFIER", 4},
            {"NODE_TYPE_GENERATOR", 5},
            {"NODE_TYPE_MODIFIER_GENERATOR", 6},
            {"NODE_TYPE_TRANSITION_EFFECT", 7},
            {"NODE_TYPE_BEHAVIOR_FILE_REFERENCE", 8},
        };
        short m_id{};

    public:
        hkbBehaviorInfoIdToNamePair() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBehaviorInfoIdToNamePair::Class;
}
