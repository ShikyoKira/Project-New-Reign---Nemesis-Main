// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbProxyModifier_1.h"

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


static const hkInternalClassEnumItem PhantomTypeEnumItems[] =
{
   {0, "PHANTOM_TYPE_SIMPLE"},
   {1, "PHANTOM_TYPE_CACHING"},
};

static const hkInternalClassEnumItem LinearVelocityModeEnumItems[] =
{
   {0, "LINEAR_VELOCITY_MODE_WORLD"},
   {1, "LINEAR_VELOCITY_MODE_MODEL"},
};

static const hkInternalClassEnum hkbProxyModifierClass_Enums[] = {
   {"PhantomType", PhantomTypeEnumItems, _countof(PhantomTypeEnumItems), HK_NULL, 0 },
   {"LinearVelocityMode", LinearVelocityModeEnumItems, _countof(LinearVelocityModeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* PhantomTypeEnum = reinterpret_cast<const hkClassEnum*>(&hkbProxyModifierClass_Enums[0]);
const hkClassEnum* LinearVelocityModeEnum = reinterpret_cast<const hkClassEnum*>(&hkbProxyModifierClass_Enums[1]);

static const hkInternalClassMember hkbProxyModifierClass_Members[] =
{
   { "proxyInfo",&hkbProxyModifierProxyInfoClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_proxyInfo) /*48*/,HK_NULL},
   { "linearVelocity",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_linearVelocity) /*128*/,HK_NULL},
   { "horizontalGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_horizontalGain) /*144*/,HK_NULL},
   { "verticalGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_verticalGain) /*148*/,HK_NULL},
   { "maxHorizontalSeparation",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_maxHorizontalSeparation) /*152*/,HK_NULL},
   { "maxVerticalSeparation",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_maxVerticalSeparation) /*156*/,HK_NULL},
   { "verticalDisplacementError",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_verticalDisplacementError) /*160*/,HK_NULL},
   { "verticalDisplacementErrorGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_verticalDisplacementErrorGain) /*164*/,HK_NULL},
   { "maxVerticalDisplacement",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_maxVerticalDisplacement) /*168*/,HK_NULL},
   { "minVerticalDisplacement",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_minVerticalDisplacement) /*172*/,HK_NULL},
   { "capsuleHeight",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_capsuleHeight) /*176*/,HK_NULL},
   { "capsuleRadius",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_capsuleRadius) /*180*/,HK_NULL},
   { "maxSlopeForRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_maxSlopeForRotation) /*184*/,HK_NULL},
   { "collisionFilterInfo",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_collisionFilterInfo) /*188*/,HK_NULL},
   { "phantomType",HK_NULL,PhantomTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_phantomType) /*192*/,HK_NULL},
   { "linearVelocityMode",HK_NULL,LinearVelocityModeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_linearVelocityMode) /*193*/,HK_NULL},
   { "ignoreIncomingRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_ignoreIncomingRotation) /*194*/,HK_NULL},
   { "ignoreCollisionDuringRotation",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_ignoreCollisionDuringRotation) /*195*/,HK_NULL},
   { "ignoreIncomingTranslation",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_ignoreIncomingTranslation) /*196*/,HK_NULL},
   { "includeDownwardMomentum",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_includeDownwardMomentum) /*197*/,HK_NULL},
   { "followWorldFromModel",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_followWorldFromModel) /*198*/,HK_NULL},
   { "isTouchingGround",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbProxyModifier,m_isTouchingGround) /*199*/,HK_NULL},
   { "characterProxy",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbProxyModifier,m_characterProxy) /*200*/,HK_NULL},
   { "phantom",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbProxyModifier,m_phantom) /*204*/,HK_NULL},
   { "phantomShape",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbProxyModifier,m_phantomShape) /*208*/,HK_NULL},
   { "horizontalDisplacement",HK_NULL,HK_NULL,hkClassMember::TYPE_VECTOR4,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbProxyModifier,m_horizontalDisplacement) /*224*/,HK_NULL},
   { "verticalDisplacement",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbProxyModifier,m_verticalDisplacement) /*240*/,HK_NULL},
   { "timestep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbProxyModifier,m_timestep) /*244*/,HK_NULL},
   { "previousFrameFollowWorldFromModel",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbProxyModifier,m_previousFrameFollowWorldFromModel) /*248*/,HK_NULL},
};

// Signature:  8a41554f
extern const hkClass hkbModifierClass;
extern const hkClass hkbProxyModifierClass;
const hkClass hkbProxyModifierClass(
    "hkbProxyModifier",
    &hkbModifierClass, // parent
    sizeof(hkbProxyModifier),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbProxyModifierClass_Enums), HK_COUNT_OF(hkbProxyModifierClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbProxyModifierClass_Members), HK_COUNT_OF(hkbProxyModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbProxyModifier, hkbProxyModifier);

