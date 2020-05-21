// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "StdAfx.h"
#include "BSSynchronizedClipGenerator_1.h"

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


static const hkInternalClassMember BSSynchronizedClipGeneratorClass_Members[] =
{
   { "pClipGenerator",&hkbGeneratorClass,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_STRUCT,0,hkClassMember::ALIGN_16,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_pClipGenerator) /*48*/,HK_NULL},
   { "SyncAnimPrefix",HK_NULL,HK_NULL,hkClassMember::TYPE_CSTRING,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_SyncAnimPrefix) /*52*/,HK_NULL},
   { "bSyncClipIgnoreMarkPlacement",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_bSyncClipIgnoreMarkPlacement) /*56*/,HK_NULL},
   { "fGetToMarkTime",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_fGetToMarkTime) /*60*/,HK_NULL},
   { "fMarkErrorThreshold",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_fMarkErrorThreshold) /*64*/,HK_NULL},
   { "bLeadCharacter",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_bLeadCharacter) /*68*/,HK_NULL},
   { "bReorientSupportChar",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_bReorientSupportChar) /*69*/,HK_NULL},
   { "bApplyMotionFromRoot",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_bApplyMotionFromRoot) /*70*/,HK_NULL},
   { "pSyncScene",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_pSyncScene) /*72*/,HK_NULL},
   { "StartMarkWS",HK_NULL,HK_NULL,hkClassMember::TYPE_QSTRANSFORM,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_StartMarkWS) /*80*/,HK_NULL},
   { "EndMarkWS",HK_NULL,HK_NULL,hkClassMember::TYPE_QSTRANSFORM,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_EndMarkWS) /*128*/,HK_NULL},
   { "StartMarkMS",HK_NULL,HK_NULL,hkClassMember::TYPE_QSTRANSFORM,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_StartMarkMS) /*176*/,HK_NULL},
   { "fCurrentLerp",HK_NULL,HK_NULL,hkClassMember::TYPE_REAL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_fCurrentLerp) /*224*/,HK_NULL},
   { "pLocalSyncBinding",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_pLocalSyncBinding) /*228*/,HK_NULL},
   { "pEventMap",HK_NULL,HK_NULL,hkClassMember::TYPE_POINTER,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_pEventMap) /*232*/,HK_NULL},
   { "sAnimationBindingIndex",HK_NULL,HK_NULL,hkClassMember::TYPE_INT16,hkClassMember::TYPE_VOID,0,hkClassMember::FLAGS_NONE,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_sAnimationBindingIndex) /*236*/,HK_NULL},
   { "bAtMark",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_bAtMark) /*238*/,HK_NULL},
   { "bAllCharactersInScene",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_bAllCharactersInScene) /*239*/,HK_NULL},
   { "bAllCharactersAtMarks",HK_NULL,HK_NULL,hkClassMember::TYPE_BOOL,hkClassMember::TYPE_VOID,0,hkClassMember::SERIALIZE_IGNORED,HK_OFFSET_OF(BSSynchronizedClipGenerator,m_bAllCharactersAtMarks) /*240*/,HK_NULL},
};

// Signature:  d83bea64
extern const hkClass hkbGeneratorClass;
extern const hkClass BSSynchronizedClipGeneratorClass;
const hkClass BSSynchronizedClipGeneratorClass(
    "BSSynchronizedClipGenerator",
    &hkbGeneratorClass, // parent
    sizeof(BSSynchronizedClipGenerator),
    HK_NULL, 0, // interfaces
    HK_NULL, 0, // enums
    reinterpret_cast<const hkClassMember*>(BSSynchronizedClipGeneratorClass_Members), HK_COUNT_OF(BSSynchronizedClipGeneratorClass_Members),
    HK_NULL, // defaults
    HK_NULL, // attributes
    0, // flags
    1 // version
 );
HK_REFLECTION_DEFINE_VIRTUAL(BSSynchronizedClipGenerator, BSSynchronizedClipGenerator);

