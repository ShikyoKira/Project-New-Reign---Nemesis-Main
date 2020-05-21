// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbHandIkModifier_0.h"

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

extern const hkClassEnum* BlendCurveEnum;

static const hkInternalClassMember hkbHandIkModifierClass_Members[] =
{
   { "hands",&hkbHandIkModifierHandClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkModifier,m_hands) /*44*/,HK_NULL},
   { "fadeInOutCurve",HK_NULL,BlendCurveEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkModifier,m_fadeInOutCurve) /*56*/,HK_NULL},
   { "internalHandData",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbHandIkModifier,m_internalHandData) /*60*/,HK_NULL},
};

// Signature:  ef8bc2f7
extern const hkClass hkbModifierClass;
extern const hkClass hkbHandIkModifierClass;
const hkClass hkbHandIkModifierClass(
    "hkbHandIkModifier",
    &hkbModifierClass, // parent
    sizeof(hkbHandIkModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbHandIkModifierClass_Members), HK_COUNT_OF(hkbHandIkModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbHandIkModifier, hkbHandIkModifier);

