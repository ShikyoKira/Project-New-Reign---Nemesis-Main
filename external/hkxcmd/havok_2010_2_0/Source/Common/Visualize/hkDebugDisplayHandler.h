/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_DISPLAY_HANDLER
#define HK_VISUALIZE_DISPLAY_HANDLER

class hkMeshBody;
class hkDisplayGeometry;
class hkDisplayGeometryBuilder;
class hkAabb;
#include <Common/Base/hkBase.h>

/// This class defines the graphical interface to the visual debugger.
/// It is also used to display worlds with the Havok demos.
///
/// IDs: IDs are used to manage persistent objects which are added/updated
/// and removed. It is the responsibility of the user to supply a unique
/// id for every persistent object added and to use this id consistently when
/// updating and removing objects.
/// The current convention when supplying ids is to supply the
/// address of the hkpCollidable where the geometries came from.
/// Geometries are built using the hkpShapeDisplayBuilder class in
/// hkvisualize/hkpShapeDisplayBuilder.h/cpp.
///
/// TAGs: Unique tags can be obtained form the hkViewerFactory::getTag().
/// These can are used for separating display information into different
/// categories so that they can be configured and filtered independently.
/// For example, each Viewer has one or more unique tags.
///
/// The id on the functions for drawing lines, text etc should also be set to
/// the address of the hkpCollidable if the drawings are annotations to a specific
/// object. This allows for filtering on object state/picking/etc to avoid
/// cluttered display. Especially useful for recorded VDB movies. If you cannot
/// associate the line/text/etc with an object, set the id to zero.
///
/// It is expected that an implementation of displayText() and display3dText() manages
/// to show the text in some manner in any case. The displayAnnotation() however can
/// have a null implementation. It is used for additional, non operation relevant
/// debug output only.
class hkDebugDisplayHandler
{
	//+hk.MemoryTracker(ignore=True)
	public:
			/// virtual Destructor
		virtual ~hkDebugDisplayHandler() {} 


		//
		// Geometry functions
		//
			/// Adds a geometry to the display world managed by this display handler.
			/// Please refer to the class description for the meaning of id and tag.
		virtual hkResult addGeometry(const hkArrayBase<hkDisplayGeometry*>& geometries, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint) = 0;

			/// Adds a single geometry.
		virtual hkResult addGeometry(hkDisplayGeometry* geometry, hkUlong id, int tag, hkUlong shapeIdHint);

			/// Adds an instanced of a geometry that has already been added to the display world managed by this display handler.
			/// Please refer to the class description for the meaning of id and tag.
		virtual hkResult addGeometryInstance(hkUlong origianalGeomId, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint) = 0;

			/// Sets the color of a geometry previously added to the display world.
			/// Please refer to the class description for the meaning of id and tag.
		virtual hkResult setGeometryColor(int color, hkUlong id, int tag) = 0;

			/// Sets the transparency of a geometry previously added to the display world.
		virtual hkResult setGeometryTransparency(float alpha, hkUlong id, int tag) = 0;

			/// Updates the transform of a body in the display world.
			/// Please refer to the class description for the meaning of id and tag.
		virtual hkResult updateGeometry(const hkTransform& transform, hkUlong id, int tag) = 0;

			/// Updates the transform of a body in the display world. The matrix can include
			/// scale. Scale is, however, only supported for geometries of type hkDisplayMesh.
		virtual hkResult updateGeometry( const hkMatrix4& transform, hkUlong id, int tag ) = 0;

			/// Updates the root transform of a body in the display world. The hkQsTransform
			/// can include scale. Scale is, however, only supported for geometries of type
			/// hkDisplayMesh.
		hkResult updateGeometry( const hkQsTransform& transform, hkUlong id, int tag );

			/// Skins a body in the display world. Only geometries of type hkDisplayMesh can be skinned.
		virtual hkResult skinGeometry(hkUlong* ids, int numIds, const hkMatrix4* poseModel, int numPoseModel, const hkMatrix4& worldFromModel, int tag ) = 0;

			/// Skins a body in the display world. Only geometries of type hkDisplayMesh can be skinned.
		hkResult skinGeometry(hkUlong* ids, int numIds, const hkQsTransform* poseModel, int numPoseModel, const hkQsTransform& worldFromModel, int tag );

			/// Removes a geometry from the display world managed by this display handler.
			/// Please refer to the class description for the meaning of id and tag.
		virtual hkResult removeGeometry(hkUlong id, int tag, hkUlong shapeIdHint) = 0;
						

		//
		// Camera Functionality
		//
		virtual hkResult updateCamera(const hkVector4& from, const hkVector4& to, const hkVector4& up, hkReal nearPlane, hkReal farPlane, hkReal fov, const char* name) = 0;

		//
		// Immediate Mode Functions
		//

			/// Puts a display point into the display buffer for display in the next frame.
		virtual hkResult displayPoint(const hkVector4& position, int colour, int id, int tag) = 0;

			/// Puts a display line into the display buffer for display in the next frame.
		virtual hkResult displayLine(const hkVector4& start, const hkVector4& end, int color, int id, int tag) = 0;

			/// Puts a display triangle into the display buffer for display in the next frame.
		virtual hkResult displayTriangle(const hkVector4& a, const hkVector4& b, const hkVector4& c, int colour, int id, int tag) = 0;

			/// Outputs user text to the display. The manner in which the text
			/// is displayed depends on the implementation of the display handler. It is not ok to ignore the data.
		virtual hkResult displayText(const char* text, int color, int id, int tag) = 0;

			/// Outputs 3D text. Same as displayText() but with position.
		virtual hkResult display3dText(const char* text, const hkVector4& pos, int color, int id, int tag) = 0;

			/// Display general annotation data. It is up to the handler implementation whether this shows up or not
			/// in the graphics display or on some other output stream. It is ok to ignore the data.
		virtual hkResult displayAnnotation(const char* text, int id, int tag) { return HK_SUCCESS; }

			/// Displays a bone. This is optional and not all display handlers implement this interface
		virtual hkResult displayBone(const hkVector4& a, const hkVector4& b, const hkQuaternion& orientation, int color, int tag ) { return HK_SUCCESS; }

			/// Displays the geometries
		virtual hkResult displayGeometry(const hkArrayBase<hkDisplayGeometry*>& geometries, const hkTransform& transform, int color, int id, int tag) = 0;

			/// Displays the geometries without transform
		virtual hkResult displayGeometry(const hkArrayBase<hkDisplayGeometry*>& geometries, int color, int id, int tag) = 0;			

		// Utility functions (just call displayLine etc above)
		void displayFrame( const hkQsTransform& worldFromLocal, hkReal size, int id, int tag );
		void displayFrame( const hkTransform& worldFromLocal, hkReal size, int id, int tag );
		void displayArrow( const hkVector4& from, const hkVector4& dir, int color, int id, int tag );
		void displayStar( const hkVector4& position, hkReal scale, int color, int id, int tag );
		void displayModelSpacePose(int numTransforms, const hkInt16* parentIndices, const hkQsTransform* modelSpacePose, const hkQsTransform& worldFromModel, int color, int id, int tag);
		void displayLocalSpacePose(int numTransforms, const hkInt16* parentIndices, const hkQsTransform* localSpacePose, const hkQsTransform& worldFromModel, int color, int id, int tag);

		//
		// Statistics functions (ideally these would be in a separate interface to the display handler)
		//
		virtual hkResult sendMemStatsDump(const char* data, int length) = 0;


			/// Ensures that the current immediate mode display information
			/// will be preserved and merged with all new immediate mode data
			/// for the next step/frame.
		virtual hkResult holdImmediate() = 0;


			/// Batch update speed increases
		virtual void lockForUpdate() { }
		virtual void unlockForUpdate() { }

		// Support for sending geometries in parts.

			/// Add a geometry by providing the ingredients needed to build it.
			/// The default implementation immediately builds the geometry and adds it.
			/// \param source the object which can be converted to geometries.
			/// \param builder an object which can convert the source to display geometries.
		virtual hkResult addGeometryLazily( const hkReferencedObject* source, hkDisplayGeometryBuilder* builder, const hkTransform& transform, hkUlong id, int tag, hkUlong shapeIdHint);

		// Support for vdb geometry caching.

		typedef hkUint64 Hash;

			/// Does this viewer support hashing?
		virtual hkBool doesSupportHashes() const { return false; }

			/// When supported, this tells the handler to add a geometry corresponding to the hash
			/// and provides the source and builder which can be used to obtain the geometries if
			/// the handler has not seen that hash before.
			/// \param source the object which can be converted to geometries.
			/// \param builder an object which can convert the source to display geometries.
			/// \param hash a hash which represents the geometry to be built.
			/// \param aabb the AABB of the geometry which can be used to represent the geometry if the hash isn't found.
		virtual hkResult addGeometryHash( const hkReferencedObject* source, hkDisplayGeometryBuilder* builder, const Hash& hash, const hkAabb& aabb, hkUint32 color, const hkTransform& transform, hkUlong id, int tag ) { return HK_FAILURE; }			

			/// Calls addGeometryHash without aabb, color or transform.
		hkResult addGeometryHash( const hkReferencedObject* source, hkDisplayGeometryBuilder* builder, const Hash& hash, hkUlong id, int tag );
};


#endif // HK_VISUALIZE_DISPLAY_HANDLER

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
