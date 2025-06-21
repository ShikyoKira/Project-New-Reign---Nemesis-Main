#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkPostFinishAttribute : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x903abb2c,
                                                "hkPostFinishAttribute",
                                                nullptr,
                                                8,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkPostFinishAttribute);

        nemesis::hkRefPtr<nemesis::HavokObject> m_postFinishFunction;

    public:
        hkPostFinishAttribute() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkPostFinishAttribute::Class;
}
