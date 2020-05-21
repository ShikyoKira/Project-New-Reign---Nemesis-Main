// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacterSteppedInfo_2.h"

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


static const hkInternalClassMember hkbCharacterSteppedInfoClass_Members[] =
{
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSteppedInfo,m_characterId) /*8*/,HK_NULL},
   { "deltaTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSteppedInfo,m_deltaTime) /*16*/,HK_NULL},
   { "worldFromModel",HK_NULL,HK_NULL,hkClassMember::TYPE_QSTRANSFORM,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSteppedInfo,m_worldFromModel) /*32*/,HK_NULL},
   { "poseModelSpace",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_QSTRANSFORM,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSteppedInfo,m_poseModelSpace) /*80*/,HK_NULL},
   { "rigidAttachmentTransforms",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_QSTRANSFORM,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSteppedInfo,m_rigidAttachmentTransforms) /*92*/,HK_NULL},
};

// Signature:  2eda84f8
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCharacterSteppedInfoClass;
const hkClass hkbCharacterSteppedInfoClass(
    "hkbCharacterSteppedInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCharacterSteppedInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbCharacterSteppedInfoClass_Members), HK_COUNT_OF(hkbCharacterSteppedInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacterSteppedInfo, hkbCharacterSteppedInfo);

