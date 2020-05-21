#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Object/hkReferencedObject.h>

class hkbCharacterSkinInfo : public hkReferencedObject
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbCharacterSkinInfo(void) {}
   HK_FORCE_INLINE hkbCharacterSkinInfo( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) 
   , m_deformableSkins(flag)
   , m_rigidSkins(flag)
      {}
   // Properties
   hkUint64 m_characterId;
   hkArray<hkUint64> m_deformableSkins;
   hkArray<hkUint64> m_rigidSkins;
};
extern const hkClass hkbCharacterSkinInfoClass;

