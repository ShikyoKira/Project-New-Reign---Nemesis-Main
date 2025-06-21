#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbProjectStringData : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x76ad60a,
                                                "hkbProjectStringData",
                                                &nemesis::hkReferencedObject::Class,
                                                120,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbProjectStringData);

        nemesis::hkArray<nemesis::hkCString> m_animationFilenames;
        nemesis::hkArray<nemesis::hkCString> m_behaviorFilenames;
        nemesis::hkArray<nemesis::hkCString> m_characterFilenames;
        nemesis::hkArray<nemesis::hkCString> m_eventNames;
        nemesis::hkStringPtr m_animationPath;
        nemesis::hkStringPtr m_behaviorPath;
        nemesis::hkStringPtr m_characterPath;
        nemesis::hkStringPtr m_fullPathToSource;
        nemesis::hkStringPtr m_rootPath;
        nemesis::hkStringPtr m_scriptsPath;

    public:
        hkbProjectStringData() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbProjectStringData::Class;
}
