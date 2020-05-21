#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbVariableInfo_1.h"
#include "hkbEventInfo_0.h"
#include "hkbVariableValue_0.h"
#include "hkbVariableValueSet_0.h"
#include "hkbBehaviorGraphStringData_1.h"

class hkbBehaviorGraphData : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbBehaviorGraphData(void) {}
   HK_FORCE_INLINE hkbBehaviorGraphData( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_attributeDefaults(flag)
   , m_variableInfos(flag)
   , m_characterPropertyInfos(flag)
   , m_eventInfos(flag)
   , m_wordMinVariableValues(flag)
   , m_wordMaxVariableValues(flag)
   , m_variableInitialValues(flag)
   , m_stringData(flag)
      {}
   // Properties
   hkArray<hkReal> m_attributeDefaults;
   hkArray<hkbVariableInfo> m_variableInfos;
   hkArray<hkbVariableInfo> m_characterPropertyInfos;
   hkArray<hkbEventInfo> m_eventInfos;
   hkArray<hkbVariableValue> m_wordMinVariableValues;
   hkArray<hkbVariableValue> m_wordMaxVariableValues;
   hkRefPtr<hkbVariableValueSet> m_variableInitialValues;
   hkRefPtr<hkbBehaviorGraphStringData> m_stringData;
};
extern const hkClass hkbBehaviorGraphDataClass;

