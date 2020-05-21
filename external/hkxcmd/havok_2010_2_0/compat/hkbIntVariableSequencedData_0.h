#pragma once

#include <Common/Base/hkBase.h>
#include "hkbSequencedData_0.h"
#include "hkbIntVariableSequencedDataSample_0.h"

class hkbIntVariableSequencedData : public hkbSequencedData
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbIntVariableSequencedData(void) {}
   HK_FORCE_INLINE hkbIntVariableSequencedData( hkFinishLoadedObjectFlag flag ) : hkbSequencedData(flag) 
   , m_samples(flag)
      {}
   // Properties
   hkArray<hkbIntVariableSequencedDataSample> m_samples;
   hkInt32 m_variableIndex;
};
extern const hkClass hkbIntVariableSequencedDataClass;

