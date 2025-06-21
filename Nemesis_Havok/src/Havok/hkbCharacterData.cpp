#include "Havok/hkbCharacterData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterData::Class; });

nemesis::hkbCharacterData::hkbCharacterData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterData::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        if (cur_ver > HavokVersion::HK_2012_2_0)
        {
            serializer.WriteObject("characterControllerSetup", m_characterControllerSetup);
            serializer.Pad(16);
        }
        else
        {
            serializer.WriteObject("characterControllerInfo",
                                   m_characterControllerInfo); // offset: 8/16 size: 16/24 align: 4/8
            serializer.Skip(8);                                // offset: 24/40 size: 8/8n: 4/8
        }
    }
    else
    {
        serializer.Pad(16);
    }

    serializer.WriteValue("modelUpMS", m_modelUpMS);           // offset: 32/48 size: 16/16 align: 16/16
    serializer.WriteValue("modelForwardMS", m_modelForwardMS); // offset: 48/64 size: 16/16 align: 16/16
    serializer.WriteValue("modelRightMS", m_modelRightMS);     // offset: 64/80 size: 16/16 align: 16/16
    serializer.WriteObject("characterPropertyInfos",
                           m_characterPropertyInfos);           // offset: 80/96 size: 12/16 align: 4/8
    serializer.WriteObject("numBonesPerLod", m_numBonesPerLod); // offset: 92/112 size: 12/16 align: 4/8
    serializer.WriteObject("characterPropertyValues",
                           m_characterPropertyValues); // offset: 104/128 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteObject("footIkDriverInfo",
                               m_footIkDriverInfo); // offset: 108/136 size: 4/8 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("handIkDriverInfo",
                               m_handIkDriverInfo); // offset: 112/144 size: 4/8 align: 4/8
    }
    
    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        serializer.WriteObject("aiControlDriverInfo", m_aiControlDriverInfo);
    }

    serializer.WriteObject("stringData", m_stringData);             // offset: 116/152 size: 4/8 align: 4/8
    serializer.WriteObject("mirroredSkeletonInfo",
                           m_mirroredSkeletonInfo);                // offset: 120/160 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteObject("boneAttachmentBoneIndices", m_boneAttachmentBoneIndices);
        serializer.WriteObject("boneAttachmentTransforms", m_boneAttachmentTransforms);
    }

    serializer.WriteValue("scale", m_scale);                       // offset: 124/168 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("numHands", m_numHands); // offset: 128/172 size: 2/2 align: 2/2
    serializer.WriteSerializeIgnoredValue("numFloatSlots",
                                          m_numFloatSlots); // offset: 130/174 size: 2/2 align: 2/2
    serializer.Pad(16);                                     // offset: 132/176 size: 12/0
    // class size: 144/176 align: 16/16
}

void nemesis::hkbCharacterData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        if (cur_ver > HavokVersion::HK_2012_2_0)
        {
            deserializer.ReadObject("characterControllerSetup",
                                    m_characterControllerSetup);
            deserializer.Pad(16);
        }
        else
        {
            deserializer.ReadObject("characterControllerInfo",
                                    m_characterControllerInfo); // offset: 8/16 size: 16/24 align: 4/8
            deserializer.Skip(8);                               // offset: 24/40 size: 8/8
        }
    }
    else
    {
        deserializer.Pad(16);
    }

    deserializer.ReadValue("modelUpMS", m_modelUpMS);           // offset: 32/48 size: 16/16 align: 16/16
    deserializer.ReadValue("modelForwardMS", m_modelForwardMS); // offset: 48/64 size: 16/16 align: 16/16
    deserializer.ReadValue("modelRightMS", m_modelRightMS);     // offset: 64/80 size: 16/16 align: 16/16
    deserializer.ReadObject("characterPropertyInfos",
                            m_characterPropertyInfos);           // offset: 80/96 size: 12/16 align: 4/8
    deserializer.ReadObject("numBonesPerLod", m_numBonesPerLod); // offset: 92/112 size: 12/16 align: 4/8
    deserializer.ReadObject("characterPropertyValues",
                            m_characterPropertyValues); // offset: 104/128 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadObject("footIkDriverInfo",
                                m_footIkDriverInfo); // offset: 108/136 size: 4/8 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("handIkDriverInfo",
                                m_handIkDriverInfo); // offset: 112/144 size: 4/8 align: 4/8
    }
    
    if (cur_ver > HavokVersion::HK_2012_2_0)
    {
        deserializer.ReadObject("aiControlDriverInfo", m_aiControlDriverInfo);
    }

    deserializer.ReadObject("stringData", m_stringData);             // offset: 116/152 size: 4/8 align: 4/8
    deserializer.ReadObject("mirroredSkeletonInfo",
                            m_mirroredSkeletonInfo);          // offset: 120/160 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadObject("boneAttachmentBoneIndices", m_boneAttachmentBoneIndices);
        deserializer.ReadObject("boneAttachmentTransforms", m_boneAttachmentTransforms);
    }

    deserializer.ReadValue("scale", m_scale);                 // offset: 124/168 size: 4/4 align: 4/4
    deserializer.ReadValue("numHands", m_numHands);           // offset: 128/172 size: 2/2 align: 2/2
    deserializer.ReadValue("numFloatSlots", m_numFloatSlots); // offset: 130/174 size: 2/2 align: 2/2
    deserializer.Pad(16);                                     // offset: 132/176 size: 12/0
    // class size: 144/176 align: 16/16
}
