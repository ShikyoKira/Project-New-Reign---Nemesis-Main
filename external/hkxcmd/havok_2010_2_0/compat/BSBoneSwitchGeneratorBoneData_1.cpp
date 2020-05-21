// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSBoneSwitchGeneratorBoneData_1.h"

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


static const hkInternalClassMember BSBoneSwitchGeneratorBoneDataClass_Members[] =
{
   { "pGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSBoneSwitchGeneratorBoneData,m_pGenerator) /*32*/,HK_NULL},
   { "spBoneWeight",&hkbBoneWeightArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSBoneSwitchGeneratorBoneData,m_spBoneWeight) /*36*/,HK_NULL},
};

// Signature:  c1215be6
extern const hkClass hkbBindableClass;
extern const hkClass BSBoneSwitchGeneratorBoneDataClass;
const hkClass BSBoneSwitchGeneratorBoneDataClass(
    "BSBoneSwitchGeneratorBoneData",
    &hkbBindableClass, // parent
    sizeof(BSBoneSwitchGeneratorBoneData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSBoneSwitchGeneratorBoneDataClass_Members), HK_COUNT_OF(BSBoneSwitchGeneratorBoneDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSBoneSwitchGeneratorBoneData, BSBoneSwitchGeneratorBoneData);

