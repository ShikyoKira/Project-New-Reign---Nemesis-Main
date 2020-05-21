// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbSetWordVariableCommand_0.h"

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

extern const hkClassEnum* VariableTypeEnum;

static const hkInternalClassMember hkbSetWordVariableCommandClass_Members[] =
{
   { "quadValue",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetWordVariableCommand,m_quadValue) /*16*/,HK_NULL},
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetWordVariableCommand,m_characterId) /*32*/,HK_NULL},
   { "variableId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetWordVariableCommand,m_variableId) /*40*/,HK_NULL},
   { "value",&hkbVariableValueClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetWordVariableCommand,m_value) /*44*/,HK_NULL},
   { "type",HK_NULL,VariableTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetWordVariableCommand,m_type) /*48*/,HK_NULL},
   { "global",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetWordVariableCommand,m_global) /*49*/,HK_NULL},
};

// Signature:  f3ae5fca
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbSetWordVariableCommandClass;
const hkClass hkbSetWordVariableCommandClass(
    "hkbSetWordVariableCommand",
    &hkReferencedObjectClass, // parent
    sizeof(hkbSetWordVariableCommand),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbSetWordVariableCommandClass_Members), HK_COUNT_OF(hkbSetWordVariableCommandClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbSetWordVariableCommand, hkbSetWordVariableCommand);

