// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbNode_1.h"

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


static const hkInternalClassEnumItem GetChildrenFlagBitsEnumItems[] =
{
   {1, "FLAG_ACTIVE_ONLY"},
   {2, "FLAG_GENERATORS_ONLY"},
   {4, "FLAG_IGNORE_REFERENCED_BEHAVIORS"},
};

static const hkInternalClassEnumItem CloneStateEnumItems[] =
{
   {0, "CLONE_STATE_DEFAULT"},
   {1, "CLONE_STATE_TEMPLATE"},
   {2, "CLONE_STATE_CLONE"},
   {3, "CLONE_STATE_SHARABLE"},
};

static const hkInternalClassEnum hkbNodeClass_Enums[] = {
   {"GetChildrenFlagBits", GetChildrenFlagBitsEnumItems, _countof(GetChildrenFlagBitsEnumItems), HK_NULL, 0 },
   {"CloneState", CloneStateEnumItems, _countof(CloneStateEnumItems), HK_NULL, 0 },
};
const hkClassEnum* GetChildrenFlagBitsEnum = reinterpret_cast<const hkClassEnum*>(&hkbNodeClass_Enums[0]);
const hkClassEnum* CloneStateEnum = reinterpret_cast<const hkClassEnum*>(&hkbNodeClass_Enums[1]);

static const hkInternalClassMember hkbNodeClass_Members[] =
{
   { "userData",HK_NULL,HK_NULL,hkClassMember::TYPE_ULONG,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbNode,m_userData) /*28*/,HK_NULL},
   { "name",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbNode,m_name) /*32*/,HK_NULL},
   { "id",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbNode,m_id) /*36*/,HK_NULL},
   { "cloneState",HK_NULL,HK_NULL,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbNode,m_cloneState) /*38*/,HK_NULL},
   { "padNode",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,1,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbNode,m_padNode) /*39*/,HK_NULL},
};

// Signature:  6d26f61d
extern const hkClass hkbBindableClass;
extern const hkClass hkbNodeClass;
const hkClass hkbNodeClass(
    "hkbNode",
    &hkbBindableClass, // parent
    sizeof(hkbNode),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbNodeClass_Enums), HK_COUNT_OF(hkbNodeClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbNodeClass_Members), HK_COUNT_OF(hkbNodeClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbNode, hkbNode);

