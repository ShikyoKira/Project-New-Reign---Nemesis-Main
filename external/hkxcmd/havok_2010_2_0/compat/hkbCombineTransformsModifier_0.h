#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbCombineTransformsModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCombineTransformsModifier(void) {}
   HK_FORCE_INLINE hkbCombineTransformsModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkVector4 m_translationOut;
   hkQuaternion m_rotationOut;
   hkVector4 m_leftTranslation;
   hkQuaternion m_leftRotation;
   hkVector4 m_rightTranslation;
   hkQuaternion m_rightRotation;
   hkBool m_invertLeftTransform;
   hkBool m_invertRightTransform;
   hkBool m_invertResult;
};
extern const hkClass hkbCombineTransformsModifierClass;

