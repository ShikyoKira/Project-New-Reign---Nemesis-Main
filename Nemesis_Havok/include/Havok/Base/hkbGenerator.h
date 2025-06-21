#pragma once

#include "Havok/Base/hkbNode.h"

namespace nemesis
{
	struct hkbGenerator : nemesis::hkbNode
	{
    protected:
        static constexpr nemesis::hkClass Class{0xd68aefc,
                                                "hkbGenerator",
                                                &nemesis::hkbNode::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    public:
        hkbGenerator(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGenerator::Class;
}
