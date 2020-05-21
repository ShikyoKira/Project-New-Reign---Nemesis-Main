// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBlenderGenerator_1.h"

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


static const hkInternalClassEnumItem BlenderFlagsEnumItems[] =
{
   {1, "FLAG_SYNC"},
   {4, "FLAG_SMOOTH_GENERATOR_WEIGHTS"},
   {8, "FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS"},
   {16, "FLAG_PARAMETRIC_BLEND"},
   {32, "FLAG_IS_PARAMETRIC_BLEND_CYCLIC"},
   {64, "FLAG_FORCE_DENSE_POSE"},
};

static const hkInternalClassEnum hkbBlenderGeneratorClass_Enums[] = {
   {"BlenderFlags", BlenderFlagsEnumItems, _countof(BlenderFlagsEnumItems), HK_NULL, 0 },
};
const hkClassEnum* BlenderFlagsEnum = reinterpret_cast<const hkClassEnum*>(&hkbBlenderGeneratorClass_Enums[0]);

static const hkInternalClassMember hkbBlenderGeneratorClass_Members[] =
{
   { "referencePoseWeightThreshold",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGenerator,m_referencePoseWeightThreshold) /*40*/,HK_NULL},
   { "blendParameter",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGenerator,m_blendParameter) /*44*/,HK_NULL},
   { "minCyclicBlendParameter",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGenerator,m_minCyclicBlendParameter) /*48*/,HK_NULL},
   { "maxCyclicBlendParameter",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGenerator,m_maxCyclicBlendParameter) /*52*/,HK_NULL},
   { "indexOfSyncMasterChild",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGenerator,m_indexOfSyncMasterChild) /*56*/,HK_NULL},
   { "flags",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGenerator,m_flags) /*58*/,HK_NULL},
   { "subtractLastChild",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGenerator,m_subtractLastChild) /*60*/,HK_NULL},
   { "children",&hkbBlenderGeneratorChildClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlenderGenerator,m_children) /*64*/,HK_NULL},
   { "childrenInternalStates",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlenderGenerator,m_childrenInternalStates) /*76*/,HK_NULL},
   { "sortedChildren",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlenderGenerator,m_sortedChildren) /*88*/,HK_NULL},
   { "endIntervalWeight",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlenderGenerator,m_endIntervalWeight) /*100*/,HK_NULL},
   { "numActiveChildren",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlenderGenerator,m_numActiveChildren) /*104*/,HK_NULL},
   { "beginIntervalIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlenderGenerator,m_beginIntervalIndex) /*108*/,HK_NULL},
   { "endIntervalIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlenderGenerator,m_endIntervalIndex) /*110*/,HK_NULL},
   { "initSync",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlenderGenerator,m_initSync) /*112*/,HK_NULL},
   { "doSubtractiveBlend",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBlenderGenerator,m_doSubtractiveBlend) /*113*/,HK_NULL},
};

// Signature:  22df7147
extern const hkClass hkbGeneratorClass;
extern const hkClass hkbBlenderGeneratorClass;
const hkClass hkbBlenderGeneratorClass(
    "hkbBlenderGenerator",
    &hkbGeneratorClass, // parent
    sizeof(hkbBlenderGenerator),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbBlenderGeneratorClass_Enums), HK_COUNT_OF(hkbBlenderGeneratorClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbBlenderGeneratorClass_Members), HK_COUNT_OF(hkbBlenderGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBlenderGenerator, hkbBlenderGenerator);

