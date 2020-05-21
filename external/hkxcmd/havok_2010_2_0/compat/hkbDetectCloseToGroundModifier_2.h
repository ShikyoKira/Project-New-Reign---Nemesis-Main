#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"

class hkbDetectCloseToGroundModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbDetectCloseToGroundModifier(void) {}
   HK_FORCE_INLINE hkbDetectCloseToGroundModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkbEventProperty m_closeToGroundEvent;
   hkReal m_closeToGroundHeight;
   hkReal m_raycastDistanceDown;
   hkUint32 m_collisionFilterInfo;
   hkInt16 m_boneIndex;
   hkInt16 m_animBoneIndex;
   hkBool m_isCloseToGround;
};
extern const hkClass hkbDetectCloseToGroundModifierClass;

