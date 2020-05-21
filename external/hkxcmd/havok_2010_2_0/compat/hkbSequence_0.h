#pragma once

#include <Common/Base/hkBase.h>
#include "hkbModifier_0.h"
#include "hkbEventSequencedData_0.h"
#include "hkbRealVariableSequencedData_0.h"
#include "hkbBoolVariableSequencedData_0.h"
#include "hkbIntVariableSequencedData_0.h"
#include "hkbSequenceStringData_0.h"

class hkbSequence : public hkbModifier
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbSequence(void) {}
   HK_FORCE_INLINE hkbSequence( hkFinishLoadedObjectFlag flag ) : hkbModifier(flag) 
   , m_eventSequencedData(flag)
   , m_realVariableSequencedData(flag)
   , m_boolVariableSequencedData(flag)
   , m_intVariableSequencedData(flag)
   , m_stringData(flag)
   , m_variableIdMap(flag)
   , m_eventIdMap(flag)
   , m_nextSampleEvents(flag)
   , m_nextSampleReals(flag)
   , m_nextSampleBools(flag)
   , m_nextSampleInts(flag)
      {}
   // Properties
   hkArray<hkbEventSequencedData*> m_eventSequencedData;
   hkArray<hkbRealVariableSequencedData*> m_realVariableSequencedData;
   hkArray<hkbBoolVariableSequencedData*> m_boolVariableSequencedData;
   hkArray<hkbIntVariableSequencedData*> m_intVariableSequencedData;
   hkInt32 m_enableEventId;
   hkInt32 m_disableEventId;
   hkRefPtr<hkbSequenceStringData> m_stringData;
   hkRefVariant m_variableIdMap;
   hkRefVariant m_eventIdMap;
   hkArray<hkRefVariant> m_nextSampleEvents;
   hkArray<hkRefVariant> m_nextSampleReals;
   hkArray<hkRefVariant> m_nextSampleBools;
   hkArray<hkRefVariant> m_nextSampleInts;
   hkReal m_time;
   hkBool m_isEnabled;
};
extern const hkClass hkbSequenceClass;

