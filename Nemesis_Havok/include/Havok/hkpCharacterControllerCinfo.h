#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpCharacterControllerCinfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xda8c7d7d,
                                                "hkpCharacterControllerCinfo",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpCharacterControllerCinfo);

    public:
        hkpCharacterControllerCinfo() noexcept;
        hkpCharacterControllerCinfo(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCharacterControllerCinfo::Class;
}
