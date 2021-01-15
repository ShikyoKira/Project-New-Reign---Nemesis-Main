#include <mutex>

#include <QtCore/QCoreApplication.h>

#include <QtConcurrent/qtconcurrentrun.h>

#include "debuglog.h"
#include "externalscript.h"
#include "nemesisinfo.h"
#include "version.h"

#include "ui/Terminator.h"

#include "utilities/renew.h"
#include "utilities/lastupdate.h"
#include "utilities/stringextension.h"
#include "utilities/filechecker.h"
#include "utilities/conditionsyntax.h"
#if MULTITHREADED_UPDATE
#include "utilities/threadpool.h"
#endif

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
    arguPack(Map<string, UPtr<Map<string, VecStr, alphanum_less>>>& n_newFile,
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
    Map<string, UPtr<Map<string, VecStr, alphanum_less>>>& newFile;

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

UpdateFilesStart::UpdateFilesStart(const NemesisInfo* _ini)
{
    nemesisInfo = _ini;
}

UpdateFilesStart::~UpdateFilesStart()
{
    if (!cmdline && error) error = false;
}

void UpdateFilesStart::startUpdatingFile()
{
    string directory        = "mod\\";
    string newAnimDirectory = "behavior templates\\";

    QtConcurrent::run([&]() {
        while (running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ++timeout_timer;

            // timeout after 5mins
            if (timeout_timer == 3000)
            {
                error = true;
                emit criticalError("TIMEOUT", "Update process timeout");
                return;
            }
        }
    });

    // Separate try-catch for easier debugging purpose

    try
    {
        ClearGlobal();
        milestoneStart(directory);

        // Check the existence of required files
        if (!FileCheck(true))
        {
            ClearGlobal();
            return;
        }
    }
    catch (exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
    catch (nemesis::exception&)
    {
        // resolved exception
        return;
    }
    catch (...)
    {
        ErrorMessage(6001, "Unknown: Update Failed");
    }

    try
    {
        RunScript(R"(scripts\update\start\)");
        DebugLogging("External script run complete");

        // clear the temp_behaviors folder to prevent it from bloating
        ClearTempBehaviors(nemesisInfo);
        DebugLogging("Temp behavior clearance complete");

        // create "temp_behaviors" folder
        if (!isFileExist(directory)) sf::create_directory(directory);

        if (error)
        {
            ClearGlobal();
            return;
        }

        emit progressUp(); // 2
    }
    catch (exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
    catch (nemesis::exception&)
    {
        // resolved exception
        return;
    }
    catch (...)
    {
        ErrorMessage(6001, "Unknown: Update Failed");
    }

    try
    {
        // copy latest vanilla into memory
        if (!VanillaUpdate() || error)
        {
            ClearGlobal();
            return;
        }
    }
    catch (exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
    catch (nemesis::exception&)
    {
        // resolved exception
        return;
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

        return;
    }

    try
    {
        DebugLogging("Data record complete");
        emit progressUp(); // 4

        // check template for association with vanilla nodes from behavior template file
        newAnimProcess(newAnimDirectory);

        // comparing if different from mod file
        JoiningEdits(directory);

        if (error || !newAnimFunction)
        {
            ClearGlobal();
            return;
        }
    }
    catch (exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
    catch (nemesis::exception&)
    {
        // resolved exception
        return;
    }
    catch (...)
    {
        ErrorMessage(6001, "Unknown: Update Failed");
    }

    try
    {
        if (newAnimFunction && !error)
        {
            DebugLogging("Modification successfully extracted");
            emit progressUp(); // 27

            // compiling all behaviors in "data/meshes" to "temp_behaviors" folder
            CombiningFiles();

            emit progressUp(); // 32
        }
    }
    catch (exception& ex)
    {
        ErrorMessage(6001, ex.what());
    }
    catch (nemesis::exception&)
    {
        // resolved exception
        return;
    }
    catch (...)
    {
        ErrorMessage(6001, "Unknown: Update Failed");
    }
}

void UpdateFilesStart::UpdateFiles()
{
    try
    {
        try
        {
            startUpdatingFile();
            ClearGlobal();

            if (!cmdline) this_thread::sleep_for(chrono::milliseconds(1500));
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
        const string cachebpj = "cache\\behavior_project";

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

        if (!sf::is_directory(curfile))
        {
            if (nemesis::iequals(curfile.extension().string(), ".xml")
                || nemesis::iequals(curfile.extension().string(), ".txt"))
            {
                string curFileName = curfile.stem().string();

                if ((nemesis::iequals(curFileName, "nemesis_animationdatasinglefile")
                     || nemesis::iequals(curFileName, "nemesis_animationsetdatasinglefile"))
                    || (wordFind(curFileName, "Nemesis_") == 0
                        && wordFind(curFileName, "_List") != curFileName.length() - 5
                        && wordFind(curFileName, "_Project") != curFileName.length() - 8)
                    || (wordFind(curFileName, "Nemesis_") == 0
                        && wordFind(curFileName, "_Project") + 8 == curFileName.length()))
                {
                    ++filenum;
                }
            }
        }
        else
        {
            GetFileLoop(newPath + "\\");
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
        else if (wordFind(curFileName, L"Nemesis_") == 0
                 && wordFind(curFileName, L"_List") != curFileName.length() - 5
                 && wordFind(curFileName, L"_Project") != curFileName.length() - 8)
        {
            wstring firstperson = L"";

            if (curBehavior->isFirstPerson) firstperson = L"_1stperson\\";

            curFileName = firstperson + curFileName.substr(8);
            nemesis::to_lower(curFileName);
            const string curFileNameA = nemesis::transform_to<string>(curFileName);
            DebugLogging(L"Behavior Disassemble start (File: " + newPath + L")");

            {
#if MULTITHREADED_UPDATE
                Lockless lock(behaviorPathLock);
#endif
                behaviorPath[curFileName]
                    = nemesis::to_lower_copy(curBehavior->file.parent_path().wstring() + L"\\"
                                             + curBehavior->file.stem().wstring().substr(8));
            }

            UPtr<Map<string, VecStr, alphanum_less>> _curNewFile
                = make_unique<Map<string, VecStr, alphanum_less>>();
            UPtr<Map<string, UMap<string, bool>>> _childrenState
                = make_unique<Map<string, UMap<string, bool>>>();
            UPtr<UMapStr2> _stateID = make_unique<UMapStr2>();
            UPtr<UMapStr2> _parent  = make_unique<UMapStr2>();

            VanillaDisassemble(newPath, _curNewFile, _childrenState, _stateID, _parent);

            {
#if MULTITHREADED_UPDATE
                Lockless lock(newFileLock);
#endif
                newFile[curFileNameA]       = move(_curNewFile);
                childrenState[curFileNameA] = move(_childrenState);
                stateID[curFileNameA]       = move(_stateID);
                parent[curFileNameA]        = move(_parent);
            }

            saveLastUpdate(nemesis::to_lower_copy(newPath), lastUpdate);

            DebugLogging(L"Behavior Disassemble complete (File: " + newPath + L")");
            emit progressUp();

            if (nemesis::to_lower_copy(curBehavior->file.parent_path().filename().wstring()).find(L"characters")
                == 0)
            {
                registeredAnim[nemesis::to_lower_copy(curFileNameA)] = SetStr();
            }
        }
        else if (wordFind(curFileName, L"Nemesis_") == 0
                 && wordFind(curFileName, L"_Project") + 8 == curFileName.length())
        {
            wstring firstperson = L"";

            if (curBehavior->isFirstPerson) firstperson = L"_1stperson\\";

            wstring curPath = nemesis::to_lower_copy(curBehavior->file.parent_path().wstring());
            curPath        = curPath.substr(curPath.find(L"\\meshes\\") + 1);
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

            if (!GetFunctionLines(newPath, storeline)) return;

            for (unsigned int j = 0; j < storeline.size(); ++j)
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

                            if (nemesis::iequals(ext, "nemesis_animationdatasinglefile")
                                || nemesis::iequals(ext, "nemesis_animationsetdatasinglefile"))
                            {
#if MULTITHREADED_UPDATE
                                Lockless locker(queueLock);
#endif
                                registeredFiles.push_back(make_shared<RegisterQueue>(curfile, false));
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
                                          UPtr<Map<string, VecStr, alphanum_less>>& curNewFile,
                                          UPtr<Map<string, UMap<string, bool>>>& childrenState,
                                          UPtr<UMapStr2>& stateID,
                                          UPtr<UMapStr2>& parent)
{
    VecStr storeline;
    storeline.reserve(2000);

    FileReader vanillafile(path);
    string curID;

    UMap<string, VecStr> statelist; // parent ID, list of children

    if (vanillafile.GetFile())
    {
        bool skip  = true;
        bool start = false;
        bool isSM  = false;
        string curline;

        while (vanillafile.GetLines(curline))
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
                                if (each != '\t' && each != '.' && each != '-' && each != ' '
                                    && !isdigit(each))
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

                        if (pos != NOT_FOUND && curline.find("<hkparam name=\"", 0) != NOT_FOUND)
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
                            string(curline),
                            nemesis::regex(".*<hkparam name=\"stateId\">([0-9]+)</hkparam>.*"),
                            string("\\1"));

                        if (stateIDStr != curline)
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
                                    for (auto& it = nemesis::regex_iterator(
                                             curline, nemesis::regex("([0-9]+(\\.[0-9]+)?)"));
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

                                for (auto& it = nemesis::regex_iterator(curline, vector4);
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
                                    spaces + curline.substr(match.position() + match.str().length()));
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

                                for (auto& it = nemesis::regex_iterator(curline, vector4);
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
                        if (curline.find("<hkobject name=\"", 0) != NOT_FOUND
                            && curline.find("signature=\"", curline.find("<hkobject name=\"")) != NOT_FOUND)
                        {
                            if (curline.find(R"(class="hkbStateMachine" signature=")") != NOT_FOUND)
                                isSM = true;
                            else
                                isSM = false;

                            if (storeline.size() != 0 && curID.length() != 0)
                            {
                                if (curID == "#0340")
                                {
                                    curID = curID;
                                }

                                storeline.shrink_to_fit();
                                (*curNewFile)[curID] = storeline;
                                storeline.reserve(2000);
                                storeline.clear();
                            }

                            size_t pos = curline.find("<hkobject name=\"#") + 16;
                            curID      = curline.substr(
                                pos, curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos);
                        }

                        storeline.push_back(curline);
                    }
                }
            }
            else if (curline.find("<hkobject name=\"#") != NOT_FOUND)
            {
                skip       = false;
                size_t pos = curline.find("<hkobject name=\"#") + 16;
                curID      = curline.substr(pos,
                                       curline.find("\" class=\"", curline.find("<hkobject name=\"")) - pos);
                storeline.push_back(curline);
            }
        }

        if (storeline.size() != 0 && curID.length() != 0)
        {
            storeline.shrink_to_fit();
            (*curNewFile)[curID] = storeline;
        }
    }
    else
    {
        ErrorMessage(2000, path);
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
    VecStr storeline;
    UMap<string, int> projectNameCount;

    int projectcounter = 0;
    size_t totallines = 0;

    string project;
    string header;

    bool special = false;
    bool isInfo  = false;
    bool end     = false;
    bool out     = true;

    if (!GetFunctionLines(path, storeline)) return false;

    {
        string strnum = nemesis::regex_replace(
            string(storeline[0]), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

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

        for (unsigned int i = num; i < storeline.size();)
        {
            unsigned int size                      = stoi(storeline[i]) + i;
            SPtr<AnimDataProject_Condt> proj = animData.projectlist[projectcounter++].raw->second;

            // start project header
            VecStr templine;
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
    catch (const exception& ex)
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

    if (!GetFunctionLines(path, storeline)) return false;

    {
        string strnum = nemesis::regex_replace(
            string(storeline[0]), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));

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

    if (node == "#0340.txt")
    {
        modlist = modlist;
    }

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
                                           lastUpdate,
                                           openAnim,
                                           openInfo);

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
                                       filepath + "\\$header",
                                       animData,
                                       true,
                                       lastUpdate,
                                       openAnim,
                                       openInfo);
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
                                           lastUpdate,
                                           openAnim,
                                           openInfo);

                            if (error) throw nemesis::exception();
                        }
                    }
                }
            }
            else if (nemesis::iequals(behavior, "animationsetdatasinglefile"))
            {
                MasterAnimSetData& animSetData = pack.at(modcode)->animSetData;

                if (!sf::is_directory(curPath)) continue;

                string project  = node + (nemesis::iequals(node, "$header$") ? "" : ".txt");

                while (project.find("~") != NOT_FOUND)
                {
                    project.replace(project.find("~"), 1, "\\");
                }

                string lowerproject = nemesis::to_lower_copy(project);

                if (lowerproject == "$header$") continue;

                VecStr uniquecodelist;
                read_directory(curPath.string(), uniquecodelist);

                bool newProject = false;
                auto& projptr = animSetData.find(lowerproject, modcode);

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

                    if (GetFunctionLines(headerPath, storeline, false))
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
            UPtr<Map<string, VecStr, alphanum_less>>& newFile(curpack->newFile[behavior]);
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
    try
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
        catch (exception& ex)
        {
            if (node.length() > 0 && behavior.length() > 0)
                DebugLogging("Failed to process file (Node: " + node + ", Behavior: " + behavior + ")");

            ErrorMessage(6001, ex.what());
        }
    }
    catch (nemesis::exception&)
    {
    }
}

void UpdateFilesStart::JoiningEdits(string directory)
{
    try
    {
        try
        {
            if (isFileExist(directory))
            {
                VecStr filelist;
                read_directory(directory, filelist);

                UMap<string, NodeU> modUpdate;
                UMap<string, VecStr> filelist2;
                SPtr<UpdateLock> modUpPtr = make_shared<UpdateLock>(modUpdate);

                atomic_flag parentLock{};
                UMap<string, SPtr<arguPack>> pack;
                vector<sf::path> pathlist;

                for (auto& modcode : filelist)
                {
                    if (!sf::is_directory(directory + modcode)) continue;

                    nemesis::to_lower(modcode);
                    read_directory(directory + modcode + "\\", filelist2[modcode]);
                    pack.insert(make_pair(modcode,
                                          make_shared<arguPack>(newFile,
                                                                parent,
                                                                animData,
                                                                animSetData,
                                                                modUpPtr
#if MULTITHREADED_UPDATE
                                                                ,
                                                                parentLock
#endif
                                                                )));
                    auto& curpack = pack[modcode];

                    for (auto& behavior : filelist2[modcode])
                    {
                        nemesis::to_lower(behavior);
                        string path = directory + modcode + "\\" + behavior;

                        if (!sf::is_directory(path)) continue;

                        path.append("\\");

                        if (behavior == "_1stperson")
                        {
                            VecStr fbehaviorlist;
                            read_directory(path, fbehaviorlist);

                            for (auto& fbehavior : fbehaviorlist)
                            {
                                nemesis::to_lower(fbehavior);
                                string fpath   = path + fbehavior + "\\";
                                string recName = behavior + "\\" + fbehavior;
                                read_directory(fpath, modFileList[modcode][recName]);
                                VecStr* curList = &modFileList[modcode][recName];

                                for (auto& node : *curList)
                                {
                                    if (sf::is_directory(fpath + node)) continue;

                                    nemesis::to_lower(node);
                                    modQueue[recName][node].push_back(modcode);
                                }

                                curpack->statelist.insert(
                                    make_pair(recName, make_unique<UMap<string, VecStr>>()));
                                curpack->n_stateID.insert(make_pair(recName, make_unique<UMapStr2>()));
                            }
                        }
                        else
                        {
                            read_directory(path, modFileList[modcode][behavior]);
                            VecStr* curList = &modFileList[modcode][behavior];

                            if (behavior != "animationdatasinglefile"
                                && behavior != "animationsetdatasinglefile")
                            {
                                for (auto& node : *curList)
                                {
                                    if (sf::is_directory(path + node)) continue;

                                    nemesis::to_lower(node);
                                    modQueue[behavior][node].push_back(modcode);
                                }

                                curpack->statelist.insert(
                                    make_pair(behavior, make_unique<UMap<string, VecStr>>()));
                                curpack->n_stateID.insert(make_pair(behavior, make_unique<UMapStr2>()));
                            }
                            else
                            {
                                for (auto& node : *curList)
                                {
                                    modQueue[behavior][node].push_back(modcode);
                                }
                            }
                        }
                    }
                }

                for (auto& behavior : modQueue)
                {
                    for (auto& node : behavior.second)
                    {
                        processQueue.emplace_back(behavior.first, node.first);
                    }
                }

                for (auto& modcode : modFileList)
                {
                    for (auto& behavior : modcode.second)
                    {
                        modFileCounter[modcode.first][behavior.first] = behavior.second.size();
                    }
                }

                queuing = 0;

                if (!processQueue.empty())
                {
                    // Using non-multi threading method due to heap corruption upon using solution below
#if MULTITHREADED_UPDATE
                    nemesis::ThreadPool multiThreads;

                    for (auto& each : processQueue)
                    {
                        multiThreads.enqueue(&UpdateFilesStart::SeparateMod, this, directory, each, std::ref(pack));
                    }

                    multiThreads.join();
#else
                    for (auto& each : processQueue)
                    {
                        SeparateMod(directory, each, pack);
                    }
#endif
                }
                else
                {
                    while (queuing < 20)
                    {
                        emit progressUp();
                        ++queuing;
                    }
                }

                if (error) throw nemesis::exception();

                emit progressUp(); // 26

                for (auto& duplicates : duplicatedStateList)
                {
                    for (auto& IDs : duplicates.second)
                    {
                        for (auto& modlist : IDs.second)
                        {
                            string mods;

                            for (auto& mod : modlist.second)
                            {
                                mods.append(mod + ", ");
                            }

                            mods.pop_back();
                            mods.pop_back();

                            if (mods.length() > 0) ErrorMessage(1189, duplicates.first, modlist.first, mods);
                        }
                    }
                }
            }
        }
        catch (nemesis::exception&)
        {
            // resolved exception
        }
    }
    catch (exception& ex)
    {
        try
        {
            ErrorMessage(6001, ex.what());
        }
        catch (nemesis::exception&)
        {
            // resolved exception
        }
    }
}

void UpdateFilesStart::CombiningFiles()
{
    VecStr fileline;
    wstring compilingfolder    = getTempBhvrPath(nemesisInfo).wstring() + L"\\";
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
        bool isOpen = false;
        string OpeningMod;
        VecStr fileline;
        fileline.reserve(behavior.second->size());

        for (auto& node : (*behavior.second)) // behavior node ID
        {
            size_t pos = node.first.find("$");

            if (pos != NOT_FOUND)
            {
                string modID = node.first.substr(1, pos - 1);

                if (OpeningMod != modID && isOpen)
                {
                    fileline.push_back(ns::Close());
                    isOpen = false;
                }

                if (!isOpen)
                {
                    fileline.push_back(ns::ModCode(modID));
                    OpeningMod = modID;
                    isOpen     = true;
                }
            }

            if (node.second.size() == 0) ErrorMessage(2008, behavior.first + " (" + node.first + ")");

            for (string& line : node.second)
            {
                if (line.find(R"(class="hkRootLevelContainer" signature="0x2772c11e">)", 0) != NOT_FOUND)
                {
                    rootID = "#"
                             + nemesis::regex_replace(
                                 string(line), nemesis::regex("[^0-9]*([0-9]+).*"), string("\\1"));
                }

                fileline.push_back(line);
            }
        }

        if (isOpen)
        {
            fileline.push_back(ns::Close());
            isOpen = false;
        }

        if (CreateFolder(compilingfolder))
        {
            const string firstp = "_1stperson";
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
                = R"(<hkpackfile classversion="8" contentsversion="hk_2010.2.0 - r1" toplevelobject=")"
                  + rootID + "\">\n\n";
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

            string behaviorName;

            for (auto& line : fileline)
            {
                writeSave(output, line + "\n", total);
                uint pos = line.find(hkxobj);

                if (line.find(xmlbhvclass, pos) != NOT_FOUND)
                {
                    behaviorRef = true;
                }

                if (behaviorRef)
                {
                    pos = line.find(xmlbhvname);

                    if (pos == NOT_FOUND) continue;

                    uint nextpos = pos + xmlbhvname.length();
                    behaviorName = line.substr(nextpos, line.find(endparam, nextpos) - nextpos);
                    behaviorRef  = false;
                }
                else
                {
                    pos = line.find(xmlbhvfile);

                    if (pos == NOT_FOUND) continue;

                    uint nextpos = pos + xmlbhvfile.length();
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
            fileline.clear();
            (firstPerson ? bigNum2 : bigNum) += CRC32Convert(total);
        }
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

void UpdateFilesStart::newAnimUpdate(string sourcefolder, string curCode)
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

            if (sf::is_directory(curfolder))
            {
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
                                                      animData,
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
                            else if (nemesis::regex_match(stemTemp, nemesis::regex("^\\$(?!" + curCode + ").+\\$(?:UC|)$")))
                            {
                                ErrorMessage(3023,
                                             "$" + curCode + "$"
                                                 + (stemTemp.find("$UC") != NOT_FOUND ? "UC" : ""));
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
                                                     newAnimAddition,
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
                                          lastUpdate))
                    {
                        newAnimFunction = false;
                        return;
                    }
                }

                DebugLogging("New Animations extraction complete (Folder: " + curfolderstr + ")");
            }
        }
    }
    else if (codefile.extension().string() == ".txt")
    {
        VecStr storeline;

        saveLastUpdate(nemesis::to_lower_copy(folderpath), lastUpdate);

        if (!GetFunctionLines(folderpath, storeline))
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

void UpdateFilesStart::newAnimProcess(string sourcefolder)
{
    newAnimFunction = true;

    if (CreateFolder(sourcefolder))
    {
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
    }
    else
    {
        ErrorMessage(2010, sourcefolder);
    }

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

    wstring curdir = QCoreApplication::applicationDirPath().toStdWString();
    replace(curdir.begin(), curdir.end(), L'/', L'\\');
    DebugLogging(L"Current Directory: " + curdir);
    DebugLogging(L"Data Directory: " + nemesisInfo->GetDataPath());
    DebugLogging("Skyrim Special Edition: " + string(SSE ? "TRUE" : "FALSE"));
    filenum = 32;
    wstring path = nemesisInfo->GetDataPath() + L"meshes";

    DebugLogging("Detecting processes...");

    if (!isFileExist(path)) sf::create_directory(path);

    queuing = 0;

    GetPathLoop(nemesisInfo->GetDataPath() + L"meshes\\", false);

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
    bool skip = false;

    for (auto& child : parent)
    {
        if (parentID == child.second)
        {
            if ((*stateID)[child.first] == sID)
            {
                for (string& match : children)
                {
                    if (match == child.first)
                    {
                        if (n_stateID->find(match) != n_stateID->end())
                        {
                            if (ID != match || ((*n_stateID)[match] == sID && ID == match))
                            {
                                skip = true;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (error) throw nemesis::exception();
    }

    if (!skip)
    {
#if MULTITHREADED_UPDATE
        Lockless lock(lockless);
#endif
        duplicatedStateList[filename][ID][sID].insert(modcode);
        duplicatedStateList[filename][ID][sID].insert("Vanilla");
    }
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
