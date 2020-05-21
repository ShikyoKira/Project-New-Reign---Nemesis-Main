#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>

class hkbCharacterAddedInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterAddedInfo(void) {}
   HK_FORCE_INLINE hkbCharacterAddedInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_instanceName(flag)
   , m_templateName(flag)
   , m_fullPathToProject(flag)
   , m_skeleton(flag)
   , m_poseModelSpace(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkStringPtr m_instanceName;
   hkStringPtr m_templateName;
   hkStringPtr m_fullPathToProject;
   hkRefPtr<hkaSkeleton> m_skeleton;
   hkQsTransform m_worldFromModel;
   hkArray<hkQsTransform> m_poseModelSpace;
};
extern const hkClass hkbCharacterAddedInfoClass;

