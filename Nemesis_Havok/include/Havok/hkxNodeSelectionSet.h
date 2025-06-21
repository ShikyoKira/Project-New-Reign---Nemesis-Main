#pragma once

#include "Havok/Base/hkxAttributeHolder.h"

#include "Havok/hkxNode.h"

namespace nemesis
{
    struct hkxNodeSelectionSet : nemesis::hkxAttributeHolder
    {
        static constexpr nemesis::hkClass Class{0xd753fc4d,
                                                "hkxNodeSelectionSet",
                                                &nemesis::hkxAttributeHolder::Class,
                                                56,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxNodeSelectionSet);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkxNode>> m_selectedNodes;
        nemesis::hkStringPtr m_name;

    public:
        hkxNodeSelectionSet() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxNodeSelectionSet::Class;
}
