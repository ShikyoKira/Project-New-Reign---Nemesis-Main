// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbClientCharacterState_1.h"

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


static const hkInternalClassMember hkbClientCharacterStateClass_Members[] =
{
   { "deformableSkinIds",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_UINT64,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_deformableSkinIds) /*8*/,HK_NULL},
   { "rigidSkinIds",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_UINT64,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_rigidSkinIds) /*20*/,HK_NULL},
   { "externalEventIds",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_externalEventIds) /*32*/,HK_NULL},
   { "auxiliaryInfo",&hkbAuxiliaryNodeInfoClass,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_auxiliaryInfo) /*44*/,HK_NULL},
   { "activeEventIds",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_activeEventIds) /*56*/,HK_NULL},
   { "activeVariableIds",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_INT16,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_activeVariableIds) /*68*/,HK_NULL},
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_characterId) /*80*/,HK_NULL},
   { "instanceName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_instanceName) /*88*/,HK_NULL},
   { "templateName",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_templateName) /*92*/,HK_NULL},
   { "fullPathToProject",HK_NULL,HK_NULL,hkClassMember::TYPE_STRINGPTR,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_fullPathToProject) /*96*/,HK_NULL},
   { "behaviorData",&hkbBehaviorGraphDataClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_behaviorData) /*100*/,HK_NULL},
   { "behaviorInternalState",&hkbBehaviorGraphInternalStateClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_behaviorInternalState) /*104*/,HK_NULL},
   { "nodeIdToInternalStateMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbClientCharacterState,m_nodeIdToInternalStateMap) /*108*/,HK_NULL},
   { "visible",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_visible) /*112*/,HK_NULL},
   { "elapsedSimulationTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_elapsedSimulationTime) /*116*/,HK_NULL},
   { "skeleton",&hkaSkeletonClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_skeleton) /*120*/,HK_NULL},
   { "worldFromModel",HK_NULL,HK_NULL,hkClassMember::TYPE_QSTRANSFORM,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_worldFromModel) /*128*/,HK_NULL},
   { "poseModelSpace",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_QSTRANSFORM,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_poseModelSpace) /*176*/,HK_NULL},
   { "rigidAttachmentTransforms",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_QSTRANSFORM,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbClientCharacterState,m_rigidAttachmentTransforms) /*188*/,HK_NULL},
};

// Signature:  a2624c97
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbClientCharacterStateClass;
const hkClass hkbClientCharacterStateClass(
    "hkbClientCharacterState",
    &hkReferencedObjectClass, // parent
    sizeof(hkbClientCharacterState),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbClientCharacterStateClass_Members), HK_COUNT_OF(hkbClientCharacterStateClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbClientCharacterState, hkbClientCharacterState);

