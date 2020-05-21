// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbGeneratorTransitionEffectInternalState_0.h"

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

extern const hkClassEnum* ToGeneratorStateEnum;
extern const hkClassEnum* StageEnum;

static const hkInternalClassMember hkbGeneratorTransitionEffectInternalStateClass_Members[] =
{
   { "timeInTransition",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_timeInTransition) /*8*/,HK_NULL},
   { "duration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_duration) /*12*/,HK_NULL},
   { "effectiveBlendInDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_effectiveBlendInDuration) /*16*/,HK_NULL},
   { "effectiveBlendOutDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_effectiveBlendOutDuration) /*20*/,HK_NULL},
   { "toGeneratorState",HK_NULL,ToGeneratorStateEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_toGeneratorState) /*24*/,HK_NULL},
   { "echoTransitionGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_echoTransitionGenerator) /*25*/,HK_NULL},
   { "echoToGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_echoToGenerator) /*26*/,HK_NULL},
   { "justActivated",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_justActivated) /*27*/,HK_NULL},
   { "updateActiveNodes",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_updateActiveNodes) /*28*/,HK_NULL},
   { "stage",HK_NULL,StageEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbGeneratorTransitionEffectInternalState,m_stage) /*29*/,HK_NULL},
};

// Signature:  d6692b5d
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbGeneratorTransitionEffectInternalStateClass;
const hkClass hkbGeneratorTransitionEffectInternalStateClass(
    "hkbGeneratorTransitionEffectInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbGeneratorTransitionEffectInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbGeneratorTransitionEffectInternalStateClass_Members), HK_COUNT_OF(hkbGeneratorTransitionEffectInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbGeneratorTransitionEffectInternalState, hkbGeneratorTransitionEffectInternalState);

