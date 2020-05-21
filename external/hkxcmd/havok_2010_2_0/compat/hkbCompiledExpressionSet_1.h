#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbCompiledExpressionSetToken_0.h"

class hkbCompiledExpressionSet : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCompiledExpressionSet(void) {}
   HK_FORCE_INLINE hkbCompiledExpressionSet( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_rpn(flag)
   , m_expressionToRpnIndex(flag)
      {}
   // Properties
   hkArray<hkbCompiledExpressionSetToken> m_rpn;
   hkArray<hkInt32> m_expressionToRpnIndex;
   hkInt8 m_numExpressions;
};
extern const hkClass hkbCompiledExpressionSetClass;

