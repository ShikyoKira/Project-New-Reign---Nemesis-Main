#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbCharacterStringData : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterStringData(void) {}
   HK_FORCE_INLINE hkbCharacterStringData( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_deformableSkinNames(flag)
   , m_rigidSkinNames(flag)
   , m_animationNames(flag)
   , m_animationFilenames(flag)
   , m_characterPropertyNames(flag)
   , m_retargetingSkeletonMapperFilenames(flag)
   , m_lodNames(flag)
   , m_mirroredSyncPointSubstringsA(flag)
   , m_mirroredSyncPointSubstringsB(flag)
   , m_name(flag)
   , m_rigName(flag)
   , m_ragdollName(flag)
   , m_behaviorFilename(flag)
      {}
   // Properties
   hkArray<hkStringPtr> m_deformableSkinNames;
   hkArray<hkStringPtr> m_rigidSkinNames;
   hkArray<hkStringPtr> m_animationNames;
   hkArray<hkStringPtr> m_animationFilenames;
   hkArray<hkStringPtr> m_characterPropertyNames;
   hkArray<hkStringPtr> m_retargetingSkeletonMapperFilenames;
   hkArray<hkStringPtr> m_lodNames;
   hkArray<hkStringPtr> m_mirroredSyncPointSubstringsA;
   hkArray<hkStringPtr> m_mirroredSyncPointSubstringsB;
   hkStringPtr m_name;
   hkStringPtr m_rigName;
   hkStringPtr m_ragdollName;
   hkStringPtr m_behaviorFilename;
};
extern const hkClass hkbCharacterStringDataClass;

