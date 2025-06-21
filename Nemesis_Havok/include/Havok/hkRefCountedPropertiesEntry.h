#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkRefCountedPropertiesEntry : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x28ef93ed,
                                                "hkRefCountedPropertiesEntry",
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
        REGISTER_HAVOK_POINTER_HEADER(hkRefCountedPropertiesEntry);

        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_object;
        unsigned short m_key;
        unsigned short m_flags;

    public:
        hkRefCountedPropertiesEntry() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkRefCountedPropertiesEntry::Class;
}
