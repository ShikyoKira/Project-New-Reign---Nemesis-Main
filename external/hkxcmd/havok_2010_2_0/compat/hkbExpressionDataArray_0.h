#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbExpressionData_0.h"

class hkbExpressionDataArray : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbExpressionDataArray(void) {}
   HK_FORCE_INLINE hkbExpressionDataArray( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_expressionsData(flag)
      {}
   // Properties
   hkArray<hkbExpressionData> m_expressionsData;
};
extern const hkClass hkbExpressionDataArrayClass;

