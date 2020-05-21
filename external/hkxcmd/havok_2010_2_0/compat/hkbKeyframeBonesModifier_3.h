#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbKeyframeBonesModifierKeyframeInfo_0.h"
#include "hkbBoneIndexArray_0.h"

class hkbKeyframeBonesModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbKeyframeBonesModifier(void) {}
   HK_FORCE_INLINE hkbKeyframeBonesModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_keyframeInfo(flag)
   , m_keyframedBonesList(flag)
      {}
   // Properties
   hkArray<hkbKeyframeBonesModifierKeyframeInfo> m_keyframeInfo;
   hkRefPtr<hkbBoneIndexArray> m_keyframedBonesList;
};
extern const hkClass hkbKeyframeBonesModifierClass;

