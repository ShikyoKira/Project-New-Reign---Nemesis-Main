/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_NATIVE_PACKFILE_UTILS_H
#define HK_NATIVE_PACKFILE_UTILS_H

#include <Common/Serialize/hkSerialize.h>

#include <Common/Serialize/Packfile/Binary/hkPackfileHeader.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Serialize/Resource/hkResource.h>

	/// The hkNativePackfileUtils functions are designed to work with native
	/// packfile and may be used only to load files in one step that do not require
	/// any versioning steps.
namespace hkNativePackfileUtils
{
		/// Simple interface to load from an in memory buffer.
		/// The returned hkResource contains a copy of the persistent parts of the buffer, thus the
		/// buffer may be freed after loading.
	hkResource* HK_CALL load(const void* packfileData, int dataSize, const hkTypeInfoRegistry* userRegistry = HK_NULL);

		/// Calculates size for memory block in bytes required to store only objects
		/// from native packfile. Returns -1 if the packfileData header is invalid.
		/// Use validatePackfileHeader to find the exact error.
	int HK_CALL getRequiredBufferSize(const void* packfileData, int dataSize);

		/// Load buffer from a preallocated memory chunk.
		/// Given a native binary packfile that is already in
		/// memory, fill in the buffer with objects only, fixup all the pointers,
		/// process some objects that require a finishing step (i.e., to initialize
		/// members which have not been serialized or to initialize vtables) and
		/// return pointer to top level object in the file.
		/// If "userRegistry" is not null, apply these finishing steps, otherwise
		/// use finish registry from hkBuiltinTypeRegistry.
		///
		/// This function does not do any memory allocation and the
		/// user must ensure that "outBuffer" remains valid for the lifetime
		/// of the packfile.
		/// The "packfileData" content is not modified and is not in use when the
		/// hkNativePackfileUtils::load is complete.
	void* HK_CALL load(const void* packfileData, int dataSize, void* outBuffer, int outBufferSize, const hkTypeInfoRegistry* userRegistry = HK_NULL);

		/// Load a native packfile in place. The objects are not copied, all pointer fixups are
		/// carried out and objects are finished, using userRegistry if supplied, hkBuiltinTypeRegistry
		/// if not.
		///
		/// The packfile is loaded into the same buffer, and does not take ownership of the contained objects
		/// The buffer must remain allocated for the lifetime of the objects, and can not be loaded more than once
	void* HK_CALL loadInPlace(void* packfileData, int dataSize, const hkTypeInfoRegistry* userRegistry = HK_NULL);

		/// Get the name of the toplevel object in packfileData.
		/// Call this method on the raw packfileData, not the outputBuffer of load() for instance.
		/// Returns null on error (for instance if the magic number doesn't seem to be a packfile or dataSize too small because of a short read).
	const char* getContentsClassName(const void* packfileData, int dataSize);

		/// Get the imports/exports a loaded packfile.
	void HK_CALL getImportsExports(const void* loadedBuffer, hkArray<hkResource::Export>& expOut, hkArray<hkResource::Import>& impOut );
		/// Get the imports/exports a loaded packfile.
	void HK_CALL getImportsExportsInPlace(const void* loadedBuffer, hkArray<hkResource::Export>& expOut, hkArray<hkResource::Import>& impOut );

		/// Unload (destruct) objects in the buffer previously loaded with the hkNativePackfileUtils::load.
		/// This function must be called when the packfile lifetime is finished.
		/// The "buffer" content is not in use when the hkNativePackfileUtils::unload is
		/// complete, and it can be released. loadedBuffer points to the buffer, not the top-level
		/// object
	void HK_CALL unload(void* loadedBuffer, int bufferSize);
		/// Unload (destruct) objects in the buffer previously loaded with the hkNativePackfileUtils::loadInPlace.
		/// This function must be called when the packfile lifetime is finished.
		/// The "buffer" content is not in use when the hkNativePackfileUtils::unloadInPlace is
		/// complete, and it can be released. loadedBuffer points to the buffer, not the top-level
		/// object
	void HK_CALL unloadInPlace(void* loadedBuffer, int bufferSize);

		/// Check that the header is valid. If errOut is not null, a more detailed
		/// description is written on failure.
	hkResult validatePackfileHeader(const void* packfileData, const char**errOut );
}

#endif // HK_NATIVE_PACKFILE_UTILS_H

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
