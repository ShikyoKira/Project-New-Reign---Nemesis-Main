/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FOOTSTEP_ANALYSIS_INFO_H
#define HK_FOOTSTEP_ANALYSIS_INFO_H

/// hkaAnimationBinding meta information
extern const class hkClass hkaFootstepAnalysisInfoClass;
extern const class hkClass hkaFootstepAnalysisInfoContainerClass;

/// The hkaFootstepAnalysisInfo class stores visualization
/// information for footstep analysis. It is intended for use by
/// the content tool chain.
class hkaFootstepAnalysisInfo : public hkReferencedObject
{
	public:

		//+version(1)

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ANIMATION);

		// Constructor
		hkaFootstepAnalysisInfo( int numFrames, hkReal duration, hkReal posTol, hkReal velTol,
								 const char* name, const char* nameStrike, const char* nameLift, const char* nameLock, const char* nameUnlock );
		hkaFootstepAnalysisInfo(hkFinishLoadedObjectFlag f);
		virtual ~hkaFootstepAnalysisInfo();

		// Update state information for a given frame
		void updatePosition( int frame, hkReal value );
		void updateVelocity( int frame, hkReal value );
		void updateBones( int frame, hkBool isDown );

		// Utility functions
		static hkReal getMax( const hkArrayBase< hkReal >& arr );
		static hkReal getMin( const hkArrayBase< hkReal >& arr );


		// Name information
		hkArray< char > m_name;
		hkArray< char > m_nameStrike;
		hkArray< char > m_nameLift;
		hkArray< char > m_nameLock;
		hkArray< char > m_nameUnlock;

		// Position and velocity information per frame
		hkArray< hkReal > m_minPos;
		hkArray< hkReal > m_maxPos;
		hkArray< hkReal > m_minVel;
		hkArray< hkReal > m_maxVel;

		// Foot state information per frame 0.0=down, 1.0=up (driven by the need to simplify display)
		hkArray< hkReal > m_allBonesDown;
		hkArray< hkReal > m_anyBonesDown;

		// Tolerances used for analysis
		hkReal m_posTol;
		hkReal m_velTol;
		hkReal m_duration;
};


// Utility class to store hkaFootstepAnalysisInfo objects
class hkaFootstepAnalysisInfoContainer : public hkReferencedObject
{
	public:

		//+version(1)

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ANIMATION);

		hkaFootstepAnalysisInfoContainer();
		virtual ~hkaFootstepAnalysisInfoContainer();

		// Creates a new hkaFootstepAnalysisInfo object and adds it to this container
		hkaFootstepAnalysisInfo* newPreviewInfo( int numFrames, hkReal duration, hkReal posTol, hkReal velTol,
			const char* name, const char* nameStrike, const char* nameLift, const char* nameLock, const char* nameUnlock );

		// Storage for hkaFootstepAnalysisInfo objects
		hkArray< hkRefPtr< class hkaFootstepAnalysisInfo > > m_previewInfo;

	public:

		// Constructor for initialisation of vtable fixup
		HK_FORCE_INLINE hkaFootstepAnalysisInfoContainer( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag), m_previewInfo(flag) {}
};

#endif // HK_FOOTSTEP_ANALYSIS_INFO_H

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
