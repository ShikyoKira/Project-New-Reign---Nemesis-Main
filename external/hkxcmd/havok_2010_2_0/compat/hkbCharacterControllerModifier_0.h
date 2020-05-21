#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbCharacterControllerControlData_0.h"

class hkbCharacterControllerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterControllerModifier(void) {}
   HK_FORCE_INLINE hkbCharacterControllerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   enum InitialVelocityCoordinates
   {
      INITIAL_VELOCITY_IN_WORLD_COORDINATES = 0,
      INITIAL_VELOCITY_IN_MODEL_COORDINATES = 1,
   };

   enum MotionMode
   {
      MOTION_MODE_FOLLOW_ANIMATION = 0,
      MOTION_MODE_DYNAMIC = 1,
   };

   // Properties
   hkbCharacterControllerControlData m_controlData;
   hkVector4 m_initialVelocity;
   hkEnum<InitialVelocityCoordinates,hkInt8> m_initialVelocityCoordinates;
   hkEnum<MotionMode,hkInt8> m_motionMode;
   hkBool m_forceDownwardMomentum;
   hkBool m_applyGravity;
   hkBool m_setInitialVelocity;
   hkBool m_isTouchingGround;
   hkVector4 m_gravity;
   hkReal m_timestep;
   hkBool m_isInitialVelocityAdded;
};
extern const hkClass hkbCharacterControllerModifierClass;

