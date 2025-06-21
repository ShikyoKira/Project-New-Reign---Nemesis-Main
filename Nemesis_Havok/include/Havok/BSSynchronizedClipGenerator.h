#pragma once

#include "Havok/Base/hkbGenerator.h"

namespace nemesis
{
    struct BSSynchronizedClipGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0xd83bea64,
                                                "BSSynchronizedClipGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                304,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSSynchronizedClipGenerator);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_pClipGenerator; // align 16
        nemesis::hkCString m_SyncAnimPrefix;
        bool m_bSyncClipIgnoreMarkPlacement;
        float m_fGetToMarkTime;
        float m_fMarkErrorThreshold;
        bool m_bLeadCharacter;
        bool m_bReorientSupportChar;
        bool m_bApplyMotionFromRoot;
        nemesis::hkRefPtr<nemesis::HavokObject> m_pSyncScene;
        nemesis::hkQsTransform m_StartMarkWS;
        nemesis::hkQsTransform m_EndMarkWS;
        nemesis::hkQsTransform m_StartMarkMS;
        float m_fCurrentLerp;
        nemesis::hkRefPtr<nemesis::HavokObject> m_pLocalSyncBinding;
        nemesis::hkRefPtr<nemesis::HavokObject> m_pEventMap;
        short m_sAnimationBindingIndex;
        bool m_bAtMark;
        bool m_bAllCharactersInScene;
        bool m_bAllCharactersAtMarks;

    public:
        BSSynchronizedClipGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSSynchronizedClipGenerator::Class;
}
