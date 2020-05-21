#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbCharacterSteppedInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterSteppedInfo(void) {}
   HK_FORCE_INLINE hkbCharacterSteppedInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_poseModelSpace(flag)
   , m_rigidAttachmentTransforms(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkReal m_deltaTime;
   hkQsTransform m_worldFromModel;
   hkArray<hkQsTransform> m_poseModelSpace;
   hkArray<hkQsTransform> m_rigidAttachmentTransforms;
};
extern const hkClass hkbCharacterSteppedInfoClass;

