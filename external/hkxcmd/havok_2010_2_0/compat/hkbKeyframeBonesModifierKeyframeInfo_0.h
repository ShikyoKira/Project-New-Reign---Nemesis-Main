#pragma once

#include <Common/Base/hkBase.h>

struct hkbKeyframeBonesModifierKeyframeInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbKeyframeBonesModifierKeyframeInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbKeyframeBonesModifierKeyframeInfo(void) {}
   // Properties
   hkVector4 m_keyframedPosition;
   hkQuaternion m_keyframedRotation;
   hkInt16 m_boneIndex;
   hkBool m_isValid;
};
extern const hkClass hkbKeyframeBonesModifierKeyframeInfoClass;

