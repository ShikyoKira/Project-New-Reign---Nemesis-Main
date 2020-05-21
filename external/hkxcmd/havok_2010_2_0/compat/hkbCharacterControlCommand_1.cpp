// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacterControlCommand_1.h"

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


static const hkInternalClassEnumItem CharacterControlCommandEnumItems[] =
{
   {0, "COMMAND_HIDE"},
   {1, "COMMAND_SHOW"},
};

static const hkInternalClassEnum hkbCharacterControlCommandClass_Enums[] = {
   {"CharacterControlCommand", CharacterControlCommandEnumItems, _countof(CharacterControlCommandEnumItems), HK_NULL, 0 },
};
const hkClassEnum* CharacterControlCommandEnum = reinterpret_cast<const hkClassEnum*>(&hkbCharacterControlCommandClass_Enums[0]);

static const hkInternalClassMember hkbCharacterControlCommandClass_Members[] =
{
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControlCommand,m_characterId) /*8*/,HK_NULL},
   { "command",HK_NULL,CharacterControlCommandEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControlCommand,m_command) /*16*/,HK_NULL},
   { "padding",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterControlCommand,m_padding) /*20*/,HK_NULL},
};

// Signature:  7a195d1d
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCharacterControlCommandClass;
const hkClass hkbCharacterControlCommandClass(
    "hkbCharacterControlCommand",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCharacterControlCommand),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbCharacterControlCommandClass_Enums), HK_COUNT_OF(hkbCharacterControlCommandClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbCharacterControlCommandClass_Members), HK_COUNT_OF(hkbCharacterControlCommandClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacterControlCommand, hkbCharacterControlCommand);

