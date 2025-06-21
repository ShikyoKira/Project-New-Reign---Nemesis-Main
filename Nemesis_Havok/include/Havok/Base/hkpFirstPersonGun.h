#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpFirstPersonGun : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x852ab70b,
                                                "hkpFirstPersonGun",
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
        using KeyboardKey = nemesis::hkInternalClassEnumItem<unsigned char>;

        unsigned char m_type;
        nemesis::hkStringPtr m_name;
        nemesis::hkEnum<KeyboardKey, unsigned char> m_keyboardKey = {
            {"KEY_F1", 112},
            {"KEY_F2", 113},
            {"KEY_F3", 114},
            {"KEY_F4", 115},
            {"KEY_F5", 116},
            {"KEY_F6", 117},
            {"KEY_F7", 118},
            {"KEY_F8", 119},
            {"KEY_F9", 120},
            {"KEY_F10", 121},
            {"KEY_F11", 122},
            {"KEY_F12", 123},
        };
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_listeners;

    public:
        hkpFirstPersonGun(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpFirstPersonGun::Class;
}
