// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSOffsetAnimationGenerator_1.h"

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


static const hkInternalClassMember BSOffsetAnimationGeneratorClass_Members[] =
{
   { "pDefaultGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_pDefaultGenerator) /*48*/,HK_NULL},
   { "pOffsetClipGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_pOffsetClipGenerator) /*64*/,HK_NULL},
   { "fOffsetVariable",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_fOffsetVariable) /*68*/,HK_NULL},
   { "fOffsetRangeStart",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_fOffsetRangeStart) /*72*/,HK_NULL},
   { "fOffsetRangeEnd",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_fOffsetRangeEnd) /*76*/,HK_NULL},
   { "BoneOffsetA",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_BoneOffsetA) /*80*/,HK_NULL},
   { "BoneIndexA",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_BoneIndexA) /*92*/,HK_NULL},
   { "fCurrentPercentage",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_fCurrentPercentage) /*104*/,HK_NULL},
   { "iCurrentFrame",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_iCurrentFrame) /*108*/,HK_NULL},
   { "bZeroOffset",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_bZeroOffset) /*112*/,HK_NULL},
   { "bOffsetValid",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSOffsetAnimationGenerator,m_bOffsetValid) /*113*/,HK_NULL},
};

// Signature:  b8571122
extern const hkClass hkbGeneratorClass;
extern const hkClass BSOffsetAnimationGeneratorClass;
const hkClass BSOffsetAnimationGeneratorClass(
    "BSOffsetAnimationGenerator",
    &hkbGeneratorClass, // parent
    sizeof(BSOffsetAnimationGenerator),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSOffsetAnimationGeneratorClass_Members), HK_COUNT_OF(BSOffsetAnimationGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSOffsetAnimationGenerator, BSOffsetAnimationGenerator);

