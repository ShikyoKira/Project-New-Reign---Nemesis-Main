#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbVariableBindingSet_2.h"

class hkbBindable : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBindable(void) {}
   HK_FORCE_INLINE hkbBindable( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_variableBindingSet(flag)
   , m_cachedBindables(flag)
      {}
   // Properties
   hkRefPtr<hkbVariableBindingSet> m_variableBindingSet;
   hkArray<hkRefVariant> m_cachedBindables;
   hkBool m_areBindablesCached;
};
extern const hkClass hkbBindableClass;

