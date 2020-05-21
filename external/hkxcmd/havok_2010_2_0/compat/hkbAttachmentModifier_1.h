#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"
#include "hkbAttachmentSetup_1.h"
#include "hkbHandle_1.h"

class hkbAttachmentModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbAttachmentModifier(void) {}
   HK_FORCE_INLINE hkbAttachmentModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_attachmentSetup(flag)
   , m_attacherHandle(flag)
   , m_attacheeHandle(flag)
   , m_attacheeRB(flag)
   , m_attachment(flag)
      {}
   // Properties
   hkbEventProperty m_sendToAttacherOnAttach;
   hkbEventProperty m_sendToAttacheeOnAttach;
   hkbEventProperty m_sendToAttacherOnDetach;
   hkbEventProperty m_sendToAttacheeOnDetach;
   hkRefPtr<hkbAttachmentSetup> m_attachmentSetup;
   hkRefPtr<hkbHandle> m_attacherHandle;
   hkRefPtr<hkbHandle> m_attacheeHandle;
   hkInt32 m_attacheeLayer;
   hkRefVariant m_attacheeRB;
   /*enum*/ hkUint8 m_oldMotionType;
   hkInt32 m_oldFilterInfo;
   hkRefVariant m_attachment;
};
extern const hkClass hkbAttachmentModifierClass;

