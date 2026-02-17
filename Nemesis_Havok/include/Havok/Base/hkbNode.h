#pragma once

#include "Havok/Base/hkbBindable.h"

namespace nemesis
{
	struct hkbNode : nemesis::hkbBindable
	{
    protected:
        static constexpr nemesis::hkClass Class{0x6d26f61d,
                                                "hkbNode",
                                                &nemesis::hkbBindable::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

        nemesis::hkUlong m_userData;
        nemesis::hkStringPtr m_name;
        short m_id{};
        char m_cloneState{};
        std::array<bool, 1> m_padNode{};

    public:
        hkbNode(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbNode::Class;
}
