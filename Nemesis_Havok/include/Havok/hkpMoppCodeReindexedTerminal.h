#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpMoppCodeReindexedTerminal : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x6ed8ac06,
                                                "hkpMoppCodeReindexedTerminal",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpMoppCodeReindexedTerminal);

        unsigned int m_origShapeKey;
        unsigned int m_reindexedShapeKey;

    public:
        hkpMoppCodeReindexedTerminal() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMoppCodeReindexedTerminal::Class;
}
