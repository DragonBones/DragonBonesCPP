DragonBones Cocos2d-x Library

Demos

How to use

Create a Cocos2d-x C++ project and import Demos files to override the project files.
Import the DragonBones common source code to project classes.
Import the DragonBones Cocos2d-x source code to project classes.
Import the 3rdParty source code to project classes.
Build project and have fun.
Notice

In Xcode: Add Files To "project" > Options > Added folders: Create groups > Add to targets: "all"
Use rapidjson/msinttypes headers only with Microsoft Visual C++ compilers.
Cocos2dx also include rapidjson, if you use the cocos2dx related json function, make sure only include rapidjson once time.
If compiler can not find headers, add Classes to project Header Search Paths, make sure project structure like this:
Classes (Include Path)
    |-- rapidjson
        |-- ...
    |-- dragonBones
        |-- ...
