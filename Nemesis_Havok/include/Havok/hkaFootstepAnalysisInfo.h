#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkaFootstepAnalysisInfo : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x824faf75,
                                                "hkaFootstepAnalysisInfo",
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
        REGISTER_HAVOK_POINTER_HEADER(hkaFootstepAnalysisInfo);

        std::string m_name;
        std::string m_nameStrike;
        std::string m_nameLift;
        std::string m_nameLock;
        std::string m_nameUnlock;
        nemesis::hkArray<float> m_minPos;
        nemesis::hkArray<float> m_maxPos;
        nemesis::hkArray<float> m_minVel;
        nemesis::hkArray<float> m_maxVel;
        nemesis::hkArray<float> m_allBonesDown;
        nemesis::hkArray<float> m_anyBonesDown;
        float m_posTol;
        float m_velTol;
        float m_duration;

    public:
        hkaFootstepAnalysisInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkaFootstepAnalysisInfo::Class;
}
