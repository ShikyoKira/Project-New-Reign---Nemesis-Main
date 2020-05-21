// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbBehaviorGraph_1.h"

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


static const hkInternalClassEnumItem VariableModeEnumItems[] =
{
   {0, "VARIABLE_MODE_DISCARD_WHEN_INACTIVE"},
   {1, "VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE"},
};

static const hkInternalClassEnum hkbBehaviorGraphClass_Enums[] = {
   {"VariableMode", VariableModeEnumItems, _countof(VariableModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* VariableModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbBehaviorGraphClass_Enums[0]);

static const hkInternalClassMember hkbBehaviorGraphClass_Members[] =
{
   { "variableMode",HK_NULL,VariableModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraph,m_variableMode) /*40*/,HK_NULL},
   { "uniqueIdPool",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_uniqueIdPool) /*44*/,HK_NULL},
   { "idToStateMachineTemplateMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_idToStateMachineTemplateMap) /*56*/,HK_NULL},
   { "mirroredExternalIdMap",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_mirroredExternalIdMap) /*60*/,HK_NULL},
   { "pseudoRandomGenerator",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_pseudoRandomGenerator) /*72*/,HK_NULL},
   { "rootGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraph,m_rootGenerator) /*76*/,HK_NULL},
   { "data",&hkbBehaviorGraphDataClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbBehaviorGraph,m_data) /*80*/,HK_NULL},
   { "rootGeneratorClone",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_rootGeneratorClone) /*84*/,HK_NULL},
   { "activeNodes",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_activeNodes) /*88*/,HK_NULL},
   { "activeNodeTemplateToIndexMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_activeNodeTemplateToIndexMap) /*92*/,HK_NULL},
   { "activeNodesChildrenIndices",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_activeNodesChildrenIndices) /*96*/,HK_NULL},
   { "globalTransitionData",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_globalTransitionData) /*100*/,HK_NULL},
   { "eventIdMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_eventIdMap) /*104*/,HK_NULL},
   { "attributeIdMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_attributeIdMap) /*108*/,HK_NULL},
   { "variableIdMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_variableIdMap) /*112*/,HK_NULL},
   { "characterPropertyIdMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_characterPropertyIdMap) /*116*/,HK_NULL},
   { "variableValueSet",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_variableValueSet) /*120*/,HK_NULL},
   { "nodeTemplateToCloneMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_nodeTemplateToCloneMap) /*124*/,HK_NULL},
   { "nodeCloneToTemplateMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_nodeCloneToTemplateMap) /*128*/,HK_NULL},
   { "stateListenerTemplateToCloneMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_stateListenerTemplateToCloneMap) /*132*/,HK_NULL},
   { "nodePartitionInfo",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_nodePartitionInfo) /*136*/,HK_NULL},
   { "numIntermediateOutputs",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_numIntermediateOutputs) /*140*/,HK_NULL},
   { "jobs",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_jobs) /*144*/,HK_NULL},
   { "allPartitionMemory",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_POINTER,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_allPartitionMemory) /*156*/,HK_NULL},
   { "numStaticNodes",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_numStaticNodes) /*168*/,HK_NULL},
   { "nextUniqueId",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_nextUniqueId) /*170*/,HK_NULL},
   { "isActive",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_isActive) /*172*/,HK_NULL},
   { "isLinked",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_isLinked) /*173*/,HK_NULL},
   { "updateActiveNodes",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_updateActiveNodes) /*174*/,HK_NULL},
   { "stateOrTransitionChanged",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbBehaviorGraph,m_stateOrTransitionChanged) /*175*/,HK_NULL},
};

// Signature:  b1218f86
extern const hkClass hkbGeneratorClass;
extern const hkClass hkbBehaviorGraphClass;
const hkClass hkbBehaviorGraphClass(
    "hkbBehaviorGraph",
    &hkbGeneratorClass, // parent
    sizeof(hkbBehaviorGraph),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbBehaviorGraphClass_Enums), HK_COUNT_OF(hkbBehaviorGraphClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbBehaviorGraphClass_Members), HK_COUNT_OF(hkbBehaviorGraphClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbBehaviorGraph, hkbBehaviorGraph);

