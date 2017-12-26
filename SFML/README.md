# DragonBones SFML Library

[中文 README](./README-zh_CN.md)

## [Demos](./Demos/)

## How to setup for Visual Studio

1. Create project in Visual Studio
1. Create solution *dragonBones*
    1. Copy content of `DragonBones/src` and `3rdParty` to main directory of this solution
    1. In **Property Pages/General** set **Configuration Type** to `Static library (.lib)`
    1. In **Property Pages/C/C++** set **Additional Include Directories** to `./.`
1. Create solution *dragonBonesSFML*
    1. Configure SFML like in [tutorial](https://www.sfml-dev.org/tutorials/2.4/start-vc.php)
    1. Copy content of `SFML/src` and `SFML/Demos` to main directory of this solution
    1. In **Property Pages/C/C++** add to **Additional Include Directories** `./.` and path to directory of *dragonBones* (`..\dragonBones`)
    1. Add Reference of solution **dragonBones** to this
1. Compile both solutions
