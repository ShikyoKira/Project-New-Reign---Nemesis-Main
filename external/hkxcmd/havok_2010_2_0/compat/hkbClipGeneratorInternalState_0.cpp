// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbClipGeneratorInternalState_0.h"

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


static const hkInternalClassMember hkbClipGeneratorInternalStateClass_Members[] =
{
   { "extractedMotion",HK_NULL,HK_NULL,hkClassMember::TYPE_QSTRANSFORM,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_extractedMotion) /*16*/,HK_NULL},
   { "echos",&hkbClipGeneratorEchoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_echos) /*64*/,HK_NULL},
   { "localTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_localTime) /*76*/,HK_NULL},
   { "time",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_time) /*80*/,HK_NULL},
   { "previousUserControlledTimeFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_previousUserControlledTimeFraction) /*84*/,HK_NULL},
   { "bufferSize",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_bufferSize) /*88*/,HK_NULL},
   { "echoBufferSize",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_echoBufferSize) /*92*/,HK_NULL},
   { "atEnd",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_atEnd) /*96*/,HK_NULL},
   { "ignoreStartTime",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_ignoreStartTime) /*97*/,HK_NULL},
   { "pingPongBackward",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClipGeneratorInternalState,m_pingPongBackward) /*98*/,HK_NULL},
};

// Signature:  26ce5bf3
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbClipGeneratorInternalStateClass;
const hkClass hkbClipGeneratorInternalStateClass(
    "hkbClipGeneratorInternalState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbClipGeneratorInternalState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbClipGeneratorInternalStateClass_Members), HK_COUNT_OF(hkbClipGeneratorInternalStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbClipGeneratorInternalState, hkbClipGeneratorInternalState);

