# DragonBones Cocos2d-x Library

<p align="center">
<h2 align="center"><a href="./README.md">English</a>          中文版</h2>
</p>

## [示例](./Demos/)


## 使用方法
1. 创建一个Cococs2d-x C++项目， 然后复制[示例文件](./Demos/)中的Classes和Resources目录替换覆盖项目文件中的对应目录。
2. 复制[DragonBones common source code](../DragonBones/src/)中的DragonBones目录到项目的Classes目录下。
3. 复制[DragonBones Cocos2d-x source code](./src/)中的DragonBones目录替换覆盖到项目的Classes目录下。
4. 复制[3rdParty source code](../3rdParty/)中的rapidjson目录到项目的Classes目录下。
5. 然后运行就行了。

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
