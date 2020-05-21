#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbMirroredSkeletonInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbMirroredSkeletonInfo(void) {}
   HK_FORCE_INLINE hkbMirroredSkeletonInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_bonePairMap(flag)
      {}
   // Properties
   hkVector4 m_mirrorAxis;
   hkArray<hkInt16> m_bonePairMap;
};
extern const hkClass hkbMirroredSkeletonInfoClass;

