#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbAuxiliaryNodeInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xca0888ca,
                                                "hkbAuxiliaryNodeInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbAuxiliaryNodeInfo);

        using NodeType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkEnum<NodeType, unsigned char> m_type = {
            {"NODE_TYPE_UNKNOWN", 0},
            {"NODE_TYPE_NODE", 1},
            {"NODE_TYPE_TRANSITION", 2},
            {"NODE_TYPE_WILDCARD_TRANSITION", 3},
            {"NODE_TYPE_STATE", 4},
            {"NODE_TYPE_STATE_MACHINE", 5},
            {"NODE_TYPE_MODIFIER_GENERATOR", 6},
            {"NODE_TYPE_MODIFIER", 7},
            {"NODE_TYPE_CLIP", 8},
            {"NODE_TYPE_BLEND", 9},
            {"NODE_TYPE_TRANSITION_EFFECT", 10},
        };
        unsigned char m_depth;
        nemesis::hkStringPtr m_referenceBehaviorName;
        nemesis::hkArray<nemesis::hkCString> m_selfTransitionNames;

    public:
        hkbAuxiliaryNodeInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbAuxiliaryNodeInfo::Class;
}
