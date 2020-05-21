/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DEFAULTFILESYSTEM
#define HK_DEFAULTFILESYSTEM

#include <Common/Base/hkBase.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>
#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>
#include <Common/Base/System/Io/Reader/Buffered/hkBufferedStreamReader.h>
#include <Common/Base/System/Io/Writer/Buffered/hkBufferedStreamWriter.h>

#if defined(HK_PLATFORM_PSP) || defined(HK_PLATFORM_UNIX) || defined(HK_PLATFORM_MAC386) || defined(HK_PLATFORM_MACARM) || defined(HK_PLATFORM_MACPPC) || defined(HK_PLATFORM_LRB)
#	include <Common/Base/System/Io/Reader/Posix/hkPosixStreamReader.h>
#	include <Common/Base/System/Io/Writer/Posix/hkPosixStreamWriter.h>

#	if defined (HK_PLATFORM_UNIX) ||  defined(HK_PLATFORM_MAC386) || defined(HK_PLATFORM_MACPPC) || defined(HK_PLATFORM_MACARM) || defined(HK_PLATFORM_LRB)
#		include <Common/Base/System/Io/Directory/Posix/hkPosixListDirectory.h>
#	else
#		include <Common/Base/System/Io/Directory/EmptyImpl/hkListDirectoryEmptyImpl.h>
#	endif

#elif defined(HK_PLATFORM_WIN32) || defined(HK_PLATFORM_XBOX) || defined(HK_PLATFORM_XBOX360)
#	include <Common/Base/System/Io/Reader/Stdio/hkStdioStreamReader.h>
#	include <Common/Base/System/Io/Writer/Stdio/hkStdioStreamWriter.h>
#	include <Common/Base/System/Io/Directory/Win32/hkWin32ListDirectory.h>

#elif defined(HK_PLATFORM_PS3_PPU)
#	include <Common/Base/System/Io/Reader/Stdio/hkStdioStreamReader.h>
#	include <Common/Base/System/Io/Writer/Stdio/hkStdioStreamWriter.h>
#	include <Common/Base/System/Io/Directory/Ps3/hkPs3ListDirectory.h> 
#	include <sys/paths.h>

#else
#	include <Common/Base/System/Io/Reader/Stdio/hkStdioStreamReader.h>
#	include <Common/Base/System/Io/Writer/Stdio/hkStdioStreamWriter.h>
#endif

// FileSystem class implementation for native file systems.
// Uses default reader, writer and file system browsers for each platform
// All the paths and file names must be in Havok standard format i.e. the
// only character accepted as a separator is '/'.
// e.g. A valid path is Dir/SubDir/SubSubDir/Filename
class hkNativeFileSystem : public hkFileSystem
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		// Function pointer type for directory listing.
	typedef hkResult (HK_CALL *ListDirectoryFunType)( const char* pathIn, hkFileSystem::DirectoryListing& directoryListingOut );
		// Function pointer type for converting Havok paths to platform paths.
	typedef const char* (HK_CALL *HavokToPlatformConvertPathFunType)( const char* pathIn, hkStringBuf& buffer );
		// Function pointer type for converting platform paths to Havok paths.
	typedef const char* (HK_CALL *PlatformToHavokConvertPathFunType)( const char* pathIn, hkStringBuf& buffer );
		// Function pointer type for finding the absolute filename from a relative one
	typedef const char* (HK_CALL *GetFullPathNameFunType)(const char* relativePath, hkStringBuf& buffer );

		// Function pointer for directory listing.
		// Replace the function to modify listDirectory behavior.
	static ListDirectoryFunType s_listDirectory;
		// Pointer to function for converting Havok paths to platform paths.
		// Replace the function to modify havokToPlatformConvertPath behavior.
	static HavokToPlatformConvertPathFunType s_havokToPlatformConvert;
		// Pointer to function for converting platform paths to Havok paths.
		// Replace the function to modify platformToHavokConvertPath behavior.
	static PlatformToHavokConvertPathFunType s_platformToHavokConvert;
		// Pointer to function for finding the absolute path of a file from a relative one.
		// Replace the function to modify platformToHavokConvertPath behavior.
	static GetFullPathNameFunType s_getFullPathName;

		// Default conversions functions
	static const char* HK_CALL nativeHavokToPlatformConvertPath( const char* pathIn, hkStringBuf& buffer );
	static const char* HK_CALL nativePlatformToHavokConvertPath( const char* pathIn, hkStringBuf& buffer );
	static const char* HK_CALL nativeGetFullPathName( const char* pathIn, hkStringBuf& buffer );

	virtual hkStreamReader* openReader( const char* name );

	virtual hkStreamWriter* openWriter( const char* name );

	static hkReferencedObject* create()
	{
		return new hkNativeFileSystem();
	}


		// Converts a Havok path to platform path. Set s_havokToPlatformConvert
	HK_FORCE_INLINE static const char* HK_CALL havokToPlatformConvertPath( const char* pathIn, hkStringBuf& buffer )
	{
		return s_havokToPlatformConvert( pathIn, buffer );
	}
		// Converts a platform path to Havok path
	HK_FORCE_INLINE static const char* HK_CALL platformToHavokConvertPath( const char* pathIn, hkStringBuf& buffer )
	{
		return s_platformToHavokConvert( pathIn, buffer );
	}

		// Gets the full path from a relative path.
	HK_FORCE_INLINE static const char* HK_CALL getFullPathName( const char* relativePathIn, hkStringBuf& buffer )
	{
		return s_getFullPathName( relativePathIn, buffer );
	}

		/// list all the directories and files in the "basePath" directory,
		/// returns HK_FAILURE if the path is not valid
		/// basePath must be in Havok format, all the generated paths and file names
		/// in listingOut will be in Havok format too.
	virtual hkResult listDirectory(const char* basePath, DirectoryListing& listingOut)
	{
		return s_listDirectory( basePath, listingOut );
	}
};

#endif //HK_DEFAULTFILESYSTEM

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
