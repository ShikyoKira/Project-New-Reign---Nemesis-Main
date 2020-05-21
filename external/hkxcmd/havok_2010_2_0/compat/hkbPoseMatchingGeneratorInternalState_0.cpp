// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbPoseMatchingGeneratorInternalState_0.h"

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


static const hkInternalClassMember hkbPoseMatchingGeneratorInternalStateClass_Members[] =
{
   { "currentMatch",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGeneratorInternalState,m_currentMatch) /*8*/,HK_NULL},
   { "bestMatch",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGeneratorInternalState,m_bestMatch) /*12*/,HK_NULL},
   { "timeSinceBetterMatch",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGeneratorInternalState,m_timeSinceBetterMatch) /*16*/,HK_NULL},
   { "error",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGeneratorInternalState,m_error) /*20*/,HK_NULL},
   { "resetCurrentMatchLocalTime",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGeneratorInternalState,m_resetCurrentMatchLocalTime) /*24*/,HK_NULL},
};

// Signature:  552d9dd4
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbPoseMatchingGeneratorInternalStateClass;
const hkClass hkbPoseMatchingGeneratorInternalStateClass(
    "hkbPoseMatchingGeneratorInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbPoseMatchingGeneratorInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbPoseMatchingGeneratorInternalStateClass_Members), HK_COUNT_OF(hkbPoseMatchingGeneratorInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbPoseMatchingGeneratorInternalState, hkbPoseMatchingGeneratorInternalState);

