/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BASE_LOCAL_FRAME_H
#define HK_BASE_LOCAL_FRAME_H


extern const class hkClass hkLocalFrameGroupClass;

extern const class hkClass hkLocalFrameClass;

extern const class hkClass hkSimpleLocalFrameClass;

class hkLocalFrameCollector;
class hkLocalFrameGroup;

	/// An abstract local frame that can be part of a hierarchy of local frames.
class hkLocalFrame : public hkReferencedObject
{
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE );

			/// Default constructor does nothing.
		hkLocalFrame() {}

			/// Finish constructor (for internal use).
		hkLocalFrame( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag) {}

			/// Get the transform from the coordinates of this local frame to the coordinates
			/// at the root of the local frame hierarchy.
		void getTransformToRoot( hkTransform& transform ) const;

			/// Get the position of this local frame in the coordinates
			/// at the root of the local frame hierarchy.
		void getPositionInRoot( hkVector4& position ) const;

			/// Return the transform from the coordinates of this frame to
			/// the coordinates of the parent frame.
		virtual void getLocalTransform( hkTransform& transform ) const = 0;

			/// Sets the local space transform
		virtual void setLocalTransform( const hkTransform& transform ) = 0;

			/// Return the position of this frame in the coordinates of the parent frame.
			/// The default implementation calls getLocalTransform() and then pulls the
			/// position from the transform.
		virtual void getLocalPosition( hkVector4& position ) const;

			/// Get all of the frames near a given point.
		virtual void getNearbyFrames( const hkVector4& target, hkReal maxDistance, hkLocalFrameCollector& collector ) const = 0;

			/// Get the name of the character (can be HK_NULL).
		virtual const char* getName() const = 0;

			/// Get the local frame that is this frame's parent.
		virtual const hkLocalFrame* getParentFrame() const = 0;

			/// Set the local frame that is this frame's parent.
		virtual void setParentFrame( const hkLocalFrame* parentFrame ) = 0;

			/// Get the number of children of this local frame
		virtual int getNumChildFrames() const = 0;

			/// Get the i'th child of this hkLocalFrame
		virtual hkLocalFrame* getChildFrame( int i ) const = 0;

			/// Get the group to which this frame belongs.
		virtual const hkLocalFrameGroup* getGroup() const = 0;

			/// Set the group to which this frame belongs.
		virtual void setGroup( const hkLocalFrameGroup* localFrameGroup ) = 0;

			/// Gets all children in the local frame and it's descendants.
		virtual void getDescendants( hkArrayBase<const hkLocalFrame*>& descendants, hkMemoryAllocator& a) const;

			/// Gets all children in the local frame and it's descendants.
		HK_FORCE_INLINE void getDescendants(hkArray<const hkLocalFrame*>& descendants ) const
		{ 
			getDescendants(descendants, hkContainerHeapAllocator().get(&descendants));
		}
};

	/// An abstract class for collecting local frames.
class hkLocalFrameCollector : public hkReferencedObject
{
	public:
	
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE );

			/// Add a local frame to the collector.
		virtual void addFrame( const hkLocalFrame* frame, hkReal distance ) = 0;
};

	/// All the local frames with the same group name share a hkLocalFrameGroup.
class hkLocalFrameGroup : public hkReferencedObject
{
	//+vtable(true)
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE );

		/// Default constructor( The memory associated with the name must be managed by the you ).
		hkLocalFrameGroup( const char* name ) : m_name(name) {}

			/// Finish constructor (for internal use).
		hkLocalFrameGroup( hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag), m_name(flag) {}

			/// Get the name of the group (cannot be HK_NULL).
		const char* getName() const { return m_name; }

	private:

			/// The name of this group.
		hkStringPtr m_name;
};


	/// A local frame that stores a transform, name, and parent and children pointers.
class hkSimpleLocalFrame : public hkLocalFrame
{
	public:
		// +version(1)

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE );

			/// Default constructor does nothing.
		hkSimpleLocalFrame() : m_parentFrame(HK_NULL), m_group(HK_NULL), m_name(HK_NULL) {}

			/// Finish constructor (for internal use).
		hkSimpleLocalFrame( hkFinishLoadedObjectFlag flag ) : hkLocalFrame(flag), m_children(flag), m_name(flag) {}

			/// Destructor removes references to the children.
		~hkSimpleLocalFrame();

			// hkLocalFrame implementation
		virtual void getLocalTransform( hkTransform& transform ) const;

			/// Sets the local space transform
		virtual void setLocalTransform( const hkTransform& transform );

			// hkLocalFrame implementation
		virtual void getLocalPosition( hkVector4& position ) const;

			// hkLocalFrame implementation
		virtual void getNearbyFrames( const hkVector4& target, hkReal maxDistance, hkLocalFrameCollector& collector ) const;

			// hkLocalFrame implementation
		virtual const char* getName() const { return m_name; }

			// hkLocalFrame implementation
		virtual const hkLocalFrame* getParentFrame() const { return m_parentFrame; }

			// hkLocalFrame implementation
		virtual void setParentFrame( const hkLocalFrame* parentFrame ) { m_parentFrame = parentFrame; }

			// hkLocalFrame implementation
		virtual int getNumChildFrames() const;

			// hkLocalFrame implementation
		virtual hkLocalFrame* getChildFrame( int i ) const;

			// hkLocalFrame implementation
		virtual const hkLocalFrameGroup* getGroup() const { return m_group; }

			// hkLocalFrame implementation
		virtual void setGroup( const hkLocalFrameGroup* group );

			/// Transforms points from this coordinate frame to the coordinate system of the parent frame.
		hkTransform m_transform;

			/// The children of this frame. This frame owns the children, so a reference will be removed
			/// when this frame is destroyed. If you build one of these objects in code, make sure
			/// to add a reference to each child.
		hkArray<class hkLocalFrame*> m_children;

			/// The parent frame. This frame does not own the parent, so we don't do any reference counting.
		const hkLocalFrame* m_parentFrame; //+owned(false)

			/// The group that this local frame belongs to (can be HK_NULL).
		const hkLocalFrameGroup* m_group;

			/// The name of this frame.
		hkStringPtr m_name;
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
