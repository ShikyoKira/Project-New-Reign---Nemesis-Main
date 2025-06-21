#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkRootLevelContainerNamedVariant : nemesis::HavokObject, nemesis::hkVariant
    {
        static constexpr nemesis::hkClass Class{0xb103a2cd,
                                                "hkRootLevelContainerNamedVariant",
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
        REGISTER_HAVOK_POINTER_HEADER(hkRootLevelContainerNamedVariant);

        nemesis::hkStringPtr m_name;
        nemesis::hkStringPtr m_className;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_variant;

    public:
        hkRootLevelContainerNamedVariant() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkRootLevelContainerNamedVariant::Class;
}
