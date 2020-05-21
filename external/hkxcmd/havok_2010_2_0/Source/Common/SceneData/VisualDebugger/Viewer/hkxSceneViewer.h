/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKX_SCENE_VIEWER_H
#define HKX_SCENE_VIEWER_H

#include <Common/Visualize/hkProcess.h>
#include <Common/SceneData/VisualDebugger/hkxSceneDataContext.h>

	/// Displays all meshes in the hkxSceneDataContext
class hkxSceneViewer: public hkReferencedObject,
					  public hkProcess,
					  public hkxSceneDataContextListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TOOLS);

			/// Creates a hkxSceneViewer.
		static hkProcess* HK_CALL create(const hkArray<hkProcessContext*>& contexts);

			/// Registers the hkxSceneViewer with the hkViewerFactory.
		static void HK_CALL registerViewer();

			/// Gets the tag associated with this viewer type
		virtual int getProcessTag() { return m_tag; }

			/// The name of the viewer shown in the VDB
		static inline const char* HK_CALL getName() { return "Scene Data"; }
		
			/// Initializes the viewer
		hkxSceneViewer( hkxSceneDataContext* context );
			/// Destroys the viewer
		virtual ~hkxSceneViewer();
			
			/// Connects the viewer to scene
		virtual void sceneAddedCallback( hkxScene* scene );
			/// Disconnects the viewer from scene
		virtual void sceneRemovedCallback( hkxScene* scene );

			/// Raised after the viewer has been initialized
		virtual void init();

	protected:
		
			/// Handle to the context which this viewer is connected to
		hkxSceneDataContext* m_context;			

			/// Builder used to build hkDisplayGeometrys from hkMeshBodys
		hkDisplayGeometryBuilder* m_displayGeometryBuilder;

			/// Unique ID for the viewer
		static int m_tag;
};

#endif

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
