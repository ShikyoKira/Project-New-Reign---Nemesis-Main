// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "hkbCharacterSkinInfo_2.h"

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


static const hkInternalClassMember hkbCharacterSkinInfoClass_Members[] =
{
   { "characterId",HK_NULL,HK_NULL,hkClassMember::TYPE_UINT64,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSkinInfo,m_characterId) /*8*/,HK_NULL},
   { "deformableSkins",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_UINT64,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSkinInfo,m_deformableSkins) /*16*/,HK_NULL},
   { "rigidSkins",HK_NULL,HK_NULL,hkClassMember::TYPE_ARRAY,hkClassMember::TYPE_UINT64,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(hkbCharacterSkinInfo,m_rigidSkins) /*28*/,HK_NULL},
};

// Signature:  180d900d
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkbCharacterSkinInfoClass;
const hkClass hkbCharacterSkinInfoClass(
    "hkbCharacterSkinInfo",
    &hkReferencedObjectClass, // parent
    sizeof(hkbCharacterSkinInfo),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(hkbCharacterSkinInfoClass_Members), HK_COUNT_OF(hkbCharacterSkinInfoClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    2 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(hkbCharacterSkinInfo, hkbCharacterSkinInfo);

