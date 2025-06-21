#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbLinkedSymbolInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x6a5094e3,
                                                "hkbLinkedSymbolInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbLinkedSymbolInfo);

        nemesis::hkArray<nemesis::hkCString> m_eventNames;
        nemesis::hkArray<nemesis::hkCString> m_variableNames;

    public:
        hkbLinkedSymbolInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbLinkedSymbolInfo::Class;
}
