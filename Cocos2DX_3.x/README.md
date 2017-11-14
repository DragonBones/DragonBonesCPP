# DragonBones Cocos2d-x Library
[中文 README](./README-zh_CN.md)
## [Demos](./Demos/)

## How to use
1. Create a Cocos2d-x C++ project.
2. Copy [DragonBones common source code](../DragonBones/src/), [Dragonbones Cocos2d-x source code](./src/), [3rdParty source code](../3rdParty/) all folders and files to the project's Classes folder.
3. If you want to run the demos, copy all folders and files in the [Demos](./Demos/) to overwrite the folders and files in the project.
4. Build project and have fun.

## Notice
* If you are upgrading the overwrite source file, there may be changes to the folder or file name, you need to pay attention to the following several situations:
    * Check for additional legacy source folders and file residues that cause redefinition and recommend that you delete all old source folders and files before upgrading.
    * Check and update all compilation paths in you compilation config file.
    * Check other compilation errors that may result from folders or files name change.
* In Xcode: Add Files To "project" > Options > Added folders: Create groups > Add to targets: "all"
* Use [rapidjson/msinttypes](../3rdParty/rapidjson/msinttypes) headers only with Microsoft Visual C++ compilers.
* Cocos2dx also include rapidjson, if you use the cocos2dx related json function, make sure only include rapidjson once time.
* If compiler can not find headers, add Classes to project Header Search Paths, make sure project structure like this:
```
Classes (Include Path)
    |-- rapidjson
        |-- ...
    |-- dragonBones
        |-- ...
```
