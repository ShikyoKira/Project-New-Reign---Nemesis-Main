/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_ATTRIBUTE_CLASS_H
#define HK_ATTRIBUTE_CLASS_H

#include <Common/Base/Reflection/hkClassMember.h>
#include <Common/Base/Reflection/hkClassEnum.h>
#include <Common/Base/Math/hkMath.h>

extern const hkClass hkRangeRealAttributeClass;

	/// An attribute for specifying the range of possible values for an hkReal.
struct hkRangeRealAttribute
{
	//+defineattribute(true)
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkRangeRealAttribute );
		HK_DECLARE_REFLECTION();

		inline hkRangeRealAttribute(hkReal absmin, hkReal absmax, hkReal softmin, hkReal softmax) :
			m_absmin(absmin), m_absmax(absmax), m_softmin(softmin), m_softmax(softmax)
		{
			HK_ASSERT2(0x2fd9e4a5, m_absmin <= m_absmax, "Make sure that absmin is less or equal than absmax.");
			HK_ASSERT2(0x2fd9e4a6, m_softmin <= m_softmax, "Make sure that softmin is less or equal than softmax.");
			HK_ASSERT2(0x2fd9e4a7, m_absmin <= m_softmin && m_softmax <= m_absmax, "Make sure that softmin and softmax are within [absmin,absmax] range.");
		}

			/// The value should never be lower than this.
			/// Default minimum is -HK_REAL_MAX.
		hkReal m_absmin;

			/// The value should never be higher than this.
			/// Default maximum is HK_REAL_MAX.
		hkReal m_absmax;

			/// The value should typically not by lower than this.
		hkReal m_softmin;

			/// The value should typically not be higher than this.
		hkReal m_softmax;
};

extern const hkClass hkRangeInt32AttributeClass;

	/// An attribute for specifying the range of possible values for an hkInt32.
struct hkRangeInt32Attribute
{
	//+defineattribute(true)
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkRangeInt32Attribute );
		HK_DECLARE_REFLECTION();
			
		inline hkRangeInt32Attribute(hkInt32 absmin, hkInt32 absmax, hkInt32 softmin, hkInt32 softmax) :
		m_absmin(absmin), m_absmax(absmax), m_softmin(softmin), m_softmax(softmax)
		{
			HK_ASSERT2(0x2fd9e4a8, m_absmin <= m_absmax, "Make sure that absmin is less or equal than absmax.");
			HK_ASSERT2(0x2fd9e4a9, m_softmin <= m_softmax, "Make sure that softmin is less or equal than softmax.");
			HK_ASSERT2(0x2fd9e4b0, m_absmin <= m_softmin && m_softmax <= m_absmax, "Make sure that softmin and softmax are within [absmin,absmax] range.");
		}
			/// The value should never be lower than this.
			/// Default minimum is HK_INT32_MIN.
		hkInt32 m_absmin;

			/// The value should never be higher than this.
			/// Default maximum is HK_INT32_MAX.
		hkInt32 m_absmax;

			/// The value should typically not by lower than this.
		hkInt32 m_softmin;

			/// The value should typically not be higher than this.
		hkInt32 m_softmax;
};

extern const hkClass hkUiAttributeClass;

	/// An attribute for specifying editor specific information (visibility, editability, ...)
struct hkUiAttribute
{
	//+defineattribute(true)
	//+version(2)
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkUiAttribute );
		HK_DECLARE_REFLECTION();

		// This is a bitfield(!) used to filter out certain modelers from displaying selected GUI elements.
		enum HideInModeler
		{
			NONE = 0,

			MAX  = 1,			///< This member will not be displayed in 3ds Max's GUI.
			MAYA = 2,			///< This member will not be displayed in Maya's GUI.
		};

			/// Defines whether the member is visible in an editor
		hkBool m_visible;

			/// Can be used to explicitly hide GUI elements in certain modelers.
		hkEnum<HideInModeler,hkInt8> m_hideInModeler;

			/// An alternative label for the GUI element.
		const char* m_label;

			/// Opens a new subgroup with the specified name.
		const char* m_group;

			/// A comma-separated list [string] used to hide any usually visible baseclass members. This tag can only be used on a class itself and will
			/// be ignored when used on a member.
		const char* m_hideBaseClassMembers;

			/// Close the current subgroup.
		hkBool m_endGroup;

			/// Close a second subgroup on the same line. Kinda hacky right now.
		hkBool m_endGroup2;

			/// If set to true, this member will only show up under the 'Advanced Settings' section in the GUI.
		hkBool m_advanced;
};

extern const hkClass hkGizmoAttributeClass;

	/// An attribute for specifying visual editing aids ('Gizmos') to be displayed in the modeler
struct hkGizmoAttribute
{
	//+defineattribute(true)
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkGizmoAttribute );
		HK_DECLARE_REFLECTION();

		enum GizmoType
		{
			POINT = 0,		///< This draws a simple point (no extent)
			SPHERE,			///< This draws a sphere
			PLANE,			///< This draws a plane
			ARROW			///< This draws an arrow
		};

			/// Defines whether the gizmo is visible in the modeler
		hkBool m_visible;

			/// An alternative label for the GUI element.
		const char* m_label;

			/// The gizmo type.
		hkEnum<GizmoType,hkInt8> m_type;
};

extern const hkClass hkModelerNodeTypeAttributeClass;

	/// An attribute for specifying the type of base modeler node this class needs
struct hkModelerNodeTypeAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkModelerNodeTypeAttribute );

	enum ModelerType
	{
		DEFAULT = 0,

		LOCATOR,	///< This class should use a locator type node.
	};

		/// Can be used to define the modeler node type
	hkEnum<ModelerType,hkInt8> m_type;
};

extern const hkClass hkLinkAttributeClass;

/// An attribute for specifying the type of link for a class member (only appropriate for a pointer or string)
struct hkLinkAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkLinkAttribute );

	enum Link
	{
		NONE = 0,
		
		DIRECT_LINK,	///< This member represents a link to the object being pointed to.
		CHILD,			///< This member represents a link to an object which must child off the current object.
		MESH,			///< This member represents a link to a mesh (as defined by the relevant external modeler/editor.
		PARENT_NAME,	///< This member represents a link to the name of the object (effectively will store the name of the object in the string)
	};

		/// Can be used to define the link type
	hkEnum<Link,hkInt8> m_type;
};

extern const hkClass hkSemanticsAttributeClass;

	/// An attribute for specifying more details about a class member
struct hkSemanticsAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkSemanticsAttribute );

	enum Semantics
	{
		UNKNOWN = 0,
		DISTANCE,
		ANGLE,
		NORMAL,
		POSITION,
		COSINE_ANGLE,
	};

		/// Can be used to define the parameter in more detail.
	hkEnum<Semantics,hkInt8> m_type;
};

extern const hkClass hkDescriptionAttributeClass;

	/// An attribute giving a string description to a class member.
struct hkDescriptionAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkDescriptionAttribute );

		/// The description string.
	const char* m_string;
};

extern const hkClass hkArrayTypeAttributeClass;

	/// An attribute for specifying the type of an hkArray
struct hkArrayTypeAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkArrayTypeAttribute );

	enum ArrayType
	{
		NONE = 0,
		POINTSOUP,	///< This member represents an array of points in space.
		ENTITIES,	///< This member represents an array of hkpEntity objects.
	};

		/// Can be used to define the array type
	hkEnum<ArrayType,hkInt8> m_type;
};

extern const hkClass hkDataObjectTypeAttributeClass;

	/// An attribute overriding reflected member type for hkDataObject.
struct hkDataObjectTypeAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkDataObjectTypeAttribute );

		/// The type name string.
	const char* m_typeName;
};

extern const hkClass hkDocumentationAttributeClass;

/// An attribute linking a class to the corresponding section in the documentation.
struct hkDocumentationAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkDocumentationAttribute );

		/// The XML tag linking to the section (SECT1) that holds the documentation.
	const char* m_docsSectionTag;
};
extern const hkClass hkPostFinishAttributeClass;

/// An attribute specifying a post-finish step after loading from a tagfile. This should be a global
/// function or static member, it is called after all objects are loaded and finished, with a void*
/// argument pointing to the loaded object
struct hkPostFinishAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkDescriptionAttribute );

	void (HK_CALL *m_postFinishFunction)(void*); //+serialized(false)
};

extern const hkClass hkReflectedFileAttributeClass;

/// An attribute giving a string description to a class member.
struct hkReflectedFileAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkReflectedFileAttribute );

	const char *m_value;
};

struct hkMemoryTrackerAttribute
{
	//+defineattribute(true)
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_HKCLASS, hkMemoryTrackerAttribute );
};
extern const hkClass hkMemoryTrackerAttributeClass;


#endif // HK_ATTRIBUTE_CLASS_H

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
