#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbVariableBindingSetBinding_1.h"

class hkbVariableBindingSet : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbVariableBindingSet(void) {}
   HK_FORCE_INLINE hkbVariableBindingSet( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_bindings(flag)
      {}
   // Properties
   hkArray<hkbVariableBindingSetBinding> m_bindings;
   hkInt32 m_indexOfBindingToEnable;
   hkBool m_hasOutputBinding;
};
extern const hkClass hkbVariableBindingSetClass;

