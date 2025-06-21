#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkxEnvironmentVariable.h"

namespace nemesis
{
    struct hkxEnvironment : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x41e1aa5,
                                                "hkxEnvironment",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxEnvironment);

        nemesis::hkArray<nemesis::hkxEnvironmentVariable> m_variables;

    public:
        hkxEnvironment() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxEnvironment::Class;
}
