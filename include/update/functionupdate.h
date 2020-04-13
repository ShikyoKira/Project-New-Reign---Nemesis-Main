#ifndef FUNCTIONUPDATE_H_
#define FUNCTIONUPDATE_H_

#include <fstream>
#include <iostream>
#include <unordered_map>

#include <boost/regex.hpp>
#include <boost/atomic.hpp>

#include "utilities/writetextfile.h"

#include "update/animdatacont.h"

#include "generate/animationsetdata.h"

class NodeU
{
#if MULTITHREADED_UPDATE
	boost::atomic_flag nodelock = BOOST_ATOMIC_FLAG_INIT;
#endif

	bool NodeUpdate(std::string modcode, std::string f2, std::string f3, std::unique_ptr<std::map<std::string, vecstr, alphanum_less>>& newFile,
		std::unique_ptr<SSMap>& stateID, std::unique_ptr<SSMap>& parent, std::unique_ptr<std::unordered_map<std::string, vecstr>>& statelist,
		std::unordered_map<std::string, std::string>& lastUpdate
#if MULTITHREADED_UPDATE
		, boost::atomic_flag& filelock, boost::atomic_flag& stateLock, boost::atomic_flag& parentLock
#endif
		);

public:
	bool FunctionUpdate(std::string modcode, std::string f2, std::string f3, std::unique_ptr<std::map<std::string, vecstr, alphanum_less>>& newFile,
		std::unique_ptr<SSMap>& stateID, std::unique_ptr<SSMap>& parent, std::unique_ptr<std::unordered_map<std::string, vecstr>>& statelist,
		std::unordered_map<std::string, std::string>& lastUpdate
#if MULTITHREADED_UPDATE
		, boost::atomic_flag& filelock, boost::atomic_flag& stateLock, boost::atomic_flag& parentLock
#endif
		);
};

bool AnimDataUpdate(std::string modcode, std::string animdatafile, std::string characterfile, std::string filepath, MasterAnimData& animData,
	bool isNewProject, std::unordered_map<std::string, std::string>& lastUpdate, bool& openAnim, bool& openInfo);

bool AnimSetDataUpdate(std::string modcode, std::string animsetdatafile, std::string projectsource, std::string projectfile, std::string filepath,
	MasterAnimSetData& animSetData, bool isNewProject, std::unordered_map<std::string, std::string>& lastUpdate);

#endif