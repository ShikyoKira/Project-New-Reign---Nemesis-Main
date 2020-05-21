#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbProjectStringData_1.h"
#include "hkbTransitionEffect_0.h"

class hkbProjectData : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbProjectData(void) {}
   HK_FORCE_INLINE hkbProjectData( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_stringData(flag)
      {}
   // Properties
   hkVector4 m_worldUpWS;
   hkRefPtr<hkbProjectStringData> m_stringData;
   hkEnum<hkbTransitionEffect::EventMode,hkInt8> m_defaultEventMode;
};
extern const hkClass hkbProjectDataClass;

