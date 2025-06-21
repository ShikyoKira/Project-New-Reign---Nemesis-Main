#pragma once

#include "Havok/hkaSkeleton.h"

namespace nemesis
{
    struct hkbCharacterAddedInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x3544e182,
                                                "hkbCharacterAddedInfo",
                                                &nemesis::hkReferencedObject::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbCharacterAddedInfo);

        uint64_t m_characterId;
        nemesis::hkStringPtr m_instanceName;
        nemesis::hkStringPtr m_templateName;
        nemesis::hkStringPtr m_fullPathToProject;
        nemesis::hkStringPtr m_localScriptsPath;
        nemesis::hkStringPtr m_remoteScriptsPath;
        nemesis::hkRefPtr<nemesis::hkaSkeleton> m_skeleton;
        nemesis::hkQsTransform m_worldFromModel;
        nemesis::hkArray<nemesis::hkQsTransform> m_poseModelSpace;

    public:
        hkbCharacterAddedInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbCharacterAddedInfo::Class;
}
