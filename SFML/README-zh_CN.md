# DragonBones Cocos2d-x 库
[README in English](./README.md)
## [示例](./Demos/)
* [Hello DragonBones](./Demos/Classes/HelloDragonBones.h)

## 使用方法
1. 创建一个 Cocos2d-x C++ 项目。
2. 分别复制 [DragonBones 公共库源码](../DragonBones/src/)、[DragonBones Cocos2d-x 库源码](./src/)、[第三方库源码](../3rdParty/) 中的所有文件夹和文件到项目的 Classes 文件夹下。
3. 如果想运行示例，复制 [示例文件夹](./Demos/) 中的所有文件夹和文件覆盖项目中的文件夹和文件。
4. 编译并运行。

## 注意事项
* 如果是升级覆盖源码文件，可能会有文件夹或文件名的变更，需要注意下面几种情况：
    * 检查是否有多余的旧源码文件夹和文件残留而导致重定义，建议升级之前先删除所有的旧源码文件夹和文件。
    * 检查并更新编译配置文件中的所有编译路径。
    * 检查其他可能由于文件夹或文件名变更而导致的编译错误。
* 在 Xcode 中，添加文件时请执行以下操作：
    * Add Files To "project" > Options > Added folders: Create groups > Add to targets: "all"。
* [rapidjson/msinttypes](../3rdParty/rapidjson/msinttypes) 的 headers 只在 Microsoft Visual C++ 编译器时才应该被引用。
* Cocos2d-x 也包含 rapidjson，使用第三方库中的 rapidjson 或者 Cocos2d-x 内置的，两者不能同时使用。
* 如果编译器找不到 headers，添加 Classes 到项目 Header 的搜索路径。
* 确保项目结构如下：
```
Your project
    |-- Classes
        |-- rapidjson
            |-- ...
        |-- dragonBones
            |-- animation
            |-- armature
            |-- ...
            |-- cocos2dx
            |-- ...
    |-- Resources
        |-- ...
```