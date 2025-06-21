#include "Havok/BSSynchronizedClipGenerator.h"

REGISTER_HAVOK_POINTER_SOURCE(BSSynchronizedClipGenerator,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSSynchronizedClipGenerator::Class; });

nemesis::BSSynchronizedClipGenerator::BSSynchronizedClipGenerator() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::BSSynchronizedClipGenerator::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSSynchronizedClipGenerator::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(16);                             // offset: 40/72 size: 8/8

    serializer.WriteObject("pClipGenerator", m_pClipGenerator); // offset: 48/80 size: 4/8 align: 16/16
    serializer.WriteValue("SyncAnimPrefix", m_SyncAnimPrefix);  // offset: 52/88 size: 4/8 align: 4/8
    serializer.WriteValue("bSyncClipIgnoreMarkPlacement",
                          m_bSyncClipIgnoreMarkPlacement); // offset: 56/96 size: 1/1 align: 1/1
    serializer.Skip(3);                                    // offset: 57/97 size: 3/3

    serializer.WriteValue("fGetToMarkTime", m_fGetToMarkTime); // offset: 60/100 size: 4/4 align: 4/4
    serializer.WriteValue("fMarkErrorThreshold",
                          m_fMarkErrorThreshold);              // offset: 64/104 size: 4/4 align: 4/4
    serializer.WriteValue("bLeadCharacter", m_bLeadCharacter); // offset: 68/108 size: 1/1 align: 1/1
    serializer.WriteValue("bReorientSupportChar",
                          m_bReorientSupportChar); // offset: 69/109 size: 1/1 align: 1/1
    serializer.WriteValue("bApplyMotionFromRoot",
                          m_bApplyMotionFromRoot); // offset: 70/110 size: 1/1 align: 1/1
    serializer.Skip(1);                            // offset: 71/111 size: 1/1

    serializer.WriteSerializeIgnoredObject("pSyncScene", m_pSyncScene); // offset: 72/112 size: 4/8 align: 4/8
    serializer.Pad(16);                                                 // offset: 76/120 size: 4/8

    serializer.WriteSerializeIgnoredValue("StartMarkWS",
                                          m_StartMarkWS); // offset: 80/128 size: 48/48 align: 16/16
    serializer.WriteSerializeIgnoredValue("EndMarkWS",
                                          m_EndMarkWS); // offset: 128/176 size: 48/48 align: 16/16
    serializer.WriteSerializeIgnoredValue("StartMarkMS",
                                          m_StartMarkMS); // offset: 176/224 size: 48/48 align: 16/16
    serializer.WriteSerializeIgnoredValue("fCurrentLerp",
                                          m_fCurrentLerp); // offset: 224/272 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());           // offset: 228/276 size: 0/4

    serializer.WriteSerializeIgnoredObject("pLocalSyncBinding",
                                           m_pLocalSyncBinding);      // offset: 228/280 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("pEventMap", m_pEventMap); // offset: 232/288 size: 4/8 align: 4/8
    serializer.WriteValue("sAnimationBindingIndex",
                          m_sAnimationBindingIndex);             // offset: 236/296 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("bAtMark", m_bAtMark); // offset: 238/298 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("bAllCharactersInScene",
                                          m_bAllCharactersInScene); // offset: 239/299 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("bAllCharactersAtMarks",
                                          m_bAllCharactersAtMarks); // offset: 240/300 size: 1/1 align: 1/1
    serializer.Pad(16);                                             // offset: 241/301 size: 15/3
    // class size: 256/304 align: 16/16
}

void nemesis::BSSynchronizedClipGenerator::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(16);                                 // offset: 40/72 size: 8/8

    deserializer.ReadObject("pClipGenerator", m_pClipGenerator); // offset: 48/80 size: 4/8 align: 16/16
    deserializer.ReadValue("SyncAnimPrefix", m_SyncAnimPrefix);  // offset: 52/88 size: 4/8 align: 4/8
    deserializer.ReadValue("bSyncClipIgnoreMarkPlacement",
                           m_bSyncClipIgnoreMarkPlacement); // offset: 56/96 size: 1/1 align: 1/1
    deserializer.Skip(3);                                   // offset: 57/97 size: 3/3

    deserializer.ReadValue("fGetToMarkTime", m_fGetToMarkTime); // offset: 60/100 size: 4/4 align: 4/4
    deserializer.ReadValue("fMarkErrorThreshold",
                           m_fMarkErrorThreshold);              // offset: 64/104 size: 4/4 align: 4/4
    deserializer.ReadValue("bLeadCharacter", m_bLeadCharacter); // offset: 68/108 size: 1/1 align: 1/1
    deserializer.ReadValue("bReorientSupportChar",
                           m_bReorientSupportChar); // offset: 69/109 size: 1/1 align: 1/1
    deserializer.ReadValue("bApplyMotionFromRoot",
                           m_bApplyMotionFromRoot); // offset: 70/110 size: 1/1 align: 1/1
    deserializer.Skip(1);                           // offset: 71/111 size: 1/1

    deserializer.ReadObject("pSyncScene", m_pSyncScene); // offset: 72/112 size: 4/8 align: 4/8
    deserializer.Pad(16);                                // offset: 76/120 size: 4/8

    deserializer.ReadValue("StartMarkWS", m_StartMarkWS);   // offset: 80/128 size: 48/48 align: 16/16
    deserializer.ReadValue("EndMarkWS", m_EndMarkWS);       // offset: 128/176 size: 48/48 align: 16/16
    deserializer.ReadValue("StartMarkMS", m_StartMarkMS);   // offset: 176/224 size: 48/48 align: 16/16
    deserializer.ReadValue("fCurrentLerp", m_fCurrentLerp); // offset: 224/272 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());        // offset: 228/276 size: 0/4

    deserializer.ReadObject("pLocalSyncBinding", m_pLocalSyncBinding); // offset: 228/280 size: 4/8 align: 4/8
    deserializer.ReadObject("pEventMap", m_pEventMap);                 // offset: 232/288 size: 4/8 align: 4/8
    deserializer.ReadValue("sAnimationBindingIndex",
                           m_sAnimationBindingIndex); // offset: 236/296 size: 2/2 align: 2/2
    deserializer.ReadValue("bAtMark", m_bAtMark);     // offset: 238/298 size: 1/1 align: 1/1
    deserializer.ReadValue("bAllCharactersInScene",
                           m_bAllCharactersInScene); // offset: 239/299 size: 1/1 align: 1/1
    deserializer.ReadValue("bAllCharactersAtMarks",
                           m_bAllCharactersAtMarks); // offset: 240/300 size: 1/1 align: 1/1
    deserializer.Pad(16);                            // offset: 241/301 size: 15/3
    // class size: 256/304 align: 16/16
}
