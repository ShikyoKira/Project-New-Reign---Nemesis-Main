#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class BSTweenerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSTweenerModifier(void) {}
   HK_FORCE_INLINE BSTweenerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkBool m_tweenPosition;
   hkBool m_tweenRotation;
   hkBool m_useTweenDuration;
   hkReal m_tweenDuration;
   hkVector4 m_targetPosition;
   hkQuaternion m_targetRotation;
   hkReal m_duration;
   hkQsTransform m_startTransform;
   hkReal m_time;
};
extern const hkClass BSTweenerModifierClass;

