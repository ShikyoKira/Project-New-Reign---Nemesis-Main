#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbGetUpModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbGetUpModifier(void) {}
   HK_FORCE_INLINE hkbGetUpModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_groundNormal;
   hkReal m_duration;
   hkReal m_alignWithGroundDuration;
   hkInt16 m_rootBoneIndex;
   hkInt16 m_otherBoneIndex;
   hkInt16 m_anotherBoneIndex;
   hkReal m_timeSinceBegin;
   hkReal m_timeStep;
   hkBool m_initNextModify;
};
extern const hkClass hkbGetUpModifierClass;

