// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbRoleAttribute_0.h"

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


static const hkInternalClassEnumItem RoleEnumItems[] =
{
   {0, "ROLE_DEFAULT"},
   {1, "ROLE_FILE_NAME"},
   {2, "ROLE_BONE_INDEX"},
   {3, "ROLE_BONE_INDEX_MAP"},
   {4, "ROLE_EVENT_ID"},
   {5, "ROLE_VARIABLE_INDEX"},
   {6, "ROLE_ATTRIBUTE_INDEX"},
   {7, "ROLE_TIME"},
};

static const hkInternalClassEnumItem RoleFlagsEnumItems[] =
{
   {0, "FLAG_NONE"},
   {1, "FLAG_RAGDOLL"},
   {2, "FLAG_NORMALIZED"},
   {4, "FLAG_NOT_VARIABLE"},
   {8, "FLAG_HIDDEN"},
   {16, "FLAG_OUTPUT"},
   {32, "FLAG_NOT_CHARACTER_PROPERTY"},
};

static const hkInternalClassEnum hkbRoleAttributeClass_Enums[] = {
   {"Role", RoleEnumItems, _countof(RoleEnumItems), HK_NULL, 0 },
   {"RoleFlags", RoleFlagsEnumItems, _countof(RoleFlagsEnumItems), HK_NULL, 0 },
};
const hkClassEnum* RoleEnum = reinterpret_cast<const hkClassEnum*>(&hkbRoleAttributeClass_Enums[0]);
const hkClassEnum* RoleFlagsEnum = reinterpret_cast<const hkClassEnum*>(&hkbRoleAttributeClass_Enums[1]);

static const hkInternalClassMember hkbRoleAttributeClass_Members[] =
{
   { "role",HK_NULL,RoleEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbRoleAttribute,m_role) /*0*/,HK_NULL},
   { "flags",HK_NULL,RoleFlagsEnum,hkClassMember::TYPE_FLAGS,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbRoleAttribute,m_flags) /*2*/,HK_NULL},
};

// Signature:  3eb2e082
extern const hkClass hkbRoleAttributeClass;
const hkClass hkbRoleAttributeClass(
    "hkbRoleAttribute",
    HK_NULL, // parent
    sizeof(hkbRoleAttribute),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbRoleAttributeClass_Enums), HK_COUNT_OF(hkbRoleAttributeClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbRoleAttributeClass_Members), HK_COUNT_OF(hkbRoleAttributeClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbRoleAttribute, hkbRoleAttribute);

