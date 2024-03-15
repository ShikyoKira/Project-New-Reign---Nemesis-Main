#include <mutex>

#include <QtCore/QCoreApplication.h>

#include <QtConcurrent/qtconcurrentrun.h>

#include "debuglog.h"
#include "externalscript.h"
#include "nemesisinfo.h"
#include "version.h"

#include "ui/Terminator.h"

//#include "../../Test/StopWatch.h"

#include "utilities/renew.h"
#include "utilities/constants.h"
#include "utilities/lastupdate.h"
#include "utilities/filechecker.h"
#include "utilities/stringextension.h"
#include "utilities/conditionsyntax.h"
#if MULTITHREADED_UPDATE
#include "utilities/threadpool.h"
#endif

#include "hkx/HkxBehaviorFile.h"

#include "update/modpatch.h"
#include "update/templatepatch.h"
#include "update/updateprocess.h"
#include "update/functionupdate.h"
#include "update/dataunification.h"

#include "generate/behaviorprocess_utility.h"

using namespace std;
namespace sf = filesystem;
namespace ns = nemesis::syntax;

using SSSMap = UMap<string, UPtr<UMapStr2>>;
using MapChildState = UMap<string, UPtr<Map<string, UMap<string, bool>>>>;

extern bool processdone;
extern mutex processlock;
extern condition_variable cv;
extern Terminator* p_terminate;
extern atomic<int> m_RunningThread;
extern atomic_flag atomic_lock;
extern atomic_flag newAnimAdditionLock;

const nemesis::regex UpdateFilesStart::num_rgx("[^0-9]*([0-9]+).*");

#if MULTITHREADED_UPDATE
mutex admtx;
mutex asdmtx;
#endif

void writeSave(FileWriter& writer, const string& line, string& store);
void writeSave(FileWriter& writer, const char* line, string& store);
void stateCheck(UMapStr2& parent,
                string parentID,
                string lowerbehaviorfile,
                string sID,
                UPtr<UMapStr2>& stateID,
                UPtr<UMapStr2>& n_stateID,
                VecStr children,
                string filename,
                string ID,
                string modcode,
                StateIDList& duplicatedStateList
#if MULTITHREADED_UPDATE
                ,
                std::atomic_flag& lockless
#endif
);

class UpdateLock
{
    UMap<string, NodeU>& modUpdate;

public:
    UpdateLock(UMap<string, NodeU>& n_modUpdate)
        : modUpdate(n_modUpdate)
    {
    }

    NodeU& operator[](string key)
    {
        Lockless lock(atomic_lock);
        return modUpdate[key];
    }
};

struct arguPack
{
    arguPack(Map<string, UPtr<Map<string, VecNstr, alphanum_less>>>& n_newFile,
             SSSMap& n_parent,
             MasterAnimData& n_animData,
             MasterAnimSetData& n_animSetData,
             SPtr<UpdateLock> n_modUpdate
#if MULTITHREADED_UPDATE
             ,
             std::atomic_flag& n_parentLock
#endif
             )
        : newFile(n_newFile)
        , parent(n_parent)
        , animData(n_animData)
        , animSetData(n_animSetData)
#if MULTITHREADED_UPDATE
        , parentLock(n_parentLock)
#endif
    {
        modUpdate = n_modUpdate;
    }

    SPtr<UpdateLock> modUpdate;
    Map<string, UPtr<Map<string, VecNstr, alphanum_less>>>& newFile;

    UMap<string, UPtr<UMapStr2>> n_stateID;
#if MULTITHREADED_UPDATE
    std::atomic_flag stateLock{};
#endif

    UMap<string, UPtr<UMap<string, VecStr>>> statelist;

    SSSMap& parent;
#if MULTITHREADED_UPDATE
    std::atomic_flag& parentLock;
#endif

    MasterAnimData& animData;
    MasterAnimSetData& animSetData;
};

UpdateFilesStart::GlobalCache::GlobalCache()
{
    ClearGlobal();
}

UpdateFilesStart::GlobalCache::~GlobalCache()
{
    ClearGlobal();
}

bool UpdateFilesStart::FileValidation()
{
    milestoneStart(mod_directory);

    // Check the existence of required files
    return FileCheck(true);
}

bool UpdateFilesStart::TryClearEngineData()
{
    RunScript(R"(scripts\update\start\)");
    DebugLogging("External script run complete");

    // clear the temp_behaviors folder to prevent it from bloating
    ClearTempBehaviors();
    DebugLogging("Temp behavior clearance complete");

    // create "temp_behaviors" folder
    if (!isFileExist(mod_directory)) sf::create_directory(mod_directory);

    if (error) return false;

    emit progressUp(); // 2
    return true;
}

bool UpdateFilesStart::TryReadBaseFiles()
{
    return VanillaUpdate() && !error;
}

bool UpdateFilesStart::TryJoinModPatches()
{
    DebugLogging("Data record complete");
    emit progressUp(); // 4

    // check template for association with vanilla nodes from behavior template file
    //newAnimProcess(newAnimDirectory);

    AddTemplates();

    // comparing if different from mod file
    AddModPatches();

    return newAnimFunction && !error;
}

void UpdateFilesStart::CompileEngineData()
{
    DebugLogging("Modification successfully extracted");
    emit progressUp(); // 27

    // compiling all behaviors in "data/meshes" to "temp_behaviors" folder
    CombiningFiles();

    emit progressUp(); // 32
}

UpdateFilesStart::UpdateFilesStart()
{
}

UpdateFilesStart::~UpdateFilesStart()
{
    if (!cmdline && error) error = false;
}

void UpdateFilesStart::startUpdatingFile()
{
    QtConcurrent::run([&]() {
        while (running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ++timeout_timer;

            // timeout after 10 mins
            if (timeout_timer == 6000)
            {
                error = true;
                emit criticalError("TIMEOUT", "Update process timeout");
                return;
            }
        }
    });

    GlobalCache cache;

    if (!FileValidation()) return;

    if (!TryClearEngineData()) return;

    if (!TryReadBaseFiles()) return;

    if (!TryJoinModPatches()) return;

    CompileEngineData();
}

void UpdateFilesStart::UpdateFiles()
{
    try
    {
        startUpdatingFile();

        if (!cmdline) this_thread::sleep_for(chrono::milliseconds(1500));
    }
    catch (nemesis::exception&)
    {
        // resolved exception
    }
    catch (exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
    catch (...)
    {
        try
        {
            ErrorMessage(6001, "Unknown: Update Failed");
        }
        catch (nemesis::exception&)
        {
            // resolved exception
        }
    }

    unregisterProcess();
    p_terminate->exitSignal();
}

bool UpdateFilesStart::VanillaUpdate()
{
    if (error) throw nemesis::exception();

#if MULTITHREADED_UPDATE
    nemesis::ThreadPool mt;

    for (SPtr<RegisterQueue>& curBehavior : registeredFiles)
    {
        mt.enqueue(&UpdateFilesStart::RegisterBehavior, this, curBehavior);
    }

    mt.join();
#else
    for (SPtr<RegisterQueue>& curBehavior : registeredFiles)
    {
        RegisterBehavior(curBehavior);
    }
#endif

    emit progressUp(); // 3

    if (!behaviorPath.empty())
    {
        const string cachebpj = "cache\\behavior_path";

        FileWriter output(cachebpj);

        if (!output.is_open()) ErrorMessage(2009, cachebpj);

        for (auto& it : behaviorPath)
        {
            output << it.first << "=" << it.second << "\n";
        }
    }

    if (!behaviorProject.empty())
    {
        const string cachebpj = "cache\\behavior_project";

        FileWriter output(cachebpj);

        if (!output.is_open()) ErrorMessage(2009, cachebpj);

        for (auto&it : behaviorProject)
        {
            output << it.first.data() << "\n";

            for (auto& pj : it.second)
            {
                output << pj << "\n";
            }

            output << "\n";
        }
    }

    if (!behaviorProjectPath.empty())
    {
        FileWriter output("cache\\behavior_project_path");

        if (output.is_open())
        {
            for (auto& it : behaviorProjectPath)
            {
                output << it.first << "=" << it.second << "\n";
            }
        }
        else
        {
            ErrorMessage(2009, "cache\\behavior_project_path");
        }
    }

    return true;
}

void UpdateFilesStart::GetFileLoop(string path)
{
    VecStr filelist;
    read_directory(path, filelist);

    for (auto& file : filelist)
    {
        string newPath = path + file;
        sf::path curfile(newPath);

        if (sf::is_directory(curfile))
        {
            GetFileLoop(newPath + "\\");
            return;
        }

        if (nemesis::iequals(curfile.extension().string(), ".xml")
            || nemesis::iequals(curfile.extension().string(), ".txt"))
        {
            string curFileName = curfile.stem().string();

            if ((nemesis::iequals(curFileName, "nemesis_animationdatasinglefile")
                 || nemesis::iequals(curFileName, "nemesis_animationsetdatasinglefile"))
                || (nemesis::isearch(curFileName, "Nemesis_") == 0
                    && nemesis::isearch(curFileName, "_List") != curFileName.length() - 5
                    && nemesis::isearch(curFileName, "_Project") != curFileName.length() - 8)
                || (nemesis::isearch(curFileName, "Nemesis_") == 0
                    && nemesis::isearch(curFileName, "_Project") + 8 == curFileName.length()))
            {
                ++filenum;
            }
        }
    }
}

void UpdateFilesStart::RegisterBehavior(SPtr<RegisterQueue> curBehavior)
{
    try
    {
        if (curBehavior == nullptr) return;

        wstring curFileName  = curBehavior->file.stem().wstring();
        wstring fileFullName = curBehavior->file.filename().wstring();
        wstring newPath      = curBehavior->file.wstring();

        if (nemesis::iequals(curFileName, L"nemesis_animationdatasinglefile"))
        {
            curFileName = curFileName.substr(8);
            DebugLogging(L"AnimData Disassemble start (File: " + newPath + L")");

            {
#if MULTITHREADED_UPDATE
                Lockless lock(behaviorPathLock);
#endif
                behaviorPath[nemesis::to_lower_copy(curFileName)]
                    = nemesis::to_lower_copy(curBehavior->file.parent_path().wstring() + L"\\" + curFileName);
            }

            if (!AnimDataDisassemble(newPath, animData)) return;

            saveLastUpdate(nemesis::to_lower_copy(newPath), lastUpdate);

            DebugLogging(L"AnimData Disassemble complete (File: " + newPath + L")");
            emit progressUp();
        }
        else if (nemesis::iequals(curFileName, L"nemesis_animationsetdatasinglefile"))
        {
            curFileName = curFileName.substr(8);
            DebugLogging(L"AnimSetData Disassemble start (File: " + newPath + L")");

            {
#if MULTITHREADED_UPDATE
                Lockless lock(behaviorPathLock);
#endif
                behaviorPath[nemesis::to_lower_copy(curFileName)]
                    = nemesis::to_lower_copy(curBehavior->file.parent_path().wstring() + L"\\" + curFileName);
            }

            if (!AnimSetDataDisassemble(newPath, animSetData)) return;

            saveLastUpdate(nemesis::to_lower_copy(newPath), lastUpdate);

            DebugLogging(L"AnimSetData Disassemble complete (File: " + newPath + L")");

            emit progressUp();
        }
        else if (StringStartWith(curFileName, L"Nemesis_"))
        {
            if (!StringEndWith(curFileName, L"_Project"))
            {
                if (StringEndWith(curFileName, L"_List")) return;

                //wstring firstperson = L"";

                //if (curBehavior->isFirstPerson) firstperson = L"_1stperson\\";

                //curFileName = firstperson + curFileName.substr(8);
                curFileName = curFileName.substr(8);
                DebugLogging(L"Behavior Disassemble start (File: " + newPath + L")");

                wstring targetpath = nemesis::to_lower_copy(curBehavior->file.parent_path().wstring() + L"\\"
                                                            + curBehavior->file.stem().wstring().substr(8));
                auto neminfo       = NemesisInfo::GetInstance();
                sf::path relativepath
                    = curBehavior->file.string().substr(neminfo->GetDataPath().string().length());
                relativepath.replace_filename(curFileName);

                {
#if MULTITHREADED_UPDATE
                    Lockless lock(behaviorPathLock);
#endif
                    behaviorPath.insert(std::make_pair(nemesis::to_lower_copy(relativepath.wstring()), targetpath));
                }

                //UPtr<Map<string, VecNstr, alphanum_less>> _curNewFile
                //    = make_unique<Map<string, VecNstr, alphanum_less>>();
                //UPtr<Map<string, UMap<string, bool>>> _childrenState
                //    = make_unique<Map<string, UMap<string, bool>>>();
                //UPtr<UMapStr2> _stateID = make_unique<UMapStr2>();
                //UPtr<UMapStr2> _parent  = make_unique<UMapStr2>();

                //interMsg(L"File: " + newPath);

                //StopWatch::Start();
                //VanillaDisassemble(newPath, _curNewFile, _childrenState, _stateID, _parent);
                //StopWatch::Stop();
                //interMsg(L"Vanilla Time: " + std::to_wstring(StopWatch::GetMilliSeconds()) + L"ms");

                //StopWatch::Start();
                auto behavior = nemesis::HkxBehaviorFile::File(newPath);
                behavior->ParseBehavior({});
                //StopWatch::Stop();
                //interMsg(L"HkxBehaviorFile Time: " + std::to_wstring(StopWatch::GetMilliSeconds()) + L"ms");

                string curFileNameA = relativepath.string();
                nemesis::to_lower(curFileNameA);

                behavior_map.insert(std::make_pair(curFileNameA, behavior));
                //
//                {
//#if MULTITHREADED_UPDATE
//                    Lockless lock(newFileLock);
//#endif
//                    newFile[curFileNameA]       = move(_curNewFile);
//                    childrenState[curFileNameA] = move(_childrenState);
//                    stateID[curFileNameA]       = move(_stateID);
//                    parent[curFileNameA]        = move(_parent);
//                }

                saveLastUpdate(nemesis::to_lower_copy(newPath), lastUpdate);

                DebugLogging(L"Behavior Disassemble complete (File: " + newPath + L")");
                emit progressUp();

                if (nemesis::to_lower_copy(curBehavior->file.parent_path().filename().wstring())
                        .find(L"characters")
                    == 0)
                {
                    registeredAnim[curFileNameA] = SetStr();
                }

                return;
            }

            wstring firstperson = L"";

            if (curBehavior->isFirstPerson) firstperson = L"_1stperson\\";

            wstring curPath = nemesis::to_lower_copy(curBehavior->file.parent_path().wstring());
            curPath         = curPath.substr(curPath.find(L"\\meshes\\") + 1);
            curFileName
                = nemesis::to_lower_copy(firstperson + curFileName.substr(8, curFileName.length() - 16));

            {
#if MULTITHREADED_UPDATE
                Lockless lock(behaviorProjectPathLock);
#endif
                behaviorProjectPath[curFileName] = curPath;
            }

            VecStr storeline;
            bool record = false;
            DebugLogging(L"Nemesis Project Record start (File: " + newPath + L")");

            if (!GetFileLines(newPath, storeline)) return;

            for (size_t j = 0; j < storeline.size(); ++j)
            {
                string& line = storeline[j];

                if (record && line.find("</hkparam>") != NOT_FOUND) break;

                if (record)
                {
                    if (line.find("<hkcstring>") == NOT_FOUND) ErrorMessage(1093, newPath, j + 1);

                    int pos = line.find("<hkcstring>") + 11;
                    string characterfile
                        = nemesis::to_lower_copy(line.substr(pos, line.find("</hkcstring>", pos) - pos));
                    characterfile = GetFileName(characterfile);

#if MULTITHREADED_UPDATE
                    Lockless lock(behaviorProjectLock);
#endif
                    behaviorProject[characterfile].push_back(nemesis::transform_to<string>(curFileName));
                }

                if (line.find(R"(<hkparam name="characterFilenames" numelements=")") != NOT_FOUND
                    && line.find("</hkparam>") == NOT_FOUND)
                    record = true;
            }

            saveLastUpdate(nemesis::to_lower_copy(newPath), lastUpdate);

            emit progressUp();
            DebugLogging(L"Nemesis Project Record complete (File: " + newPath + L")");
        }
    }
    catch (const exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
}

void UpdateFilesStart::GetPathLoop(const filesystem::path& path, bool isFirstPerson)
{
    try
    {
        try
        {
            VecWstr filelist;
            read_directory(path, filelist);

            for (auto& file : filelist)
            {
                wstring newPath = path.wstring() + file;

                sf::path curfile(newPath);

                if (!sf::is_directory(curfile))
                {
                    string ext = curfile.extension().string();

                    if (nemesis::iequals(ext, ".xml") || nemesis::iequals(ext, ".txt"))
                    {
                        string curFileName = curfile.stem().string();
                        ext                = curFileName.data();

                        if (curfile.string().find("]") != NOT_FOUND)
                        {
                            curFileName = curfile.stem().string();
                        }

                        if (nemesis::iequals(ext, "nemesis_animationdatasinglefile")
                            || nemesis::iequals(ext, "nemesis_animationsetdatasinglefile"))
                        {
#if MULTITHREADED_UPDATE
                            Lockless locker(queueLock);
#endif
                            registeredFiles.push_back(make_shared<RegisterQueue>(curfile, false));

                            if (registeredFiles.back()->file.string().find("]") != NOT_FOUND)
                            {
                                curFileName = curfile.stem().string();
                            }
                        }
                        else if ((wordFind(curFileName, "Nemesis_") == 0
                                  && wordFind(curFileName, "_List") != curFileName.length() - 5
                                  && wordFind(curFileName, "_Project") != curFileName.length() - 8)
                                 || (wordFind(curFileName, "Nemesis_") == 0
                                     && wordFind(curFileName, "_Project") + 8 == curFileName.length()))
                        {
#if MULTITHREADED_UPDATE
                            Lockless locker(queueLock);
#endif
                            registeredFiles.push_back(make_shared<RegisterQueue>(curfile, isFirstPerson));
                        }
                    }
                }
                else
                {
                    GetPathLoop(newPath + L"\\",
                                (nemesis::iequals(file, L"_1stperson") ? true : isFirstPerson));
                }

                if (error) throw nemesis::exception();
            }
        }
        catch (exception& ex)
        {
            ErrorMessage(6001, ex.what());
        }
    }
    catch (nemesis::exception&)
    {
        // resolved exception
    }
    catch (...)
    {
        try
        {
            ErrorMessage(6001, "Unknown: GetPathLoop");
        }
        catch (nemesis::exception&)
        {
            // resolved exception
        }
    }
}

bool UpdateFilesStart::VanillaDisassemble(const wstring& path,
                                          UPtr<Map<string, VecNstr, alphanum_less>>& curNewFile,
                                          UPtr<Map<string, UMap<string, bool>>>& childrenState,
                                          UPtr<UMapStr2>& stateID,
                                          UPtr<UMapStr2>& parent)
{
    VecNstr storeline;
    storeline.reserve(2000);

	if (!isFileExist(path)) ErrorMessage(1068, path);

    FileReader vanillafile(path);
    string curID;

    UMap<string, VecStr> statelist; // parent ID, list of children

    if (!vanillafile.TryGetFile()) ErrorMessage(2000, path, vanillafile.ErrorMessage());

    bool skip  = true;
    bool start = false;
    bool isSM  = false;
    nemesis::Line curline;

    while (vanillafile.TryGetLines(curline))
    {
        if (curline.find("	</hksection>") != NOT_FOUND) break;

        if (!skip)
        {
            if (curline.find("SERIALIZE_IGNORED") == NOT_FOUND)
            {
                bool isVector4 = false;

                if (!storeline.empty() && storeline.back().find("numelements=\"") != NOT_FOUND
                    && storeline.back().find("</hkparam>", storeline.back().find("numelements=\""))
                           == NOT_FOUND)
                {
                    if (curline.find("			#") != NOT_FOUND)
                    {
                        start = true;
                    }
                    else
                    {
                        bool check = true;

                        for (auto& each : curline)
                        {
                            if (each != '\t' && each != '.' && each != '-' && each != ' ' && !isdigit(each))
                            {
                                check = false;
                                break;
                            }
                        }

                        if (check) start = true;
                    }
                }
                else
                {
                    size_t pos = curline.find("\">(");

                    if (pos != NOT_FOUND && curline.find("<hkparam name=\"") != NOT_FOUND)
                    {
                        pos += 3;
                        char curchr = curline[pos];
                        size_t pos2 = curline.find(")</hkparam>", pos);

                        if ((isdigit(curchr) || curchr == '-') && pos2 != NOT_FOUND)
                        {
                            bool check  = true;
                            string test = curline.substr(pos, pos2 - pos);

                            for (auto& each : test)
                            {
                                if (each != '\t' && each != '.' && each != '-' && each != ' '
                                    && !isdigit(each))
                                {
                                    check = false;
                                    break;
                                }
                            }

                            if (check)
                            {
                                isVector4 = true;
                                start     = true;
                            }
                        }
                    }
                    else if (start && curline.find("</hkparam>") != NOT_FOUND)
                    {
                        start = false;
                    }
                }

                if (curline.find("<hkparam name=\"stateId\">") != NOT_FOUND)
                {
                    string stateIDStr = nemesis::regex_replace(
                        curline, nemesis::regex(".*<hkparam name=\"stateId\">([0-9]+)</hkparam>.*"), "$1");

                    if (curline != stateIDStr)
                    {
                        (*stateID)[curID] = stateIDStr;
                    }
                }

                if (start)
                {
                    if (curline.find("			#") != NOT_FOUND)
                    {
                        VecStr curElements;
                        string temp   = curline.substr(0, curline.find("#"));
                        string spaces = string(count(temp.begin(), temp.end(), '\t'), '\t');
                        StringSplit(curline, curElements);

                        if (isSM)
                        {
                            for (auto& element : curElements)
                            {
                                storeline.push_back(spaces + element);
                                statelist[curID].push_back(element);
                                (*parent)[element] = curID;
                            }
                        }
                        else
                        {
                            for (auto& element : curElements)
                            {
                                storeline.push_back(spaces + element);
                            }
                        }
                    }
                    else
                    {
                        bool bone = false;
                        nemesis::regex vector4("\\(((?:-|)[0-9]+\\.[0-9]+) ((?:-|)[0-9]+\\.[0-9]+) "
                                               "((?:-|)[0-9]+\\.[0-9]+) ((?:-|)[0-9]+\\.[0-9]+)\\)");
                        nemesis::smatch match;

                        if (!nemesis::regex_search(curline, match, vector4))
                        {
                            string spaces;

                            for (auto& ch : curline)
                            {
                                if (ch == '\t')
                                    spaces.push_back(ch);
                                else
                                    break;
                            }

                            if (curline.find("<!-- Bone$N -->") == NOT_FOUND)
                            {
                                nemesis::regex bone_rgx("([0-9]+(\\.[0-9]+)?)");

                                for (auto it = nemesis::regex_iterator(curline, bone_rgx);
                                     it != nemesis::regex_iterator();
                                     ++it)
                                {
                                    storeline.push_back(spaces + it->str(1));
                                    bone = true;
                                }
                            }
                        }
                        else if (isVector4)
                        {
                            bone          = true;
                            string spaces = "\t";

                            for (auto& ch : curline)
                            {
                                if (ch == '\t')
                                    spaces.push_back(ch);
                                else
                                    break;
                            }

                            storeline.push_back(curline.substr(0, match.position()));

                            for (nemesis::regex_iterator it(curline, vector4);
                                 it != nemesis::regex_iterator();
                                 ++it)
                            {
                                storeline.push_back(spaces + it->str(1));
                                storeline.push_back(spaces + it->str(2));
                                storeline.push_back(spaces + it->str(3));
                                storeline.push_back(spaces + it->str(4));
                            }

                            spaces.pop_back();
                            storeline.push_back(
                                spaces + curline.substr(match.position() + match.str().length()).ToString());
                            start = false;
                        }
                        else
                        {
                            bone = true;
                            string spaces;

                            for (auto& ch : curline)
                            {
                                if (ch == '\t')
                                    spaces.push_back(ch);
                                else
                                    break;
                            }

                            for (nemesis::regex_iterator it(curline, vector4);
                                 it != nemesis::regex_iterator();
                                 ++it)
                            {
                                storeline.push_back(spaces + it->str(1));
                                storeline.push_back(spaces + it->str(2));
                                storeline.push_back(spaces + it->str(3));
                                storeline.push_back(spaces + it->str(4));
                            }
                        }

                        if (!bone) storeline.push_back(curline);
                    }
                }
                else
                {
                    if (curline.find("<hkobject name=\"") != NOT_FOUND
                        && curline.find("signature=\"", curline.find("<hkobject name=\"")) != NOT_FOUND)
                    {
                        isSM = curline.find(R"(class="hkbStateMachine" signature=")") != NOT_FOUND;

                        if (!storeline.empty() && !curID.empty())
                        {
                            storeline.shrink_to_fit();
                            (*curNewFile)[curID] = storeline;
                            storeline.reserve(2000);
                            storeline.clear();
                        }

                        size_t pos = curline.find("<hkobject name=\"#") + 16;
                        curID      = curline.substr(
                            pos, curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos).ToString();
                    }

                    storeline.push_back(curline);
                }
            }
        }
        else if (curline.find("<hkobject name=\"#") != NOT_FOUND)
        {
            skip       = false;
            size_t pos = curline.find("<hkobject name=\"#") + 16;
            curID = curline.substr(pos, curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos)
                        .ToString();
            storeline.push_back(curline);
        }
    }

    if (!storeline.empty() && !curID.empty())
    {
        storeline.shrink_to_fit();
        (*curNewFile)[curID] = storeline;
    }

    for (auto& state : statelist)
    {
        for (auto& ID : state.second)
        {
            string sid = (*stateID)[ID];

            if (sid.empty()) ErrorMessage(1187);

            (*childrenState)[state.first][sid] = true;
        }
    }

    return true;
}

bool UpdateFilesStart::AnimDataDisassemble(const wstring& path, MasterAnimData& animData)
{
#if MULTITHREADED_UPDATE
    scoped_lock<mutex> adlock(admtx);
#endif
    size_t num;
    VecNstr storeline;
    UMap<string, int> projectNameCount;

    int projectcounter = 0;
    size_t totallines = 0;

    string project;
    string header;

    bool special = false;
    bool isInfo  = false;
    bool end     = false;
    bool out     = true;

    if (!GetFileLines(path, storeline)) return false;

    {
        string strnum = nemesis::regex_replace(storeline[0], num_rgx, "$1");

        if (!isOnlyNumber(strnum) || stoi(strnum) < 10) ErrorMessage(3014);

        num = stoi(strnum) + 1;
    }

    for (size_t i = 1; i < num; ++i)
    {
        animData.add(storeline[i], i);
    }

    try
    {
        while (storeline.back().length() == 0)
        {
            storeline.pop_back();
        }

        for (size_t i = num; i < storeline.size();)
        {
            size_t size                      = stoi(storeline[i]) + i;
            SPtr<AnimDataProject_Condt> proj = animData.projectlist[projectcounter++].raw->second;

            // start project header
            VecNstr templine;
            templine.push_back(storeline[i]);
            templine.push_back(storeline[++i]);
            templine.push_back(storeline[++i]);

            while (!isOnlyNumber(storeline[++i]))
            {
                templine.push_back(storeline[i]);
            }

            templine.push_back(storeline[i++]);
            proj->update("original", templine, i - templine.size() + 1);
            templine.clear();

            if (proj->childActive == "0") continue;

            // start anim data section
            while (i <= size)
            {
                templine.push_back(storeline[i]);

                if (templine.back().length() == 0)
                {
                    proj->aadd(templine[0], "original", templine, i - templine.size() + 2);
                    templine.clear();
                }

                ++i;
            }

            // start info data section
            size = stoi(storeline[i]) + i;
            templine.clear();
            ++i;

            while (i <= size)
            {
                templine.push_back(storeline[i]);

                if (templine.back().length() == 0)
                {
                    proj->iadd(templine[0], "original", templine, i - templine.size() + 2);
                    templine.clear();
                }

                ++i;
            }
        }
    }
    catch (const exception&)
    {
        ErrorMessage(3014);
    }

    return true;
}

bool UpdateFilesStart::AnimSetDataDisassemble(const wstring& path, MasterAnimSetData& animSetData)
{
#if MULTITHREADED_UPDATE
    scoped_lock<mutex> asdlock(asdmtx);
#endif
    VecNstr storeline;
    size_t num;
    VecStr newline;
    newline.reserve(500);

    if (!GetFileLines(path, storeline)) return false;

    {
        string strnum = nemesis::regex_replace(storeline.front(), num_rgx, "$1");

        if (!isOnlyNumber(strnum) || stoi(strnum) < 10) ErrorMessage(3014);

        num = stoi(strnum) + 1;
    }

    // Adding project
    for (size_t i = 1; i < num; ++i)
    {
        animSetData.add(storeline[i]);
    }

    for (auto& project : animSetData.projects)
    {
        // Adding data set
        project.raw->ImportData(path, ++num, storeline);

        for (auto& dataset : project.raw->getAnimSetData())
        {
            num += 2;
            dataset.raw->ImportData(path, num, storeline);
        }
    }

    return true;
}

void UpdateFilesStart::ModThread(const string& directory,
                                 const string& node,
                                 const string& behavior,
                                 UMap<string, SPtr<arguPack>>& pack)
{
    VecStr* modlist = &modQueue[behavior][node];

    for (string& modcode : *modlist)
    {
        if (error) throw nemesis::exception();

        sf::path curPath(directory + modcode + "\\" + behavior + "\\" + node);
        auto& curpack = pack.at(modcode);

        if (sf::is_directory(curPath))
        {
            if (nemesis::iequals(behavior, "animationdatasinglefile"))
            {
                MasterAnimData& animData(curpack->animData);

                if (animData.projectlist.empty())
                {
                    ErrorMessage(3017, "nemesis_animationdatasinglefile.txt");
                }

                bool newChar       = false;
                bool openAnim      = false;
                bool openInfo      = false;
                string projectname = node;

                if (projectname != "$header$")
                {
                    size_t pos = projectname.find_last_of("~");

                    if (pos != NOT_FOUND) projectname.replace(pos, 0, ".txt");
                }

                VecStr uniquecodelist;
                string filepath = curPath.string();
                read_directory(filepath, uniquecodelist);

                if (projectname == "$header$")
                {
                    string fullpath = filepath + "\\$header$.txt";

                    if (!isFileExist(fullpath)) ErrorMessage(2002, fullpath, "-", "-");

                    saveLastUpdate(nemesis::to_lower_copy(fullpath), lastUpdate);
                    animData.projectListUpdate(modcode, fullpath, false);
                }
                else if (!animData.find(projectname, modcode))
                {
                    if (!animData.contains(projectname))
                    {
                        string fullpath = directory + modcode + "\\" + behavior + "\\$header$\\$header$.txt";

                        if (!isFileExist(fullpath)) ErrorMessage(2002, fullpath, "-", "-");

                        saveLastUpdate(nemesis::to_lower_copy(fullpath), lastUpdate);
                        animData.projectListUpdate(modcode, fullpath, false);
                    }

                    if (!animData.contains(projectname)) ErrorMessage(3011, "$header$.txt", projectname, modcode);

                    for (string& uniquecode : uniquecodelist)
                    {
                        if (!sf::is_directory(filepath + "\\" + uniquecode))
                        {
                            AnimDataUpdate(modcode,
                                           behavior,
                                           projectname,
                                           filepath + "\\" + uniquecode,
                                           animData,
                                           true,
                                           lastUpdate);

                            if (error) throw nemesis::exception();
                        }
                    }
                }
                else
                {
                    if (isFileExist(filepath + "\\$header$"))
                    {
                        AnimDataUpdate(modcode,
                                       behavior,
                                       projectname,
                                       filepath + "\\$header$",
                                       animData,
                                       true,
                                       lastUpdate);
                    }

                    for (string& uniquecode : uniquecodelist)
                    {
                        if (nemesis::iequals(uniquecode, "$header$")) continue;

                        if (!sf::is_directory(filepath + "\\" + uniquecode))
                        {
                            AnimDataUpdate(modcode,
                                           behavior,
                                           projectname,
                                           filepath + "\\" + uniquecode,
                                           animData,
                                           false,
                                           lastUpdate);

                            if (error) throw nemesis::exception();
                        }
                    }
                }
            }
            else if (nemesis::iequals(behavior, "animationsetdatasinglefile"))
            {
                MasterAnimSetData& animSetData = pack.at(modcode)->animSetData;

                if (!sf::is_directory(curPath)) continue;

                if (nemesis::iequals(node, "$header$")) continue;

                string project  = node + ".txt";

                while (project.find("~") != NOT_FOUND)
                {
                    project.replace(project.find("~"), 1, "\\");
                }

                string lowerproject = nemesis::to_lower_copy(project);

                VecStr uniquecodelist;
                read_directory(curPath.string(), uniquecodelist);

                bool newProject = false;
                auto projptr = animSetData.find(lowerproject, modcode);

                if (!projptr)
                {
                    string fullpath = directory + modcode + "\\" + behavior + "\\$header$\\$header$.txt";

                    if (!isFileExist(fullpath)) ErrorMessage(2002, fullpath, "-", "-");

                    saveLastUpdate(nemesis::to_lower_copy(fullpath), lastUpdate);

                    newProject = true;
                    animSetData.projectListUpdate(modcode, fullpath, nemesis::CondDetails::modCheck);
                }

                if (!animSetData.contains(project)) ErrorMessage(5000, "$header$.txt", project, modcode);

                filesystem::path headerPath = curPath.string() + "\\$header$.txt";

                if (sf::exists(headerPath) && !sf::is_directory(headerPath))
                {
                    VecNstr storeline;
                    projptr = animSetData.find(lowerproject, modcode);

                    if (!projptr) ErrorMessage(5000, "$header$.txt", project, modcode);

                    if (GetFileLines(headerPath, storeline, false))
                    {
                        size_t num = 1;

                        if (newProject)
                        {
                            projptr->ImportData(headerPath, num, storeline);
                        }
                        else
                        {
                            projptr->ImportData(headerPath, num, storeline, modcode);
                        }
                    }
                }

                for (string& uniquecode : uniquecodelist)
                {
                    if (uniquecode == "$header$.txt") continue;

                    if (!sf::is_directory(curPath.string() + "\\" + uniquecode))
                    {
                        AnimSetDataUpdate(modcode,
                                          behavior,
                                          node,
                                          project,
                                          curPath.string() + "\\" + uniquecode,
                                          animSetData,
                                          newProject,
                                          lastUpdate);

                        if (error) throw nemesis::exception();
                    }
                }
            }
        }
        else
        {
            string filepath = "mod\\" + modcode + "\\" + behavior + "\\" + node;

            UPtr<Map<string, VecNstr, alphanum_less>>& newFile(curpack->newFile[behavior]);
            SPtr<UpdateLock> modUpdate(curpack->modUpdate);

            (*modUpdate)[behavior + node.substr(0, node.find_last_of("."))].FunctionUpdate(
                modcode,
                behavior,
                node,
                newFile,
                curpack->n_stateID[behavior],
                curpack->parent[behavior],
                curpack->statelist[behavior],
                lastUpdate
#if MULTITHREADED_UPDATE
                ,
                newFileLock,
                curpack->stateLock,
                curpack->parentLock
#endif
            );

            if (error) throw nemesis::exception();
        }

        continue;

#if MULTITHREADED_UPDATE
        Lockless lock(fileCountLock);
        size_t counter = --modFileCounter[modcode][behavior];
        lock.Unlock();
#else
        size_t counter = --modFileCounter[modcode][behavior];
#endif

        if (counter <= 0 && behavior != "animationdatasinglefile" && behavior != "animationsetdatasinglefile")
        {
#if MULTITHREADED_UPDATE
            Lockless plock(pack[modcode]->parentLock);
            UMapStr2 parent = *pack[modcode]->parent[behavior];
            plock.Unlock();
#else
            UMapStr2 parent = *pack[modcode]->parent[behavior];
#endif
            string spath = directory + modcode + "\\" + behavior + "\\";

            for (auto& curNode : modFileList[modcode][behavior])
            {
                if (error) throw nemesis::exception();

                if (sf::is_directory(spath + curNode)) continue;

                UMap<string, bool> skipped;
                UPtr<UMapStr2>& _stateID(pack[modcode]->n_stateID[behavior]);
                auto& statelist = pack[modcode]->statelist[behavior];

                for (auto& state : *statelist)
                {
                    for (string& ID : state.second) // state machine info node ID
                    {
                        if (error) throw nemesis::exception();

                        string filename = spath + ID + ".txt";

                        if (_stateID->find(ID) != _stateID->end())
                        {
                            string sID  = (*_stateID)[ID];
                            skipped[ID] = true;

                            if (sID.empty()) ErrorMessage(1188, modcode, filename);

                            if ((*childrenState[behavior])[state.first][sID])
                            {
                                stateCheck(parent,
                                           state.first,
                                           behavior,
                                           sID,
                                           stateID[behavior],
                                           _stateID,
                                           state.second,
                                           filename,
                                           ID,
                                           modcode,
                                           duplicatedStateList
#if MULTITHREADED_UPDATE
                                           ,
                                           duplicatedLock
#endif
                                );
                            }
                            else
                            {
#if MULTITHREADED_UPDATE
                                Lockless locker(stateListLock);
                                set<string> list = modStateList[behavior][state.first][sID];
                                locker.Unlock();
#else
                                set<string> list = modStateList[behavior][state.first][sID];
#endif

                                if (list.size() > 0)
                                {
#if MULTITHREADED_UPDATE
                                    Lockless curlock(duplicatedLock);
#endif

                                    for (auto& modname : list)
                                    {
                                        duplicatedStateList[filename][ID][sID].insert(modname);
                                    }

                                    duplicatedStateList[filename][ID][sID].insert(modcode);
                                }

                                list.insert(modcode);
#if MULTITHREADED_UPDATE
                                Lockless lockee(stateListLock);
#endif
                                modStateList[behavior][ID][sID] = list;
                            }
                        }
                        else if (ID.find("$") != NOT_FOUND)
                        {
                            ErrorMessage(1190, modcode, behavior, state.first, ID);
                        }
                    }
                }

                for (auto& ID : (*_stateID))
                {
                    if (error) throw nemesis::exception();

                    if (skipped[ID.first]) continue;

                    bool skip       = false;
                    string parentID = parent[ID.first];
                    string filename = spath + ID.first + ".txt";

                    if (parentID.empty())
                    {
                        if (ID.first.find("$") != NOT_FOUND)
                        {
                            skip = true;
                        }
                        else
                        {
                            ErrorMessage(1133, modcode, ID.first);
                        }
                    }

                    if (skip) continue;

                    if ((*stateID[behavior])[ID.first] != ID.second)
                    {
                        if ((*childrenState[behavior])[parentID][ID.second])
                        {
                            stateCheck(parent,
                                       parentID,
                                       behavior,
                                       ID.second,
                                       stateID[behavior],
                                       _stateID,
                                       (*pack[modcode]->statelist[behavior])[parentID],
                                       filename,
                                       ID.first,
                                       modcode,
                                       duplicatedStateList
#if MULTITHREADED_UPDATE
                                       ,
                                       duplicatedLock
#endif
                            );
                        }
                        else
                        {
#if MULTITHREADED_UPDATE
                            Lockless locker(stateListLock);
                            set<string> list = modStateList[behavior][parentID][ID.second];
                            locker.Unlock();
#else
                            set<string> list = modStateList[behavior][parentID][ID.second];
#endif

                            if (list.size() > 0)
                            {
#if MULTITHREADED_UPDATE
                                Lockless curlock(duplicatedLock);
#endif

                                for (auto& modname : list)
                                {
                                    duplicatedStateList[filename][ID.first][ID.second].insert(modname);
                                }

                                duplicatedStateList[filename][ID.first][ID.second].insert(modcode);
                            }

                            list.insert(modcode);
#if MULTITHREADED_UPDATE
                            Lockless lockee(stateListLock);
#endif
                            modStateList[behavior][parentID][ID.second] = list;
                        }
                    }
                }
            }
        }
    }

    if (error) throw nemesis::exception();
}

void UpdateFilesStart::SeparateMod(const string& directory,
                                   TargetQueue target,
                                   UMap<string, SPtr<arguPack>>& pack)
{
    string node     = target.node;
    string behavior = target.file;

    try
    {
        int curQueue;
        int nextQueue;

#if MULTITHREADED_UPDATE
        Lockless locker(stackLock);
#endif

        size_t thisQueue = queuing;
        queuing++;

#if MULTITHREADED_UPDATE
        locker.Unlock();
#endif

        curQueue = thisQueue * 20 / processQueue.size();
        thisQueue++;
        nextQueue = thisQueue * 20 / processQueue.size();

        ModThread(directory, node, behavior, pack);

        while (curQueue < nextQueue && curQueue < 20)
        {
            emit progressUp();
            ++curQueue;
        }
    }
    catch (exception&)
    {
        if (node.empty() || behavior.empty()) throw;

        DebugLogging("Failed to process file (Node: " + node + ", Behavior: " + behavior + ")");
        throw;
    }
}

void UpdateFilesStart::AddModPatches()
{
    if (!sf::exists(nemesis::directory::mods)) return;

    VecStr filelist;
    read_directory(nemesis::directory::mods, filelist);

    //UMap<string, NodeU> modUpdate;
    //UMap<string, VecStr> filelist2;
    //SPtr<UpdateLock> modUpPtr = make_shared<UpdateLock>(modUpdate);

    //atomic_flag parentLock{};
    //UMap<string, SPtr<arguPack>> pack;
    //vector<sf::path> pathlist;

    for (auto& modcode : filelist)
    {
        auto codepath = nemesis::directory::mods / modcode;

        if (!sf::is_directory(codepath)) continue;

        nemesis::to_lower(modcode);
        AddCodedModPatch(modcode, codepath);
//
//        read_directory(nemesis::directory::mods / modcode, filelist2[modcode]);
//        pack.insert(make_pair(modcode,
//                              make_shared<arguPack>(newFile,
//                                                    parent,
//                                                    animData,
//                                                    animSetData,
//                                                    modUpPtr
//#if MULTITHREADED_UPDATE
//                                                    ,
//                                                    parentLock
//#endif
//                                                    )));
//        auto& curpack = pack[modcode];
//
//        for (auto& behavior : filelist2[modcode])
//        {
//            nemesis::to_lower(behavior);
//            string path = nemesis::directory::mods.string() + modcode + "\\" + behavior;
//
//            if (!sf::is_directory(path)) continue;
//
//            path.append("\\");
//
//            if (behavior == "_1stperson")
//            {
//                VecStr fbehaviorlist;
//                read_directory(path, fbehaviorlist);
//
//                for (auto& fbehavior : fbehaviorlist)
//                {
//                    nemesis::to_lower(fbehavior);
//                    string fpath   = path + fbehavior + "\\";
//                    string recName = behavior + "\\" + fbehavior;
//                    VecStr& nodelist = modFileList[modcode][recName];
//                    read_directory(fpath, nodelist);
//
//                    for (auto& node : nodelist)
//                    {
//                        auto filepath = fpath + node;
//
//                        if (sf::is_directory(filepath)) continue;
//
//                        nemesis::ModPatch patch(modcode, filepath);
//                        behavior_map[recName]->AddPatch(patch);
//
//                        nemesis::to_lower(node);
//                        modQueue[recName][node].push_back(modcode);
//                    }
//
//                    curpack->statelist.insert(make_pair(recName, make_unique<UMap<string, VecStr>>()));
//                    curpack->n_stateID.insert(make_pair(recName, make_unique<UMapStr2>()));
//                }
//
//                continue;
//            }
//
//            read_directory(path, modFileList[modcode][behavior]);
//            VecStr& nodelist = modFileList[modcode][behavior];
//
//            if (behavior == "animationdatasinglefile" || behavior == "animationsetdatasinglefile")
//            {
//                for (auto& node : nodelist)
//                {
//                    modQueue[behavior][node].push_back(modcode);
//                }
//
//                continue;
//            }
//
//            for (auto& node : nodelist)
//            {
//                auto filepath = path + node;
//
//                if (sf::is_directory(filepath)) continue;
//
//                nemesis::ModPatch patch(modcode, filepath);
//                behavior_map[behavior]->AddPatch(patch);
//
//                nemesis::to_lower(node);
//                modQueue[behavior][node].push_back(modcode);
//            }
//
//            curpack->statelist.insert(make_pair(behavior, make_unique<UMap<string, VecStr>>()));
//            curpack->n_stateID.insert(make_pair(behavior, make_unique<UMapStr2>()));
//        }
    }
//
//    for (auto& behavior : modQueue)
//    {
//        for (auto& node : behavior.second)
//        {
//            processQueue.emplace_back(behavior.first, node.first);
//        }
//    }
//
//    for (auto& modcode : modFileList)
//    {
//        for (auto& behavior : modcode.second)
//        {
//            modFileCounter[modcode.first][behavior.first] = behavior.second.size();
//        }
//    }
//
//    queuing = 0;
//
//    if (!processQueue.empty())
//    {
//        // Using non-multi threading method due to heap corruption upon using solution below
//#if MULTITHREADED_UPDATE
//        nemesis::ThreadPool multiThreads;
//
//        for (auto& each : processQueue)
//        {
//            multiThreads.enqueue(&UpdateFilesStart::SeparateMod, this, directory, each, std::ref(pack));
//        }
//
//        multiThreads.join();
//#else
//        for (auto& each : processQueue)
//        {
//            SeparateMod(mod_directory, each, pack);
//        }
//#endif
//    }
//    else
//    {
//        while (queuing < 20)
//        {
//            emit progressUp();
//            ++queuing;
//        }
//    }
//
//    if (error) throw nemesis::exception();
//
//    emit progressUp(); // 26
//
//    for (auto& duplicates : duplicatedStateList)
//    {
//        for (auto& IDs : duplicates.second)
//        {
//            for (auto& modlist : IDs.second)
//            {
//                string mods;
//
//                for (auto& mod : modlist.second)
//                {
//                    mods.append(mod + ", ");
//                }
//
//                mods.pop_back();
//                mods.pop_back();
//
//                if (mods.length() > 0) ErrorMessage(1189, duplicates.first, modlist.first, mods);
//            }
//        }
//    }
}

void UpdateFilesStart::AddCodedModPatch(const std::string& code, const std::filesystem::path& path)
{
    if (!sf::exists(path / nemesis::filename::info)) return;

    VecWstr contents;
    read_directory(path, contents);

    for (auto& each : contents)
    {
        auto each_path = path / each;

        if (!sf::is_directory(each_path)) continue;

        VecWstr contents2;
        read_directory(each_path, contents2);

        for (auto& each2 : contents2)
        {
            auto each_path2 = each_path / each2;

            if (!sf::is_directory(each_path2)) continue;

            if (nemesis::iequals(each2, nemesis::foldername::animdata))
            {
                MasterAnimData::ModPatch patch(code, each_path2);
                animData.AddModPatch(corefiles, patch);
                //AddAnimDataModPatch(code, filepath);
                continue;
            }

            if (nemesis::iequals(each2, nemesis::foldername::animsetdata))
            {
                MasterAnimSetData::ModPatch patch(code, each_path2);
                animSetData.AddModPatch(corefiles, patch);
                //AddAnimSetDataModPatch(patch);
                continue;
            }

            LoopModPatchFolders(code, sf::path(each) / each2, each_path2);
        }
    }
}

void UpdateFilesStart::AddAnimDataModPatch(const std::string& code, const std::filesystem::path& path)
{
    MasterAnimData::ModPatch patch(code, path);
    animData.AddModPatch(corefiles, patch);

    //auto headerdir = path / MasterAnimData::header_str;

    //if (sf::exists(headerdir))
    //{
    //    AddAnimDataHeaderModPatch(code, headerdir);
    //    std::remove_if(projfiles.begin(), projfiles.end(), [&](const std::string& projfile) {
    //        return nemesis::iequals(projfile, MasterAnimData::header_str.data());
    //    });
    //}

    //for (auto& projfile : projfiles)
    //{
    //    if (projfile != MasterAnimData::header_str)
    //    {
    //        size_t pos = projfile.find_last_of("~");

    //        if (pos != NOT_FOUND) projfile.replace(pos, 0, ".txt");
    //    }

    //    AddAnimDataProjModPatch(code, projfile, path);
    //}
}

void UpdateFilesStart::AddAnimDataHeaderModPatch(const std::string& code, const std::filesystem::path& path)
{
    auto fullpath = path / MasterAnimData::header_file;

    if (!sf::exists(fullpath)) ErrorMessage(2002, fullpath, "-", "-");

    corefiles.emplace_back(fullpath);
    animData.projectListUpdate(code, fullpath, false);
}

void UpdateFilesStart::AddAnimDataProjModPatch(const std::string& code,
                                               const std::string& project,
                                               const std::filesystem::path& path)
{
    VecStr uniquecodelist;
    string filepath = path.string();
    read_directory(path, uniquecodelist);

    if (!animData.find(project, code))
    {
        if (!animData.contains(project)) ErrorMessage(3011, MasterAnimData::header_file, project, code);

        for (auto& uniquecode : uniquecodelist)
        {
            if (error) throw nemesis::exception();

            if (sf::is_directory(path / uniquecode)) continue;

            AddAnimDataNewCodeModPatch(code, project, path / uniquecode);
        }

        return;
    }

    if (sf::exists(path / L"$header$"))
    {
        AddAnimDataNewCodeModPatch(code, project, path / L"$header$");
    }

    for (auto& uniquecode : uniquecodelist)
    {
        if (error) throw nemesis::exception();

        if (sf::is_directory(path / uniquecode)) continue;

        AddAnimDataExistingCodeModPatch(code, project, path / uniquecode);
    }
}

void UpdateFilesStart::AddAnimDataNewCodeModPatch(const std::string& code,
                                                  const std::string& project,
                                                  const std::filesystem::path& path)
{
    if (behaviorPath[nemesis::foldername::animdata].empty())
    {
        ErrorMessage(2007, nemesis::foldername::animdata);
    }

    VecNstr storeline;
    string filename = path.stem().string();
    corefiles.emplace_back(path);

    if (!GetFileLines(path, storeline, !nemesis::iequals(filename, "$header$"))) return;

    auto projindex = animData.getIndex(project);

    // must not replace storeline with animData.newAnimData[projectfile][filename] for the total line will not get counted
    if (nemesis::iequals(filename, "$header$"))
    {
        auto& proj = animData.projectlist[projindex];

        for (auto& cond : proj.nestedcond)
        {
            if (cond.conditions == code && !cond.rawlist[0].raw->second)
            {
                storeline.insert(storeline.begin(), "");
                cond.rawlist[0].raw->second = make_shared<AnimDataProject_Condt>(storeline);
                return;
            }
        }

        auto fullpath = path.parent_path().parent_path() / L"$header$" / L"$header$.txt";

        if (!sf::exists(fullpath)) ErrorMessage(2002, fullpath, "-", "-");

        animData.projectListUpdate(code, fullpath, false);

        for (auto& cond : animData.projectlist[projindex].nestedcond)
        {
            if (cond.conditions != code) continue;

            for (auto& each : cond.rawlist)
            {
                if (!each.raw) continue;

                storeline.insert(storeline.begin(), "");
                each.raw->second = make_shared<AnimDataProject_Condt>(storeline);
                return;
            }
        }

        ErrorMessage(2025);
    }

    MasterAnimData::ProjectPtr projptr;

    for (auto& cond : animData.projectlist[projindex].nestedcond)
    {
        if (cond.conditions != code) continue;

        for (auto& each : cond.rawlist)
        {
            if (!each.raw) continue;

            projptr = each.raw->second;
            break;
        }

        if (projptr) break;
    }

    if (!projptr) ErrorMessage(3021, project);

    if (isOnlyNumber(filename)) // info data
    {
        projptr->iadd(filename, "original", storeline, 1);
        return;
    }
    
    if (filename.find("~") == NOT_FOUND) ErrorMessage(2004, path);

    string tempname
        = nemesis::regex_replace(string(filename), nemesis::regex("[^~]*~([0-9]+)"), string("$1"));

    if (tempname == filename) ErrorMessage(2004, path);

    projptr->aadd(filename, "original", storeline, 1);
}

void UpdateFilesStart::AddAnimDataExistingCodeModPatch(const std::string& code,
                                                       const std::string& project,
                                                       const std::filesystem::path& path)
{
    if (nemesis::iequals(project, "$header$")) // header file
    {
        animData.projectListUpdate(code, path);
        return;
    }

    SPtr<AnimDataProject_Condt> projData = animData.find(project);

    if (projData == nullptr) ErrorMessage(1232, code, project, path);

    VecNstr storeline;
    string filename = path.stem().string();

    if (nemesis::iequals(filename, "$header$")) // project header
    {
        if (!GetFileLines(path, storeline)) return;

        projData->modify(code, storeline);
        return;
    }

    bool edited       = false;
    bool originalopen = false;
    bool editopen     = false;
    std::string modcode_cond = ns::ModCode(code);

    if (!GetFileLines(path, storeline, [&](std::string& line) {
            if (line.find("<!--") != NOT_FOUND)
            {
                if (!edited && line.find(modcode_cond) != NOT_FOUND)
                {
                    edited = true;
                }
                else if (line.find(ns::ModOriginal()) != NOT_FOUND)
                {
                    originalopen = true;
                }
                else if (line.find(ns::Close()) != NOT_FOUND)
                {
                    originalopen = false;
                    edited       = false;
                }

                return false;
            }

            return !originalopen;
        }))
    {
        return;
    }

    if (filename.find(code + "$") == NOT_FOUND)
    {
        // anim data
        if (!isOnlyNumber(filename))
        {
            // anim data
            if (!hasAlpha(storeline[0])) ErrorMessage(3006, project, filename);

            string check
                = nemesis::regex_replace(string(filename), nemesis::regex("^([^~]+)~[0-9]+$"), string("$1"));

            auto* curptr = projData->afindlist(check);

            if (check == filename || projData->ifindlist(filename.substr(check.length() + 1)) == nullptr)
            {
                ErrorMessage(2004, path);
            }

            if (!curptr)
            {
                auto pair
                    = make_pair(std::string(storeline[0]), nemesis::LinkedVar(AnimDataPack_Condt(storeline)));
                auto condpair = nemesis::CondVar(pair, code, nemesis::CondType::MOD_CODE);
                projData->animdatalist.push_back(condpair);
                return;
            }

            curptr->addCond(
                nemesis::LinkedVar(AnimDataPack_Condt(storeline)), code, nemesis::CondType::MOD_CODE);
            return;
        }

        // info data confirmation
        if (!isOnlyNumber(storeline[0].ToString())) ErrorMessage(3006, project, filename);

        auto* curptr = projData->ifindlist(filename);

        if (curptr == nullptr) ErrorMessage(2004, path);

        curptr->addCond(nemesis::LinkedVar(InfoDataPack_Condt(storeline)), code, nemesis::CondType::MOD_CODE);
        return;
    }

    //string tempID = nemesis::regex_replace(
    //    string(filename), nemesis::regex("[^~]*~" + code + "[$]([0-9]+)"), string("$1"));

    //if (filename != tempID && isOnlyNumber(tempID)) // anim data
    //{
    //    addAnimDataPack(project, filename, code, projData, storeline);
    //    return;
    //}

    //addInfoDataPack(path, filename, code, projData, storeline);
}

void UpdateFilesStart::AddAnimSetDataModPatch(const MasterAnimSetData::ModPatch& patch)
{
    animSetData.AddModPatch(corefiles, patch);
}
//
//void UpdateFilesStart::AddAnimSetDataModPatch(const std::string& code, const std::filesystem::path& path)
//{
//    sf::path curPath = path;
//    std::string node, directory, behavior;
//
//    if (!sf::is_directory(curPath)) return;
//
//    string project = node + (nemesis::iequals(node, "$header$") ? "" : ".txt");
//
//    std::replace_if(
//        project.begin(), project.end(), [&](const char& ch) { return ch == '~'; }, '\\');
//    string lowerproject = nemesis::to_lower_copy(project);
//
//    if (lowerproject == "$header$") return;
//
//    bool newProject = false;
//    auto projptr    = animSetData.find(lowerproject, code);
//
//    if (!projptr)
//    {
//        string fullpath = directory + code + "\\" + behavior + "\\$header$\\$header$.txt";
//
//        if (!isFileExist(fullpath)) ErrorMessage(2002, fullpath, "-", "-");
//
//        saveLastUpdate(nemesis::to_lower_copy(fullpath), lastUpdate);
//
//        newProject = true;
//        animSetData.projectListUpdate(code, fullpath, nemesis::CondDetails::modCheck);
//    }
//
//    if (!animSetData.contains(project)) ErrorMessage(5000, "$header$.txt", project, code);
//
//    VecStr uniquecodelist;
//    read_directory(curPath, uniquecodelist);
//
//    for (string& uniquecode : uniquecodelist)
//    {
//        auto filepath = curPath / uniquecode;
//
//        if (sf::is_directory(filepath)) continue;
//
//        if (uniquecode == "$header$.txt")
//        {
//            VecNstr storeline;
//            projptr = animSetData.find(lowerproject, code);
//
//            if (!projptr) ErrorMessage(5000, "$header$.txt", project, code);
//
//            if (!GetFileLines(filepath, storeline, false)) continue;
//
//            size_t num = 1;
//
//            if (newProject)
//            {
//                projptr->ImportData(filepath, num, storeline);
//                continue;
//            }
//
//            projptr->ImportData(filepath, num, storeline, code);
//            continue;
//        }
//
//        if (error) throw nemesis::exception();
//
//        if (behaviorPath.find(L"animationsetdatasinglefile") == behaviorPath.end())
//        {
//            ErrorMessage(2007, L"animationsetdatasinglefile");
//        }
//
//        corefiles.emplace_back(filepath);
//
//        VecNstr storeline;
//
//        if (!GetFileLines(filepath, storeline, false)) continue;
//
//        size_t num       = 2;
//        string lowername = nemesis::to_lower_copy(uniquecode);
//        auto masterProj  = animSetData.find(project, code);
//        size_t pos       = lowername.find(nemesis::to_lower_copy(code) + "$");
//
//        if (pos == 0)
//        {
//            masterProj->find(lowername.substr(pos + code.length() + 1), code)
//                ->ImportData(filepath, num, storeline, code);
//        }
//        else
//        {
//            masterProj->find(lowername)->ImportData(filepath, num, storeline, code);
//        }
//
//        if (error) throw nemesis::exception();
//    }
//}

void UpdateFilesStart::LoopModPatchFolders(const std::string& code,
                                           const std::filesystem::path& bhvpath,
                                           const std::filesystem::path& path)
{
    VecWstr filelist;
    read_directory(path, filelist);

    for (auto& file : filelist)
    {
        auto filepath = path / file;

        if (sf::is_directory(filepath))
        {
            LoopModPatchFolders(code, bhvpath / file, filepath);
            continue;
        }

        auto behavior_itr = behavior_map.find(nemesis::to_lower_copy(bhvpath.string()));

        if (behavior_itr == behavior_map.end()) ErrorMessage(1233, code, filepath);

        nemesis::ModPatch patch(code, filepath);
        behavior_map[nemesis::to_lower_copy(bhvpath.string())]->AddPatch(patch);
    }
}

void UpdateFilesStart::AddTemplates()
{
    sf::create_directories(nemesis::directory::templates);

    if (!sf::exists(nemesis::directory::templates)) ErrorMessage(2010, nemesis::directory::templates);

    VecStr codelist;
    read_directory(nemesis::directory::templates, codelist);

    for (auto& code : codelist)
    {
        if (!sf::exists(nemesis::directory::templates / code / nemesis::filename::optlist)) continue;

        nemesis::to_lower(code);
        nemesis::CodedTemplates codedtemplates(nemesis::directory::templates / code);
        AddCodedTemplates(codedtemplates);
    }
}

void UpdateFilesStart::AddCodedTemplates(const nemesis::CodedTemplates& codedtemplates)
{
    for (auto& patch : codedtemplates.GetPatches())
    {
        behavior_map[patch.GetDataPath().string()]->AddPatch(patch);
    }

    auto adpatches = codedtemplates.GetADPatches();
    corefiles.insert(corefiles.end(), adpatches.begin(), adpatches.end());

    for (auto& patch : codedtemplates.GetASDPatches())
    {
        auto proj = animSetData.find(std::string(patch.GetProject()), std::string(patch.GetCode()));
        VecNstr storeline;

        if (!patch.TryGetLines(storeline)) continue;

        size_t num            = 2;
        auto filepath         = patch.GetFilePath();
        std::string lowername = nemesis::to_lower_copy(filepath.stem().string());
        proj->find(lowername + filepath.extension().string())
            ->ImportDataTemplate(filepath, num, storeline, std::string(patch.GetCode()));
    }
}

void UpdateFilesStart::AddCodedTemplates(const std::string& code, const std::filesystem::path& path)
{
    if (!sf::exists(path / nemesis::filename::optlist)) return;

    VecWstr filelist;
    read_directory(path, filelist);

    for (auto& file : filelist)
    {
        auto filepath = path / file;

        if (!sf::is_directory(filepath)) continue;

        if (nemesis::iequals(file, nemesis::foldername::animdata))
        {
            AddAnimDataTemplate(code, filepath);
            continue;
        }

        if (nemesis::iequals(file, nemesis::foldername::animsetdata))
        {
            AddAnimSetDataTemplate(code, filepath);
            continue;
        }

        LoopTemplateFolders(code, file, filepath);
    }
}

void UpdateFilesStart::AddAnimDataTemplate(const std::string& code, const std::filesystem::path& path)
{
    VecStr characterlist;
    read_directory(path, characterlist);

    for (auto& character : characterlist)
    {
        sf::path characterfolder(path / character);

        if (sf::is_directory(characterfolder))
        {
            AddCharcAnimDataTemplate(code, character, characterfolder);
            continue;
        }

        string stemTemp = characterfolder.stem().string();

        if (stemTemp == "$header$")
        {
            VecNstr storeline;

            if (!GetFileLines(characterfolder, storeline)) continue;

            if (hasAlpha(storeline[0])) continue;
            
            if (isOnlyNumber(storeline[0])) continue;

            ErrorMessage(3006, stemTemp, characterfolder);
        }
        
        if (!nemesis::regex_match(stemTemp, nemesis::regex("^\\$(?!" + code + ").+\\$(?:UC|)$"))) continue;

        ErrorMessage(3023, "$" + code + "$" + (stemTemp.find("$UC") != NOT_FOUND ? "UC" : ""));
    }
}

void UpdateFilesStart::AddCharcAnimDataTemplate(const std::string& code,
                                                const std::string& character,
                                                const std::filesystem::path& path)
{
    VecStr headerlist;
    read_directory(path, headerlist);

    for (auto& header : headerlist)
    {
        sf::path curfile(path / header);

        if (sf::is_directory(curfile)) continue;

        corefiles.emplace_back(curfile);
        ValidateAnimDataTemplateFile(curfile);
        ValidateAnimDataTemplateCharc(character, curfile);

        if (error) throw nemesis::exception();
    }
}

void UpdateFilesStart::ValidateAnimDataTemplateFile(const std::filesystem::path& path)
{
    string filename = path.stem().string();
    string tempname
        = nemesis::regex_replace(string(filename), nemesis::regex("[^~]*~([0-9]+)"), string("$1"));

    if (tempname == filename) return;
    
    if (isOnlyNumber(tempname) && hasAlpha(filename.substr(0, filename.find("~")))) return;

    ErrorMessage(2004, path);
}

void UpdateFilesStart::ValidateAnimDataTemplateCharc(const std::string& character,
                                                     const std::filesystem::path& path)
{
    if (character.find("~") != NOT_FOUND) return;

    if (character != "$header$") return;

    ErrorMessage(3022, path);
}

void UpdateFilesStart::AddAnimSetDataTemplate(const std::string& code, const std::filesystem::path& path)
{
    VecStr projectfilelist;
    read_directory(path, projectfilelist);

    for (const auto& file : projectfilelist)
    {
        string projfile   = nemesis::to_lower_copy(file);
        sf::path filepath = path / file;

        if (!sf::is_directory(filepath) || file.find("~") == NOT_FOUND) continue;

        std::replace_if(
            projfile.begin(), projfile.end(), [](const char& ch) { return ch == '~'; }, '\\');
        AddAnimSetDataProjectTemplate(code, projfile, filepath);
    }
}

void UpdateFilesStart::AddAnimSetDataProjectTemplate(const std::string& code,
                                                     const std::string& project,
                                                     const std::filesystem::path& path)
{
    VecStr headerfiles;
    read_directory(path, headerfiles);

    for (auto& file : headerfiles)
    {
        std::filesystem::path filepath(path / file);

        if (std::filesystem::is_directory(filepath)) continue;

        if (!nemesis::iequals(filepath.extension().string(), ".txt")) continue;

        string lowername = nemesis::to_lower_copy(filepath.stem().string());

        if (lowername.length() > 2 && lowername.front() == '$' && lowername.back() == '$') continue;

        corefiles.emplace_back(filepath);

        size_t num      = 2;
        auto masterProj = animSetData.find(project, code);
        VecNstr storeline;

        if (!GetFileLines(filepath, storeline)) continue;

        masterProj->find(lowername + filepath.extension().string())
            ->ImportDataTemplate(filepath, num, storeline, code);
    }
}

void UpdateFilesStart::LoopTemplateFolders(const std::string& code,
                                           const std::filesystem::path& bhvpath,
                                           const std::filesystem::path& path)
{
    VecWstr filelist;
    read_directory(path, filelist);

    for (auto& file : filelist)
    {
        auto filepath = path / file;

        if (sf::is_directory(filepath))
        {
            LoopTemplateFolders(code, bhvpath / file, filepath);
            continue;
        }

        if (file.front() != '#') continue;

        nemesis::TemplatePatch patch(code, filepath, bhvpath);
        behavior_map[bhvpath.string()]->AddPatch(patch);
    }
}

void UpdateFilesStart::CombiningFiles()
{
    wstring compilingfolder    = getTempBhvrPath().wstring() + L"\\";
    unsigned long long bigNum  = CRC32Convert(GetNemesisVersion());
    unsigned long long bigNum2 = bigNum;

    for (auto& newAnim : newAnimAddition)
    {
        string total = newAnim.first;

        for (auto& line : newAnim.second)
        {
            total.append(line);
        }

        bigNum2 += CRC32Convert(total);
    }

    for (auto& behavior : newFile) // behavior file name
    {
        string rootID;
        UMap<VecStr*, bool> isOpen;
        string OpeningMod;
        VecStr filelines;
        VecStr datalines;
        VecStr* container_ptr;

        behavior_map[behavior.first]->SaveAsStaged(behavior.first);

        for (auto& node : (*behavior.second)) // behavior node ID
        {
            if (node.second.empty())
            {
                ErrorMessage(2008, behavior.first + " (" + node.first + ")");
            }

            auto& firstline = node.second.front();

            if (firstline.find(R"(class="hkRootLevelContainer" signature="0x2772c11e">)") != NOT_FOUND)
            {
                rootID        = "#" + nemesis::regex_replace(firstline, num_rgx, "$1");
                container_ptr = &datalines;
            }
            else if (firstline.find(R"(class="hkbBehaviorGraphStringData" signature="0xc713064e">)")
                         != NOT_FOUND
                     || firstline.find(R"(class="hkbBehaviorGraphData" signature="0x95aca5d">)")
                            != NOT_FOUND
                     || firstline.find(R"(class="hkbVariableValueSet" signature="0x27812d8d">)") != NOT_FOUND)
            {
                container_ptr = &datalines;
            }
            else
            {
                container_ptr = &filelines;
            }

            size_t pos = node.first.find("$");

            if (pos != NOT_FOUND)
            {
                string modID = node.first.substr(1, pos - 1);

                if (OpeningMod != modID && isOpen[container_ptr])
                {
                    container_ptr->emplace_back(ns::Close());
                    isOpen[container_ptr] = false;
                }

                if (!isOpen[container_ptr])
                {
                    container_ptr->emplace_back(ns::ModCode(modID));
                    OpeningMod            = modID;
                    isOpen[container_ptr] = true;
                }
            }
            else if (isOpen[container_ptr])
            {
                container_ptr->emplace_back(ns::Close());
                isOpen[container_ptr] = false;
            }

            container_ptr->insert(container_ptr->end(), node.second.begin(), node.second.end());
        }

        for (auto& each : isOpen)
        {
            if (!each.second) continue;

            each.first->emplace_back(ns::Close());
        }

        if (!CreateFolder(compilingfolder)) continue;

        const string firstp   = "_1stperson";
        const wstring wfirstp = L"_1stperson";

        string lowerBehaviorFile = behavior.first;
        wstring filepath = compilingfolder + nemesis::transform_to<wstring>(lowerBehaviorFile) + L".txt";
        bool firstPerson = lowerBehaviorFile.find(firstp + "\\") == 0;

        if (firstPerson) sf::create_directory(compilingfolder + wfirstp + L"\\");

        FileWriter output(filepath);

        if (!output.is_open()) ErrorMessage(2009, output.GetFilePath());

        bool behaviorRef = false;
        string total     = nemesis::transform_to<string>(filepath) + "\n";

        const string hkxheader = "<?xml version=\"1.0\" encoding=\"ascii\"?>\n";
        const string hkxpackfile
            = R"(<hkpackfile classversion="8" contentsversion="hk_2010.2.0-r1" toplevelobject=")" + rootID
              + "\">\n\n";
        const string hkxsection = "	<hksection name=\"__data__\">\n\n";

        const string endhkxsection  = "	</hksection>\n\n";
        const string endhkxpackfile = "</hkpackfile>\n";

        const string xmlbhvclass = R"(class="hkbBehaviorReferenceGenerator" signature=")";

        const string xmlbhvname = R"(<hkparam name="behaviorName">)";
        const string xmlbhvfile = R"(<hkparam name="behaviorFilename">)";
        const string endparam   = "</hkparam>";
        const string hkxobj     = R"(<hkobject name=")";

        writeSave(output, hkxheader, total);
        writeSave(output, hkxpackfile, total);
        writeSave(output, hkxsection, total);

        for (auto& line : datalines)
        {
            writeSave(output, line + "\n", total);
        }

        string behaviorName;

        for (auto& line : filelines)
        {
            writeSave(output, line + "\n", total);
            size_t pos = line.find(hkxobj);

            if (line.find(xmlbhvclass, pos) != NOT_FOUND)
            {
                behaviorRef = true;
            }

            if (behaviorRef)
            {
                pos = line.find(xmlbhvname);

                if (pos == NOT_FOUND) continue;

                size_t nextpos = pos + xmlbhvname.length();
                behaviorName = line.substr(nextpos, line.find(endparam, nextpos) - nextpos);
                behaviorRef  = false;
            }
            else
            {
                pos = line.find(xmlbhvfile);

                if (pos == NOT_FOUND) continue;

                size_t nextpos = pos + xmlbhvfile.length();
                behaviorName = line.substr(nextpos, line.find(endparam, nextpos) - nextpos);
            }

            behaviorName = GetFileName(behaviorName);
            nemesis::to_lower(behaviorName);

            if (lowerBehaviorFile.find(firstp) != NOT_FOUND)
            {
                behaviorName = firstp + "\\" + behaviorName;
            }

            behaviorJoints[behaviorName].push_back(lowerBehaviorFile);
        }

        writeSave(output, endhkxsection, total);
        writeSave(output, endhkxpackfile, total);
        (firstPerson ? bigNum2 : bigNum) += CRC32Convert(total);
    }

    emit progressUp(); // 28
    behaviorJointsOutput();

    if (CreateFolder(compilingfolder))
    {
        wstring filepath  = compilingfolder + L"animationdatasinglefile.txt";
        const auto& total = animData.SaveTemplateAs(filepath);
        bigNum2 += CRC32Convert(total);
    }

    emit progressUp(); // 29

    if (CreateFolder(compilingfolder))
    {
        wstring filepath  = compilingfolder + L"animationsetdatasinglefile.txt";
        const auto& total = animSetData.SaveTemplateAs(filepath);
        bigNum2 += CRC32Convert(total);
    }

    emit progressUp(); // 30

    FileWriter lastmod("cache\\engine_update");

    if (!lastmod.is_open()) ErrorMessage(2009, "cache\\engine_update");

    lastmod << GetNemesisVersion() << "\n";
    engineVersion = to_string(bigNum % 10000) + "-" + to_string(bigNum2 % 10000);
    lastmod << engineVersion << "\n";

    for (auto& it : lastUpdate)
    {
        lastmod << it.first + L">>" + it.second + L"\n";
    }

    emit progressUp(); // 31
}

void UpdateFilesStart::newAnimUpdate(const string& sourcefolder, const string& curCode)
{
    if (!newAnimFunction) return;

    string folderpath = sourcefolder + curCode;
    sf::path codefile(folderpath);

    if (sf::is_directory(codefile))
    {
        VecStr behaviorlist;
        read_directory(folderpath, behaviorlist);

        for (auto& beh : behaviorlist)
        {
            string curfolderstr = folderpath + "\\" + beh;
            sf::path curfolder(curfolderstr);

            if (!sf::is_directory(curfolder)) continue;

            DebugLogging("New Animations extraction start (Folder: " + curfolderstr + ")");

            if (nemesis::iequals(beh, "animationdatasinglefile"))
            {
                VecStr characterlist;
                read_directory(curfolderstr, characterlist);

                for (auto& character : characterlist)
                {
                    sf::path characterfolder(curfolderstr + "\\" + character);

                    if (sf::is_directory(characterfolder))
                    {
                        if (!newAnimDataUpdateExt(curfolderstr + "\\" + character,
                                                  curCode,
                                                  character,
                                                  newAnimAddition,
                                                  lastUpdate))
                        {
                            newAnimFunction = false;
                            return;
                        }
                    }
                    else
                    {
                        string stemTemp = characterfolder.stem().string();

                        if (stemTemp == "$header$")
                        {
                            if (!animDataHeaderUpdate(
                                    curfolderstr + "\\" + character, curCode, animData, lastUpdate))
                            {
                                newAnimFunction = false;
                                return;
                            }
                        }
                        else if (nemesis::regex_match(stemTemp,
                                                      nemesis::regex("^\\$(?!" + curCode + ").+\\$(?:UC|)$")))
                        {
                            ErrorMessage(
                                3023, "$" + curCode + "$" + (stemTemp.find("$UC") != NOT_FOUND ? "UC" : ""));
                        }
                    }
                }
            }
            else if (nemesis::iequals(beh, "animationsetdatasinglefile"))
            {
                VecStr projectfilelist;
                read_directory(curfolderstr, projectfilelist);

                for (const auto& file : projectfilelist)
                {
                    string projfile = file;

                    if (!sf::is_directory(curfolderstr + "\\" + file) || file.find("~") == NOT_FOUND)
                    {
                        continue;
                    }

                    nemesis::to_lower(projfile);

                    while (projfile.find("~") != NOT_FOUND)
                    {
                        projfile.replace(projfile.find("~"), 1, "\\");
                    }

                    if (!newAnimDataSetUpdateExt(curfolderstr + "\\" + file,
                                                 curCode,
                                                 projfile + ".txt",
                                                 animSetData,
                                                 lastUpdate))
                    {
                        newAnimFunction = false;
                        return;
                    }
                }
            }
            else
            {
                if (nemesis::iequals(beh, "_1stperson")) ErrorMessage(6004, curfolderstr);

                if (!newAnimUpdateExt(folderpath,
                                      curCode,
                                      nemesis::to_lower_copy(beh),
                                      *newFile[nemesis::to_lower_copy(beh)],
                                      newAnimAddition,
                                      lastUpdate,
                                      behavior_map))
                {
                    newAnimFunction = false;
                    return;
                }
            }

            DebugLogging("New Animations extraction complete (Folder: " + curfolderstr + ")");
        }
    }
    else if (codefile.extension().string() == ".txt")
    {
        saveLastUpdate(nemesis::to_lower_copy(folderpath), lastUpdate);
        VecNstr storeline;

        if (!GetFileLines(folderpath, storeline))
        {
            newAnimFunction = false;
            return;
        }

#if MULTITHREADED_UPDATE
        Lockless lock(newAnimAdditionLock);
#endif
        newAnimAddition[nemesis::to_lower_copy(codefile.string())] = storeline;
    }
    else
    {
        ErrorMessage(2024, folderpath);
    }
}

void UpdateFilesStart::newAnimProcess(const string& sourcefolder)
{
    newAnimFunction = true;

    if (!CreateFolder(sourcefolder)) ErrorMessage(2010, sourcefolder);

    VecStr codelist;
    read_directory(sourcefolder, codelist);

#if MULTITHREADED_UPDATE
    nemesis::ThreadPool multiThreads;

    for (auto& curCode : codelist)
    {
        multiThreads.enqueue(&UpdateFilesStart::newAnimUpdate, this, sourcefolder, curCode);
    }

    multiThreads.join();
#else
    for (auto& curCode : codelist)
    {
        newAnimUpdate(sourcefolder, curCode);
    }
#endif

    DebugLogging("New Animations record complete");
    emit progressUp(); // 5
}

void UpdateFilesStart::milestoneStart(string directory)
{
    m_RunningThread = 1;
    UpdateReset();
    start_time   = chrono::steady_clock::now();
    namespace bf = sf;

    try
    {
        DebugLogging("Nemesis Behavior Version: v" + GetNemesisVersion());
    }
    catch (int)
    {
        DebugLogging("Nemesis Behavior Version: Failed to get tool version");
        return;
    }

    auto nemesisinfo = NemesisInfo::GetInstance();
    wstring curdir = QCoreApplication::applicationDirPath().toStdWString();
    wstring datapath = nemesisinfo->GetDataPath().wstring();
    replace(curdir.begin(), curdir.end(), L'/', L'\\');
    DebugLogging(L"Current Directory: " + curdir);
    DebugLogging(L"Data Directory: " + datapath);
    DebugLogging("Skyrim Special Edition: " + string(SSE ? "TRUE" : "FALSE"));
    filenum = 32;
    wstring path = datapath + L"meshes";

    DebugLogging("Detecting processes...");

    sf::create_directory(path);
    registeredFiles.clear();
    GetPathLoop(datapath + L"meshes\\", false);
    filenum += registeredFiles.size();

    DebugLogging("Process count: " + to_string(filenum));
    emit progressMax(filenum);
    connectProcess(this);

    if (error) throw nemesis::exception();

    emit progressUp(); // 1
}

void UpdateFilesStart::message(string input)
{
    emit incomingMessage(QString::fromStdString(input));
}

void UpdateFilesStart::message(wstring input)
{
    emit incomingMessage(QString::fromStdWString(input));
}

void UpdateFilesStart::unregisterProcess()
{
    running = false;

    if (!error)
    {
        RunScript("scripts\\update\\end\\");

        if (error)
        {
            wstring msg = TextBoxMessage(1008);
            interMsg(msg);
            DebugLogging(msg);
        }
        else
        {
            wstring msg;
            chrono::duration diff = chrono::steady_clock::now() - start_time;

#if MILLISECONDS
            size_t second = chrono::duration_cast<chrono::milliseconds>(diff).count();

            if (second > 1000)
            {
                string milli = to_string(second % 1000);

                while (milli.length() < 3)
                {
                    milli.insert(0, "0");
                }

                msg = TextBoxMessage(1007) + ": " + to_string(second / 1000) + "," + milli + " "
                      + TextBoxMessage(1011);
            }
            else
            {
                msg = TextBoxMessage(1007) + ": " + to_string(second) + " " + TextBoxMessage(1011);
            }
#else
            msg = TextBoxMessage(1007) + L": "
                  + to_wstring(chrono::duration_cast<chrono::seconds>(diff).count()) + L" "
                  + TextBoxMessage(1012);
#endif

            interMsg(msg);
            DebugLogging(msg);
            msg = TextBoxMessage(1017) + L": " + nemesis::transform_to<wstring>(engineVersion);
            interMsg(msg);
            DebugLogging(msg);
            emit disableLaunch(false);
        }
    }
    else
    {
        wstring msg = TextBoxMessage(1008);
        interMsg(msg);
        DebugLogging(msg);
    }

    DebugOutput();
    disconnectProcess();

    if (cmdline)
    {
        scoped_lock<mutex> lock(processlock);
        processdone = true;
        cv.notify_one();
    }
    else
    {
        emit disable(false);
        emit hide(true);
    }

    if (error) emit disableLaunch(true);

    m_RunningThread = 0;
    emit end();
}

void stateCheck(UMapStr2& parent,
                string parentID,
                string lowerbehaviorfile,
                string sID,
                UPtr<UMapStr2>& stateID,
                UPtr<UMapStr2>& n_stateID,
                VecStr children,
                string filename,
                string ID,
                string modcode,
                StateIDList& duplicatedStateList
#if MULTITHREADED_UPDATE
                ,
                std::atomic_flag& lockless
#endif
)
{
    for (auto& child : parent)
    {
        if (parentID != child.second) continue;

        if ((*stateID)[child.first] != sID) continue;

        for (string& match : children)
        {
            if (match != child.first) continue;

            if (n_stateID->find(match) == n_stateID->end()) continue;

            if (ID == match && (*n_stateID)[match] != sID) continue;

            return;
        }

        if (error) throw nemesis::exception();
    }

#if MULTITHREADED_UPDATE
    Lockless lock(lockless);
#endif
    duplicatedStateList[filename][ID][sID].insert(modcode);
    duplicatedStateList[filename][ID][sID].insert("Vanilla");
}

void writeSave(FileWriter& writer, const string& line, string& store)
{
    writer << line;
    store.append(line);
}

void writeSave(FileWriter& writer, const char* line, string& store)
{
    writer << line;
    store.append(line);
}
