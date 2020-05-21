// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacterStringData_5.h"

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


static const hkInternalClassMember hkbCharacterStringDataClass_Members[] =
{
   { "deformableSkinNames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_deformableSkinNames) /*8*/,HK_NULL},
   { "rigidSkinNames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_rigidSkinNames) /*20*/,HK_NULL},
   { "animationNames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_animationNames) /*32*/,HK_NULL},
   { "animationFilenames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_animationFilenames) /*44*/,HK_NULL},
   { "characterPropertyNames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_characterPropertyNames) /*56*/,HK_NULL},
   { "retargetingSkeletonMapperFilenames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_retargetingSkeletonMapperFilenames) /*68*/,HK_NULL},
   { "lodNames",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_lodNames) /*80*/,HK_NULL},
   { "mirroredSyncPointSubstringsA",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_mirroredSyncPointSubstringsA) /*92*/,HK_NULL},
   { "mirroredSyncPointSubstringsB",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_STRINGPTR,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_mirroredSyncPointSubstringsB) /*104*/,HK_NULL},
   { "name",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_name) /*116*/,HK_NULL},
   { "rigName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_rigName) /*120*/,HK_NULL},
   { "ragdollName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_ragdollName) /*124*/,HK_NULL},
   { "behaviorFilename",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterStringData,m_behaviorFilename) /*128*/,HK_NULL},
};

// Signature:  655b42bc
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCharacterStringDataClass;
const hkClass hkbCharacterStringDataClass(
    "hkbCharacterStringData",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCharacterStringData),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbCharacterStringDataClass_Members), HK_COUNT_OF(hkbCharacterStringDataClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    5 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacterStringData, hkbCharacterStringData);

