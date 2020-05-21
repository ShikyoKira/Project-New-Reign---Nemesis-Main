#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbHandle_1.h"
#include "hkbSenseHandleModifierRange_0.h"

class hkbSenseHandleModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSenseHandleModifier(void) {}
   HK_FORCE_INLINE hkbSenseHandleModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_ranges(flag)
   , m_handleOut(flag)
   , m_handleIn(flag)
   , m_localFrameName(flag)
   , m_sensorLocalFrameName(flag)
      {}
   enum SensingMode
   {
      SENSE_IN_NEARBY_RIGID_BODIES = 0,
      SENSE_IN_RIGID_BODIES_OUTSIDE_THIS_CHARACTER = 1,
      SENSE_IN_OTHER_CHARACTER_RIGID_BODIES = 2,
      SENSE_IN_THIS_CHARACTER_RIGID_BODIES = 3,
      SENSE_IN_GIVEN_CHARACTER_RIGID_BODIES = 4,
      SENSE_IN_GIVEN_RIGID_BODY = 5,
      SENSE_IN_OTHER_CHARACTER_SKELETON = 6,
      SENSE_IN_THIS_CHARACTER_SKELETON = 7,
      SENSE_IN_GIVEN_CHARACTER_SKELETON = 8,
      SENSE_IN_GIVEN_LOCAL_FRAME_GROUP = 9,
   };

   // Properties
   hkbHandle m_handle;
   hkVector4 m_sensorLocalOffset;
   hkArray<hkbSenseHandleModifierRange> m_ranges;
   hkRefPtr<hkbHandle> m_handleOut;
   hkRefPtr<hkbHandle> m_handleIn;
   hkStringPtr m_localFrameName;
   hkStringPtr m_sensorLocalFrameName;
   hkReal m_minDistance;
   hkReal m_maxDistance;
   hkReal m_distanceOut;
   hkUint32 m_collisionFilterInfo;
   hkInt16 m_sensorRagdollBoneIndex;
   hkInt16 m_sensorAnimationBoneIndex;
   hkEnum<SensingMode,hkInt8> m_sensingMode;
   hkBool m_extrapolateSensorPosition;
   hkBool m_keepFirstSensedHandle;
   hkBool m_foundHandleOut;
   hkReal m_timeSinceLastModify;
   hkInt32 m_rangeIndexForEventToSendNextUpdate;
};
extern const hkClass hkbSenseHandleModifierClass;

