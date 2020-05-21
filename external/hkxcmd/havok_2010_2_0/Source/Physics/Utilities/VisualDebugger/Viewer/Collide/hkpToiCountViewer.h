/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TOI_COUNT_VIEWER_H
#define HK_TOI_COUNT_VIEWER_H

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpWorldViewerBase.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

class hkpDebugDisplayHandler;

/// This viewer displays a count of TOIs for those bodies involved in TOI collisions.
/// Counts for the current frame and a total (since the viewer was added) are displayed.
/// 
/// Invariant: The viewer is an entity listener for exactly the set of bodies which are keys in its map.
class hkpToiCountViewer : public hkpWorldViewerBase, public hkpContactListener, public hkpWorldPostSimulationListener, hkpEntityListener
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Constructor.
		hkpToiCountViewer( const hkArray<hkProcessContext*>& contexts );

			/// Destructor.
		~hkpToiCountViewer();

			/// Resets the counts for all bodies.
		void reset();

		// hkpWorldViewerBase interface.
		virtual void worldRemovedCallback( hkpWorld* world );
		virtual void worldAddedCallback( hkpWorld* world );

		// For registering the viewer.
		static const char* getName() { return "TOI counter"; }
		static hkProcess* HK_CALL create( const hkArray<hkProcessContext*>& contexts );
		static void HK_CALL registerViewer();
		
		// hkpProcess interface.
		virtual void init();
		virtual int getProcessTag();

		// hkpContactListener interface.
		virtual void contactPointCallback( const hkpContactPointEvent& event );

		// hkpWorldPostSimulationListener interface.
		virtual void postSimulationCallback( hkpWorld* world );

		// hkpEntityListener interface.
		virtual void entityRemovedCallback( hkpEntity* entity );
		virtual void entityDeletedCallback( hkpEntity* entity );
	public:
			/// The type of the map from entities to counts.
			/// High bits of the value type are used for the frame count, and low bits for the total.
		typedef hkPointerMap<hkpRigidBody*, hkUint32> MapType;
		
			/// The count storage.
		MapType m_toiCounts;

			/// The process tag.
		static int s_tag;

	public:
			/// Text is displayed at the face of the AABB specified by these values.
		enum DisplayPosition 
		{
			POSITIVE_X,
			POSITIVE_Y,
			POSITIVE_Z,
			NEGATIVE_X,
			NEGATIVE_Y,
			NEGATIVE_Z
		};

			/// Given a gravity vector, return the displayPosition closest to up.
		static DisplayPosition HK_CALL getDisplayPositionFromGravityVector( const hkVector4& gravity );

			/// Display text "at" a rigid body. The text is put just beyond the specified face of the AABB and,
			/// optionally, a line is drawn from the text to the body's center of gravity.
			/// \param tag see hkDebugDisplayHandler.
			/// \param extraDistance the distance beyond the face of the AABB that the text is displayed.
		static void HK_CALL displayTextAtBody( hkDebugDisplayHandler* handler, int tag, hkpRigidBody* body, DisplayPosition pos, const char* text, int color, hkReal extraDistance = 0.1f, hkBool drawLine = true );

	public:
			/// Display the provided body's TOI counts and zero its count for this frame.
		void displayCountForBody( hkpRigidBody* body, DisplayPosition displayPosition );
};

#endif // HK_TOI_COUNT_VIEWER_H

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
