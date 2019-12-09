#ifndef UPDATEPROCESS_H_
#define UPDATEPROCESS_H_

#include <QtCore\QObject>

#include "Global.h"
#include "progressup.h"
#include "behaviortype.h"

#include "add animation\import.h"
#include "add animation\animationdata.h"

#include "src\update\queuestack.h"

class NemesisMainGUI;
class arguPack;

typedef std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, std::set<std::string>>>> StateIDList;

class UpdateFilesStart : public QObject
{
	Q_OBJECT

	typedef std::unordered_map<std::string, std::unique_ptr<SSMap>> SSSMap;

	struct TargetQueue
	{
		std::string file, node;

		TargetQueue(std::string _file, std::string _node) : node(_node), file(_file) {}
	};

	struct RegisterQueue
	{
		boost::filesystem::path file;
		bool isFirstPerson;

		RegisterQueue(std::string _file, bool _first) : file(_file), isFirstPerson(_first) {}
		RegisterQueue(boost::filesystem::path _file, bool _first) : file(_file), isFirstPerson(_first) {}
	};

	struct GetPathLoopLink
	{
		std::string path;
		bool first;

		GetPathLoopLink(std::string _path, bool _first) : path(_path), first(_first) {}
	};

	struct controledRegisterFile
	{
		int cur = 0;
		std::mutex m_file;
		std::vector<std::shared_ptr<RegisterQueue>> files;

		std::shared_ptr<RegisterQueue> getNext()
		{
			std::lock_guard<std::mutex> lock(m_file);;

			if (cur >= files.size()) return nullptr;

			return files[cur++];
		}
	};

public:
	bool cmdline = false;

	UpdateFilesStart();
	virtual ~UpdateFilesStart();
	void milestoneStart(std::string directory);
	void message(std::string input);
	void GetFileLoop(std::string newPath);
	bool VanillaUpdate();
	void GetPathLoop(std::string path, bool isFirstPerson, bool newThread);
	void RegisterBehavior();
	bool VanillaDisassemble(std::string path, std::unique_ptr<std::map<std::string, vecstr, alphanum_less>>& newFile,
		std::unique_ptr<std::map<std::string, std::unordered_map<std::string, bool>>>& childrenState, std::unique_ptr<SSMap>& stateID, std::unique_ptr<SSMap>& n_parent);
	bool AnimDataDisassemble(std::string path, MasterAnimData& animData);
	bool AnimSetDataDisassemble(std::string path, MasterAnimSetData& animSetData);
	bool newAnimUpdate(std::string sourcefolder);
	void SeparateMod(std::string directory, std::unordered_map<std::string, std::shared_ptr<arguPack>>& pack);
	void ModThread(std::string directory, std::string node, std::string behavior, std::unordered_map<std::string, std::shared_ptr<arguPack>>& pack);
	void JoiningEdits(std::string directory);
	void CombiningFiles();
	void unregisterProcess();
	bool QueueThreadEnd();
	bool PathThreadCheck();

	public slots :
	void UpdateFiles();

signals:
	void progressMax(int);
	void progressUp();
	void end();
	void disable(bool);
	void disableLaunch(bool);
	void hide(bool);
	void incomingMessage(QString);

private:
	int filenum;
	ProgressUp behaviorProcess;
	std::string engineVersion;
	boost::atomic_flag stackLock = BOOST_ATOMIC_FLAG_INIT;
	boost::atomic_flag queueLock = BOOST_ATOMIC_FLAG_INIT;

	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> modQueue;	// behavior, node, list of mod
	std::vector<TargetQueue> processQueue;
	size_t queuing;
	std::atomic<size_t> waiting;

	std::atomic<size_t> ending;

	std::vector<std::unique_ptr<std::thread>> multiThreads;

	controledRegisterFile registeredFiles;
	std::vector<std::unique_ptr<GetPathLoopLink>> getPathLoopList;
	boost::atomic_flag pathLoopLock = BOOST_ATOMIC_FLAG_INIT;

	std::condition_variable cv1;
	std::condition_variable cv2;
	std::condition_variable cv3;
	std::mutex m_cv1;
	std::mutex m_cv2;
	std::mutex m_cv3;

	StateIDList modStateList;				// behavior file, SM ID, state ID, list of mods
	boost::atomic_flag stateListLock = BOOST_ATOMIC_FLAG_INIT;

	StateIDList duplicatedStateList;		// behavior file, SM ID, state ID, list of conflicting mods
	boost::atomic_flag duplicatedLock = BOOST_ATOMIC_FLAG_INIT;

	// update data container
	std::map<std::string, std::unique_ptr<std::map<std::string, vecstr, alphanum_less>>> newFile;
	// behavior file, node ID, node data lines; memory to access each node
	boost::atomic_flag newFileLock = BOOST_ATOMIC_FLAG_INIT;
	
	std::unordered_map<std::string, std::unique_ptr<std::map<std::string, std::unordered_map<std::string, bool>>>> childrenState;
	// behavior file, node ID, children of state machine; ensure no state conflict

	SSSMap stateID;		// behavior file, children state machine info, state ID
	SSSMap parent;		// behavior file, children state machine info, parent state machine

	MasterAnimData animData;
	boost::atomic_flag animDataLock = BOOST_ATOMIC_FLAG_INIT;

	MasterAnimSetData animSetData;
	boost::atomic_flag animSetDataLock = BOOST_ATOMIC_FLAG_INIT;

	std::map<std::string, vecstr> newAnimAddition;
	std::unordered_map<std::string, std::string> lastUpdate;
	std::unordered_map<std::string, std::unordered_map<std::string, size_t>> modFileCounter;			// modcode, behavior, number of files
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> modFileList;				// modcode, behavior, node files
	boost::atomic_flag fileCountLock = BOOST_ATOMIC_FLAG_INIT;

	// global container locker
	boost::atomic_flag behaviorPathLock = BOOST_ATOMIC_FLAG_INIT;
	boost::atomic_flag behaviorProjectLock = BOOST_ATOMIC_FLAG_INIT;
	boost::atomic_flag behaviorProjectPathLock = BOOST_ATOMIC_FLAG_INIT;

	// timer
	boost::posix_time::ptime start_time;
};



#endif
