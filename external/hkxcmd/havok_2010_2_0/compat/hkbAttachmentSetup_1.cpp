// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbAttachmentSetup_1.h"

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


static const hkInternalClassEnumItem AttachmentTypeEnumItems[] =
{
   {0, "ATTACHMENT_TYPE_KEYFRAME_RIGID_BODY"},
   {1, "ATTACHMENT_TYPE_BALL_SOCKET_CONSTRAINT"},
   {2, "ATTACHMENT_TYPE_RAGDOLL_CONSTRAINT"},
   {3, "ATTACHMENT_TYPE_SET_WORLD_FROM_MODEL"},
   {4, "ATTACHMENT_TYPE_NONE"},
};

static const hkInternalClassEnum hkbAttachmentSetupClass_Enums[] = {
   {"AttachmentType", AttachmentTypeEnumItems, _countof(AttachmentTypeEnumItems), HK_NULL, 0 },
};
const hkClassEnum* AttachmentTypeEnum = reinterpret_cast<const hkClassEnum*>(&hkbAttachmentSetupClass_Enums[0]);

static const hkInternalClassMember hkbAttachmentSetupClass_Members[] =
{
   { "blendInTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentSetup,m_blendInTime) /*8*/,HK_NULL},
   { "moveAttacherFraction",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentSetup,m_moveAttacherFraction) /*12*/,HK_NULL},
   { "gain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentSetup,m_gain) /*16*/,HK_NULL},
   { "extrapolationTimeStep",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentSetup,m_extrapolationTimeStep) /*20*/,HK_NULL},
   { "fixUpGain",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentSetup,m_fixUpGain) /*24*/,HK_NULL},
   { "maxLinearDistance",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentSetup,m_maxLinearDistance) /*28*/,HK_NULL},
   { "maxAngularDistance",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentSetup,m_maxAngularDistance) /*32*/,HK_NULL},
   { "attachmentType",HK_NULL,AttachmentTypeEnum,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_INT8,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentSetup,m_attachmentType) /*36*/,HK_NULL},
};

// Signature:  0774632b
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbAttachmentSetupClass;
const hkClass hkbAttachmentSetupClass(
    "hkbAttachmentSetup",
    &hkReferencedObjectClass, // parent
    sizeof(hkbAttachmentSetup),
    HK_NULL, 0, // interfaces
    reinterpret_cast<const hkClassEnum*>(hkbAttachmentSetupClass_Enums), HK_COUNT_OF(hkbAttachmentSetupClass_Enums),
    reinterpret_cast<const hkClassMember*>(hkbAttachmentSetupClass_Members), HK_COUNT_OF(hkbAttachmentSetupClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbAttachmentSetup, hkbAttachmentSetup);

