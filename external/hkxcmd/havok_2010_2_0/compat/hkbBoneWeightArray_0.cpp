// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBoneWeightArray_0.h"

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


static const hkInternalClassMember hkbBoneWeightArrayClass_Members[] =
{
   { "boneWeights",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_REAL,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBoneWeightArray,m_boneWeights) /*28*/,HK_NULL},
};

// Signature:  cd902b77
extern const hkClass hkbBindableClass;
extern const hkClass hkbBoneWeightArrayClass;
const hkClass hkbBoneWeightArrayClass(
    "hkbBoneWeightArray",
    &hkbBindableClass, // parent
    sizeof(hkbBoneWeightArray),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBoneWeightArrayClass_Members), HK_COUNT_OF(hkbBoneWeightArrayClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBoneWeightArray, hkbBoneWeightArray);

