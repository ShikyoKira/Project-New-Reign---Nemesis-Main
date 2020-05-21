#pragma once

#include <Common/Base/hkBase.h>

struct hkbProxyModifierProxyInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbProxyModifierProxyInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbProxyModifierProxyInfo(void) {}
   // Properties
   hkReal m_dynamicFriction;
   hkReal m_staticFriction;
   hkReal m_keepContactTolerance;
   hkVector4 m_up;
   hkReal m_keepDistance;
   hkReal m_contactAngleSensitivity;
   hkUint32 m_userPlanes;
   hkReal m_maxCharacterSpeedForSolver;
   hkReal m_characterStrength;
   hkReal m_characterMass;
   hkReal m_maxSlope;
   hkReal m_penetrationRecoverySpeed;
   hkInt32 m_maxCastIterations;
   hkBool m_refreshManifoldInCheckSupport;
};
extern const hkClass hkbProxyModifierProxyInfoClass;

