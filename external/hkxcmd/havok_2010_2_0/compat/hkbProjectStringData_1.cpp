// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbProjectStringData_1.h"

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


static const hkInternalClassMember hkbProjectStringDataClass_Members[] =
{
   { "animationFilenames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectStringData,m_animationFilenames) /*8*/,HK_NULL},
   { "behaviorFilenames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectStringData,m_behaviorFilenames) /*20*/,HK_NULL},
   { "characterFilenames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectStringData,m_characterFilenames) /*32*/,HK_NULL},
   { "eventNames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectStringData,m_eventNames) /*44*/,HK_NULL},
   { "animationPath",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectStringData,m_animationPath) /*56*/,HK_NULL},
   { "behaviorPath",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectStringData,m_behaviorPath) /*60*/,HK_NULL},
   { "characterPath",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectStringData,m_characterPath) /*64*/,HK_NULL},
   { "fullPathToSource",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProjectStringData,m_fullPathToSource) /*68*/,HK_NULL},
   { "rootPath",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbProjectStringData,m_rootPath) /*72*/,HK_NULL},
};

// Signature:  076ad60a
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbProjectStringDataClass;
const hkClass hkbProjectStringDataClass(
    "hkbProjectStringData",
    &hkReferencedObjectClass, // parent
    sizeof(hkbProjectStringData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbProjectStringDataClass_Members), HK_COUNT_OF(hkbProjectStringDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbProjectStringData, hkbProjectStringData);

