#pragma once

#include "Havok/Base/hkbNode.h"

namespace nemesis
{
	struct hkbModifier : nemesis::hkbNode
	{
    protected:
        static constexpr nemesis::hkClass Class{0x96ec5ced,
                                                "hkbModifier",
                                                &nemesis::hkbNode::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

        bool m_enable;
        std::array<bool, 3> m_padModifier;

	public:
        hkbModifier(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbModifier::Class;
}
