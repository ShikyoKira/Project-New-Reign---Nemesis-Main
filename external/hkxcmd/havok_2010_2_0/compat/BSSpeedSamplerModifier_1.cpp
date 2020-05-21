// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSSpeedSamplerModifier_1.h"

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


static const hkInternalClassMember BSSpeedSamplerModifierClass_Members[] =
{
   { "state",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSpeedSamplerModifier,m_state) /*44*/,HK_NULL},
   { "direction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSpeedSamplerModifier,m_direction) /*48*/,HK_NULL},
   { "goalSpeed",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSpeedSamplerModifier,m_goalSpeed) /*52*/,HK_NULL},
   { "speedOut",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSpeedSamplerModifier,m_speedOut) /*56*/,HK_NULL},
};

// Signature:  d297fda9
extern const hkClass hkbModifierClass;
extern const hkClass BSSpeedSamplerModifierClass;
const hkClass BSSpeedSamplerModifierClass(
    "BSSpeedSamplerModifier",
    &hkbModifierClass, // parent
    sizeof(BSSpeedSamplerModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSSpeedSamplerModifierClass_Members), HK_COUNT_OF(BSSpeedSamplerModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSSpeedSamplerModifier, BSSpeedSamplerModifier);

