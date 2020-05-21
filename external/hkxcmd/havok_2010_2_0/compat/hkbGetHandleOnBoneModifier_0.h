#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbHandle_1.h"

class hkbGetHandleOnBoneModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbGetHandleOnBoneModifier(void) {}
	   HK_FORCE_INLINE hkbGetHandleOnBoneModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_handleOut(flag)
   , m_localFrameName(flag)
      {}
   // Properties
   hkRefPtr<hkbHandle> m_handleOut;
   hkStringPtr m_localFrameName;
   hkInt16 m_ragdollBoneIndex;
   hkInt16 m_animationBoneIndex;
};
extern const hkClass hkbGetHandleOnBoneModifierClass;

