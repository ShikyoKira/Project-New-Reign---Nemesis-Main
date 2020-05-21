// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbParticleSystemEventPayload_0.h"

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


static const hkInternalClassEnumItem SystemTypeEnumItems[] =
{
   {0, "DEBRIS"},
   {1, "DUST"},
   {2, "EXPLOSION"},
   {3, "SMOKE"},
   {4, "SPARKS"},
};

static const hkInternalClassEnum hkbParticleSystemEventPayloadClass_Enums[] = {
   {"SystemType", SystemTypeEnumItems, _countof(SystemTypeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* SystemTypeEnum = reinterpret_cast<const hkClassEnum*>(&hkbParticleSystemEventPayloadClass_Enums[0]);

static const hkInternalClassMember hkbParticleSystemEventPayloadClass_Members[] =
{
   { "type",HK_NULL,SystemTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbParticleSystemEventPayload,m_type) /*8*/,HK_NULL},
   { "emitBoneIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbParticleSystemEventPayload,m_emitBoneIndex) /*10*/,HK_NULL},
   { "offset",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbParticleSystemEventPayload,m_offset) /*16*/,HK_NULL},
   { "direction",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbParticleSystemEventPayload,m_direction) /*32*/,HK_NULL},
   { "numParticles",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbParticleSystemEventPayload,m_numParticles) /*48*/,HK_NULL},
   { "speed",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbParticleSystemEventPayload,m_speed) /*52*/,HK_NULL},
};

// Signature:  9df46cd6
extern const hkClass hkbEventPayloadClass;
extern const hkClass hkbParticleSystemEventPayloadClass;
const hkClass hkbParticleSystemEventPayloadClass(
    "hkbParticleSystemEventPayload",
    &hkbEventPayloadClass, // parent
    sizeof(hkbParticleSystemEventPayload),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbParticleSystemEventPayloadClass_Enums), HK_COUNT_OF(hkbParticleSystemEventPayloadClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbParticleSystemEventPayloadClass_Members), HK_COUNT_OF(hkbParticleSystemEventPayloadClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbParticleSystemEventPayload, hkbParticleSystemEventPayload);

