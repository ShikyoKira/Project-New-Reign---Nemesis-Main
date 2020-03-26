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