// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbPoseMatchingGenerator_2.h"

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


static const hkInternalClassEnumItem ModeEnumItems[] =
{
   {0, "MODE_MATCH"},
   {1, "MODE_PLAY"},
};

static const hkInternalClassEnum hkbPoseMatchingGeneratorClass_Enums[] = {
   {"Mode", ModeEnumItems, _countof(ModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* ModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbPoseMatchingGeneratorClass_Enums[0]);

static const hkInternalClassMember hkbPoseMatchingGeneratorClass_Members[] =
{
   { "worldFromModelRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_QUATERNION,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_worldFromModelRotation) /*128*/,HK_NULL},
   { "blendSpeed",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_blendSpeed) /*144*/,HK_NULL},
   { "minSpeedToSwitch",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_minSpeedToSwitch) /*148*/,HK_NULL},
   { "minSwitchTimeNoError",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_minSwitchTimeNoError) /*152*/,HK_NULL},
   { "minSwitchTimeFullError",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_minSwitchTimeFullError) /*156*/,HK_NULL},
   { "startPlayingEventId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_startPlayingEventId) /*160*/,HK_NULL},
   { "startMatchingEventId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_startMatchingEventId) /*164*/,HK_NULL},
   { "rootBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_rootBoneIndex) /*168*/,HK_NULL},
   { "otherBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_otherBoneIndex) /*170*/,HK_NULL},
   { "anotherBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_anotherBoneIndex) /*172*/,HK_NULL},
   { "pelvisIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_pelvisIndex) /*174*/,HK_NULL},
   { "mode",HK_NULL,ModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_mode) /*176*/,HK_NULL},
   { "currentMatch",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_currentMatch) /*180*/,HK_NULL},
   { "bestMatch",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_bestMatch) /*184*/,HK_NULL},
   { "timeSinceBetterMatch",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_timeSinceBetterMatch) /*188*/,HK_NULL},
   { "error",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_error) /*192*/,HK_NULL},
   { "resetCurrentMatchLocalTime",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_resetCurrentMatchLocalTime) /*196*/,HK_NULL},
   { "poseMatchingUtility",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbPoseMatchingGenerator,m_poseMatchingUtility) /*200*/,HK_NULL},
};

// Signature:  29e271b4
extern const hkClass hkbBlenderGeneratorClass;
extern const hkClass hkbPoseMatchingGeneratorClass;
const hkClass hkbPoseMatchingGeneratorClass(
    "hkbPoseMatchingGenerator",
    &hkbBlenderGeneratorClass, // parent
    sizeof(hkbPoseMatchingGenerator),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbPoseMatchingGeneratorClass_Enums), HK_COUNT_OF(hkbPoseMatchingGeneratorClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbPoseMatchingGeneratorClass_Members), HK_COUNT_OF(hkbPoseMatchingGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbPoseMatchingGenerator, hkbPoseMatchingGenerator);

