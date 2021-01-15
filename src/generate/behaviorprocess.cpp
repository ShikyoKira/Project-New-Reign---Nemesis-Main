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

void addOnInstall(string templine,
                  string& elementLine,
                  vector<VecStr>& groupAddOnElement,
                  ImportContainer& addOn,
                  UMap<string, UMap<string, VecStr>>& groupAddOn);

BehaviorStart::BehaviorStart(const NemesisInfo* _ini)
{
    nemesisInfo = _ini;
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

    QtConcurrent::run([&]() {
        while (running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ++timeout_timer;

            // timeout after 5mins
            if (timeout_timer == 3000)
            {
                error = true;
                emit criticalError("TIMEOUT", "Behavior process timeout");
            }
        }
    });

    try
    {
        extraCore = 0;
        ClearGlobal();
        milestoneStart();
        string version;

        if (!isEngineUpdated(version, nemesisInfo))
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
        ClearTempXml(nemesisInfo);

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
    GenerateQueryList();
    GetBehaviorList();
    //ExportBehaviorList();
    return;

    // register animation & organize AE n Var
    wstring directory = getTempBhvrPath(nemesisInfo).wstring() + L"\\";
    UMap<string, int>
        animationCount; // animation type counter; use to determine how many of the that type of animation have been installed
    SPtr<TemplateInfo> BehaviorTemplate = make_shared<TemplateInfo>(); // get animation type

    Vec<UPtr<registerAnimation>> animationList
        = move(openFile(BehaviorTemplate.get(), nemesisInfo)); // get anim list installed by mods
    UMap<string, Vec<SPtr<NewAnimation>>>
        newAnimation; // record each of the animation created from registerAnimation

    UMapSetStr newAnimEvent;    // template code, list of events
    UMapSetStr newAnimVariable; // template code, list of variables

    UMap<string, var> AnimVar;
    UMap<string, Vec<string>>
        modAnimBehavior; // behavior directory, list of behavior files; use to get behavior reference
    UMap<string, UMap<int, bool>>
        ignoreFunction; // behavior file, function ID, true/false; is the function part of animation template?

    if (error) throw nemesis::exception();

    if (PCEACheck(nemesisInfo)) ReadPCEA(nemesisInfo);

    checkThread = std::make_unique<std::thread>(checkAllFiles, nemesisInfo->GetDataPath() + L"meshes\\actors");

    if (!animReplaced.empty() || !animationList.empty()) interMsg("");

    DebugLogging("Registering new animations...");

    // read each animation list file'
    for (const auto & i : animationList)
    {
        set<string> coreRegistered; // list of registered core behavior
        string modID = i->modID;

        // read each animation in a group of the same type
        for (auto it = i->templateType.begin(); it != i->templateType.end();
             ++it)
        {
            if (error) throw nemesis::exception();

            string firstAE;
            string coreModName;
            string templatecode = it->first;
            DebugLogging("Registering " + templatecode + " animations");
            vector<shared_ptr<AnimationInfo>> animInfoGroup;
            shared_ptr<UMap<string, AnimTemplate>> animTemplate
                = make_shared<UMap<string, AnimTemplate>>(); // behavior file, template

            // project, header, template
            shared_ptr<UMap<string, UMap<string, AnimTemplate>>> animdataTemplate
                = make_shared<UMap<string, UMap<string, AnimTemplate>>>();

            // project, header, template
            shared_ptr<UMap<string, map<string, AnimTemplate, alphanum_less>>> asdTemplate
                = make_shared<UMap<string, map<string, AnimTemplate, alphanum_less>>>();

            UMap<string, VecStr>* functionlines = &BehaviorTemplate->behaviortemplate[templatecode];
            UMap<string, UMap<string, VecStr>>* animdatalines
                = &BehaviorTemplate->animdatatemplate[templatecode];
            UMap<string, map<string, VecStr, alphanum_less>>* asdlines
                = &BehaviorTemplate->asdtemplate[templatecode];
            newAnimation[templatecode].reserve(50 * memory);
            animationCount[templatecode]++;
            bool isGroup;
            bool ignoreGroup = BehaviorTemplate->optionlist[templatecode].ignoreGroup;

            // Create proxy new animation to link master behavior branch with sub-branch, which is used as a core
            if (BehaviorTemplate->coreTemplate[templatecode].length() > 0)
            {
                coreModName     = "Nemesis_" + modID + "_";
                wstring corepath = wstring(behaviorPath[nemesis::transform_to<wstring>(BehaviorTemplate->coreTemplate[templatecode])]);
                wstring corename = GetFileName(corepath);
                corepath        = corepath.substr(0, corepath.length() - corename.length());
                DebugLogging(L"Core behavior name: " + corename);
                DebugLogging(L"Core behavior destination: " + corepath);

                if (!isFileExist(corepath + L"nemesis_" + corename + L".txt")
                    && !isFileExist(corepath + L"nemesis_" + corename + L".xml"))
                {
                    WarningMessage(1004,
                                   BehaviorTemplate->optionlist[templatecode].coreBehavior,
                                   templatecode,
                                   corepath + L"nemesis_" + corename + L".txt");
                }
                else
                {
                    string corecode
                        = BehaviorTemplate->coreBehaviorCode[BehaviorTemplate->coreTemplate[templatecode]];

                    if (coreRegistered.find(corecode) == coreRegistered.end())
                    {
                        DebugLogging("Core Registration: " + corecode);
                        coreRegistered.insert(corecode);
                        UMap<string, VecStr>* c_functionlines
                            = &BehaviorTemplate->behaviortemplate[corecode];
                        shared_ptr<AnimationInfo> dummy = make_shared<AnimationInfo>();

                        if (isFileExist(corepath + L"FNIS_" + nemesis::transform_to<wstring>(modID) + L"_"
                                        + nemesis::transform_to<wstring>(
                                            BehaviorTemplate->optionlist[corecode].coreBehavior)
                                        + L".hkx"))

                        {
                            dummy->addFilename("FNIS_" + modID + "_"
                                               + BehaviorTemplate->optionlist[corecode].coreBehavior
                                               + ".hkx");
                        }
                        else
                        {
                            dummy->addFilename(
                                coreModName + BehaviorTemplate->optionlist[corecode].coreBehavior + ".hkx");
                            coreModList[nemesis::transform_to<wstring>(nemesis::to_lower_copy(
                                            BehaviorTemplate->optionlist[corecode].coreBehavior)
                                        + ".txt")]
                                .push_back(nemesis::transform_to<wstring>(coreModName));
                        }

                        dummy->ignoreGroup                 = false;
                        dummy->optionPickedCount[corecode] = 1;
                        animationCount[corecode]++;

                        if (BehaviorTemplate->optionlist[corecode].core)
                        {
                            newAnimation[corecode].emplace_back(
                                make_unique<NewAnimation>(make_shared<UMap<string, AnimTemplate>>(),
                                                          corecode,
                                                          *c_functionlines,
                                                          animationCount[corecode],
                                                          "",
                                                          *dummy));
                        }
                        else
                        {
                            // behavior file, template
                            shared_ptr<UMap<string, AnimTemplate>> c_animTemplate
                                = make_shared<UMap<string, AnimTemplate>>();

                            for (auto& func : *c_functionlines)
                            {
                                if ((*c_animTemplate)[func.first].size == 0)
                                {
                                    (*c_animTemplate)[func.first].hasDuration = false;
                                    (*c_animTemplate)[func.first].ExamineTemplate(
                                        templatecode,
                                        func.first,
                                        func.second,
                                        false,
                                        false,
                                        BehaviorTemplate->optionlist[corecode]);
                                }
                            }

                            newAnimation[corecode].emplace_back(
                                make_unique<NewAnimation>(c_animTemplate,
                                                          corecode,
                                                          UMap<string, VecStr>(),
                                                          animationCount[corecode],
                                                          "",
                                                          *dummy));
                        }

                        vector<shared_ptr<AnimationInfo>> animInfoGroup = {dummy};
                        newAnimation[corecode].back()->addGroupAnimInfo(animInfoGroup);
                        newAnimation[corecode].back()->setLastOrder(0);
                    }
                }
            }

            if (BehaviorTemplate->behaviortemplate.find(templatecode + "_group")
                    != BehaviorTemplate->behaviortemplate.end()
                && BehaviorTemplate->behaviortemplate[templatecode + "_group"].size() != 0)
            {
                animInfoGroup.reserve(100);
                isGroup = true;
            }
            else
            {
                animInfoGroup.reserve(50 * memory);
                isGroup = false;
            }

            // Behavior
            for (auto& func : *functionlines)
            {
                if ((*animTemplate)[func.first].size == 0)
                {
                    (*animTemplate)[func.first].ExamineTemplate(templatecode,
                                                                func.first,
                                                                func.second,
                                                                false,
                                                                false,
                                                                BehaviorTemplate->optionlist[templatecode]);
                }
            }

            for (auto& proj : *animdatalines)
            {
                for (auto& header : proj.second)
                {
                    if ((*animdataTemplate)[proj.first][header.first].size == 0)
                    {
                        (*animdataTemplate)[proj.first][header.first].ExamineTemplate(
                            templatecode,
                            "animationdatasinglefile.txt",
                            header.second,
                            false,
                            false,
                            BehaviorTemplate->optionlist[templatecode]);
                    }
                }
            }

            for (auto& proj : *asdlines)
            {
                for (auto& header : proj.second)
                {
                    if ((*asdTemplate)[proj.first][header.first].size == 0)
                    {
                        (*asdTemplate)[proj.first][header.first].ExamineTemplate(
                            templatecode,
                            "animationsetdatasinglefile.txt",
                            header.second,
                            false,
                            false,
                            BehaviorTemplate->optionlist[templatecode]);
                    }
                }
            }

            // read each line and generate lines categorized using template code (animation type)
            try
            {
                for (int j = 0; j < it->second; ++j)
                {
                    int order     = i->isMulti[templatecode][j];
                    int lastOrder = i->last[templatecode][j];

                    newAnimation[templatecode].emplace_back(
                        make_unique<NewAnimation>(animTemplate,
                                                  templatecode,
                                                  *functionlines,
                                                  animationCount[templatecode],
                                                  modID + "\\",
                                                  *i->animInfo[templatecode][j]));

                    newAnimation[templatecode].back()->addAnimData(*animdataTemplate);
                    newAnimation[templatecode].back()->addAnimSetData(*asdTemplate);

                    newAnimation[templatecode].back()->coreModID = coreModName;
                    VecStr tempEventID    = newAnimation[templatecode].back()->GetEventID();
                    VecStr tempVariableID = newAnimation[templatecode].back()->GetVariableID();
                    newAnimEvent[templatecode + coreModName].insert(tempEventID.begin(), tempEventID.end());
                    newAnimVariable[templatecode + coreModName].insert(tempVariableID.begin(),
                                                                       tempVariableID.end());

                    if (order != 0)
                    {
                        newAnimation[templatecode].back()->setOrder(order);
                    }
                    else
                    {
                        firstAE = newAnimation[templatecode].back()->mainAnimEvent;

                        if (!(BehaviorTemplate->optionlist[templatecode].eleEventGroupF).empty())
                        {
                            vector<VecStr>* elementList
                                = &BehaviorTemplate->optionlist[templatecode].eleEventGroupF;
                            vector<string>* elementListLine
                                = &BehaviorTemplate->optionlist[templatecode].eleEventGroupFLine;
                            ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
                            UMap<string, UMap<string, VecStr>> groupAddOn
                                = newAnimation[templatecode].back()->GetGroupAddition();

                            for (uint k = 0; k < elementList->size(); ++k)
                            {
                                VecStr* element    = &(*elementList)[k];
                                string elementLine = (*elementListLine)[k];
                                vector<VecStr> groupAddOnElement;
                                groupAddOnElement.reserve(memory / 10);

                                for (auto& templine : *element)
                                {
                                    if (nemesis::iequals(templine, "main_anim_event"))
                                    {
                                        templine = newAnimation[templatecode].back()->mainAnimEvent;
                                        elementLine.replace(elementLine.find("$$"), 2, templine);
                                    }
                                    else
                                    {
                                        addOnInstall(
                                            templine, elementLine, groupAddOnElement, addOn, groupAddOn);
                                    }
                                }

                                if (!groupAddOnElement.empty())
                                {
                                    VecStr animEvent = newAnimationElement(elementLine, groupAddOnElement, 0);

                                    for (auto& l : animEvent)
                                    {
                                        newAnimEvent[templatecode + coreModName].insert(l);
                                    }
                                }
                                else
                                {
                                    newAnimEvent[templatecode + coreModName].insert(elementLine);
                                }
                            }
                        }

                        if (!(BehaviorTemplate->optionlist[templatecode].eleVarGroupF).empty())
                        {
                            vector<VecStr>* elementList
                                = &BehaviorTemplate->optionlist[templatecode].eleVarGroupF;
                            vector<string>* elementListLine
                                = &BehaviorTemplate->optionlist[templatecode].eleVarGroupFLine;
                            ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
                            UMap<string, UMap<string, VecStr>> groupAddOn
                                = newAnimation[templatecode].back()->GetGroupAddition();

                            for (uint k = 0; k < elementList->size(); ++k)
                            {
                                VecStr* element    = &(*elementList)[k];
                                string elementLine = (*elementListLine)[k];
                                vector<VecStr> groupAddOnElement;
                                groupAddOnElement.reserve(memory / 10);

                                for (auto& templine : *element)
                                {
                                    bool isBreak    = false;

                                    for (auto& it : addOn)
                                    {
                                        for (auto iter = it.second.begin(); iter != it.second.end(); ++iter)
                                        {
                                            if (nemesis::iequals(templine,
                                                                 it.first + "[" + iter->first + "]"))
                                            {
                                                elementLine.replace(elementLine.find("$$"), 2, iter->second);
                                                isBreak = true;
                                                break;
                                            }
                                        }

                                        if (isBreak) break;
                                    }

                                    if (!isBreak)
                                    {
                                        bool isDone = false;

                                        for (auto& it : groupAddOn)
                                        {
                                            for (auto iter = it.second.begin(); iter != it.second.end();
                                                 ++iter)
                                            {
                                                if (nemesis::iequals(templine,
                                                                     it.first + "[" + iter->first + "]"))
                                                {
                                                    elementLine.replace(elementLine.find("$$"), 2, "##");
                                                    isDone = true;
                                                    break;
                                                }

                                                groupAddOnElement.push_back(iter->second);
                                            }

                                            if (isDone) break;
                                        }
                                    }
                                }

                                if (!groupAddOnElement.empty())
                                {
                                    VecStr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

                                    for (auto& var : animVar)
                                    {
                                        newAnimVariable[templatecode + coreModName].insert(var);
                                    }
                                }
                                else
                                {
                                    newAnimVariable[templatecode + coreModName].insert(elementLine);
                                }
                            }
                        }
                    }

                    newAnimation[templatecode].back()->setLastOrder(lastOrder);
                    animInfoGroup.push_back(move(i->animInfo[templatecode][j]));

                    if (!ignoreGroup) animationCount[templatecode]++;

                    if (lastOrder == order)
                    {
                        size_t animationSize = newAnimation[templatecode].size();
                        animInfoGroup.shrink_to_fit();

                        for (int k = 0; k < order + 1; ++k)
                        {
                            newAnimation[templatecode][animationSize - k - 1]->addGroupAnimInfo(
                                animInfoGroup);
                        }

                        animInfoGroup.clear();
                        animInfoGroup.reserve(100);

                        if (ignoreGroup)
                        {
                            animationCount[templatecode]++;
                        }
                        else
                        {
                            if (BehaviorTemplate->optionlist[templatecode].eleEventGroupL.size() != 0)
                            {
                                vector<VecStr>* elementList
                                    = &BehaviorTemplate->optionlist[templatecode].eleEventGroupL;
                                vector<string>* elementListLine
                                    = &BehaviorTemplate->optionlist[templatecode].eleEventGroupLLine;
                                ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
                                UMap<string, UMap<string, VecStr>> groupAddOn
                                    = newAnimation[templatecode].back()->GetGroupAddition();

                                for (uint k = 0; k < elementList->size(); ++k)
                                {
                                    VecStr* element    = &(*elementList)[k];
                                    string elementLine = (*elementListLine)[k];
                                    vector<VecStr> groupAddOnElement;
                                    groupAddOnElement.reserve(memory / 10);

                                    for (auto& templine : *element)
                                    {
                                        if (nemesis::iequals(templine, "main_anim_event"))
                                        {
                                            templine = newAnimation[templatecode].back()->mainAnimEvent;
                                            elementLine.replace(elementLine.find("$$"), 2, templine);
                                        }
                                        else
                                        {
                                            addOnInstall(
                                                templine, elementLine, groupAddOnElement, addOn, groupAddOn);
                                        }
                                    }

                                    if (groupAddOnElement.size() != 0)
                                    {
                                        VecStr animEvent
                                            = newAnimationElement(elementLine, groupAddOnElement, 0);

                                        for (auto& evnt : animEvent)
                                        {
                                            newAnimEvent[templatecode + coreModName].insert(evnt);
                                        }
                                    }
                                    else
                                    {
                                        newAnimEvent[templatecode + coreModName].insert(elementLine);
                                    }
                                }
                            }

                            if (BehaviorTemplate->optionlist[templatecode].eleVarGroupL.size() != 0)
                            {
                                vector<VecStr>* elementList
                                    = &BehaviorTemplate->optionlist[templatecode].eleVarGroupL;
                                vector<string>* elementListLine
                                    = &BehaviorTemplate->optionlist[templatecode].eleVarGroupLLine;
                                ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
                                UMap<string, UMap<string, VecStr>> groupAddOn
                                    = newAnimation[templatecode].back()->GetGroupAddition();

                                for (uint k = 0; k < elementList->size(); ++k)
                                {
                                    VecStr* element    = &(*elementList)[k];
                                    string elementLine = (*elementListLine)[k];
                                    vector<VecStr> groupAddOnElement;
                                    groupAddOnElement.reserve(memory / 10);

                                    for (auto& templine : *element)
                                    {
                                        addOnInstall(
                                            templine, elementLine, groupAddOnElement, addOn, groupAddOn);
                                    }

                                    if (!groupAddOnElement.empty())
                                    {
                                        VecStr animVar
                                            = newAnimationElement(elementLine, groupAddOnElement, 0);

                                        for (auto& var : animVar)
                                        {
                                            newAnimVariable[templatecode + coreModName].insert(var);
                                        }
                                    }
                                    else
                                    {
                                        newAnimVariable[templatecode + coreModName].insert(elementLine);
                                    }
                                }
                            }
                        }
                    }

                    if (BehaviorTemplate->optionlist[templatecode].eleEvent.size() != 0)
                    {
                        vector<VecStr>* elementList = &BehaviorTemplate->optionlist[templatecode].eleEvent;
                        vector<string>* elementListLine
                            = &BehaviorTemplate->optionlist[templatecode].eleEventLine;
                        ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
                        UMap<string, UMap<string, VecStr>> groupAddOn
                            = newAnimation[templatecode].back()->GetGroupAddition();

                        for (uint k = 0; k < elementList->size(); ++k)
                        {
                            VecStr* element    = &(*elementList)[k];
                            string elementLine = (*elementListLine)[k];
                            vector<VecStr> groupAddOnElement;
                            groupAddOnElement.reserve(memory / 10);

                            for (auto& templine : *element)
                            {
                                if (nemesis::iequals(templine, "main_anim_event"))
                                {
                                    templine = newAnimation[templatecode].back()->mainAnimEvent;
                                    elementLine.replace(elementLine.find("$$"), 2, templine);
                                }
                                else
                                {
                                    addOnInstall(templine, elementLine, groupAddOnElement, addOn, groupAddOn);
                                }
                            }

                            if (groupAddOnElement.size() != 0)
                            {
                                VecStr animEvent = newAnimationElement(elementLine, groupAddOnElement, 0);

                                for (auto& evnt : animEvent)
                                {
                                    newAnimEvent[templatecode + coreModName].insert(evnt);
                                }
                            }
                            else
                            {
                                newAnimEvent[templatecode + coreModName].insert(elementLine);
                            }
                        }
                    }

                    if (BehaviorTemplate->optionlist[templatecode].eleVar.size() != 0)
                    {
                        vector<VecStr>* elementList = &BehaviorTemplate->optionlist[templatecode].eleVar;
                        vector<string>* elementListLine
                            = &BehaviorTemplate->optionlist[templatecode].eleVarLine;
                        ImportContainer addOn = newAnimation[templatecode].back()->GetAddition();
                        UMap<string, UMap<string, VecStr>> groupAddOn
                            = newAnimation[templatecode].back()->GetGroupAddition();

                        for (uint k = 0; k < elementList->size(); ++k)
                        {
                            VecStr* element    = &(*elementList)[k];
                            string elementLine = (*elementListLine)[k];
                            vector<VecStr> groupAddOnElement;
                            groupAddOnElement.reserve(memory / 10);

                            for (auto& templine : *element)
                            {
                                if (nemesis::iequals(templine, "main_anim_event"))
                                {
                                    templine = newAnimation[templatecode].back()->mainAnimEvent;
                                    elementLine.replace(elementLine.find("$$"), 2, templine);
                                }
                                else
                                {
                                    addOnInstall(templine, elementLine, groupAddOnElement, addOn, groupAddOn);
                                }
                            }

                            if (groupAddOnElement.size() != 0)
                            {
                                VecStr animVar = newAnimationElement(elementLine, groupAddOnElement, 0);

                                for (auto& var : animVar)
                                {
                                    newAnimVariable[templatecode + coreModName].insert(var);
                                }
                            }
                            else
                            {
                                newAnimVariable[templatecode + coreModName].insert(elementLine);
                            }
                        }
                    }

                    for (auto& name : tempVariableID)
                    {
                        AnimVar[name] = i->AnimVar[name];
                    }
                }
            }
            catch (nemesis::exception)
            {
                ErrorMessage(6000);
            }

            newAnimation[templatecode].shrink_to_fit();

            if (error) throw nemesis::exception();
        }
    }

    if (error) throw nemesis::exception();

    {
        int multi = 0;

        for (auto& modlist : coreModList)
        {
            multi = int(modlist.second.size());
            --multi;
        }

        filenum += (multi * 10);
        DebugLogging("Process count: " + to_string(filenum));
        emit progressMax(filenum);
    }

    DebugLogging("New animations registration complete");
    emit progressUp();

    // Ignore function in master branch
    for (auto it = BehaviorTemplate->grouplist.begin(); it != BehaviorTemplate->grouplist.end(); ++it)
    {
        for (auto& templatecode : it->second)
        {
            // existing function
            for (uint k = 0; k < BehaviorTemplate->existingFunctionID[templatecode][it->first].size(); ++k)
            {
                ignoreFunction[it->first + ".txt"]
                              [BehaviorTemplate->existingFunctionID[templatecode][it->first][k]]
                    = true;
            }
        }
    }

    if (error) throw nemesis::exception();
    emit progressUp();

    if (pcealist.size() > 0 || alternateAnim.size() > 0)
    {
        InstallScripts* ScriptWorker = new InstallScripts(nemesisInfo); // install PCEA & AA script
        connect(ScriptWorker, SIGNAL(end()), this, SLOT(EndAttempt()));
        connect(ScriptWorker, SIGNAL(end()), ScriptWorker, SLOT(deleteLater()));
        QtConcurrent::run(ScriptWorker, &InstallScripts::Run);
    }

    {
        wstring filename(nemesisInfo->GetDataPath() + L"scripts\\FNIS.pex");
        sf::copy_file(
            sf::path("alternate animation\\FNBE.pex"), filename, sf::copy_options::overwrite_existing);
        FILE* f;
        _wfopen_s(&f, filename.c_str(), L"r+b");

        if (f)
        {
            std::fseek(f, 20, SEEK_SET);
            unsigned char charcode = 73;
            std::fwrite(&charcode, sizeof(charcode), 1, f);

            std::fseek(f, 21, SEEK_SET);
            charcode = 83;
            std::fwrite(&charcode, sizeof(charcode), 1, f);

            std::fseek(f, 45, SEEK_SET);
            charcode = 73;
            std::fwrite(&charcode, sizeof(charcode), 1, f);

            std::fseek(f, 46, SEEK_SET);
            charcode = 83;
            std::fwrite(&charcode, sizeof(charcode), 1, f);

            std::fseek(f, 495, SEEK_SET);
            charcode = 49;
            std::fwrite(&charcode, sizeof(charcode), 1, f);

            std::fclose(f);
        }
        else
        {
            ErrorMessage(3002, filename);
        }
    }

    if (error) throw nemesis::exception();

    VecWstr filelist;
    read_directory(directory, filelist);
    emit progressUp();
    behaviorRun = 1;

    for (auto file : filelist)
    {
        if (!sf::is_directory(directory + file))
        {
            wstring lowerFileName = nemesis::to_lower_copy(file);

            if (coreModList.find(lowerFileName) != coreModList.end())
            {
                int repeat = int(coreModList[lowerFileName].size()) - 1;
                filenum += repeat * 10;
            }
        }
        else if (wordFind(file, L"_1stperson") != NOT_FOUND)
        {
            VecWstr fpfilelist;
            read_directory(directory + file, fpfilelist);

            for (auto fpfile : fpfilelist)
            {
                wstring lowerFileName = nemesis::to_lower_copy(file + L"\\" + fpfile);

                if (coreModList.find(lowerFileName) != coreModList.end())
                {
                    int repeat = int(coreModList[lowerFileName].size()) - 1;
                    filenum += repeat * 10;
                }
            }
        }
    }

    nemesis::ThreadPool tp;
    vector<BehaviorSub*> behaviorSubList;

    try
    {
        for (auto file : filelist)
        {
            if (error) throw nemesis::exception();

            if (!sf::is_directory(directory + file))
            {
                string modID          = "";
                bool isCore           = false;
                int repeatcount       = 0;
                int repeat            = 1;
                wstring lowerFileName = nemesis::to_lower_copy(file);

                if (coreModList.find(lowerFileName) != coreModList.end())
                {
                    repeat = int(coreModList[lowerFileName].size());
                    isCore = true;
                }

                while (repeatcount < repeat)
                {
                    if (error) throw nemesis::exception();

                    if (isCore) modID = nemesis::transform_to<string>(coreModList[lowerFileName][repeatcount]);

                    bool skip            = false;
                    wstring tempfilename = lowerFileName.substr(0, lowerFileName.find_last_of(L"."));
                    wstring temppath     = behaviorPath[tempfilename];

                    if (temppath.length() != 0)
                    {
                        size_t nextpos = 0;
                        size_t lastpos = temppath.find_last_of(L"\\");

                        while (temppath.find(L"\\", nextpos) != lastpos)
                        {
                            nextpos = temppath.find(L"\\", nextpos) + 1;
                        }

                        temppath = temppath.substr(nextpos, lastpos - nextpos);
                    }

                    if (error) throw nemesis::exception();

                    BehaviorSub* worker = new BehaviorSub(nemesisInfo);
                    behaviorSubList.push_back(worker);

                    worker->addInfo(directory,
                                    nemesis::transform_to<string>(file),
                                    BehaviorTemplate,
                                    newAnimation,
                                    AnimVar,
                                    newAnimEvent,
                                    newAnimVariable,
                                    ignoreFunction,
                                    false,
                                    modID,
                                    this);

                    if (!cmdline)
                    {
                        behaviorProcess.newProcess(worker);
                        behaviorProcess.connect();
                    }

                    if (lowerFileName == L"animationdatasinglefile.txt")
                    {
                        tp.enqueue(&BehaviorSub::AnimDataCompilation, worker); // 9 progress ups
                    }
                    else if (lowerFileName == L"animationsetdatasinglefile.txt")
                    {
                        tp.enqueue(&BehaviorSub::ASDCompilation, worker);
                    }
                    else
                    {
                        if (temppath.find(L"characters") == 0) worker->isCharacter = true;

                        tp.enqueue(&BehaviorSub::BehaviorCompilation, worker);
                    }

                    ++repeatcount;
                }
            }
            else if (wordFind(file, L"_1stperson") != NOT_FOUND)
            {
                VecWstr fpfilelist;
                read_directory(directory + file, fpfilelist);

                for (auto& curfile : fpfilelist)
                {
                    curfile = file + L"\\" + curfile;
                }

                for (auto fpfile : fpfilelist)
                {
                    if (error) throw nemesis::exception();

                    if (!sf::is_directory(directory + fpfile))
                    {
                        string modID         = "";
                        bool isCore          = false;
                        int repeatcount      = 0;
                        int repeat           = 1;
                        wstring lowerFileName = nemesis::to_lower_copy(fpfile);

                        if (coreModList.find(lowerFileName) != coreModList.end())
                        {
                            repeat = int(coreModList[lowerFileName].size());
                            isCore = true;

                            if (repeat > 1) filenum += (repeat - 1) * 10;
                        }

                        while (repeatcount < repeat)
                        {
                            if (error) throw nemesis::exception();

                            if (isCore) modID = nemesis::transform_to<string>(coreModList[lowerFileName][repeatcount]);

                            bool skip           = false;
                            wstring tempfilename = lowerFileName.substr(0, lowerFileName.find_last_of(L"."));
                            wstring temppath     = behaviorPath[nemesis::transform_to<wstring>(tempfilename)];

                            if (temppath.length() != 0)
                            {
                                size_t nextpos = 0;
                                size_t lastpos = temppath.find_last_of(L"\\");

                                while (temppath.find(L"\\", nextpos) != lastpos)
                                {
                                    nextpos = temppath.find(L"\\", nextpos) + 1;
                                }

                                temppath = temppath.substr(nextpos, lastpos - nextpos);
                            }

                            BehaviorSub* worker = new BehaviorSub(nemesisInfo);
                            behaviorSubList.push_back(worker);

                            worker->addInfo(directory,
                                            nemesis::transform_to<string>(fpfile),
                                            BehaviorTemplate,
                                            newAnimation,
                                            AnimVar,
                                            newAnimEvent,
                                            newAnimVariable,
                                            ignoreFunction,
                                            false,
                                            modID,
                                            this);

                            if (!cmdline)
                            {
                                behaviorProcess.newProcess(worker);
                                behaviorProcess.connect();
                            }

                            if (lowerFileName == L"animationdatasinglefile.txt")
                            {
                                tp.enqueue(&BehaviorSub::AnimDataCompilation,
                                           worker); // 9 progress ups
                            }
                            else if (lowerFileName == L"animationsetdatasinglefile.txt")
                            {
                                tp.enqueue(&BehaviorSub::ASDCompilation, worker);
                            }
                            else
                            {
                                if (temppath.find(L"characters") == 0) worker->isCharacter = true;

                                tp.enqueue(&BehaviorSub::BehaviorCompilation, worker);
                            }

                            ++repeatcount;
                        }
                    }
                }
            }
        }
    }
    catch (exception& ex)
    {
        tp.join_all();

        for (auto& each : behaviorSubList)
        {
            delete each;
        }

        throw ex;
    }

    {
        lock_guard<mutex> lg(cv2_m);
        --behaviorRun;
    }

    cv2.notify_one();
    tp.join_all();

    for (auto& each : behaviorSubList)
    {
        delete each;
    }
}

void BehaviorStart::GenerateAnimTemplate()
{
    animtemp = make_unique<nemesis::AnimTemplate>("behavior templates");
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
}

void BehaviorStart::GetBehaviorList()
{
    int counter = 0;
    nemesis::HkxBehavior::AddFileLines();
    nemesis::ThreadPool tp;

    for (auto& each : behaviorPath)
    {
        if (nemesis::iequals(each.first, L"animationdatasinglefile")) continue;

        if (nemesis::iequals(each.first, L"animationsetdatasinglefile")) continue;

        sf::path path    = L"temp_behaviors\\" + nemesis::to_lower_copy(each.first) + L".txt";
        auto behaviorptr = nemesis::HkxBehavior::File(path);
        behaviorlist.emplace_back(behaviorptr);
        //tp.enqueue(&BehaviorStart::BehaviorInitialize, this, each.first, behaviorptr);
        BehaviorInitialize(each.first, behaviorptr);

        if (counter++ == 2) break;
    }

    tp.join_all();
}

void BehaviorStart::ExportBehaviorList()
{
    nemesis::ThreadPool tp;

    for (auto& behavior : behaviorlist)
    {
        auto lowerbehaviorname = nemesis::to_lower_copy(behavior->GetBehaviorName());
        tp.enqueue(&nemesis::HkxBehavior::SaveAsHkx, behavior, behaviorPath[lowerbehaviorname]);
    }
}

void BehaviorStart::BehaviorInitialize(const std::wstring& behaviorfile,
                                       SPtr<nemesis::HkxBehavior>& behaviorptr)
{
    try
    {
        auto templatelist = animtemp->GetBehaviorTemplateList(behaviorfile);
        behaviorptr->ReadAndProcess(behaviorPriority);
        behaviorptr->AddQueries(querylist);
        behaviorptr->AddTemplateList(templatelist);
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

        TryAddModQueryList(itempath);
    }
}

void BehaviorStart::TryAddModQueryList(const std::filesystem::path& moddir)
{
    VecWstr contents;
    read_directory(moddir, contents);

    for (auto& item : contents)
    {
        sf::path itempath(moddir.wstring() + L"\\" + item);

        if (sf::is_directory(itempath)) continue;

        if (nemesis::AnimQueryList::GetListFileType(itempath) == nemesis::AnimQueryList::NONE) continue;

        querylist.emplace_back(animtemp->ReadListFile(itempath));
    }
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
    DebugLogging(L"Data Directory: " + nemesisInfo->GetDataPath());
    DebugLogging("Skyrim Special Edition: " + string(SSE ? "TRUE" : "FALSE"));
    int counter = 0;

    connectProcess(this);
    wstring directory   = getTempBhvrPath(nemesisInfo).wstring();
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
    running = false;

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

    if (m_RunningThread == 0)
    {
        checkAllStoredHKX();
        newMilestone();

        try
        {
            for (int i = 0; i < failedBehaviors.size(); i += 2)
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
        {}
        catch (...)
        {}

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
        {}
    }
}

void BehaviorStart::increaseAnimCount()
{
    if (!error)
    {
        lock_guard<mutex> lock(anim_lock);
        ++animCount;
        emit totalAnim(animCount);
    }
}

void BehaviorStart::newMilestone()
{
    Lockless lock(upFlag);

    if (!error)
    {
        emit progressUp();
    }
}

void addOnInstall(string templine,
                  string& elementLine,
                  vector<VecStr>& groupAddOnElement,
                  ImportContainer& addOn,
                  UMap<string, UMap<string, VecStr>>& groupAddOn)
{
    for (auto it = addOn.begin(); it != addOn.end(); ++it)
    {
        for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
        {
            if (nemesis::iequals(templine, it->first + "[" + iter->first + "]"))
            {
                elementLine.replace(elementLine.find("$$"), 2, iter->second);
                return;
            }
        }
    }

    for (auto it = groupAddOn.begin(); it != groupAddOn.end(); ++it)
    {
        for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
        {
            if (nemesis::iequals(templine, it->first + "[" + iter->first + "]"))
            {
                elementLine.replace(elementLine.find("$$"), 2, "##");
                groupAddOnElement.push_back(iter->second);
                return;
            }

            groupAddOnElement.push_back(iter->second);
        }
    }
}
