// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSCyclicBlendTransitionGenerator_1.h"

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


static const hkInternalClassEnumItem CurrentBlendModeEnumItems[] =
{
   {-1, "MODE_INACTIVE"},
   {0, "MODE_DEFAULT"},
   {1, "MODE_FROZEN"},
   {2, "MODE_BLENDING"},
   {3, "MODE_WAITINGFORBLENDING"},
};

static const hkInternalClassEnum BSCyclicBlendTransitionGeneratorClass_Enums[] = {
   {"CurrentBlendMode", CurrentBlendModeEnumItems, _countof(CurrentBlendModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* CurrentBlendModeEnum = reinterpret_cast<const hkClassEnum*>(&BSCyclicBlendTransitionGeneratorClass_Enums[0]);
extern const hkClassEnum* BlendCurveEnum;

static const hkInternalClassMember BSCyclicBlendTransitionGeneratorClass_Members[] =
{
   { "pBlenderGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_pBlenderGenerator) /*48*/,HK_NULL},
   { "EventToFreezeBlendValue",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_EventToFreezeBlendValue) /*52*/,HK_NULL},
   { "EventToCrossBlend",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_EventToCrossBlend) /*60*/,HK_NULL},
   { "fBlendParameter",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_fBlendParameter) /*68*/,HK_NULL},
   { "fTransitionDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_fTransitionDuration) /*72*/,HK_NULL},
   { "eBlendCurve",HK_NULL,BlendCurveEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_eBlendCurve) /*76*/,HK_NULL},
   { "pTransitionBlenderGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::ALIGN_16 | hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_pTransitionBlenderGenerator) /*80*/,HK_NULL},
   { "pTransitionEffect",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::ALIGN_16 | hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_pTransitionEffect) /*96*/,HK_NULL},
   { "currentMode",HK_NULL,HK_NULL,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSCyclicBlendTransitionGenerator,m_currentMode) /*100*/,HK_NULL},
};

// Signature:  5119eb06
extern const hkClass hkbGeneratorClass;
extern const hkClass BSCyclicBlendTransitionGeneratorClass;
const hkClass BSCyclicBlendTransitionGeneratorClass(
    "BSCyclicBlendTransitionGenerator",
    &hkbGeneratorClass, // parent
    sizeof(BSCyclicBlendTransitionGenerator),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(BSCyclicBlendTransitionGeneratorClass_Enums), HK_COUNT_OF(BSCyclicBlendTransitionGeneratorClass_Enums),
    reinterpret_cast<const hkClassMember*>(BSCyclicBlendTransitionGeneratorClass_Members), HK_COUNT_OF(BSCyclicBlendTransitionGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSCyclicBlendTransitionGenerator, BSCyclicBlendTransitionGenerator);

