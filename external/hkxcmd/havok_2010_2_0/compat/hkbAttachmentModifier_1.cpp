// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbAttachmentModifier_1.h"

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


static const hkInternalClassMember hkbAttachmentModifierClass_Members[] =
{
   { "sendToAttacherOnAttach",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentModifier,m_sendToAttacherOnAttach) /*44*/,HK_NULL},
   { "sendToAttacheeOnAttach",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentModifier,m_sendToAttacheeOnAttach) /*52*/,HK_NULL},
   { "sendToAttacherOnDetach",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentModifier,m_sendToAttacherOnDetach) /*60*/,HK_NULL},
   { "sendToAttacheeOnDetach",&hkbEventPropertyClass,HK_NULL,hkClassMember::TYPE_STRUCT,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentModifier,m_sendToAttacheeOnDetach) /*68*/,HK_NULL},
   { "attachmentSetup",&hkbAttachmentSetupClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentModifier,m_attachmentSetup) /*76*/,HK_NULL},
   { "attacherHandle",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentModifier,m_attacherHandle) /*80*/,HK_NULL},
   { "attacheeHandle",&hkbHandleClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentModifier,m_attacheeHandle) /*84*/,HK_NULL},
   { "attacheeLayer",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbAttachmentModifier,m_attacheeLayer) /*88*/,HK_NULL},
   { "attacheeRB",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbAttachmentModifier,m_attacheeRB) /*92*/,HK_NULL},
   { "oldMotionType",HK_NULL,HK_NULL,hkClassMember::TYPE_ENUM,hkClassMember::TYPE_UINT8,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbAttachmentModifier,m_oldMotionType) /*96*/,HK_NULL},
   { "oldFilterInfo",HK_NULL,HK_NULL,hkClassMember::TYPE_INT32,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbAttachmentModifier,m_oldFilterInfo) /*100*/,HK_NULL},
   { "attachment",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(hkbAttachmentModifier,m_attachment) /*104*/,HK_NULL},
};

// Signature:  cc0aab32
extern const hkClass hkbModifierClass;
extern const hkClass hkbAttachmentModifierClass;
const hkClass hkbAttachmentModifierClass(
    "hkbAttachmentModifier",
    &hkbModifierClass, // parent
    sizeof(hkbAttachmentModifier),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbAttachmentModifierClass_Members), HK_COUNT_OF(hkbAttachmentModifierClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbAttachmentModifier, hkbAttachmentModifier);

