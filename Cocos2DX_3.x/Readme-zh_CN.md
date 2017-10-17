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

## 注意事项
* 在Xcode中，如果运行出错执行以下操作: Add Files To "project" > Options > Added folders: Create groups > Add to targets: "all"。
* [rapidjson/msinttypes](../3rdParty/rapidjson/msinttypes) 的 headers 只在 Microsoft Visual C++ 编译器中有效。
* Cocos2dx 也包含 rapidjson，使用3rdParty中的或者Cocos2dx的，两者不能同时使用。
* 如果编译器找不到 headers，添加 Classes 到项目 Header 的搜索路径, 确保项目结构如下：
```
Classes (Include Path)
    |-- rapidjson
        |-- ...
    |-- dragonBones
        |-- ...
```
