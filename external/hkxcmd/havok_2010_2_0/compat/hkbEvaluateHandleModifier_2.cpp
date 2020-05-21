// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbEvaluateHandleModifier_2.h"

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

static const hkInternalClassEnum hkbEvaluateHandleModifierClass_Enums[] = {
   {"HandleChangeMode", HandleChangeModeEnumItems, _countof(HandleChangeModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* HandleChangeModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbEvaluateHandleModifierClass_Enums[0]);

static const hkInternalClassMember hkbEvaluateHandleModifierClass_Members[] =
{
   { "handle",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_handle) /*44*/,HK_NULL},
   { "handlePositionOut",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_handlePositionOut) /*48*/,HK_NULL},
   { "handleRotationOut",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_handleRotationOut) /*64*/,HK_NULL},
   { "isValidOut",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_isValidOut) /*80*/,HK_NULL},
   { "extrapolationTimeStep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_extrapolationTimeStep) /*84*/,HK_NULL},
   { "handleChangeSpeed",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_handleChangeSpeed) /*88*/,HK_NULL},
   { "handleChangeMode",HK_NULL,HandleChangeModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_handleChangeMode) /*92*/,HK_NULL},
   { "oldHandle",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_oldHandle) /*96*/,HK_NULL},
   { "oldHandlePosition",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_oldHandlePosition) /*128*/,HK_NULL},
   { "oldHandleRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_oldHandleRotation) /*144*/,HK_NULL},
   { "timeSinceLastModify",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_timeSinceLastModify) /*160*/,HK_NULL},
   { "smoothlyChangingHandles",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbEvaluateHandleModifier,m_smoothlyChangingHandles) /*164*/,HK_NULL},
};

// Signature:  79757102
extern const hkClass hkbModifierClass;
extern const hkClass hkbEvaluateHandleModifierClass;
const hkClass hkbEvaluateHandleModifierClass(
    "hkbEvaluateHandleModifier",
    &hkbModifierClass, // parent
    sizeof(hkbEvaluateHandleModifier),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbEvaluateHandleModifierClass_Enums), HK_COUNT_OF(hkbEvaluateHandleModifierClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbEvaluateHandleModifierClass_Members), HK_COUNT_OF(hkbEvaluateHandleModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbEvaluateHandleModifier, hkbEvaluateHandleModifier);

