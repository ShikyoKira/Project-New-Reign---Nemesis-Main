// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbHandIkDriverInfo_0.h"

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

static const hkInternalClassMember hkbHandIkDriverInfoClass_Members[] =
{
   { "hands",&hkbHandIkDriverInfoHandClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfo,m_hands) /*8*/,HK_NULL},
   { "fadeInOutCurve",HK_NULL,BlendCurveEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbHandIkDriverInfo,m_fadeInOutCurve) /*20*/,HK_NULL},
};

// Signature:  c299090a
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbHandIkDriverInfoClass;
const hkClass hkbHandIkDriverInfoClass(
    "hkbHandIkDriverInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbHandIkDriverInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbHandIkDriverInfoClass_Members), HK_COUNT_OF(hkbHandIkDriverInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbHandIkDriverInfo, hkbHandIkDriverInfo);

