// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbVariableInfo_1.h"

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


static const hkInternalClassEnumItem VariableTypeEnumItems[] =
{
   {-1, "VARIABLE_TYPE_INVALID"},
   {0, "VARIABLE_TYPE_BOOL"},
   {1, "VARIABLE_TYPE_INT8"},
   {2, "VARIABLE_TYPE_INT16"},
   {3, "VARIABLE_TYPE_INT32"},
   {4, "VARIABLE_TYPE_REAL"},
   {5, "VARIABLE_TYPE_POINTER"},
   {6, "VARIABLE_TYPE_VECTOR3"},
   {7, "VARIABLE_TYPE_VECTOR4"},
   {8, "VARIABLE_TYPE_QUATERNION"},
};

static const hkInternalClassEnum hkbVariableInfoClass_Enums[] = {
   {"VariableType", VariableTypeEnumItems, _countof(VariableTypeEnumItems), HK_NULL, 0 },
};
extern const hkClassEnum* VariableTypeEnum;
const hkClassEnum* VariableTypeEnum = reinterpret_cast<const hkClassEnum*>(&hkbVariableInfoClass_Enums[0]);

static const hkInternalClassMember hkbVariableInfoClass_Members[] =
{
   { "role",&hkbRoleAttributeClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbVariableInfo,m_role) /*0*/,HK_NULL},
   { "type",HK_NULL,VariableTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbVariableInfo,m_type) /*4*/,HK_NULL},
};

// Signature:  9e746ba2
extern const hkClass hkbVariableInfoClass;
const hkClass hkbVariableInfoClass(
    "hkbVariableInfo",
    HK_NULL, // parent
    sizeof(hkbVariableInfo),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbVariableInfoClass_Enums), HK_COUNT_OF(hkbVariableInfoClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbVariableInfoClass_Members), HK_COUNT_OF(hkbVariableInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbVariableInfo, hkbVariableInfo);

