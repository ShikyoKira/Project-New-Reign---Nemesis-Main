#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbSetLocalTimeOfClipGeneratorCommand : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xfab12b45,
                                                "hkbSetLocalTimeOfClipGeneratorCommand",
                                                &nemesis::hkReferencedObject::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbSetLocalTimeOfClipGeneratorCommand);

        uint64_t m_characterId;
        float m_localTime;
        short m_nodeId;

    public:
        hkbSetLocalTimeOfClipGeneratorCommand() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbSetLocalTimeOfClipGeneratorCommand::Class;
}
