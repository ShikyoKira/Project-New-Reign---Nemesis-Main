# Project-New-Reign---Nemesis-Main

How to use Nemesis for users:
1. If this is your first time running Nemesis, click Update Engine to install the latest engine version
2. After updating the engine, check the mod you are using in the box
3. Click Launch Nemesis Unlimited Behavior Engine to generate behavior files
4. Launch skyrim

Build requirement for developers:
- [Qt5](https://www.qt.io/download-open-source)
- [boost](https://www.boost.org/users/download/) 
- [Python-3.5.6](https://www.python.org/downloads/release/python-356/) - Prebuilt binary version [here](https://drive.google.com/open?id=1Uqq0iQhc0qsWAiz3EFzqMWxB27BG-M5T)

How to build Nemesis for developers:
1. Download and install Qt5
2. Download and install boost
3. Download Python 3.5.6
4. Open NemesisEngine.sln
5. Go to Project Properties -> C++ -> General -> Additional Include Directories, update the boost and python directories
6. Go to Project Properties -> Linker -> General -> Additional Library Directories, update the boost and python directories
7. Build
8. Test your build in "test environment"


Youtube tutorial on how to use Nemesis: https://www.youtube.com/watch?v=mug-KFBwJTo

Before you open an issue
1. research the issue, see if there is any similar issue has been reported
2. remove all mods except essential mods to duplicate the issues as well as SKSE and alternate start and try to recreate the issue under this setup
3. TUDM dodge not working? Are you using alternate start? if yes, get out of the cell
4. Test test test. Make sure it is consistent
5. Always provide necessary information including instruction on how to duplicate the issue. Report such as "it doesn't work" will be ignored
6. Duplicated issue will be closed
7. Be responsible to find out what + Nemesis = issue
8. Note that opening an issue is not equavilent to tech support ticket. We don't provide you tech support
9. CBE is buggy in itself
10. FNIS and Nemesis aren't compatible. Delete FNIS and the FNIS output to get your game to work
11. Is it about TUDM, PCEA or something else other than Nemesis? Then don't report it here.
12. Is anything broken for you? No, then ignore the warnings. Don't bother me with your "I got 1000 warnings" if everything is working fine
13. Movement issues (dodging in place, no forward movement...) it can be due to a Nemesis issue. Nemesis will not always generate a animationdatasinglefile.txt and will instead edit an existing file in your load order .This file contains the motion data. After running Nemesis, search for the most recent animationdatasinglefile.txt in your load order. It is located in the meshes folder of behavior mods (CGO, SkySA, TUDM...). Most of the time FNIS mods won't contain some but you must verify. Once you have found it, copy and paste it in the meshes folder of the Nemesis output folder". This issue can never be solved by Nemesis as file management is not done by Nemesis
14. Is the error message telling you to inform the "mod author"? If yes, then don't report to me, report it to the **MOD AUTHOR** not Nemesis' author!!!
15. Program not launching? Check out [this issue](https://github.com/ShikyoKira/Project-New-Reign---Nemesis-Main/issues/211) and follow what's recommended by users there. I don't use windows 10 so I can't replicate it
16. enquiry is not an issue. Google it or post on r/skyrimmods subreddit. Quite a number of people should be able to help answer your question