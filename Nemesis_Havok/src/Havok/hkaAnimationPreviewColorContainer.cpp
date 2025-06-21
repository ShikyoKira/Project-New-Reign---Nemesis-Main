#include "Havok/hkaAnimationPreviewColorContainer.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaAnimationPreviewColorContainer,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaAnimationPreviewColorContainer::Class; });

nemesis::hkaAnimationPreviewColorContainer::hkaAnimationPreviewColorContainer() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaAnimationPreviewColorContainer::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaAnimationPreviewColorContainer::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer);   // offset: 0/0 size: 8/16 align: 4/8
        serializer.WriteObject("previewColor", m_previewColor_1); // offset: 8/16 size: 12/16 align: 4/8
    }
    else
    {
        serializer.WriteObject("previewColor", m_previewColor_0);
        serializer.WriteValue("", m_numPreviewColor);
    }

    // class size: 20/32 align: 4/8
}

void nemesis::hkaAnimationPreviewColorContainer::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
        deserializer.ReadObject("previewColor", m_previewColor_1);  // offset: 8/16 size: 12/16 align: 4/8
    }
    else
    {
        deserializer.ReadObject("previewColor", m_previewColor_0);
        deserializer.ReadValue("", m_numPreviewColor);
    }

    // class size: 20/32 align: 4/8
}
