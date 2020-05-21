/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SERIALIZE_UTIL_H
#define HK_SERIALIZE_UTIL_H

#include <Common/Serialize/Util/hkStructureLayout.h>
#include <Common/Serialize/Tagfile/hkTagfileWriter.h>
#include <Common/Serialize/Packfile/hkPackfileWriter.h>
#include <Common/Serialize/Resource/hkObjectResource.h>

class hkClass;
class hkDataObject;
class hkStreamReader;
class hkStreamWriter;
class hkClassNameRegistry;
class hkTypeInfoRegistry;

	/// Utility functions to load and save packfiles and tagfiles.
namespace hkSerializeUtil
{
		/// Error details class for loading packfile/tagfile.
	struct ErrorDetails
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, ErrorDetails );

			/// ErrorDetails constructor.
		ErrorDetails() : id(ERRORID_NONE) {}

			/// Error ids for loading packfile/tagfile.
		enum ErrorID
		{
				/// No errors reading from stream.
			ERRORID_NONE,
				/// Failed to read from stream.
			ERRORID_READ_FAILED,
				/// Unsupported file format.
			ERRORID_UNSUPPORTED_FORMAT,
				/// Packfile cannot be read on this platform.
			ERRORID_PACKFILE_PLATFORM,
				/// Versioning cannot be applied to loaded objects.
			ERRORID_VERSIONING_FAILED,
				/// Found object with members that cannot clean up memory when destroyed.
			ERRORID_NONHEAP_OBJECT,
				/// Stream was ok but some error in the data.
			ERRORID_LOAD_FAILED,
				/// The file contains deprecated format but hkFeature_serializeDeprecatedPre700() was not called
			ERRORID_DEPRECATED_NOT_INITIALIZED,
				/// Max error ID
			ERRORID_MAX_ID,

		};

		void set( ErrorID i, const char* msg ) { id = i; defaultMessage = msg; }
			/// Error id. Default is ErrorDetails::ERROR_NOT_FOUND.
		hkEnum<ErrorID, hkInt32> id;
			/// Detailed error message, if error id is not ErrorDetails::ERROR_NOT_FOUND.
		hkStringPtr defaultMessage;
	};

		/// Option bits for saving.
	enum SaveOptionBits
	{
		SAVE_DEFAULT					= 0x00,		///< All flags default to OFF, enable whichever are needed
		SAVE_TEXT_FORMAT				= 0x01,		///< Use text (usually XML) format, default is binary format if available.
		SAVE_SERIALIZE_IGNORED_MEMBERS	= 0x02,		///< Write members which are usually ignored, default is to skip SERIALIZE_IGNORED members.
		SAVE_WRITE_ATTRIBUTES			= 0x04,		///< Include extended attributes in metadata, default is to write minimum metadata.
		SAVE_CONCISE					= 0x08,		///< Doesn't provide any extra information which would make the file easier to interpret. E.g. additionally write hex floats as text comments.
		SAVE_TEXT_NUMBERS				= 0x10,		///< Floating point numbers output as text, not as binary. Makes them easily readable/editable, but values may not be exact.
	};
		/// Options for saving.
	struct SaveOptions : public hkFlags<SaveOptionBits, int>
	{
		SaveOptions(SaveOptionBits b=SAVE_DEFAULT) : hkFlags<SaveOptionBits,int>(b) {}
		SaveOptions& useText(hkBool32 b) { useBinary(!b); return *this; }
		inline SaveOptions& useBinary(hkBool32 b);
		inline SaveOptions& useConcise(hkBool32 b);
		inline SaveOptions& useTextNumbers(hkBool32 b);
	};

		/// Option bits for loading.
	enum LoadOptionBits
	{
		LOAD_DEFAULT			= 0x00,	///< All flags default to OFF, enable whichever are needed
		LOAD_FAIL_IF_VERSIONING	= 0x01,	///< Allows a way to skip versioning paths (if current version needs to be enforced)
	};
		/// Options for loading.
	struct LoadOptions : public hkFlags<LoadOptionBits, int>
	{
		LoadOptions(LoadOptionBits b=LOAD_DEFAULT) : hkFlags<LoadOptionBits,int>(b) {}
	};


		/// Load serialized objects from stream and return pointer
		/// to hkResource object.
		/// To access hkResource objects you should call hkResource::getContents()
		/// or hkResource::getContentsWithRegistry() specifying type of top level
		/// object pointer you expect.
		/// hkResource is reference counted and users are responsible to
		/// remove reference when they do not need the hkResource content.
		/// The objects owned by hkResource are not reference counted.
		///
		/// Load serialized objects from stream and return pointer
		/// to hkResource object. If 'classReg' is HK_NULL the
		/// class name registry from hkBuiltinTypeRegistry is used instead.
		/// The function does additional check for the stream format, versioning and finishing steps.
		/// It will return HK_NULL if the checks failed. The error details are
		/// returned in 'detailsOut'.
		/// Usually you should use hkSerializeUtil::load().
	hkResource* HK_CALL load( hkStreamReader* stream, ErrorDetails* detailsOut=HK_NULL, const hkClassNameRegistry* classReg=HK_NULL, LoadOptions options=LOAD_DEFAULT );

		/// Load serialized objects from a file using name and return pointer
		/// to hkResource object.
		/// See hkSerializeUtil::load( hkStreamReader* sr ) for details.
	hkResource* HK_CALL load( const char* filename, ErrorDetails* detailsOut=HK_NULL, const hkClassNameRegistry* classReg=HK_NULL, LoadOptions options=LOAD_DEFAULT );

		/// Load serialized objects from buffer and return pointer
		/// to hkResource object. The buffer may be freed after loading.
		/// See hkSerializeUtil::load( hkStreamReader* sr ) for details.
	hkResource* HK_CALL load( const void* buf, int buflen, ErrorDetails* detailsOut=HK_NULL, const hkClassNameRegistry* classReg=HK_NULL, LoadOptions options=LOAD_DEFAULT );


		/// Load serialized objects from stream and return pointer
		/// to hkObjectResource object.
		/// To access hkObjectResource objects you should call hkObjectResource::getContents()
		/// or hkObjectResource::getContentsWithRegistry() specifying type of top level
		/// object pointer you expect.
		/// hkObjectResource is reference counted and users are responsible to
		/// remove reference when they do not need its content.
		/// The objects owned by hkObjectResource are allocated on heap and reference counted.
		///
		/// Load serialized objects from stream and return pointer to hkObjectResource object.
		/// If 'classReg' or 'typeReg' are null the ones from hkBuiltinTypeRegistry are used.
		/// The function does additional check for the stream format, versioning and finishing steps.
		/// It will return HK_NULL if the checks failed. The error details are
		/// returned in 'detailsOut'.
	hkObjectResource* HK_CALL loadOnHeap( hkStreamReader* stream, ErrorDetails* errorOut=HK_NULL, const hkClassNameRegistry* classReg=HK_NULL, const hkTypeInfoRegistry* typeReg=HK_NULL, LoadOptions options=LOAD_DEFAULT );

		/// Load serialized objects from a file using name and return pointer
		/// to hkObjectResource object.
		/// See hkSerializeUtil::load( hkStreamReader* sr ) for details.
	hkObjectResource* HK_CALL loadOnHeap( const char* filename, ErrorDetails* errorOut=HK_NULL, const hkClassNameRegistry* classReg=HK_NULL, const hkTypeInfoRegistry* typeReg=HK_NULL, LoadOptions options=LOAD_DEFAULT );

		/// Load serialized objects from a file using name and return pointer
		/// to hkObjectResource object.
		/// See hkSerializeUtil::load( hkStreamReader* sr ) for details.
	hkObjectResource* HK_CALL loadOnHeap( const void* buf, int buflen, ErrorDetails* errorOut=HK_NULL, const hkClassNameRegistry* classReg=HK_NULL, const hkTypeInfoRegistry* typeReg=HK_NULL, LoadOptions options=LOAD_DEFAULT );


	//
	// Shortcut loads
	//

		/// Shortcut to combine loadOnHeap and hkObjectResource::stealContents.
	template<typename T> inline T* loadObject(hkStreamReader* reader, ErrorDetails* errorOut=HK_NULL);

		/// Shortcut to combine loadOnHeap and hkObjectResource::stealContents.
	template<typename T> inline T* loadObject(const char* filename, ErrorDetails* errorOut=HK_NULL);

		/// Shortcut to combine loadOnHeap and hkObjectResource::stealContents.
	template<typename T> inline T* loadObject(const void* buf, int buflen, ErrorDetails* errorOut=HK_NULL);

		

		/// Save a snapshot of a given object in packfile form using provided writer.
		/// Returns HK_SUCCESS if successful.
		/// If you don't provide a target layout then the current host layout is assumed.
		/// NOTE: Some objects are inherently not serializable because they point to
		/// external memory, i.e., the vertex and index arrays of an hkpMeshShape.
		/// These objects should be converted before saving, i.e hkpMeshShape -> hkpStorageMeshShape
		/// (see hkpHavokSnapshot::ConvertListener class for details).
		/// If you want these conversions applied you should provide a hkpHavokSnapshot::ConvertListener as
		/// a value for userListener.
	hkResult HK_CALL savePackfile( const void* object, const hkClass& klass, hkStreamWriter* writer, const hkPackfileWriter::Options& packFileOptions, hkPackfileWriter::AddObjectListener* userListener=HK_NULL, SaveOptions options=SAVE_DEFAULT );

		/// Save a snapshot of a given object in tagfile form using provided writer.
		/// Returns HK_SUCCESS if successful.
		/// NOTE: Some objects are inherently not serializable because they point to
		/// external memory, i.e., the vertex and index arrays of an hkpMeshShape.
		/// These objects should be converted before saving, i.e hkpMeshShape -> hkpStorageMeshShape
		/// (see hkpHavokSnapshot::ConvertListener class for details).
		/// If you want these conversions applied you should provide a hkpHavokSnapshot::ConvertListener as
		/// a value for userListener.
	hkResult HK_CALL saveTagfile( const void* object, const hkClass& klass, hkStreamWriter* writer, hkPackfileWriter::AddObjectListener* userListener=HK_NULL, SaveOptions options=SAVE_DEFAULT );

		/// Save a given data object in tagfile form using provided writer.
		/// Returns HK_SUCCESS if successful.
		/// Note that the text format (binary==false) is only for debugging and cannot be loaded.
	hkResult HK_CALL saveTagfile( const hkDataObject& object, hkStreamWriter* writer, SaveOptions options=SAVE_DEFAULT );

		/// Save with the default options in the default file format (currently tagfile)
	hkResult HK_CALL save( const hkDataObject& object, hkStreamWriter* writer, SaveOptions options=SAVE_DEFAULT );

		/// Save with the default options in the default file format (currently tagfile)
	hkResult HK_CALL save( const void* object, const hkClass& klass, hkStreamWriter* writer, SaveOptions options=SAVE_DEFAULT );

		/// The format of an asset file.
	enum FormatType
	{
			/// read or open error.
		FORMAT_ERROR,
			/// readable but not recognised.
		FORMAT_UNKNOWN,
			/// binary packfile
		FORMAT_PACKFILE_BINARY,
			/// XML packfile
		FORMAT_PACKFILE_XML,
			/// Binary tagfile
		FORMAT_TAGFILE_BINARY,
			/// XML tagfile
		FORMAT_TAGFILE_XML,
	};

		/// Information returned by detectFormat.
	struct FormatDetails
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(0, FormatDetails);
			/// The type of the file
		hkEnum<FormatType,hkInt32> m_formatType;
			/// The SDK version which created this file, if known.
		hkStringPtr m_version;
			/// For binary packfiles, the binary format.
		hkStructureLayout::LayoutRules m_layoutRules;
	};

		/// Detect the type of a packfile stream.
	hkEnum<FormatType,hkInt32> HK_CALL detectFormat( hkStreamReader* reader );

		/// Extract some information from the file header.
		/// See FormatDetails for the available information.
	void HK_CALL detectFormat( hkStreamReader* reader, FormatDetails& detailsOut );

		/// Is the stream a tagfile, XML packfile or binary packfile for the current platform?
	hkBool32 HK_CALL isLoadable(hkStreamReader* sr);
}

#include <Common/Serialize/Util/hkSerializeUtil.inl>

#endif // HK_SERIALIZE_UTIL_H

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
