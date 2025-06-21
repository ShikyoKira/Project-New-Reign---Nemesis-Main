#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpAction : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xbdf70a51,
                                                "hkpAction",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpAction);

        nemesis::hkRefPtr<nemesis::HavokObject> m_world;
        nemesis::hkRefPtr<nemesis::HavokObject> m_island;
        nemesis::hkUlong m_userData;
        nemesis::hkStringPtr m_name;

    public:
        hkpAction() noexcept;
        hkpAction(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAction::Class;
}
