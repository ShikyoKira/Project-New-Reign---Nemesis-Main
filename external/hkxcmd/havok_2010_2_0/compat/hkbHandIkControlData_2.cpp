// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbHandIkControlData_2.h"

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


static const hkInternalClassEnumItem HandleChangeModeEnumItems[] =
{
   {0, "HANDLE_CHANGE_MODE_ABRUPT"},
   {1, "HANDLE_CHANGE_MODE_CONSTANT_VELOCITY"},
};

static const hkInternalClassEnum hkbHandIkControlDataClass_Enums[] = {
   {"HandleChangeMode", HandleChangeModeEnumItems, _countof(HandleChangeModeEnumItems), HK_NULL, 0 },
};
//const hkClassEnum* HandleChangeModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbHandIkControlDataClass_Enums[0]);
extern const hkClassEnum* HandleChangeModeEnum;

static const hkInternalClassMember hkbHandIkControlDataClass_Members[] =
{
   { "targetPosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_targetPosition) /*0*/,HK_NULL},
   { "targetRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_targetRotation) /*16*/,HK_NULL},
   { "targetNormal",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_targetNormal) /*32*/,HK_NULL},
   { "targetHandle",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_targetHandle) /*48*/,HK_NULL},
   { "transformOnFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_transformOnFraction) /*52*/,HK_NULL},
   { "normalOnFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_normalOnFraction) /*56*/,HK_NULL},
   { "fadeInDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_fadeInDuration) /*60*/,HK_NULL},
   { "fadeOutDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_fadeOutDuration) /*64*/,HK_NULL},
   { "extrapolationTimeStep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_extrapolationTimeStep) /*68*/,HK_NULL},
   { "handleChangeSpeed",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_handleChangeSpeed) /*72*/,HK_NULL},
   { "handleChangeMode",HK_NULL,HandleChangeModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_handleChangeMode) /*76*/,HK_NULL},
   { "fixUp",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkControlData,m_fixUp) /*77*/,HK_NULL},
};

// Signature:  d72b8d17
extern const hkClass hkbHandIkControlDataClass;
const hkClass hkbHandIkControlDataClass(
    "hkbHandIkControlData",
    HK_NULL, // parent
    sizeof(hkbHandIkControlData),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbHandIkControlDataClass_Enums), HK_COUNT_OF(hkbHandIkControlDataClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbHandIkControlDataClass_Members), HK_COUNT_OF(hkbHandIkControlDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbHandIkControlData, hkbHandIkControlData);

