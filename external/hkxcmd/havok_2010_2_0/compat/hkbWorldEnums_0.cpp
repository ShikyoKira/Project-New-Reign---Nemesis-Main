// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbWorldEnums_0.h"

#include <Common/Serialize/hkSerialize.h>
#include <Common/Serialize/Util/hkSerializeUtil.h>
#include <Common/Serialize/Version/hkVersionPatchManager.h>
#include <Common/Serialize/Data/Dict/hkDataObjectDict.h>
#include <Common/Serialize/Data/Native/hkDataObjectNative.h>
#include <Common/Serialize/Data/Util/hkDataObjectUtil.h>
#include <Common/Base/Reflection/Registry/hkDynamicClassNameRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Serialize/Util/hkSerializationCheckingUtils.h>
#include <Common/Serialize/Util/hkVersionCheckingUtils.h>


static const hkInternalClassEnumItem SimulationStateEnumItems[] =
{
   {0, "SIMULATION_STATE_PLAY"},
   {1, "SIMULATION_STATE_PAUSE"},
   {2, "SIMULATION_STATE_STEP"},
   {3, "SIMULATION_STATE_STOP"},
};

static const hkInternalClassEnumItem AccumulateMotionStateEnumItems[] =
{
   {0, "ACCUMULATE_MOTION"},
   {1, "DO_NOT_ACCUMULATE_MOTION"},
};

static const hkInternalClassEnum hkbWorldEnumsClass_Enums[] = {
   {"SimulationState", SimulationStateEnumItems, _countof(SimulationStateEnumItems), HK_NULL, 0 },
   {"AccumulateMotionState", AccumulateMotionStateEnumItems, _countof(AccumulateMotionStateEnumItems), HK_NULL, 0 },
};
extern const hkClassEnum* SimulationStateEnum;
extern const hkClassEnum* AccumulateMotionStateEnum;

const hkClassEnum* SimulationStateEnum = reinterpret_cast<const hkClassEnum*>(&hkbWorldEnumsClass_Enums[0]);
const hkClassEnum* AccumulateMotionStateEnum = reinterpret_cast<const hkClassEnum*>(&hkbWorldEnumsClass_Enums[1]);

// Signature:  25640b46
extern const hkClass hkbWorldEnumsClass;
const hkClass hkbWorldEnumsClass(
    "hkbWorldEnums",
    HK_NULL, // parent
    sizeof(hkbWorldEnums),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbWorldEnumsClass_Enums), HK_COUNT_OF(hkbWorldEnumsClass_Enums),
    HK_NULL, 0, // members
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbWorldEnums, hkbWorldEnums);

