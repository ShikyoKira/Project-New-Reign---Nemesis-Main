/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKANIMATION_SKINNING_UTILS_H
#define HKANIMATION_SKINNING_UTILS_H

class hkaSkeleton;
class hkaMeshBinding;
class hkxSkinBinding;

	/// Utility methods for skinned meshes.
class hkaSkinningUtils
{
	public:			
			/// Changes binding so that no section in the binding's mesh requires more
			/// than boneLimit bones during skinning. This is typically used to setup a
			/// binding for hardware skinning that has an upper limit on the number of
			/// bones it can handle. Can only limit the binding if it has a simple mapping
			/// (i.e., hkaMeshBinding::m_mappings is empty). Returns true if the binding's
			/// mesh was changed, false otherwise.
		static hkBool HK_CALL limitMesh( hkaMeshBinding* binding, int boneLimit );
		
			/// Changes binding so that no section in the binding's mesh requires more
			/// than boneLimit bones during skinning. Same as limitMesh above but takes
			/// a mapping of bones used in the binding to what index they correspond to
			/// in the skeleton the skin is bound to.
		static hkBool HK_CALL limitMesh( hkaMeshBinding* binding, const hkArray<hkInt16>& mapOrigToSkel, int boneLimit );					
};

#endif // HKANIMATION_SKINNING_UTILS_H

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
