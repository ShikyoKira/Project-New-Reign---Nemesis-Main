#pragma once

#include <Common/Base/hkBase.h>
#include "hkbHandle_1.h"

struct hkbHandIkControlData
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbHandIkControlData );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbHandIkControlData(void) {}
   enum HandleChangeMode
   {
      HANDLE_CHANGE_MODE_ABRUPT = 0,
      HANDLE_CHANGE_MODE_CONSTANT_VELOCITY = 1,
   };

   // Properties
   hkVector4 m_targetPosition;
   hkQuaternion m_targetRotation;
   hkVector4 m_targetNormal;
   hkRefPtr<hkbHandle> m_targetHandle;
   hkReal m_transformOnFraction;
   hkReal m_normalOnFraction;
   hkReal m_fadeInDuration;
   hkReal m_fadeOutDuration;
   hkReal m_extrapolationTimeStep;
   hkReal m_handleChangeSpeed;
   hkEnum<HandleChangeMode,hkInt8> m_handleChangeMode;
   hkBool m_fixUp;
};
extern const hkClass hkbHandIkControlDataClass;

