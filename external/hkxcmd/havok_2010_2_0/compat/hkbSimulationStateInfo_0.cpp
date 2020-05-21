// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbSimulationStateInfo_0.h"

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

extern const hkClassEnum* SimulationStateEnum;

static const hkInternalClassMember hkbSimulationStateInfoClass_Members[] =
{
   { "simulationState",HK_NULL,SimulationStateEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSimulationStateInfo,m_simulationState) /*8*/,HK_NULL},
};

// Signature:  a40822b4
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbSimulationStateInfoClass;
const hkClass hkbSimulationStateInfoClass(
    "hkbSimulationStateInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbSimulationStateInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbSimulationStateInfoClass_Members), HK_COUNT_OF(hkbSimulationStateInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbSimulationStateInfo, hkbSimulationStateInfo);

