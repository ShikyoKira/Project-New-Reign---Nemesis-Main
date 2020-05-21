#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include <Animation/Animation/Mapper/hkaSkeletonMapper.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include "hkbCharacterData_7.h"

class hkbCharacterSetup : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterSetup(void) {}
   HK_FORCE_INLINE hkbCharacterSetup( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_retargetingSkeletonMappers(flag)
   , m_animationSkeleton(flag)
   , m_ragdollToAnimationSkeletonMapper(flag)
   , m_animationToRagdollSkeletonMapper(flag)
   , m_animationBindingSet(flag)
   , m_data(flag)
   , m_mirroredSkeleton(flag)
   , m_characterPropertyIdMap(flag)
      {}
   // Properties
   hkArray<hkaSkeletonMapper*> m_retargetingSkeletonMappers;
   hkRefPtr<hkaSkeleton> m_animationSkeleton;
   hkRefPtr<hkaSkeletonMapper> m_ragdollToAnimationSkeletonMapper;
   hkRefPtr<hkaSkeletonMapper> m_animationToRagdollSkeletonMapper;
   hkRefVariant m_animationBindingSet;
   hkRefPtr<hkbCharacterData> m_data;
   hkRefVariant m_mirroredSkeleton;
   hkRefVariant m_characterPropertyIdMap;
};
extern const hkClass hkbCharacterSetupClass;

