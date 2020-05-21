// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbStateMachineNestedStateMachineData_0.h"

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


static const hkInternalClassMember hkbStateMachineNestedStateMachineDataClass_Members[] =
{
   { "nestedStateMachine",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachineNestedStateMachineData,m_nestedStateMachine) /*0*/,HK_NULL},
   { "eventIdMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbStateMachineNestedStateMachineData,m_eventIdMap) /*4*/,HK_NULL},
};

// Signature:  7358f5da
extern const hkClass hkbStateMachineNestedStateMachineDataClass;
const hkClass hkbStateMachineNestedStateMachineDataClass(
    "hkbStateMachineNestedStateMachineData",
    HK_NULL, // parent
    sizeof(hkbStateMachineNestedStateMachineData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbStateMachineNestedStateMachineDataClass_Members), HK_COUNT_OF(hkbStateMachineNestedStateMachineDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbStateMachineNestedStateMachineData, hkbStateMachineNestedStateMachineData);

