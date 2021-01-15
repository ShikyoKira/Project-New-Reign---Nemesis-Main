#ifndef UPDATEPROCESS_H_
#define UPDATEPROCESS_H_

#include <QtCore/QObject>

#include "ui/ProgressUp.h"

#include "update/animdata/animdatacond.h"

#include "update/animsetdata/masteranimsetdata.h"

#include "generate/animationdata.h"

#include "generate/animation/import.h"

class NemesisEngine;
class arguPack;

typedef UMap<std::string, Map<std::string, UMap<std::string, SetStr>>> StateIDList;

class UpdateFilesStart : public QObject
{
	Q_OBJECT

	typedef UMap<std::string, UPtr<UMapStr2>> SSSMap;

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
    void message(std::wstring input);
    void GetFileLoop(std::string newPath);
    bool VanillaUpdate();
    void GetPathLoop(const std::filesystem::path& path, bool isFirstPerson);
    void RegisterBehavior(SPtr<RegisterQueue> curBehavior);
    bool VanillaDisassemble(const std::wstring& path,
                            UPtr<std::map<std::string, VecStr, alphanum_less>>& newFile,
                            UPtr<std::map<std::string, UMap<std::string, bool>>>& childrenState,
                            UPtr<UMapStr2>& stateID,
                            UPtr<UMapStr2>& n_parent);
    bool AnimDataDisassemble(const std::wstring& path, MasterAnimData& animData);
    bool AnimSetDataDisassemble(const std::wstring& path, MasterAnimSetData& animSetData);
    void newAnimUpdate(std::string sourcefolder, std::string curCode);
    void newAnimProcess(std::string sourcefolder);
    void SeparateMod(const std::string& directory,
                     TargetQueue target,
                     UMap<std::string, SPtr<arguPack>>& pack);
    void ModThread(const std::string& directory,
                   const std::string& node,
                   const std::string& behavior,
                   UMap<std::string, SPtr<arguPack>>& pack);
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
    void criticalError(QString, QString);

private:
    bool running      = true;
    int timeout_timer = 0;
    int filenum;
    bool newAnimFunction = true;
    ProgressUp behaviorProcess;
    std::string engineVersion;
#if MULTITHREADED_UPDATE
    std::atomic_flag stackLock{};
    std::atomic_flag queueLock{};
#endif

	UMap<std::string, UMap<std::string, VecStr>> modQueue; // behavior, node, list of mod
	Vec<TargetQueue> processQueue;
	size_t queuing;

	Vec<SPtr<RegisterQueue>> registeredFiles;

	StateIDList modStateList;				// behavior file, SM ID, state ID, list of mods
#if MULTITHREADED_UPDATE
    std::atomic_flag stateListLock{};
#endif

	StateIDList duplicatedStateList;		// behavior file, SM ID, state ID, list of conflicting mods
#if MULTITHREADED_UPDATE
    std::atomic_flag duplicatedLock{};
#endif

	// update data container
    Map<std::string, UPtr<Map<std::string, VecStr, alphanum_less>>> newFile;
	// behavior file, node ID, node data lines; memory to access each node
#if MULTITHREADED_UPDATE
    std::atomic_flag newFileLock{};
#endif

    UMap<std::string, UPtr<Map<std::string, UMap<std::string, bool>>>>
        childrenState;
    // behavior file, node ID, children of state machine; ensure no state conflict

	SSSMap stateID;		// behavior file, children state machine info, state ID
	SSSMap parent;		// behavior file, children state machine info, parent state machine

	USet<std::string> newProjectList;
	MasterAnimData animData;
	MasterAnimSetData animSetData;

    Map<std::string, VecStr> newAnimAddition;
    UMap<std::wstring, std::wstring> lastUpdate;
    UMap<std::string, UMap<std::string, size_t>> modFileCounter; // modcode, behavior, number of files
    UMap<std::string, UMap<std::string, VecStr>> modFileList;    // modcode, behavior, node files
    std::atomic_flag fileCountLock{};

    // global container locker
#if MULTITHREADED_UPDATE
    std::atomic_flag behaviorPathLock       {};
    std::atomic_flag behaviorProjectLock    {};
    std::atomic_flag behaviorProjectPathLock{};
#endif

    // nemesis ini
    const NemesisInfo* nemesisInfo;

    // timer
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
};

#endif
