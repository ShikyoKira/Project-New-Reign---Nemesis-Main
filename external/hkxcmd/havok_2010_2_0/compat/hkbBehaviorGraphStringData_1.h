#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbBehaviorGraphStringData : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBehaviorGraphStringData(void) {}
   HK_FORCE_INLINE hkbBehaviorGraphStringData( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_eventNames(flag)
   , m_attributeNames(flag)
   , m_variableNames(flag)
   , m_characterPropertyNames(flag)
      {}
   // Properties
   hkArray<hkStringPtr> m_eventNames;
   hkArray<hkStringPtr> m_attributeNames;
   hkArray<hkStringPtr> m_variableNames;
   hkArray<hkStringPtr> m_characterPropertyNames;
};
extern const hkClass hkbBehaviorGraphStringDataClass;

