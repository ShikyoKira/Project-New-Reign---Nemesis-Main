#pragma once

#include "Havok/hkRootLevelContainerNamedVariant.h"

namespace nemesis
{
    struct hkRootLevelContainer : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x2772c11e,
                                                "hkRootLevelContainer",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkRootLevelContainer);

        nemesis::hkArray<nemesis::hkRootLevelContainerNamedVariant> m_namedVariants;

    public:
        hkRootLevelContainer() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkRootLevelContainer::Class;
}
