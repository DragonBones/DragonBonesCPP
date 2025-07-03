# DragonBones Android JNI/Compose 库

[English README](./README.md)

## [示例](./Demos/)

## 如何构建和运行

本项目已为 Android Studio 和 Gradle 构建系统配置完毕。

1.  **打开项目**: 在 Android Studio 中，打开 `DragonBonesCPP/AndroidCompose/Demos/Viewer/` 目录。
2.  **Gradle 同步**: 等待项目在打开时自动完成 Gradle 同步。
3.  **构建和运行**: 在配置菜单中选择 `app`，然后点击'运行'按钮。这将会编译 C++ 代码、构建应用，并将其部署到模拟器或安卓设备上。

与其它平台不同，您无需手动复制任何源代码文件。Gradle 构建系统已被配置为直接使用位于仓库根目录的 DragonBones C++ 核心库源代码。 