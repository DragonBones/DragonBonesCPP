# DragonBones Cocos2d-x Library
<p align="center">
[English](./Readme.md) 中文 
</p>
## [示例](./Demos/)

## How to use
1. Create a Cocos2d-x C++ project and import [Demos files](./Demos/) to override the project files.
2. Import the [DragonBones common source code](../DragonBones/src/) to project classes.
3. Import the [DragonBones Cocos2d-x source code](./src/) to project classes.
4. Import the [3rdParty source code](../3rdParty/) to project classes.
5. Build project and have fun.

## Notice
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
