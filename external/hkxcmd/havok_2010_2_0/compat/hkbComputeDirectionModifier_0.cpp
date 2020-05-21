// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbComputeDirectionModifier_0.h"

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


static const hkInternalClassMember hkbComputeDirectionModifierClass_Members[] =
{
   { "pointIn",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_pointIn) /*48*/,HK_NULL},
   { "pointOut",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_pointOut) /*64*/,HK_NULL},
   { "groundAngleOut",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_groundAngleOut) /*80*/,HK_NULL},
   { "upAngleOut",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_upAngleOut) /*84*/,HK_NULL},
   { "verticalOffset",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_verticalOffset) /*88*/,HK_NULL},
   { "reverseGroundAngle",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_reverseGroundAngle) /*92*/,HK_NULL},
   { "reverseUpAngle",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_reverseUpAngle) /*93*/,HK_NULL},
   { "projectPoint",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_projectPoint) /*94*/,HK_NULL},
   { "normalizePoint",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_normalizePoint) /*95*/,HK_NULL},
   { "computeOnlyOnce",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_computeOnlyOnce) /*96*/,HK_NULL},
   { "computedOutput",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbComputeDirectionModifier,m_computedOutput) /*97*/,HK_NULL},
};

// Signature:  df358bd3
extern const hkClass hkbModifierClass;
extern const hkClass hkbComputeDirectionModifierClass;
const hkClass hkbComputeDirectionModifierClass(
    "hkbComputeDirectionModifier",
    &hkbModifierClass, // parent
    sizeof(hkbComputeDirectionModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbComputeDirectionModifierClass_Members), HK_COUNT_OF(hkbComputeDirectionModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbComputeDirectionModifier, hkbComputeDirectionModifier);

