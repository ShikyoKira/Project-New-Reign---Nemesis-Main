#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkMultiThreadCheck : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x11e4408b,
                                                "hkMultiThreadCheck",
                                                nullptr,
                                                12,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkMultiThreadCheck);

        unsigned int m_threadId{};
        int m_stackTraceId{};
        unsigned short m_markCount{};
        unsigned short m_markBitStack{};

    public:
        hkMultiThreadCheck() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkMultiThreadCheck::Class;
}
