#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbProxyModifierProxyInfo_0.h"

class hkbProxyModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbProxyModifier(void) {}
   HK_FORCE_INLINE hkbProxyModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_characterProxy(flag)
   , m_phantom(flag)
   , m_phantomShape(flag)
      {}
   enum PhantomType
   {
      PHANTOM_TYPE_SIMPLE = 0,
      PHANTOM_TYPE_CACHING = 1,
   };

   enum LinearVelocityMode
   {
      LINEAR_VELOCITY_MODE_WORLD = 0,
      LINEAR_VELOCITY_MODE_MODEL = 1,
   };

   // Properties
   hkbProxyModifierProxyInfo m_proxyInfo;
   hkVector4 m_linearVelocity;
   hkReal m_horizontalGain;
   hkReal m_verticalGain;
   hkReal m_maxHorizontalSeparation;
   hkReal m_maxVerticalSeparation;
   hkReal m_verticalDisplacementError;
   hkReal m_verticalDisplacementErrorGain;
   hkReal m_maxVerticalDisplacement;
   hkReal m_minVerticalDisplacement;
   hkReal m_capsuleHeight;
   hkReal m_capsuleRadius;
   hkReal m_maxSlopeForRotation;
   hkUint32 m_collisionFilterInfo;
   hkEnum<PhantomType,hkInt8> m_phantomType;
   hkEnum<LinearVelocityMode,hkInt8> m_linearVelocityMode;
   hkBool m_ignoreIncomingRotation;
   hkBool m_ignoreCollisionDuringRotation;
   hkBool m_ignoreIncomingTranslation;
   hkBool m_includeDownwardMomentum;
   hkBool m_followWorldFromModel;
   hkBool m_isTouchingGround;
   hkRefVariant m_characterProxy;
   hkRefVariant m_phantom;
   hkRefVariant m_phantomShape;
   hkVector4 m_horizontalDisplacement;
   hkReal m_verticalDisplacement;
   hkReal m_timestep;
   hkBool m_previousFrameFollowWorldFromModel;
};
extern const hkClass hkbProxyModifierClass;

