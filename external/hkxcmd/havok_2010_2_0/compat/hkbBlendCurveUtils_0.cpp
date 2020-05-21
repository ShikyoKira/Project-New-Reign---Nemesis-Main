// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBlendCurveUtils_0.h"

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


static const hkInternalClassEnumItem BlendCurveEnumItems[] =
{
   {0, "BLEND_CURVE_SMOOTH"},
   {1, "BLEND_CURVE_LINEAR"},
   {2, "BLEND_CURVE_LINEAR_TO_SMOOTH"},
   {3, "BLEND_CURVE_SMOOTH_TO_LINEAR"},
};

static const hkInternalClassEnum hkbBlendCurveUtilsClass_Enums[] = {
   {"BlendCurve", BlendCurveEnumItems, _countof(BlendCurveEnumItems), HK_NULL, 0 },
};
extern const hkClassEnum* BlendCurveEnum;
const hkClassEnum* BlendCurveEnum = reinterpret_cast<const hkClassEnum*>(&hkbBlendCurveUtilsClass_Enums[0]);

// Signature:  23041af0
extern const hkClass hkbBlendCurveUtilsClass;
const hkClass hkbBlendCurveUtilsClass(
    "hkbBlendCurveUtils",
    HK_NULL, // parent
    sizeof(hkbBlendCurveUtils),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbBlendCurveUtilsClass_Enums), HK_COUNT_OF(hkbBlendCurveUtilsClass_Enums),
    HK_NULL, 0, // members
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbBlendCurveUtils, hkbBlendCurveUtils);

