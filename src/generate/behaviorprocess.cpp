#include "Global.h"

#include <QThread>

#include <QtCore/QCoreApplication.h>

#include <QtConcurrent/qtconcurrentrun.h>

#include "version.h"
#include "debuglog.h"
#include "nemesisinfo.h"
#include "externalscript.h"

#include "ui/Terminator.h"
#include "ui/MessageHandler.h"

#include "utilities/renew.h"
#include "utilities/threadpool.h"
#include "utilities/atomiclock.h"
#include "utilities/filechecker.h"
#include "utilities/animtemplate.h"

#include "hkx/hkxbehavior.h"

#include "generate/addanims.h"
#include "generate/hkxcompiler.h"
#include "generate/behaviorcheck.h"
#include "generate/installscripts.h"
#include "generate/papyruscompile.h"
#include "generate/behaviorprocess.h"
#include "generate/playerexclusive.h"
#include "generate/generator_utility.h"
#include "generate/behaviorsubprocess.h"
#include "generate/behaviorprocess_utility.h"

#include "generate/animation/registeranimation.h"
#include "generate/animation/singletemplate.h"

using namespace std;
namespace sf = filesystem;

extern bool processdone;
extern mutex processlock;
extern mutex cv2_m;
extern condition_variable cv2;
extern condition_variable cv;
extern Terminator* p_terminate;
extern VecWstr failedBehaviors;
extern atomic<int> m_RunningThread;
extern atomic<int> behaviorRun;
extern atomic<int> extraCore;

mutex anim_lock;
std::atomic_flag atomic_lock{};

extern VecWstr fileCheckMsg;

BehaviorStart::BehaviorStart()
{
}

BehaviorStart::~BehaviorStart()
{
    if (!cmdline && error) error = false;
}

void BehaviorStart::addBehaviorPick(NemesisEngine* newWidget,
                                    VecStr behaviorOrder,
                                    UMap<string, bool> behaviorPick)
{
    VecStr* bp                         = (VecStr*) &behaviorPriority;
    *bp                                = behaviorOrder;
    UMap<string, bool>* bpick = (UMap<string, bool>*) &chosenBehavior;
    *bpick                             = behaviorPick;
    behaviorProcess.newWidget(newWidget);
}

void BehaviorStart::addBehaviorPick(VecStr behaviorOrder, UMap<string, bool> behaviorPick)
{
    VecStr* bp                         = (VecStr*) &behaviorPriority;
    *bp                                = behaviorOrder;
    UMap<string, bool>* bpick = (UMap<string, bool>*) &chosenBehavior;
    *bpick                             = behaviorPick;
    cmdline                            = true;
}

void BehaviorStart::message(string input)
{
    emit incomingMessage(QString::fromStdString(input));
}

void BehaviorStart::message(wstring input)
{
    emit incomingMessage(QString::fromStdWString(input));
}

void BehaviorStart::InitializeGeneration()
{
    UPtr<std::thread> checkThread;
    SetTimeout(NemesisInfo::GetInstance()->GetTimeout());

    try
    {
        extraCore = 0;
        ClearGlobal();
        milestoneStart();
        string version;

        if (!isEngineUpdated(version))
        {
            interMsg(TextBoxMessage(1000));
            m_RunningThread = 0;
            unregisterProcess(true);
            emit disableLaunch();
            return;
        }

        interMsg(TextBoxMessage(1003));
        interMsg(TextBoxMessage(1017) + L": " + nemesis::transform_to<wstring>(version));
        interMsg("");
        interMsg(TextBoxMessage(1004));
        interMsg("");

        if (error) throw nemesis::exception();

        // Check the existence of required files
        FileCheck();

        // Script Run
        RunScript(R"(scripts\launcher\start\)");
        ClearGlobal();
        characterHKX();
        GetBehaviorPath();
        GetBehaviorProject();
        GetBehaviorProjectPath();
        behaviorActivateMod(behaviorPriority);
        ClearTempXml();

        if (error) throw nemesis::exception();

        GenerateBehavior(checkThread);
    }
    catch (exception& ex)
    {
        try
        {
            ErrorMessage(6002, "None", ex.what());
        }
        catch (nemesis::exception&)
        {
            // resolved exception
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
            ErrorMessage(6002, "None", "Unknown");
        }
        catch (nemesis::exception&)
        {
            // resolved exception
        }
    }

    if (checkThread)
    {
        try
        {
            if (checkThread->joinable()) checkThread->join();
        }
        catch (exception)
        {}
    }

    EndAttempt();
}

void BehaviorStart::GenerateBehavior(UPtr<std::thread>& checkThread)
{
    GenerateAnimTemplate();
    GetBehaviorList();
    LinkTemplateToBehaviors();
    GenerateQueryList();
    AddQueriesToBehaviors();
    ParseBehaviorList();
    ExportBehaviorList();
}

void BehaviorStart::GenerateAnimTemplate()
{
    animtemp = make_unique<nemesis::AnimTemplate>("behavior templates");

    if (error) throw nemesis::exception();
}

void BehaviorStart::GetBehaviorList()
{
    int counter = 0;
    adsf        = std::make_unique<nemesis::animdata::SingleFile>();

    for (auto& each : behaviorPath)
    {
        if (nemesis::iequals(each.first, L"animationdatasinglefile")) continue;

        if (nemesis::iequals(each.first, L"animationsetdatasinglefile")) continue;

        //if (!nemesis::iequals(each.first, L"0_master") && !nemesis::iequals(each.first, L"behavior"))
        //    continue;

        sf::path path    = L"temp_behaviors\\" + nemesis::to_lower_copy(each.first) + L".txt";
        auto behaviorptr = nemesis::HkxBehavior::File(path);
        behaviorptr->SetAnimTemplate(animtemp.get());
        behaviorlist.emplace_back(behaviorptr);
        //BehaviorInitialize(behaviorptr);

        //if (counter++ == 4) break;
    }

    if (error) throw nemesis::exception();
}

void BehaviorStart::LinkTemplateToBehaviors()
{
    animtemp->LinkToBehaviorList(behaviorlist);
}

void BehaviorStart::ParseBehaviorList()
{
    nemesis::ThreadPool tp;

    for (auto& behaviorptr : behaviorlist)
    {
        //tp.enqueue(&BehaviorStart::BehaviorInitialize, this, behaviorptr.get());
    }

    tp.join_all();

    if (error) throw nemesis::exception();
}

void BehaviorStart::GenerateQueryList()
{
    USet<wstring> registered;

    for (auto& each : behaviorPath)
    {
        auto pathstr = nemesis::to_lower_copy(each.second);
        auto pos     = pathstr.rfind(L"\\behaviors\\");

        if (pos == NOT_FOUND) continue;

        sf::path animdir(nemesis::to_lower_copy(pathstr.substr(0, pos)) + L"\\animations");

        if (registered.find(animdir.wstring()) != registered.end()) continue;

        registered.insert(animdir);
        CheckQueryInAnimDir(animdir);
    }

    if (error) throw nemesis::exception();
}

void BehaviorStart::AddQueriesToBehaviors()
{
    for (auto& behavior : behaviorlist)
    {
        //behavior->AddQueries(querylist);
        behavior->AddQueries(*querymanager);
    }

    if (error) throw nemesis::exception();
}

void BehaviorStart::ExportBehaviorList()
{
    nemesis::ThreadPool tp;

    for (auto& behavior : behaviorlist)
    {
        auto lowerbehaviorname = nemesis::to_lower_copy(behavior->GetBehaviorName());

        if (behavior->GetType() == nemesis::File::FileType::NEMESIS_TEMPLATE)
        {
            //tp.enqueue(&nemesis::HkxBehavior::SaveAsTemplateBehavior, behavior);
            continue;
        }

        sf::path xmlpath = L"temp_behaviors\\xml"
                       + behaviorPath[lowerbehaviorname].substr(sf::current_path().string().length()) + L".txt";
        sf::create_directories(xmlpath.parent_path());
        sf::path hkxpath = behaviorPath[lowerbehaviorname] + L".hkx";
        //tp.enqueue(&nemesis::HkxBehavior::SaveAsHkx, behavior, xmlpath, hkxpath);
    }

    tp.join_all();
}

void BehaviorStart::BehaviorInitialize(nemesis::HkxBehavior* behaviorptr)
{
    try
    {
        behaviorptr->ParseBehavior(behaviorPriority);
    }
    catch (const nemesis::exception&)
    {
    }
    catch (const std::exception& ex)
    {
        ErrorMessage(6002, "None", ex.what());
    }
}

void BehaviorStart::CheckQueryInAnimDir(const std::filesystem::path& animdir)
{
    VecWstr contents;
    read_directory(animdir, contents);

    for (auto& item : contents)
    {
        sf::path itempath(animdir.wstring() + L"\\" + item);

        if (!sf::is_directory(itempath)) continue;

        AddModQueryList(itempath);
    }
}

void BehaviorStart::AddModQueryList(const std::filesystem::path& moddir)
{
    VecWstr contents;
    read_directory(moddir, contents);

    for (auto& item : contents)
    {
        sf::path itempath(moddir.wstring() + L"\\" + item);

        if (sf::is_directory(itempath)) continue;

        if (nemesis::AnimQueryFile::GetListFileType(itempath) == nemesis::File::FileType::NONE) continue;

        //querylist.emplace_back(animtemp->ReadListFile(itempath));
        querymanager->AddFile(animtemp->ReadListFile(itempath));
    }
}

void BehaviorStart::SetTimeout(size_t ms)
{
    QtConcurrent::run([&]() {
        WPtr<bool> running_wptr       = running_ptr;
        WPtr<size_t> timeout_timer_wptr = timeout_timer_ptr;

        while (!running_wptr.expired() && *running_wptr.lock())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (timeout_timer_wptr.expired()) return;

            // timeout after 5mins
            if (++(*timeout_timer_wptr.lock()) < ms) continue;

            error = true;
            emit criticalError("TIMEOUT", "Behavior process timeout");
        }
    });
}

atomic_flag& BehaviorStart::getNewAnimFlag()
{
    return newAnimFlag;
}

void BehaviorStart::milestoneStart()
{
    m_RunningThread = 1;
    failedBehaviors.clear();
    PatchReset();
    start_time = std::chrono::high_resolution_clock::now();

    DebugLogging("Nemesis Behavior Version: v" + GetNemesisVersion());
    wstring curdir = QCoreApplication::applicationDirPath().toStdWString();
    replace(curdir.begin(), curdir.end(), L'/', L'\\');
    DebugLogging(L"Current Directory: " + curdir);
    DebugLogging(L"Data Directory: " + NemesisInfo::GetInstance()->GetDataPath().wstring());
    DebugLogging("Skyrim Special Edition: " + string(SSE ? "TRUE" : "FALSE"));
    int counter = 0;

    connectProcess(this);
    wstring directory   = getTempBhvrPath().wstring();
    wstring fpdirectory = directory + L"\\_1stperson";
    VecWstr filelist;
    int include = 0;
    int add     = 5;

    if (!isFileExist(directory))
    {
        sf::create_directories(sf::path(directory));
        ErrorMessage(6006);
    }

    DebugLogging("Detecting processes...");
    read_directory(directory, filelist);

    if (isFileExist(directory + L"\\animationdatasinglefile.txt")) --add;

    if (isFileExist(directory + L"\\animationsetdatasinglefile.txt")) --add;

    if (isFileExist(fpdirectory) && sf::is_directory(fpdirectory))
    {
        VecWstr fpfilelist;
        read_directory(fpdirectory, fpfilelist);

        for (auto& file : fpfilelist)
        {
            if (!sf::is_directory(fpdirectory + L"\\" + file) && file.find(L".txt") == file.length() - 4) ++include;
        }
    }

    for (auto& file : filelist)
    {
        if (!sf::is_directory(directory + L"\\" + file) && file.find(L".txt") == file.length() - 4) ++include;
    }

    filenum = (include * 10) + add;
}

void BehaviorStart::unregisterProcess(bool skip)
{
    (*running_ptr) = false;

    if (!skip)
    {
        if (!error)
        {
            bool hasScripts = false;
            RunScript("scripts\\launcher\\end\\", hasScripts);

            if (hasScripts) interMsg("");

            if (error)
            {
                wstring msg = TextBoxMessage(1010);
                interMsg(msg);
                DebugLogging(msg);
            }
            else
            {
                wstring msg;
                auto diff = std::chrono::high_resolution_clock::now() - start_time;
                int seconds = std::chrono::duration_cast<std::chrono::seconds>(diff).count();

                DebugLogging("Number of animations: " + to_string(animCount));
                msg = TextBoxMessage(1009) + L": " + to_wstring(seconds) + L" " + TextBoxMessage(1012);

                interMsg(msg);
                DebugLogging(msg);
                emit disableCheck(false);
            }
        }
        else
        {
            wstring msg = TextBoxMessage(1010);
            interMsg(msg);
            DebugLogging(msg);
        }
    }

    DebugOutput();
    disconnectProcess();

    if (cmdline)
    {
        lock_guard<mutex> lock(processlock);
        processdone = true;
        cv.notify_one();
    }
    else
    {
        emit disable(false);
        emit hide(true);
    }

    emit end();
}

void BehaviorStart::EndAttempt()
{
    Lockless lock(atomic_lock);

    --m_RunningThread;

    if (m_RunningThread != 0) return;

    checkAllStoredHKX();
    newMilestone();

    try
    {
        for (size_t i = 0; i < failedBehaviors.size(); i += 2)
        {
            HkxCompiler::hkxcmdProcess(failedBehaviors[i], failedBehaviors[i + 1], true);
            DebugLogging(L"Processing behavior: " + failedBehaviors[i]
                         + L" (Check point #, Behavior compile complete)");
            emit progressUp();
        }

        for (auto& msg : fileCheckMsg)
        {
            interMsg(msg);
        }

        failedBehaviors.clear();
        behaviorCheck(this);

        if (isFileExist(papyrusTempCompile())) sf::remove_all(papyrusTempCompile());

        emit progressUp();
    }
    catch (nemesis::exception&)
    {
    }
    catch (...)
    {
    }

    if (error)
    {
        ClearGlobal();
    }
    else
    {
        ClearGlobal(false);

        if (!cmdline) std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    try
    {
        unregisterProcess();
        p_terminate->exitSignal();
    }
    catch (...)
    {
    }
}

void BehaviorStart::increaseAnimCount()
{
    if (error) return;

    lock_guard<mutex> lock(anim_lock);
    ++animCount;
    emit totalAnim(animCount);
}

void BehaviorStart::newMilestone()
{
    Lockless lock(upFlag);

    if (error) return;

    emit progressUp();
}
