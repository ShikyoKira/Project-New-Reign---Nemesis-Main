// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBlenderGeneratorChild_2.h"

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


static const hkInternalClassMember hkbBlenderGeneratorChildClass_Members[] =
{
   { "generator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(hkbBlenderGeneratorChild,m_generator) /*32*/,HK_NULL},
   { "boneWeights",&hkbBoneWeightArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorChild,m_boneWeights) /*36*/,HK_NULL},
   { "weight",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorChild,m_weight) /*40*/,HK_NULL},
   { "worldFromModelWeight",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGeneratorChild,m_worldFromModelWeight) /*44*/,HK_NULL},
};

// Signature:  e2b384b0
extern const hkClass hkbBindableClass;
extern const hkClass hkbBlenderGeneratorChildClass;
const hkClass hkbBlenderGeneratorChildClass(
    "hkbBlenderGeneratorChild",
    &hkbBindableClass, // parent
    sizeof(hkbBlenderGeneratorChild),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBlenderGeneratorChildClass_Members), HK_COUNT_OF(hkbBlenderGeneratorChildClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBlenderGeneratorChild, hkbBlenderGeneratorChild);

