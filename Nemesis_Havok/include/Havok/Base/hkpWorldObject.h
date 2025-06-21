#pragma once

#include "Havok/Base/hkaAnimatedReferenceFrame.h"
#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkpLinkedCollidable.h"
#include "Havok/hkMultiThreadCheck.h"
#include "Havok/hkSimpleProperty.h"

namespace nemesis
{
    struct hkpWorldObject : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x49fb6f2e,
                                                "hkpWorldObject",
                                                &nemesis::hkReferencedObject::Class,
                                                208,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        nemesis::hkRefPtr<nemesis::HavokObject> m_world;
        nemesis::hkUlong m_userData;
        nemesis::hkpLinkedCollidable m_collidable;
        nemesis::hkMultiThreadCheck m_multiThreadCheck;
        nemesis::hkStringPtr m_name;
        nemesis::hkArray<nemesis::hkpProperty> m_properties_0;
        nemesis::hkArray<nemesis::hkSimpleProperty> m_properties_1;
        nemesis::hkRefPtr<nemesis::HavokObject> m_treeData;

    public:
        hkpWorldObject(const nemesis::hkClass& cls);

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpWorldObject::Class;
}
