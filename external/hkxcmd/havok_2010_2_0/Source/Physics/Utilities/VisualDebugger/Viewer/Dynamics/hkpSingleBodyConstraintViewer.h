/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SINGLE_BODY_CONSTRAINT_VIEWER
#define HK_SINGLE_BODY_CONSTRAINT_VIEWER

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpWorldViewerBase.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
class hkpWorld;
class hkVector4;

/// Server side mouse picking implementation.
class hkpSingleBodyConstraintViewer : public hkpWorldViewerBase, protected hkpWorldPostSimulationListener, public hkpEntityListener
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		static hkProcess* HK_CALL create(const hkArray<hkProcessContext*>& contexts);

			/// Registers the hkDebugDisplayProcess with the hkProcessFactory.
		static void HK_CALL registerViewer();

			/// Gets the tag associated with this viewer type
		virtual int getProcessTag() { return m_tag; }

		static inline const char* HK_CALL getName() { return "Constraint Viewer - Single Body"; }

		virtual void postSimulationCallback( hkpWorld* world );


		//
		// Process interface
		//

		virtual void getConsumableCommands( hkUint8*& commands, int& numCommands );

			/// Given a command (that has already been parsed from
			/// the stream), consume the rest of the data.
		virtual void consumeCommand( hkUint8 command );

		virtual void worldRemovedCallback( hkpWorld* world );

		//
		// Entity listener interface
		//

			// We need to know when our picked body is no longer available to query constraints from
		virtual void entityRemovedCallback( hkpEntity* entity );
		virtual void entityDeletedCallback( hkpEntity* entity );


		//
		// Own interface
		//

			/// 'Binds' this viewer as listening to the picked rigid body so it may draw its constraints
		hkBool pickObject( hkUint64 displayObject, const hkVector4& worldPosition );

			/// Removes this object as an entity listener
		void releaseObject();


	protected:

		/// Constructor.
		hkpSingleBodyConstraintViewer(const hkArray<hkProcessContext*>& contexts);

		/// Destructor.
		~hkpSingleBodyConstraintViewer();

		hkpWorld * m_currentWorld;
		// The body whose constraints are currently being displayed
		hkpRigidBody * m_pickedBody;
		
		static int m_tag;
};

#endif // HK_MOUSE_PICKING_HANDLER

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
