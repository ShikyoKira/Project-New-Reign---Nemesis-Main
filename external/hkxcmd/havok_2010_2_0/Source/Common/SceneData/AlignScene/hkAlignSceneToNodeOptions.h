/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKSCENEDATA_ALIGNSCENE_ALIGNSCENETONODE_H
#define HKSCENEDATA_ALIGNSCENE_ALIGNSCENETONODE_H

/// hkAlignSceneToNodeOptions meta information
extern const class hkClass hkAlignSceneToNodeOptionsClass;

/// For more explanation of why this class declaration is in the SDK, refer to EXP-1656 and EXP-1655
/// This class is used to communicate information between the "Align Scene To Node"
/// and the "Create Animations" filters.
///
/// Describes the align scene to node options
/// This class is an exact mirror of the hctAlignSceneToNodeOptions class. Please only update one with a similar change to the other
class hkAlignSceneToNodeOptions : public hkReferencedObject
{
	//+version(2)
	//+vtable(true)
	public:
	
		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ANIMATION);

		hkAlignSceneToNodeOptions();
		hkAlignSceneToNodeOptions(hkFinishLoadedObjectFlag f);

		void setNodeName( const char* name );
		const hkStringPtr& getNodeName();

		//
		// Members
		//
	public:

		hkBool m_invert;
		hkBool m_transformPositionX;
		hkBool m_transformPositionY;
		hkBool m_transformPositionZ;
		hkBool m_transformRotation;
		hkBool m_transformScale;
		hkBool m_transformSkew;
		hkInt32 m_keyframe;

	private:

		hkStringPtr m_nodeName;

};

#endif // HKSCENEDATA_ALIGNSCENE_ALIGNSCENETONODE_H

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
