/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKCOLLIDE_SHAPE_SAMPLEDHEIGHTFIELD_HKSAMPLEDHEIGHTFIELDBASECINFO_XML_H
#define HKCOLLIDE_SHAPE_SAMPLEDHEIGHTFIELD_HKSAMPLEDHEIGHTFIELDBASECINFO_XML_H

/// hkpSampledHeightFieldBaseCinfo meta information
extern const class hkClass hkpSampledHeightFieldBaseCinfoClass;

/// The information needed to construct an hkSampledHeightFieldBase
class hkpSampledHeightFieldBaseCinfo
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpSampledHeightFieldBaseCinfo );
	
			/// Default constructor
		hkpSampledHeightFieldBaseCinfo()
		{
			m_scale.set(1.0f, 1.0f, 1.0f);
			m_xRes = 2;
			m_zRes = 2;
			m_minHeight = 0.0f;
			m_maxHeight = -1.0f;
			m_useProjectionBasedHeight = false;
		}			
		
		//
		// Members
		//
	public:
		
			/// The heightfield scale in x,y,z.
		hkVector4 m_scale;
		
			/// The resolution along x.
		hkInt32 m_xRes;
		
			/// The resolution along z (y up).
		hkInt32 m_zRes;
		
			/// The minimum height returned by the heightfield.
		hkReal m_minHeight;
		
			/// The maximum height returned by the heightfield. Note: If m_maxHeight is set to
			/// less than m_minHeight than constructor will scan the entire heightfield)
		hkReal m_maxHeight;

			/// The method used to calculated the height.
			/// By default (m_useProjectionBasedHeight = false) the height will be measured by
			/// projecting the vertex down onto the heightfield and using this pure vertical
			/// distance. This results in a smooth distance function but fails for steep slopes. As
			/// a result objects hitting a steep slope can get extra energy.
			/// By setting m_useProjectionBasedHeight to true, the real distance between the point
			/// and the triangle will be measured. This allows for steep
			/// slopes but results in a non continuous distance function, which will cause objects
			/// to jitter when they cross triangle boundaries, especially objects with a big radius
			/// like spheres.
		hkBool m_useProjectionBasedHeight;
};

#endif // HKCOLLIDE_SHAPE_SAMPLEDHEIGHTFIELD_HKSAMPLEDHEIGHTFIELDBASECINFO_XML_H

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
