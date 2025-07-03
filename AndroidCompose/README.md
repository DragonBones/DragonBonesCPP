# DragonBones Android JNI/Compose Library

[中文 README](./README-zh_CN.md)

## [Demos](./Demos/)

## How to Build and Run

The project is configured for Android Studio and the Gradle build system.

1.  **Open Project**: In Android Studio, open the `DragonBonesCPP/AndroidCompose/Demos/Viewer/` directory.
2.  **Gradle Sync**: Allow the project to sync with Gradle. This happens automatically on project open.
3.  **Build and Run**: Select the `app` run configuration and press the 'Run' button. This will build the C++ code, the app, and deploy it to an emulator or device.

Unlike other platform setups, there is no need to manually copy source files. The Gradle build system is configured to use the central DragonBones C++ source files from the repository root directly.
