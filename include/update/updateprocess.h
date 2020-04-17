#ifndef UPDATEPROCESS_H_
#define UPDATEPROCESS_H_

#include <QtCore/QObject>

#include <boost/asio/thread_pool.hpp>

#include "ui/ProgressUp.h"

#include "update/animdata/animdatacond.h"

#include "generate/behaviortype.h"
#include "generate/animationdata.h"
#include "generate/behaviortype.h"

#include "generate/animation/import.h"

class NemesisEngine;
class arguPack;

typedef std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, SetStr>>> StateIDList;

class UpdateFilesStart : public QObject
{
	Q_OBJECT

	typedef std::unordered_map<std::string, std::unique_ptr<SSMap>> SSSMap;

	struct TargetQueue
	{
		std::string file, node;

        TargetQueue(std::string _file, std::string _node)
            : node(_node)
            , file(_file)
        {}
    };

	struct RegisterQueue
	{
		std::filesystem::path file;
		bool isFirstPerson;

        RegisterQueue(std::string _file, bool _first)
            : file(_file)
            , isFirstPerson(_first)
        {}
        RegisterQueue(std::filesystem::path _file, bool _first)
            : file(_file)
            , isFirstPerson(_first)
        {}
    };

public:
	bool cmdline = false;

    UpdateFilesStart(const NemesisInfo* _ini);
    virtual ~UpdateFilesStart();
    void startUpdatingFile();
    void milestoneStart(std::string directory);
    void message(std::string input);
    void GetFileLoop(std::string newPath);
    bool VanillaUpdate();
    void GetPathLoop(std::string path, bool isFirstPerson);
    void RegisterBehavior(std::shared_ptr<RegisterQueue> curBehavior);
    bool VanillaDisassemble(
        std::string path,
        std::unique_ptr<std::map<std::string, VecStr, alphanum_less>>& newFile,
        std::unique_ptr<std::map<std::string, std::unordered_map<std::string, bool>>>& childrenState,
        std::unique_ptr<SSMap>& stateID,
        std::unique_ptr<SSMap>& n_parent);
    bool AnimDataDisassemble(std::string path, MasterAnimData& animData);
    bool AnimSetDataDisassemble(std::string path, MasterAnimSetData& animSetData);
    void newAnimUpdate(std::string sourcefolder, std::string curCode);
    void newAnimProcess(std::string sourcefolder);
    void SeparateMod(std::string directory,
                     TargetQueue target,
                     std::unordered_map<std::string, std::shared_ptr<arguPack>>& pack);
    void ModThread(const std::string& directory,
                   const std::string& node,
                   const std::string& behavior,
                   std::unordered_map<std::string, std::shared_ptr<arguPack>>& pack);
    void JoiningEdits(std::string directory);
    void CombiningFiles();
    void unregisterProcess();

public slots:
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
    bool newAnimFunction = true;
    ProgressUp behaviorProcess;
    std::string engineVersion;
    std::atomic_flag stackLock{};
    std::atomic_flag queueLock{};

	std::unordered_map<std::string, std::unordered_map<std::string, VecStr>> modQueue;	// behavior, node, list of mod
	std::vector<TargetQueue> processQueue;
	size_t queuing;

	std::vector<std::shared_ptr<RegisterQueue>> registeredFiles;
	
	StateIDList modStateList;				// behavior file, SM ID, state ID, list of mods
#if MULTITHREADED_UPDATE
    std::atomic_flag stateListLock{};
#endif

	StateIDList duplicatedStateList;		// behavior file, SM ID, state ID, list of conflicting mods
#if MULTITHREADED_UPDATE
    std::atomic_flag duplicatedLock{};
#endif

	// update data container
	std::map<std::string, std::unique_ptr<std::map<std::string, VecStr, alphanum_less>>> newFile;
	// behavior file, node ID, node data lines; memory to access each node
#if MULTITHREADED_UPDATE
    std::atomic_flag newFileLock{};
#endif

    std::unordered_map<std::string,
                       std::unique_ptr<std::map<std::string, std::unordered_map<std::string, bool>>>>
        childrenState;
    // behavior file, node ID, children of state machine; ensure no state conflict

	SSSMap stateID;		// behavior file, children state machine info, state ID
	SSSMap parent;		// behavior file, children state machine info, parent state machine

	std::unordered_set<std::string> newProjectList;
	MasterAnimData animData;
	MasterAnimSetData animSetData;

    std::map<std::string, VecStr> newAnimAddition;
    std::unordered_map<std::string, std::string> lastUpdate;
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>>
        modFileCounter; // modcode, behavior, number of files
    std::unordered_map<std::string, std::unordered_map<std::string, VecStr>>
        modFileList; // modcode, behavior, node files
    std::atomic_flag fileCountLock{};

    // global container locker
    std::atomic_flag behaviorPathLock       {};
    std::atomic_flag behaviorProjectLock    {};
    std::atomic_flag behaviorProjectPathLock{};

    // nemesis ini
    const NemesisInfo* nemesisInfo;

    // timer
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
};

#endif
