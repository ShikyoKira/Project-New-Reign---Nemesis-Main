#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbVariableValue_0.h"

class hkbVariableValueSet : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbVariableValueSet(void) {}
   HK_FORCE_INLINE hkbVariableValueSet( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_wordVariableValues(flag)
   , m_quadVariableValues(flag)
   , m_variantVariableValues(flag)
      {}
   // Properties
   hkArray<hkbVariableValue> m_wordVariableValues;
   hkArray<hkVector4> m_quadVariableValues;
   hkArray<hkReferencedObject*> m_variantVariableValues;
};
extern const hkClass hkbVariableValueSetClass;

