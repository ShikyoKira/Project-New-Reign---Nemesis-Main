// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacter_2.h"

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


static const hkInternalClassMember hkbCharacterClass_Members[] =
{
   { "nearbyCharacters",&hkbCharacterClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacter,m_nearbyCharacters) /*8*/,HK_NULL},
   { "currentLod",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacter,m_currentLod) /*20*/,HK_NULL},
   { "numTracksInLod",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_numTracksInLod) /*22*/,HK_NULL},
   { "name",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacter,m_name) /*24*/,HK_NULL},
   { "ragdollDriver",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_ragdollDriver) /*28*/,HK_NULL},
   { "characterControllerDriver",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_characterControllerDriver) /*32*/,HK_NULL},
   { "footIkDriver",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_footIkDriver) /*36*/,HK_NULL},
   { "handIkDriver",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_handIkDriver) /*40*/,HK_NULL},
   { "setup",&hkbCharacterSetupClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacter,m_setup) /*44*/,HK_NULL},
   { "behaviorGraph",&hkbBehaviorGraphClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacter,m_behaviorGraph) /*48*/,HK_NULL},
   { "projectData",&hkbProjectDataClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacter,m_projectData) /*52*/,HK_NULL},
   { "animationBindingSet",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_animationBindingSet) /*56*/,HK_NULL},
   { "raycastInterface",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_raycastInterface) /*60*/,HK_NULL},
   { "world",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::NOT_OWNED | hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_world) /*64*/,HK_NULL},
   { "eventQueue",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_eventQueue) /*68*/,HK_NULL},
   { "worldFromModel",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_worldFromModel) /*72*/,HK_NULL},
   { "poseLocal",HK_NULL,HK_NULL,hkClassMember::TYPE_SIMPLEARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_poseLocal) /*76*/,HK_NULL},
   { "deleteWorldFromModel",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_deleteWorldFromModel) /*84*/,HK_NULL},
   { "deletePoseLocal",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbCharacter,m_deletePoseLocal) /*85*/,HK_NULL},
};

// Signature:  3088a5c5
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCharacterClass;
const hkClass hkbCharacterClass(
    "hkbCharacter",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCharacter),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbCharacterClass_Members), HK_COUNT_OF(hkbCharacterClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacter, hkbCharacter);

