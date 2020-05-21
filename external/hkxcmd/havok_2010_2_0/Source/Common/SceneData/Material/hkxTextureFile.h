/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_MATERIAL_HKXTEXTUREFILE_HKCLASS_H
#define HKSCENEDATA_MATERIAL_HKXTEXTUREFILE_HKCLASS_H

/// hkxTextureFile meta information
extern const class hkClass hkxTextureFileClass;

/// A file based texture.
class hkxTextureFile : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA );
		HK_DECLARE_REFLECTION();
	
		hkxTextureFile() { }
		hkxTextureFile(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_filename(f), m_name(f), m_originalFilename(f) { }
		
		//
		// Members
		//
	public:
		
			/// The filename of the texture data
		hkStringPtr m_filename;	

			/// Name so we can set specific options to convert it in the pipeline
		hkStringPtr m_name;
		hkStringPtr m_originalFilename; // name from original material, so we can always get back to source 

};

#endif // HKSCENEDATA_MATERIAL_HKXTEXTUREFILE_HKCLASS_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
