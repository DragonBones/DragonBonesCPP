# DragonBones Android JNI/Compose 库

[English README](./README.md)

本库提供了一套 Jetpack Compose 组件，用于在 Android 应用中高性能地渲染 DragonBones 骨骼动画。它底层使用 C++ 和 OpenGL ES 进行绘制，以确保流畅的动画性能，同时提供简洁的 Compose API 方便集成。

## 特性

*   **高性能渲染**: 基于 DragonBones C++ 核心库和 OpenGL ES，提供接近原生的渲染性能。
*   **Jetpack Compose 集成**: 以 Composable 函数的形式无缝集成到您的 Compose UI 中。
*   **响应式 API**: 使用 `State` 和协程，以声明式的方式控制动画。
*   **动态控制**: 支持播放、停止、淡入淡出、切换动画。
*   **模型管理**: 支持在运行时动态加载和切换不同的 DragonBones 模型。
*   **交互支持**: 支持获取点击的插槽（slot）名称。

## 如何构建和运行示例

示例应用位于 `Demos/DragonBonesAndroid` 目录下。

1.  **打开项目**: 在 Android Studio 中，打开 `DragonBonesCPP/AndroidCompose/Demos/DragonBonesAndroid/` 目录。
2.  **Gradle 同步**: 等待项目在打开时自动完成 Gradle 同步。
3.  **构建和运行**: 在配置菜单中选择 `app`，然后点击'运行'按钮。这将会编译 C++ 代码、构建应用，并将其部署到模拟器或安卓设备上。

与其它平台不同，您无需手动复制任何源代码文件。Gradle 构建系统已被配置为直接使用位于仓库根目录的 DragonBones C++ 核心库源代码。

## 如何在你的项目中使用

### 1. 添加依赖

(此部分假设库已作为模块发布或本地模块依赖)

确保你的项目 `build.gradle` 文件中包含了对 `dragonBones` 模块的引用。

```groovy
dependencies {
    implementation(project(":dragonBones"))
    // ... other dependencies
}
```

### 2. 添加资源文件

将你的 DragonBones 资源文件（骨骼 `_ske.json`，纹理集 `_tex.json` 和 `_tex.png`）放置到你的 Android 项目的 `assets` 目录下。例如：`src/main/assets/dragonbones/models/my_model/`。

### 3. 基本用法

在你的 Composable 函数中使用 `DragonBonesViewCompose`。

```kotlin
import androidx.compose.runtime.Composable
import androidx.compose.runtime.remember
import com.dragonbones.DragonBonesModel
import com.dragonbones.DragonBonesViewCompose
import com.dragonbones.rememberDragonBonesController

@Composable
fun MyAnimationScreen() {
    // 1. 创建并记住一个控制器实例
    val controller = rememberDragonBonesController()

    // 2. 定义你的 DragonBones 模型
    //    路径相对于 assets 目录
    val myModel = remember {
        DragonBonesModel(
            skeletonPath = "dragonbones/models/my_model/my_model_ske.json",
            textureJsonPath = "dragonbones/models/my_model/my_model_tex.json",
            textureImagePath = "dragonbones/models/my_model/my_model_tex.png"
        )
    }

    // 3. 将 DragonBonesViewCompose 添加到你的 UI 中
    DragonBonesViewCompose(
        model = myModel,
        controller = controller
    )
}
```

### 4. 控制动画

使用 `DragonBonesController` 来控制动画播放。

#### 播放和循环动画

推荐使用 `fadeInAnimation` 方法，它可以更好地控制混合和过渡。

```kotlin
// 播放 "walk" 动画，无限循环 (loop=0)
controller.fadeInAnimation(name = "walk", layer = 0, loop = 0, fadeInTime = 0.3f)

// 播放 "jump" 动画一次 (loop=1)，在更高的图层上
controller.fadeInAnimation(name = "jump", layer = 1, loop = 1, fadeInTime = 0.1f)
```

#### 停止动画

```kotlin
controller.stopAnimation(name = "walk")
```

#### 清空动画队列

当你要切换模型时，这是一个很重要的步骤，可以防止旧的动画命令在新模型上播放。

```kotlin
controller.clearAnimationQueue()
currentModel = newModel
```

### 5. 获取动画列表

`DragonBonesController` 会自动获取并持有一个可用的动画名称列表。你可以在你的 UI 中响应式地使用它。

```kotlin
val animations = controller.animationNames

// 当动画列表加载后，显示一个按钮列表
if (animations.isNotEmpty()) {
    LazyRow {
        items(animations) { animName ->
            Button(onClick = {
                controller.fadeInAnimation(name = animName, layer = 1, loop = 1)
            }) {
                Text(text = animName)
            }
        }
    }
}
```
`controller.animationNames` 是一个 `State<List<String>>`，当新模型加载完成并解析出动画列表后，它会自动更新。

### 6. 切换模型

你可以通过更新传递给 `DragonBonesViewCompose` 的 `model` 参数来动态切换模型。

```kotlin
var currentModel by remember { mutableStateOf<DragonBonesModel?>(bearModel) }

// ...

Button(onClick = {
    // 切换模型前清除动画队列
    controller.clearAnimationQueue()
    currentModel = if (currentModel == bearModel) loongModel else bearModel
}) {
    Text("Switch Model")
}

DragonBonesViewCompose(
    model = currentModel,
    controller = controller
)
```

## API 参考

### `DragonBonesViewCompose`

| 参数 | 类型 | 描述 |
| --- | --- | --- |
| `modifier` | `Modifier` | Compose Modifier。 |
| `model` | `DragonBonesModel?` | 要加载的 DragonBones 模型。传 `null` 或改变此值会触发模型的加载/切换。 |
| `controller` | `DragonBonesController` | 用于控制视图的控制器。 |
| `zOrderOnTop` | `Boolean` | 是否将视图置于顶层。默认为 `true`。 |
| `onError` | `(String) -> Unit` | 当发生错误时（如模型加载失败）的回调。 |
| `onRenderStart`| `() -> Unit` | 渲染循环开始时的回调。 |
| `onRenderFrame`| `() -> Unit` | 每一帧渲染时的回调。 |
| `onRenderEnd`  | `() -> Unit` | 渲染循环结束时的回调。 |

### `DragonBonesController`

#### 属性

*   `animationNames: List<String>`: (State) 当前模型可用的动画名称列表。
*   `scale: Float`: (State) 整体缩放。
*   `translationX: Float`: (State) X轴平移。
*   `translationY: Float`: (State) Y轴平移。
*   `onSlotTap: ((String) -> Unit)?`: 当用户点击一个插槽时触发的回调，参数为插槽名称。

#### 方法

*   `fadeInAnimation(name: String, layer: Int, loop: Int, fadeInTime: Float = 0.3f)`: 在指定图层上淡入播放一个动画。
*   `stopAnimation(name: String)`: 停止播放指定名称的动画。
*   `clearAnimationQueue()`: 清除所有待处理的动画命令。
*   `overrideBonePosition(boneName: String, x: Float, y: Float)`: 覆盖骨骼的局部位置。
*   `resetBone(boneName: String)`: 重置骨骼的局部位置。 