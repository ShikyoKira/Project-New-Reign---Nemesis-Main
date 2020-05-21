// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacterSetup_2.h"

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


static const hkInternalClassMember hkbCharacterSetupClass_Members[] =
{
   { "retargetingSkeletonMappers",&hkaSkeletonMapperClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSetup,m_retargetingSkeletonMappers) /*8*/,HK_NULL},
   { "animationSkeleton",&hkaSkeletonClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSetup,m_animationSkeleton) /*20*/,HK_NULL},
   { "ragdollToAnimationSkeletonMapper",&hkaSkeletonMapperClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSetup,m_ragdollToAnimationSkeletonMapper) /*24*/,HK_NULL},
   { "animationToRagdollSkeletonMapper",&hkaSkeletonMapperClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSetup,m_animationToRagdollSkeletonMapper) /*28*/,HK_NULL},
   { "animationBindingSet",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacterSetup,m_animationBindingSet) /*32*/,HK_NULL},
   { "data",&hkbCharacterDataClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSetup,m_data) /*36*/,HK_NULL},
   { "mirroredSkeleton",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacterSetup,m_mirroredSkeleton) /*40*/,HK_NULL},
   { "characterPropertyIdMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacterSetup,m_characterPropertyIdMap) /*44*/,HK_NULL},
};

// Signature:  e5a2a413
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCharacterSetupClass;
const hkClass hkbCharacterSetupClass(
    "hkbCharacterSetup",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCharacterSetup),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbCharacterSetupClass_Members), HK_COUNT_OF(hkbCharacterSetupClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacterSetup, hkbCharacterSetup);

