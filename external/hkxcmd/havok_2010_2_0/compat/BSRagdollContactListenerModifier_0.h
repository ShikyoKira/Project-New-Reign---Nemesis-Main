#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventProperty_1.h"
#include "hkbBoneIndexArray_0.h"

class BSRagdollContactListenerModifier : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSRagdollContactListenerModifier(void) {}
   HK_FORCE_INLINE BSRagdollContactListenerModifier( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_bones(flag)
   , m_ragdollRigidBodies(flag)
      {}
   // Properties
   hkChar m_padding_1[4];
   hkbEventProperty m_contactEvent;
   hkRefPtr<hkbBoneIndexArray> m_bones;
   hkBool m_throwEvent;
   hkArray<hkRefVariant> m_ragdollRigidBodies;
};
extern const hkClass BSRagdollContactListenerModifierClass;

