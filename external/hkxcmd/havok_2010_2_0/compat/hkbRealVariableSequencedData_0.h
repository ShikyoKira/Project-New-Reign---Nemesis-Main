#pragma once

#include <Common/Base/hkBase.h>
#include "hkbSequencedData_0.h"
#include "hkbRealVariableSequencedDataSample_0.h"

class hkbRealVariableSequencedData : public hkbSequencedData
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbRealVariableSequencedData(void) {}
   HK_FORCE_INLINE hkbRealVariableSequencedData( hkFinishLoadedObjectFlag flag ) : hkbSequencedData(flag) 
   , m_samples(flag)
      {}
   // Properties
   hkArray<hkbRealVariableSequencedDataSample> m_samples;
   hkInt32 m_variableIndex;
};
extern const hkClass hkbRealVariableSequencedDataClass;

