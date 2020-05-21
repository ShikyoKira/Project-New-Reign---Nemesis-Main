/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKSCENEDATA_HKX_ATTRIBUTE__H
#define HKSCENEDATA_HKX_ATTRIBUTE__H

/// hkxAttribute meta information
extern const class hkClass hkxAttributeClass;
class hkReferencedObject;
	/// An attribute is nothing more than a named variant, i.e., a variant (type+value) with a name.
	/// Only some specific types are expected though : hkxAnimatedFloat, hkxAnimatedMatrix, hkxAnimatedQuaternion,
	/// hkxAnimatedVector, hkxSparselyAnimatedBool, hkxSparselyAnimatedEnum, hkxSparselyAnimatedInt and hkxSparselyAnimatedString.
struct hkxAttribute
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkxAttribute );
	HK_DECLARE_REFLECTION();

	hkxAttribute();
	hkxAttribute(hkFinishLoadedObjectFlag f);
	
	void setValue( hkReferencedObject* obj, const hkClass* klass );

		/// This hint specifies extra information for the attribute; in particular, for numeric (float, vector) attributes
		/// it describes how it should be transformed if the scene is transformed.
	enum Hint 
	{
			/// No hint.
		HINT_NONE = 0, 
			/// This attribute should be ignored.
		HINT_IGNORE = 1, 
			/// This attribute should be fully transformed (rotated) but not scaled: it is a direction.
		HINT_TRANSFORM = 2, 
			/// This attribute should be scaled : it is a distance.
		HINT_SCALE = 4, 
			/// This attribute should be fully transformed and scaled : it is a position.
		HINT_TRANSFORM_AND_SCALE= 6, 
			/// This attribute should be flipped upon handiness flip : it is an angle.
		HINT_FLIP = 8 
	};

		/// Name of the attribute.
	hkStringPtr m_name; 
		
		/// Type (hkClass pointer) and pointer to object holding the
		/// data of this attribute. It is owned by this struct
	hkRefVariant m_value;
};

#endif // HKSCENEDATA_HKX_ATTRIBUTE__H

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
