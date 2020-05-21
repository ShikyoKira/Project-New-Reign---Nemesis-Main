#pragma once

#include <Common/Base/hkBase.h>
#include "hkbSequencedData_0.h"
#include "hkbBoolVariableSequencedDataSample_0.h"

class hkbBoolVariableSequencedData : public hkbSequencedData
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBoolVariableSequencedData(void) {}
   HK_FORCE_INLINE hkbBoolVariableSequencedData( hkFinishLoadedObjectFlag flag ) : hkbSequencedData(flag) 
   , m_samples(flag)
      {}
   // Properties
   hkArray<hkbBoolVariableSequencedDataSample> m_samples;
   hkInt32 m_variableIndex;
};
extern const hkClass hkbBoolVariableSequencedDataClass;

