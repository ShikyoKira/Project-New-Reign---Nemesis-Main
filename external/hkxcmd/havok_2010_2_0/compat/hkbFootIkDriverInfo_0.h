#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbFootIkDriverInfoLeg_0.h"

class hkbFootIkDriverInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbFootIkDriverInfo(void) {}
   HK_FORCE_INLINE hkbFootIkDriverInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_legs(flag)
      {}
   // Properties
   hkArray<hkbFootIkDriverInfoLeg> m_legs;
   hkReal m_raycastDistanceUp;
   hkReal m_raycastDistanceDown;
   hkReal m_originalGroundHeightMS;
   hkReal m_verticalOffset;
   hkUint32 m_collisionFilterInfo;
   hkReal m_forwardAlignFraction;
   hkReal m_sidewaysAlignFraction;
   hkReal m_sidewaysSampleWidth;
   hkBool m_lockFeetWhenPlanted;
   hkBool m_useCharacterUpVector;
   hkBool m_isQuadrupedNarrow;
};
extern const hkClass hkbFootIkDriverInfoClass;

