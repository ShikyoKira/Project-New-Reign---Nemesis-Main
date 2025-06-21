#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbBehaviorGraphStringData : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc713064e,
                                                "hkbBehaviorGraphStringData",
                                                &nemesis::hkReferencedObject::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBehaviorGraphStringData);

        nemesis::hkArray<nemesis::hkCString> m_eventNames;
        nemesis::hkArray<nemesis::hkCString> m_attributeNames;
        nemesis::hkArray<nemesis::hkCString> m_variableNames;
        nemesis::hkArray<nemesis::hkCString> m_characterPropertyNames;

    public:
        hkbBehaviorGraphStringData() noexcept;

        const nemesis::hkArray<nemesis::hkCString>& GetEventNames() const;
        const nemesis::hkArray<nemesis::hkCString>& GetAttributeNames() const;
        const nemesis::hkArray<nemesis::hkCString>& GetVariableNames() const;
        const nemesis::hkArray<nemesis::hkCString>& GetCharacterPropertyNames() const;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBehaviorGraphStringData::Class;
}
