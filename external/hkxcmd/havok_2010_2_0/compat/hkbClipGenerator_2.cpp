// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbClipGenerator_2.h"

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


static const hkInternalClassEnumItem PlaybackModeEnumItems[] =
{
   {0, "MODE_SINGLE_PLAY"},
   {1, "MODE_LOOPING"},
   {2, "MODE_USER_CONTROLLED"},
   {3, "MODE_PING_PONG"},
   {4, "MODE_COUNT"},
};

static const hkInternalClassEnumItem ClipFlagsEnumItems[] =
{
   {1, "FLAG_CONTINUE_MOTION_AT_END"},
   {2, "FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE"},
   {4, "FLAG_MIRROR"},
   {8, "FLAG_FORCE_DENSE_POSE"},
   {16, "FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS"},
   {32, "FLAG_IGNORE_MOTION"},
};

static const hkInternalClassEnum hkbClipGeneratorClass_Enums[] = {
   {"PlaybackMode", PlaybackModeEnumItems, _countof(PlaybackModeEnumItems), HK_NULL, 0 },
   {"ClipFlags", ClipFlagsEnumItems, _countof(ClipFlagsEnumItems), HK_NULL, 0 },
};
const hkClassEnum* PlaybackModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbClipGeneratorClass_Enums[0]);
const hkClassEnum* ClipFlagsEnum = reinterpret_cast<const hkClassEnum*>(&hkbClipGeneratorClass_Enums[1]);

static const hkInternalClassMember hkbClipGeneratorClass_Members[] =
{
   { "animationName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_animationName) /*40*/,HK_NULL},
   { "triggers",&hkbClipTriggerArrayClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_triggers) /*44*/,HK_NULL},
   { "cropStartAmountLocalTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_cropStartAmountLocalTime) /*48*/,HK_NULL},
   { "cropEndAmountLocalTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_cropEndAmountLocalTime) /*52*/,HK_NULL},
   { "startTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_startTime) /*56*/,HK_NULL},
   { "playbackSpeed",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_playbackSpeed) /*60*/,HK_NULL},
   { "enforcedDuration",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_enforcedDuration) /*64*/,HK_NULL},
   { "userControlledTimeFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_userControlledTimeFraction) /*68*/,HK_NULL},
   { "animationBindingIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_animationBindingIndex) /*72*/,HK_NULL},
   { "mode",HK_NULL,PlaybackModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_mode) /*74*/,HK_NULL},
   { "flags",HK_NULL,HK_NULL,hkClassMember::TYPE_INT8,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGenerator,m_flags) /*75*/,HK_NULL},
   { "animDatas",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_animDatas) /*76*/,HK_NULL},
   { "animationControl",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_animationControl) /*88*/,HK_NULL},
   { "originalTriggers",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_originalTriggers) /*92*/,HK_NULL},
   { "mapperData",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_mapperData) /*96*/,HK_NULL},
   { "binding",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_binding) /*100*/,HK_NULL},
   { "mirroredAnimation",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_mirroredAnimation) /*104*/,HK_NULL},
   { "extractedMotion",HK_NULL,HK_NULL,hkClassMember::TYPE_QSTRANSFORM,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_extractedMotion) /*112*/,HK_NULL},
   { "echos",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_echos) /*160*/,HK_NULL},
   { "localTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_localTime) /*172*/,HK_NULL},
   { "time",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_time) /*176*/,HK_NULL},
   { "previousUserControlledTimeFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_previousUserControlledTimeFraction) /*180*/,HK_NULL},
   { "bufferSize",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_bufferSize) /*184*/,HK_NULL},
   { "echoBufferSize",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_echoBufferSize) /*188*/,HK_NULL},
   { "atEnd",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_atEnd) /*192*/,HK_NULL},
   { "ignoreStartTime",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_ignoreStartTime) /*193*/,HK_NULL},
   { "pingPongBackward",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClipGenerator,m_pingPongBackward) /*194*/,HK_NULL},
};

// Signature:  333b85b9
extern const hkClass hkbGeneratorClass;
extern const hkClass hkbClipGeneratorClass;
const hkClass hkbClipGeneratorClass(
    "hkbClipGenerator",
    &hkbGeneratorClass, // parent
    sizeof(hkbClipGenerator),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbClipGeneratorClass_Enums), HK_COUNT_OF(hkbClipGeneratorClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbClipGeneratorClass_Members), HK_COUNT_OF(hkbClipGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbClipGenerator, hkbClipGenerator);

