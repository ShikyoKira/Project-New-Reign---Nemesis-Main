#ifndef UPDATEPROCESS_H_
#define UPDATEPROCESS_H_

#include <QtCore/QObject>

#include "ui/ProgressUp.h"

#include "update/codedtemplates.h"

#include "update/animdata/animdatacond.h"

#include "update/animsetdata/masteranimsetdata.h"

#include "generate/animationdata.h"

#include "generate/animation/import.h"

class NemesisEngine;

struct arguPack;

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
        {
        }
    };

    struct RegisterQueue
    {
        std::filesystem::path file;
        bool isFirstPerson;

        RegisterQueue(std::string _file, bool _first)
            : file(_file)
            , isFirstPerson(_first)
        {
        }
        RegisterQueue(std::filesystem::path _file, bool _first)
            : file(_file)
            , isFirstPerson(_first)
        {
        }
    };

    struct GlobalCache
    {
    public:
        GlobalCache();
        ~GlobalCache();
    };

private:
    bool FileValidation();
    bool TryClearEngineData();
    bool TryReadBaseFiles();
    bool TryJoinModPatches();
    void CompileEngineData();

public:
    bool cmdline = false;

    UpdateFilesStart();
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
                            UPtr<std::map<std::string, VecNstr, alphanum_less>>& newFile,
                            UPtr<std::map<std::string, UMap<std::string, bool>>>& childrenState,
                            UPtr<UMapStr2>& stateID,
                            UPtr<UMapStr2>& n_parent);
    bool AnimDataDisassemble(const std::wstring& path, MasterAnimData& animData);
    bool AnimSetDataDisassemble(const std::wstring& path, MasterAnimSetData& animSetData);
    void newAnimUpdate(const std::string& sourcefolder, const std::string& curCode);
    void newAnimProcess(const std::string& sourcefolder);
    void
    SeparateMod(const std::string& directory, TargetQueue target, UMap<std::string, SPtr<arguPack>>& pack);
    void ModThread(const std::string& directory,
                   const std::string& node,
                   const std::string& behavior,
                   UMap<std::string, SPtr<arguPack>>& pack);
    void AddModPatches();
    void AddCodedModPatch(const std::string& code, const std::filesystem::path& path);
    void AddAnimDataModPatch(const std::string& code, const std::filesystem::path& path);
    void AddAnimDataHeaderModPatch(const std::string& code, const std::filesystem::path& path);
    void AddAnimDataProjModPatch(const std::string& code,
                                 const std::string& project,
                                 const std::filesystem::path& path);
    void AddAnimDataNewCodeModPatch(const std::string& code,
                                    const std::string& project,
                                    const std::filesystem::path& path);
    void AddAnimDataExistingCodeModPatch(const std::string& code,
                                         const std::string& project,
                                         const std::filesystem::path& path);
    void AddAnimSetDataModPatch(const MasterAnimSetData::ModPatch& patch);
    //void AddAnimSetDataModPatch(const std::string& code, const std::filesystem::path& path);
    void LoopModPatchFolders(const std::string& code,
                             const std::filesystem::path& bhvpath,
                             const std::filesystem::path& path);

    void AddTemplates();
    void AddCodedTemplates(const nemesis::CodedTemplates& codedtemplates);
    void AddCodedTemplates(const std::string& code, const std::filesystem::path& path);
    void AddAnimDataTemplate(const std::string& code, const std::filesystem::path& path);
    void AddCharcAnimDataTemplate(const std::string& code,
                                  const std::string& character,
                                  const std::filesystem::path& path);
    void ValidateAnimDataTemplateFile(const std::filesystem::path& path);
    void ValidateAnimDataTemplateCharc(const std::string& character, const std::filesystem::path& path);
    void AddAnimSetDataTemplate(const std::string& code, const std::filesystem::path& path);
    void AddAnimSetDataProjectTemplate(const std::string& code,
                                       const std::string& project,
                                       const std::filesystem::path& path);
    void LoopTemplateFolders(const std::string& code,
                             const std::filesystem::path& bhvpath,
                             const std::filesystem::path& path);

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
    const std::string newAnimDirectory = "behavior templates\\";
    const std::string mod_directory    = "mod\\";
    bool running                       = true;
    int timeout_timer                  = 0;
    bool newAnimFunction               = true;

    int filenum;
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

    StateIDList modStateList; // behavior file, SM ID, state ID, list of mods
#if MULTITHREADED_UPDATE
    std::atomic_flag stateListLock{};
#endif

    StateIDList duplicatedStateList; // behavior file, SM ID, state ID, list of conflicting mods
#if MULTITHREADED_UPDATE
    std::atomic_flag duplicatedLock{};
#endif

    // update data container
    Map<std::string, UPtr<Map<std::string, VecNstr, alphanum_less>>> newFile;
    Map<std::string, SPtr<nemesis::HkxBehaviorFile>> behavior_map;
    Vec<nemesis::File> corefiles;

    // behavior file, node ID, node data lines; memory to access each node
#if MULTITHREADED_UPDATE
    std::atomic_flag newFileLock{};
#endif

    UMap<std::string, UPtr<Map<std::string, UMap<std::string, bool>>>> childrenState;
    // behavior file, node ID, children of state machine; ensure no state conflict

    SSSMap stateID; // behavior file, children state machine info, state ID
    SSSMap parent;  // behavior file, children state machine info, parent state machine

    USet<std::string> newProjectList;
    MasterAnimData animData;
    MasterAnimSetData animSetData;

    Map<std::string, VecNstr> newAnimAddition;
    UMap<std::wstring, std::wstring> lastUpdate;
    UMap<std::string, UMap<std::string, size_t>> modFileCounter; // modcode, behavior, number of files
    UMap<std::string, UMap<std::string, VecStr>> modFileList;    // modcode, behavior, node files
    std::atomic_flag fileCountLock{};

    // global container locker
#if MULTITHREADED_UPDATE
    std::atomic_flag behaviorPathLock{};
    std::atomic_flag behaviorProjectLock{};
    std::atomic_flag behaviorProjectPathLock{};
#endif

    // timer
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

    static const nemesis::regex num_rgx;
};

#endif
