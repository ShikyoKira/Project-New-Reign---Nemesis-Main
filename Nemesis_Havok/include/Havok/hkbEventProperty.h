#pragma once

#include "Havok/Base/hkbEventBase.h"

namespace nemesis
{
	struct hkbEventProperty : nemesis::hkbEventBase
    {
        static constexpr nemesis::hkClass Class{0xdb38a15,
                                                "hkbEventProperty",
                                                &nemesis::hkbEventBase::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbEventProperty);

	public:
        hkbEventProperty() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventProperty::Class;
}
