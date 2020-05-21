#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>
#include "hkbCharacterDataCharacterControllerInfo_0.h"
#include "hkbVariableInfo_1.h"
#include "hkbVariableValueSet_0.h"
#include "hkbFootIkDriverInfo_0.h"
#include "hkbHandIkDriverInfo_0.h"
#include "hkbCharacterStringData_5.h"
#include "hkbMirroredSkeletonInfo_0.h"

class hkbCharacterData : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterData(void) {}
   HK_FORCE_INLINE hkbCharacterData( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_characterPropertyInfos(flag)
   , m_numBonesPerLod(flag)
   , m_characterPropertyValues(flag)
   , m_footIkDriverInfo(flag)
   , m_handIkDriverInfo(flag)
   , m_stringData(flag)
   , m_mirroredSkeletonInfo(flag)
      {}
   // Properties
   hkbCharacterDataCharacterControllerInfo m_characterControllerInfo;
   hkVector4 m_modelUpMS;
   hkVector4 m_modelForwardMS;
   hkVector4 m_modelRightMS;
   hkArray<hkbVariableInfo> m_characterPropertyInfos;
   hkArray<hkInt32> m_numBonesPerLod;
   hkRefPtr<hkbVariableValueSet> m_characterPropertyValues;
   hkRefPtr<hkbFootIkDriverInfo> m_footIkDriverInfo;
   hkRefPtr<hkbHandIkDriverInfo> m_handIkDriverInfo;
   hkRefPtr<hkbCharacterStringData> m_stringData;
   hkRefPtr<hkbMirroredSkeletonInfo> m_mirroredSkeletonInfo;
   hkReal m_scale;
   hkInt16 m_numHands;
   hkInt16 m_numFloatSlots;
};
extern const hkClass hkbCharacterDataClass;

