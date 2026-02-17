#pragma once

#include "Havok/Base/hkReferencedObject.h"
#include "Havok/Base/hkLocalFrame.h"

#include "Havok/hkbCharacter.h"

namespace nemesis
{
    struct hkbHandle : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xd8b6401c,
                                                "hkbHandle",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbHandle);

        nemesis::hkRefPtr<nemesis::hkLocalFrame> m_frame;
        nemesis::hkRefPtr<nemesis::hkReferencedObject> m_rigidBody;
        nemesis::hkRefPtr<nemesis::hkbCharacter> m_character;
        short m_animationBoneIndex{};

    public:
        hkbHandle() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbHandle::Class;
}
