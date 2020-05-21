// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbSetLocalTimeOfClipGeneratorCommand_0.h"

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


static const hkInternalClassMember hkbSetLocalTimeOfClipGeneratorCommandClass_Members[] =
{
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetLocalTimeOfClipGeneratorCommand,m_characterId) /*8*/,HK_NULL},
   { "localTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetLocalTimeOfClipGeneratorCommand,m_localTime) /*16*/,HK_NULL},
   { "nodeId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbSetLocalTimeOfClipGeneratorCommand,m_nodeId) /*20*/,HK_NULL},
};

// Signature:  fab12b45
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbSetLocalTimeOfClipGeneratorCommandClass;
const hkClass hkbSetLocalTimeOfClipGeneratorCommandClass(
    "hkbSetLocalTimeOfClipGeneratorCommand",
    &hkReferencedObjectClass, // parent
    sizeof(hkbSetLocalTimeOfClipGeneratorCommand),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbSetLocalTimeOfClipGeneratorCommandClass_Members), HK_COUNT_OF(hkbSetLocalTimeOfClipGeneratorCommandClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbSetLocalTimeOfClipGeneratorCommand, hkbSetLocalTimeOfClipGeneratorCommand);

