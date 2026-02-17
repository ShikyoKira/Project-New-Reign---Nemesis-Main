#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbVariableValue.h"

namespace nemesis
{
    struct hkbSetNodePropertyCommand : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xc5160b64,
                                                "hkbSetNodePropertyCommand",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbSetNodePropertyCommand);

        uint64_t m_characterId{};
        nemesis::hkStringPtr m_nodeName;
        nemesis::hkStringPtr m_propertyName;
        nemesis::hkbVariableValue m_propertyValue;
        int m_padding{};

    public:
        hkbSetNodePropertyCommand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSetNodePropertyCommand::Class;
}
