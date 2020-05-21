/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULT_ERROR
#define HK_DEFAULT_ERROR

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

/// This is the default implementation of our error handling interface.
/// It supports error and assert enabling and disabling and where possible
/// it walks and reports the full stack trace leading up to the error.
/// Usually you will derive from this class and override showMessage when
/// integrating this class in you game.
class hkDefaultError : public hkError
{
  public:
  	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
	hkDefaultError( hkErrorReportFunction errorReportFunction, void* errorReportObject = HK_NULL );

	virtual void setEnabled( int id, hkBool enabled );

	virtual hkBool isEnabled( int id );

	virtual void enableAll();

	virtual int message(hkError::Message msg, int id, const char* description, const char* file, int line);

	virtual void sectionBegin(int id, const char* sectionName);

	virtual void sectionEnd();

  protected:

	virtual void showMessage(const char* what, int id, const char* desc, const char* file, int line, hkBool stackTrace=true);

  public:

	/// A map of the disabled assert IDs.
	hkPointerMap<int, int> m_disabledAssertIds;

	hkArray<int> m_sectionIds;

	/// This function will be used to report errors.
	hkErrorReportFunction m_errorFunction;

	void* m_errorObject;
};

#endif // HK_DEFAULT_ERROR

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
