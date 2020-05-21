// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbVariableBindingSetBinding_1.h"

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


static const hkInternalClassEnumItem BindingTypeEnumItems[] =
{
   {0, "BINDING_TYPE_VARIABLE"},
   {1, "BINDING_TYPE_CHARACTER_PROPERTY"},
};

static const hkInternalClassEnumItem InternalBindingFlagsEnumItems[] =
{
   {0, "FLAG_NONE"},
   {1, "FLAG_OUTPUT"},
};

static const hkInternalClassEnum hkbVariableBindingSetBindingClass_Enums[] = {
   {"BindingType", BindingTypeEnumItems, _countof(BindingTypeEnumItems), HK_NULL, 0 },
   {"InternalBindingFlags", InternalBindingFlagsEnumItems, _countof(InternalBindingFlagsEnumItems), HK_NULL, 0 },
};
const hkClassEnum* BindingTypeEnum = reinterpret_cast<const hkClassEnum*>(&hkbVariableBindingSetBindingClass_Enums[0]);
const hkClassEnum* InternalBindingFlagsEnum = reinterpret_cast<const hkClassEnum*>(&hkbVariableBindingSetBindingClass_Enums[1]);

static const hkInternalClassMember hkbVariableBindingSetBindingClass_Members[] =
{
   { "memberPath",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_memberPath) /*0*/,HK_NULL},
   { "memberClass",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_memberClass) /*4*/,HK_NULL},
   { "offsetInObjectPlusOne",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_offsetInObjectPlusOne) /*8*/,HK_NULL},
   { "offsetInArrayPlusOne",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_offsetInArrayPlusOne) /*12*/,HK_NULL},
   { "rootVariableIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_rootVariableIndex) /*16*/,HK_NULL},
   { "variableIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_variableIndex) /*20*/,HK_NULL},
   { "bitIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT8,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_bitIndex) /*24*/,HK_NULL},
   { "bindingType",HK_NULL,BindingTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_bindingType) /*25*/,HK_NULL},
   { "memberType",HK_NULL,HK_NULL,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_memberType) /*26*/,HK_NULL},
   { "variableType",HK_NULL,HK_NULL,hkClassMember::TYPE_INT8,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_variableType) /*27*/,HK_NULL},
   { "flags",HK_NULL,HK_NULL,hkClassMember::TYPE_FLAGS,hkClassMember::TYPE_INT8,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbVariableBindingSetBinding,m_flags) /*28*/,HK_NULL},
};

// Signature:  4d592f72
extern const hkClass hkbVariableBindingSetBindingClass;
const hkClass hkbVariableBindingSetBindingClass(
    "hkbVariableBindingSetBinding",
    HK_NULL, // parent
    sizeof(hkbVariableBindingSetBinding),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbVariableBindingSetBindingClass_Enums), HK_COUNT_OF(hkbVariableBindingSetBindingClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbVariableBindingSetBindingClass_Members), HK_COUNT_OF(hkbVariableBindingSetBindingClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbVariableBindingSetBinding, hkbVariableBindingSetBinding);

