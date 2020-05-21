/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_COMMON_RESOURCE_HANDLE_H
#define INC_COMMON_RESOURCE_HANDLE_H


#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkClassMemberAccessor.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>

extern const class hkClass hkMemoryResourceHandleExternalLinkClass;

extern const class hkClass hkResourceContainerClass;

extern const class hkClass hkResourceHandleClass;

extern const class hkClass hkResourceBaseClass;


extern const hkClass hkMemoryResourceHandleClass;
extern const hkClass hkMemoryResourceContainerClass;


class hkResourceContainer;

class hkResourceMap;

class hkResourceBase: public hkReferencedObject
{
	public:

		//+vtable(true)
		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT );

			//
		hkResourceBase(): hkReferencedObject() {}

			// serializing constructor
		hkResourceBase(hkFinishLoadedObjectFlag flag): hkReferencedObject(flag) {}

		enum Type
		{
			TYPE_RESOURCE,
			TYPE_CONTAINER
		};

			/// return the type of this object
		virtual Type getType() const = 0;

			/// Returns the name of the resource.
			/// If the name has to be created on the fly the supplied buffer will be used.
		virtual const char* getName(hkStringBuf& buffer) const = 0;
};


	/// A virtual interface to a resource which is owned by the hkResourceContainer
class hkResourceHandle: public hkResourceBase
{
	public:

		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT );

		virtual Type getType() const { return TYPE_RESOURCE; }




			/// An external link
		struct Link
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkResourceHandle::Link);
			Link() {}
			Link(void* object, const hkClassMember* member) : m_memberAccessor(object, member) {}

			const char*				m_memberName;
			const char*				m_externalId;
			hkClassMemberAccessor	m_memberAccessor;
		};



			/// Set the name of the resource.
			/// A copy of the name will be stored.
		virtual void setName(const char* name) = 0;

			/// Returns a pointer to the object.
		virtual void* getObject() const = 0;

			/// Returns a pointer to the class.
		virtual const hkClass* getClass() const = 0;

			/// Set the object and class. If the object is of type hkReferencedObject, a reference will be added.
		virtual void setObject(void* object, const hkClass* klass) = 0;

			/// Adds a new external link, specified by the 'name' of the referencing object as well as the memberName.
			/// If the object uses nested structures, the memberName will look like: xxx.yyy.zzz
		virtual void addExternalLink(const char* memberName, const char* m_externalId) = 0;

			/// removes an internal link
		virtual void removeExternalLink( const char* memberName ) = 0;

			/// Returns a list with all unresolved external links.
		virtual void getExternalLinks(hkArray<Link>& linksOut) = 0;

			/// Clears the list with all external links.
		virtual void clearExternalLinks() = 0;

			/// tryToResolveLinks
		virtual void tryToResolveLinks(hkResourceMap& map);


	protected:
		hkResourceHandle(hkFinishLoadedObjectFlag flag): hkResourceBase(flag) {}
		hkResourceHandle() : hkResourceBase() {}
		virtual ~hkResourceHandle() {}
};

	/// The owner of a resource handle
class hkResourceContainer: public hkResourceBase
{
	public:

		//+vtable(true)
		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT );

			// hkResourceBase implementation
		virtual Type getType() const { return TYPE_RESOURCE; }

			/// Create an owned resource
		virtual hkResourceHandle* createResource(const char* name, void* object, const hkClass* klass) = 0;

			/// Destroys a resource
		virtual void destroyResource( hkResourceHandle* handle ) = 0;

			///	Get number of resources
		virtual int getNumResources() = 0;

			/// Tries to find a named resource with given hkClass.
			/// It returns the first object after \a prevObject, with \a resourceName.
			///   - If \a prevObject is null then the search begins from the start of the container.
			///   - If \a resourceName is null than all objects will match and the method will return the first match.
		virtual hkResourceHandle* findResourceByName( const char* resourceName, const hkClass* klass = HK_NULL, const hkResourceHandle* prevObject = HK_NULL ) const = 0;

			/// Simple helper function to find a resource which is identified by a path (using '/' to split the path).
		//hkResourceHandle* findResourceRecursively( const char* resourcePath);

			/// Simple helper function to get all terminal resources
		void findAllResourceRecursively( hkArray<hkResourceHandle*>& resourcesOut );


			/// Simple helper function to get all terminal resources
		void findAllContainersRecursively( hkArray<hkResourceContainer*>& resourcesOut );

		void getPath( hkStringBuf& pathOut );

			/// Get my parent container if any
		virtual hkResourceContainer* getParent() = 0;

			/// Create child container, if the container already exists, simply return the existing one.
		virtual hkResourceContainer* createContainer(const char* path) = 0;

			/// Destroys a child container recursively
		virtual void destroyContainer( hkResourceContainer* container ) = 0;

			///	Get number of child containers
		virtual int getNumContainers() = 0;

			/// Tries to find a named container.
			/// It returns the first object after \a prevContainer, with \a containerName.
			///   - If \a prevContainer is null then the search begins from the start of the child containers.
			///   - If \a containerName is null then all objects will match and the method will return the first match.
		virtual hkResourceContainer* findContainerByName( const char* containerName, const hkResourceContainer* prevContainer = HK_NULL ) const = 0;

			/// move this container to a new parent
		virtual hkResult parentTo( hkResourceContainer* newParent ) = 0;

		//
		//	Simple helper functions
		//
			/// helper function which tries to resolve external links
		virtual void tryToResolveLinks( hkResourceMap& resourceMap );

		template<typename T>
		T* findResource( const char* name, const hkClass* klass )
		{
			hkResourceHandle* handle = findResourceByName( name, klass );
			if (!handle)
			{
				return HK_NULL;
			}
			return reinterpret_cast<T*>(handle->getObject());
		}

	protected: 
			// Empty constructor
		hkResourceContainer() : hkResourceBase() {}
		virtual ~hkResourceContainer() {}

			// Serializing constructor
		hkResourceContainer(hkFinishLoadedObjectFlag flag) : hkResourceBase(flag) {}
};


	/// Simple class which allows for fast searching of object/classes
class hkResourceMap
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkResourceMap);

			/// Tries to find a resource with matching name and hkClass
			/// to the first object after prevObject, with a name corresponding to 'objectName'.
			///   - If prevObject is null then the search begins from the start of the container.
			///   - If objectName is null than all objects will match.
			///   = \a klass is only used for extra debugging checks, it cannot be used for searching for a type!
		virtual void* findObjectByName( const char* objectName, const hkClass** klassOut = HK_NULL ) const = 0;

	protected:

		virtual ~hkResourceMap(){}

		hkResourceMap(){}
};




	/// A simple Havok serializable version of a resource
class hkMemoryResourceHandle: public hkResourceHandle
{
	//+version(2)
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT );
		HK_DECLARE_REFLECTION();

			/// Constructor
		hkMemoryResourceHandle();

			// Serializing Constructor
		hkMemoryResourceHandle( hkFinishLoadedObjectFlag flag);
			
			/// Destructor
		virtual ~hkMemoryResourceHandle();

			/// This structure stores information about an external link.
		struct ExternalLink 
		{
			//+version(1)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT, hkMemoryResourceHandle::ExternalLink );
			HK_DECLARE_REFLECTION();

			ExternalLink() {}
			ExternalLink(hkFinishLoadedObjectFlag f) : m_memberName(f), m_externalId(f) {}

				/// The member name that stores the link to the external object.
				/// If the name will look like xxx.yyy.zzz
			hkStringPtr m_memberName;

				/// The 'name' of the linked object.
			hkStringPtr m_externalId;
		};

		virtual const char* getName(hkStringBuf& buffer) const;

			/// Set the name of the resource.
			/// A copy of the name will be stored.
		void setName(const char* name);

			/// Returns a pointer to the object.
		void* getObject() const;

			/// Returns a pointer to the class.
		const hkClass* getClass() const;

			/// Set the object and class.
		void setObject(void* object, const hkClass* klass);

		void addExternalLink(const char* memberName, const char* m_externalId);

			/// removes an external link
		void removeExternalLink( const char* memberName );

			/// Returns a list with all unresolved external links.
		void getExternalLinks(hkArray<Link>& linksOut);

			/// Clears the list with all external links.
		void clearExternalLinks();

	protected:

		hkRefVariant m_variant;
			// Set m_objectIsRerencedObject default to old behavior 
		hkStringPtr m_name;

		hkArray<struct hkMemoryResourceHandle::ExternalLink> m_references;
};





	/// Simple Havok serializable version of a hkResourceContainer
class hkMemoryResourceContainer : public hkResourceContainer
{
	//+version(1)
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_EXPORT );
		HK_DECLARE_REFLECTION();

		hkMemoryResourceContainer( const char* name = "");

		hkMemoryResourceContainer(hkFinishLoadedObjectFlag flag);

		virtual ~hkMemoryResourceContainer();

		virtual const char* getName(hkStringBuf& buffer) const;

		hkResourceHandle* createResource(const char* name, void* object, const hkClass* klass);

		int getNumResources() { return m_resourceHandles.getSize(); }

		hkResourceHandle* findResourceByName( const char* objectName, const hkClass* klass = HK_NULL, const hkResourceHandle* prevObject = HK_NULL ) const;

		void destroyResource(hkResourceHandle* resourceHandle);

		virtual hkResourceContainer* getParent() {	return m_parent;	}

		virtual hkResourceContainer* createContainer(const char* name);

		virtual void destroyContainer( hkResourceContainer* container );

		virtual int getNumContainers();

		virtual hkResult parentTo( hkResourceContainer* newParent );

		virtual hkResourceContainer* findContainerByName( const char* containerName, const hkResourceContainer* prevContainer = HK_NULL ) const;

	protected:
		hkStringPtr m_name;

		hkMemoryResourceContainer* m_parent;		//+serialized(false)

		hkArray< hkRefPtr<hkMemoryResourceHandle> > m_resourceHandles; 

		hkArray< hkRefPtr<hkMemoryResourceContainer> > m_children; 
};


	/// A simple hash table to hkResourceHandles
class hkContainerResourceMap: public hkResourceMap
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkContainerResourceMap);
		hkContainerResourceMap( class hkResourceContainer* container );

		virtual void* findObjectByName( const char* objectName, const hkClass** klassOut = HK_NULL ) const;

	public:

		hkStringMap<hkResourceHandle*> m_resources;
};



#endif // INC_COMMON_RESOURCE_HANDLE_H

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
