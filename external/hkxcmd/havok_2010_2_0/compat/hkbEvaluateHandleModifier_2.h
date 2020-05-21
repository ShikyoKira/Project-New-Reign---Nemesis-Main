#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbHandle_1.h"

class hkbEvaluateHandleModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbEvaluateHandleModifier(void) {}
   HK_FORCE_INLINE hkbEvaluateHandleModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_handle(flag)
      {}
   enum HandleChangeMode
   {
      HANDLE_CHANGE_MODE_ABRUPT = 0,
      HANDLE_CHANGE_MODE_CONSTANT_VELOCITY = 1,
   };

   // Properties
   hkRefPtr<hkbHandle> m_handle;
   hkVector4 m_handlePositionOut;
   hkQuaternion m_handleRotationOut;
   hkBool m_isValidOut;
   hkReal m_extrapolationTimeStep;
   hkReal m_handleChangeSpeed;
   hkEnum<HandleChangeMode,hkInt8> m_handleChangeMode;
   hkbHandle m_oldHandle;
   hkVector4 m_oldHandlePosition;
   hkQuaternion m_oldHandleRotation;
   hkReal m_timeSinceLastModify;
   hkBool m_smoothlyChangingHandles;
};
extern const hkClass hkbEvaluateHandleModifierClass;

