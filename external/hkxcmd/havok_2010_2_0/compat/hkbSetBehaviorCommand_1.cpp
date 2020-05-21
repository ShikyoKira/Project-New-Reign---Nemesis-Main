// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbSetBehaviorCommand_1.h"

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


static const hkInternalClassMember hkbSetBehaviorCommandClass_Members[] =
{
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetBehaviorCommand,m_characterId) /*8*/,HK_NULL},
   { "behavior",&hkbBehaviorGraphClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetBehaviorCommand,m_behavior) /*16*/,HK_NULL},
   { "rootGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetBehaviorCommand,m_rootGenerator) /*20*/,HK_NULL},
   { "referencedBehaviors",&hkbBehaviorGraphClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetBehaviorCommand,m_referencedBehaviors) /*24*/,HK_NULL},
   { "startStateIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetBehaviorCommand,m_startStateIndex) /*36*/,HK_NULL},
   { "randomizeSimulation",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetBehaviorCommand,m_randomizeSimulation) /*40*/,HK_NULL},
   { "padding",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetBehaviorCommand,m_padding) /*44*/,HK_NULL},
};

// Signature:  e18b74b9
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbSetBehaviorCommandClass;
const hkClass hkbSetBehaviorCommandClass(
    "hkbSetBehaviorCommand",
    &hkReferencedObjectClass, // parent
    sizeof(hkbSetBehaviorCommand),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbSetBehaviorCommandClass_Members), HK_COUNT_OF(hkbSetBehaviorCommandClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbSetBehaviorCommand, hkbSetBehaviorCommand);

