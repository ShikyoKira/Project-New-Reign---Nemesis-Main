// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbSimulationControlCommand_0.h"

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


static const hkInternalClassEnumItem SimulationControlCommandEnumItems[] =
{
   {0, "COMMAND_PLAY"},
   {1, "COMMAND_PAUSE"},
   {2, "COMMAND_STEP"},
   {3, "COMMAND_STOP"},
   {4, "COMMAND_ACCUMULATE_MOTION"},
   {5, "COMMAND_DO_NOT_ACCUMULATE_MOTION"},
};

static const hkInternalClassEnum hkbSimulationControlCommandClass_Enums[] = {
   {"SimulationControlCommand", SimulationControlCommandEnumItems, _countof(SimulationControlCommandEnumItems), HK_NULL, 0 },
};
const hkClassEnum* SimulationControlCommandEnum = reinterpret_cast<const hkClassEnum*>(&hkbSimulationControlCommandClass_Enums[0]);

static const hkInternalClassMember hkbSimulationControlCommandClass_Members[] =
{
   { "command",HK_NULL,SimulationControlCommandEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSimulationControlCommand,m_command) /*8*/,HK_NULL},
};

// Signature:  2a241367
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbSimulationControlCommandClass;
const hkClass hkbSimulationControlCommandClass(
    "hkbSimulationControlCommand",
    &hkReferencedObjectClass, // parent
    sizeof(hkbSimulationControlCommand),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbSimulationControlCommandClass_Enums), HK_COUNT_OF(hkbSimulationControlCommandClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbSimulationControlCommandClass_Members), HK_COUNT_OF(hkbSimulationControlCommandClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbSimulationControlCommand, hkbSimulationControlCommand);

