// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbProxyModifierProxyInfo_0.h"

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


static const hkInternalClassMember hkbProxyModifierProxyInfoClass_Members[] =
{
   { "dynamicFriction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_dynamicFriction) /*0*/,HK_NULL},
   { "staticFriction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_staticFriction) /*4*/,HK_NULL},
   { "keepContactTolerance",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_keepContactTolerance) /*8*/,HK_NULL},
   { "up",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_up) /*16*/,HK_NULL},
   { "keepDistance",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_keepDistance) /*32*/,HK_NULL},
   { "contactAngleSensitivity",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_contactAngleSensitivity) /*36*/,HK_NULL},
   { "userPlanes",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_userPlanes) /*40*/,HK_NULL},
   { "maxCharacterSpeedForSolver",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_maxCharacterSpeedForSolver) /*44*/,HK_NULL},
   { "characterStrength",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_characterStrength) /*48*/,HK_NULL},
   { "characterMass",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_characterMass) /*52*/,HK_NULL},
   { "maxSlope",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_maxSlope) /*56*/,HK_NULL},
   { "penetrationRecoverySpeed",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_penetrationRecoverySpeed) /*60*/,HK_NULL},
   { "maxCastIterations",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_maxCastIterations) /*64*/,HK_NULL},
   { "refreshManifoldInCheckSupport",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifierProxyInfo,m_refreshManifoldInCheckSupport) /*68*/,HK_NULL},
};

// Signature:  39de637e
extern const hkClass hkbProxyModifierProxyInfoClass;
const hkClass hkbProxyModifierProxyInfoClass(
    "hkbProxyModifierProxyInfo",
    HK_NULL, // parent
    sizeof(hkbProxyModifierProxyInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbProxyModifierProxyInfoClass_Members), HK_COUNT_OF(hkbProxyModifierProxyInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    0 // version
 );
HK_REFLECTION_DEFINE_SIMPLE(hkbProxyModifierProxyInfo, hkbProxyModifierProxyInfo);

