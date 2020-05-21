// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBlendingTransitionEffectInternalState_0.h"

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


static const hkInternalClassMember hkbBlendingTransitionEffectInternalStateClass_Members[] =
{
   { "characterPoseAtBeginningOfTransition",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_QSTRANSFORM,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffectInternalState,m_characterPoseAtBeginningOfTransition) /*8*/,HK_NULL},
   { "timeRemaining",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffectInternalState,m_timeRemaining) /*20*/,HK_NULL},
   { "timeInTransition",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffectInternalState,m_timeInTransition) /*24*/,HK_NULL},
   { "applySelfTransition",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffectInternalState,m_applySelfTransition) /*28*/,HK_NULL},
   { "initializeCharacterPose",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBlendingTransitionEffectInternalState,m_initializeCharacterPose) /*29*/,HK_NULL},
};

// Signature:  b18c70c2
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbBlendingTransitionEffectInternalStateClass;
const hkClass hkbBlendingTransitionEffectInternalStateClass(
    "hkbBlendingTransitionEffectInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbBlendingTransitionEffectInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbBlendingTransitionEffectInternalStateClass_Members), HK_COUNT_OF(hkbBlendingTransitionEffectInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBlendingTransitionEffectInternalState, hkbBlendingTransitionEffectInternalState);

