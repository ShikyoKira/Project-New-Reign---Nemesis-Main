#include "Havok/hkaFootstepAnalysisInfoContainer.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaFootstepAnalysisInfoContainer,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaFootstepAnalysisInfoContainer::Class; });

nemesis::hkaFootstepAnalysisInfoContainer::hkaFootstepAnalysisInfoContainer() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaFootstepAnalysisInfoContainer::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaFootstepAnalysisInfoContainer::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("previewInfo", m_previewInfo); // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkaFootstepAnalysisInfoContainer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("previewInfo", m_previewInfo);      // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}
