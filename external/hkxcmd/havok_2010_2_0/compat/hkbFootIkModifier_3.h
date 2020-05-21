#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbFootIkGains_0.h"
#include "hkbFootIkModifierLeg_2.h"
#include "hkbFootIkModifierInternalLegData_1.h"

class hkbFootIkModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbFootIkModifier(void) {}
   HK_FORCE_INLINE hkbFootIkModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_legs(flag)
   , m_internalLegData(flag)
      {}
   enum AlignMode
   {
      ALIGN_MODE_FORWARD_RIGHT = 0,
      ALIGN_MODE_FORWARD = 1,
   };

   // Properties
   hkbFootIkGains m_gains;
   hkArray<hkbFootIkModifierLeg> m_legs;
   hkReal m_raycastDistanceUp;
   hkReal m_raycastDistanceDown;
   hkReal m_originalGroundHeightMS;
   hkReal m_errorOut;
   hkVector4 m_errorOutTranslation;
   hkQuaternion m_alignWithGroundRotation;
   hkReal m_verticalOffset;
   hkUint32 m_collisionFilterInfo;
   hkReal m_forwardAlignFraction;
   hkReal m_sidewaysAlignFraction;
   hkReal m_sidewaysSampleWidth;
   hkBool m_useTrackData;
   hkBool m_lockFeetWhenPlanted;
   hkBool m_useCharacterUpVector;
   hkEnum<AlignMode,hkInt8> m_alignMode;
   hkArray<hkbFootIkModifierInternalLegData> m_internalLegData;
   hkReal m_prevIsFootIkEnabled;
   hkBool m_isSetUp;
   hkBool m_isGroundPositionValid;
   hkReal m_timeStep;
};
extern const hkClass hkbFootIkModifierClass;

