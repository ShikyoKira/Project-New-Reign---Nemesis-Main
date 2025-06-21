#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkbEventInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x5874eed4,
                                                "hkbEventInfo",
                                                nullptr,
                                                4,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
		REGISTER_HAVOK_POINTER_HEADER(hkbEventInfo);

		using Flags = nemesis::hkInternalClassEnumItem<unsigned int, true>;

        nemesis::hkEnum<Flags, unsigned int> m_flags = {
            {"0", 0},
            {"FLAG_SILENT", 1},
            {"FLAG_SYNC_POINT", 2},
        };

	public:
        hkbEventInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbEventInfo::Class;
}
