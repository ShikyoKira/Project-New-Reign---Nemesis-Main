#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"

class hkbTransformVectorModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbTransformVectorModifier(void) {}
   HK_FORCE_INLINE hkbTransformVectorModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
      {}
   // Properties
   hkQuaternion m_rotation;
   hkVector4 m_translation;
   hkVector4 m_vectorIn;
   hkVector4 m_vectorOut;
   hkBool m_rotateOnly;
   hkBool m_inverse;
   hkBool m_computeOnActivate;
   hkBool m_computeOnModify;
};
extern const hkClass hkbTransformVectorModifierClass;

