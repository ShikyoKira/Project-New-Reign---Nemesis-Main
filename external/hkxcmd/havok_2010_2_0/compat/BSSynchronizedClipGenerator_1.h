#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGenerator_0.h"

class BSSynchronizedClipGenerator : public hkbGenerator
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE BSSynchronizedClipGenerator(void) {}
   HK_FORCE_INLINE BSSynchronizedClipGenerator( hkFinishLoadedObjectFlag flag ) : hkbGenerator(flag) 
   , m_pClipGenerator(flag)
   , m_pSyncScene(flag)
   , m_pLocalSyncBinding(flag)
   , m_pEventMap(flag)
      {}
   // Properties
   hkChar m_padding[8];
   hkRefPtr<hkbGenerator> m_pClipGenerator;
   char* m_SyncAnimPrefix;
   hkBool m_bSyncClipIgnoreMarkPlacement;
   hkReal m_fGetToMarkTime;
   hkReal m_fMarkErrorThreshold;
   hkBool m_bLeadCharacter;
   hkBool m_bReorientSupportChar;
   hkBool m_bApplyMotionFromRoot;
   hkRefVariant m_pSyncScene;
   hkQsTransform m_StartMarkWS;
   hkQsTransform m_EndMarkWS;
   hkQsTransform m_StartMarkMS;
   hkReal m_fCurrentLerp;
   hkRefVariant m_pLocalSyncBinding;
   hkRefVariant m_pEventMap;
   hkInt16 m_sAnimationBindingIndex;
   hkBool m_bAtMark;
   hkBool m_bAllCharactersInScene;
   hkBool m_bAllCharactersAtMarks;
};
extern const hkClass BSSynchronizedClipGeneratorClass;

