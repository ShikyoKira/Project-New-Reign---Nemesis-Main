#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbCharacterSkinInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x180d900d,
                                                "hkbCharacterSkinInfo",
                                                &nemesis::hkReferencedObject::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterSkinInfo);

        uint64_t m_characterId{};
        nemesis::hkArray<uint64_t> m_skinIds;
        nemesis::hkArray<uint64_t> m_deformableSkins;
        nemesis::hkArray<uint64_t> m_rigidSkins;

    public:
        hkbCharacterSkinInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterSkinInfo::Class;
}
