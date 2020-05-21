// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BGSGamebryoSequenceGenerator_2.h"

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


static const hkInternalClassEnumItem BlendModeFunctionEnumItems[] =
{
   {0, "BMF_NONE"},
   {1, "BMF_PERCENT"},
   {2, "BMF_ONE_MINUS_PERCENT"},
};

static const hkInternalClassEnum BGSGamebryoSequenceGeneratorClass_Enums[] = {
   {"BlendModeFunction", BlendModeFunctionEnumItems, _countof(BlendModeFunctionEnumItems), HK_NULL, 0 },
};
const hkClassEnum* BlendModeFunctionEnum = reinterpret_cast<const hkClassEnum*>(&BGSGamebryoSequenceGeneratorClass_Enums[0]);

static const hkInternalClassMember BGSGamebryoSequenceGeneratorClass_Members[] =
{
   { "pSequence",HK_NULL,HK_NULL,hkClassMember::TYPE_CSTRING,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BGSGamebryoSequenceGenerator,m_pSequence) /*40*/,HK_NULL},
   { "eBlendModeFunction",HK_NULL,BlendModeFunctionEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BGSGamebryoSequenceGenerator,m_eBlendModeFunction) /*44*/,HK_NULL},
   { "fPercent",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BGSGamebryoSequenceGenerator,m_fPercent) /*48*/,HK_NULL},
   { "events",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BGSGamebryoSequenceGenerator,m_events) /*52*/,HK_NULL},
   { "fTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BGSGamebryoSequenceGenerator,m_fTime) /*64*/,HK_NULL},
   { "bDelayedActivate",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BGSGamebryoSequenceGenerator,m_bDelayedActivate) /*68*/,HK_NULL},
   { "bLooping",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BGSGamebryoSequenceGenerator,m_bLooping) /*69*/,HK_NULL},
};

// Signature:  c8df2d77
extern const hkClass hkbGeneratorClass;
extern const hkClass BGSGamebryoSequenceGeneratorClass;
const hkClass BGSGamebryoSequenceGeneratorClass(
    "BGSGamebryoSequenceGenerator",
    &hkbGeneratorClass, // parent
    sizeof(BGSGamebryoSequenceGenerator),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(BGSGamebryoSequenceGeneratorClass_Enums), HK_COUNT_OF(BGSGamebryoSequenceGeneratorClass_Enums),
    reinterpret_cast<const hkClassMember*>(BGSGamebryoSequenceGeneratorClass_Members), HK_COUNT_OF(BGSGamebryoSequenceGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BGSGamebryoSequenceGenerator, BGSGamebryoSequenceGenerator);

