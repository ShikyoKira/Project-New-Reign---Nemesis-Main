#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbProjectStringData : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbProjectStringData(void) {}
   HK_FORCE_INLINE hkbProjectStringData( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_animationFilenames(flag)
   , m_behaviorFilenames(flag)
   , m_characterFilenames(flag)
   , m_eventNames(flag)
   , m_animationPath(flag)
   , m_behaviorPath(flag)
   , m_characterPath(flag)
   , m_fullPathToSource(flag)
   , m_rootPath(flag)
      {}
   // Properties
   hkArray<hkStringPtr> m_animationFilenames;
   hkArray<hkStringPtr> m_behaviorFilenames;
   hkArray<hkStringPtr> m_characterFilenames;
   hkArray<hkStringPtr> m_eventNames;
   hkStringPtr m_animationPath;
   hkStringPtr m_behaviorPath;
   hkStringPtr m_characterPath;
   hkStringPtr m_fullPathToSource;
   hkStringPtr m_rootPath;
};
extern const hkClass hkbProjectStringDataClass;

