#pragma once

#include "Havok/Base/hkaAnimation.h"
#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkaAnimationBinding.h"
#include "Havok/hkaBoneAttachment.h"
#include "Havok/hkaMeshBinding.h"
#include "Havok/hkaSkeleton.h"

namespace nemesis
{
    struct hkaAnimationContainer : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x8dc20333,
                                                "hkaAnimationContainer",
                                                &nemesis::hkReferencedObject::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkaAnimationContainer);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkaSkeleton>> m_skeletons;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkaAnimation>> m_animations;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkaAnimationBinding>> m_bindings;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkaBoneAttachment>> m_attachments;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkaMeshBinding>> m_skins;

    public:
        hkaAnimationContainer() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaAnimationContainer::Class;
}
