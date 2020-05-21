// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbWorldFromModelModeData_0.h"

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


static const hkInternalClassEnumItem WorldFromModelModeEnumItems[] =
{
   {0, "WORLD_FROM_MODEL_MODE_USE_OLD"},
   {1, "WORLD_FROM_MODEL_MODE_USE_INPUT"},
   {2, "WORLD_FROM_MODEL_MODE_COMPUTE"},
   {3, "WORLD_FROM_MODEL_MODE_NONE"},
   {4, "WORLD_FROM_MODEL_MODE_RAGDOLL"},
};

static const hkInternalClassEnum hkbWorldFromModelModeDataClass_Enums[] = {
   {"WorldFromModelMode", WorldFromModelModeEnumItems, _countof(WorldFromModelModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* WorldFromModelModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbWorldFromModelModeDataClass_Enums[0]);

static const hkInternalClassMember hkbWorldFromModelModeDataClass_Members[] =
{
   { "poseMatchingBone0",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbWorldFromModelModeData,m_poseMatchingBone0) /*0*/,HK_NULL},
   { "poseMatchingBone1",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbWorldFromModelModeData,m_poseMatchingBone1) /*2*/,HK_NULL},
   { "poseMatchingBone2",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbWorldFromModelModeData,m_poseMatchingBone2) /*4*/,HK_NULL},
   { "mode",HK_NULL,WorldFromModelModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbWorldFromModelModeData,m_mode) /*6*/,HK_NULL},
};

// Signature:  a3af8783
extern const hkClass hkbWorldFromModelModeDataClass;
const hkClass hkbWorldFromModelModeDataClass(
    "hkbWorldFromModelModeData",
    HK_NULL, // parent
    sizeof(hkbWorldFromModelModeData),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbWorldFromModelModeDataClass_Enums), HK_COUNT_OF(hkbWorldFromModelModeDataClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbWorldFromModelModeDataClass_Members), HK_COUNT_OF(hkbWorldFromModelModeDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbWorldFromModelModeData, hkbWorldFromModelModeData);

