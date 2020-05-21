// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSBoneSwitchGenerator_1.h"

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


static const hkInternalClassMember BSBoneSwitchGeneratorClass_Members[] =
{
   { "pDefaultGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSBoneSwitchGenerator,m_pDefaultGenerator) /*48*/,HK_NULL},
   { "ChildrenA",&BSBoneSwitchGeneratorBoneDataClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSBoneSwitchGenerator,m_ChildrenA) /*52*/,HK_NULL},
};

// Signature:  f33d3eea
extern const hkClass hkbGeneratorClass;
extern const hkClass BSBoneSwitchGeneratorClass;
const hkClass BSBoneSwitchGeneratorClass(
    "BSBoneSwitchGenerator",
    &hkbGeneratorClass, // parent
    sizeof(BSBoneSwitchGenerator),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSBoneSwitchGeneratorClass_Members), HK_COUNT_OF(BSBoneSwitchGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSBoneSwitchGenerator, BSBoneSwitchGenerator);

