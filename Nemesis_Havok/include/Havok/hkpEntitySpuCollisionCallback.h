#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpEntitySpuCollisionCallback : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x81147f05,
                                                "hkpEntitySpuCollisionCallback",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpEntitySpuCollisionCallback);

        nemesis::hkRefPtr<nemesis::HavokObject> m_util;
        unsigned short m_capacity;
        unsigned char m_eventFilter;
        unsigned char m_userFilter;

    public:
        hkpEntitySpuCollisionCallback() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpEntitySpuCollisionCallback::Class;
}
