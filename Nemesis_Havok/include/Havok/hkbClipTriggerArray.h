#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbClipTrigger.h"

namespace nemesis
{
    struct hkbClipTriggerArray : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x59c23a0f,
                                                "hkbClipTriggerArray",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbClipTriggerArray);

        nemesis::hkArray<nemesis::hkbClipTrigger> m_triggers;

    public:
        hkbClipTriggerArray() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbClipTriggerArray::Class;
}
