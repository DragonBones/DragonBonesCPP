# DragonBones Android JNI/Compose Library

[中文 README](./README-zh_CN.md)

This library provides a set of Jetpack Compose components for rendering DragonBones skeletal animations with high performance in Android applications. It uses C++ and OpenGL ES for drawing to ensure smooth animation performance, while offering a clean Compose API for easy integration.

## Features

*   **High-Performance Rendering**: Based on the DragonBones C++ core library and OpenGL ES for near-native rendering performance.
*   **Jetpack Compose Integration**: Seamlessly integrate into your Compose UI as a Composable function.
*   **Reactive API**: Control animations in a declarative way using `State` and coroutines.
*   **Dynamic Control**: Supports playing, stopping, fading in/out, and switching animations.
*   **Model Management**: Supports dynamic loading and switching of different DragonBones models at runtime.
*   **Interaction Support**: Supports getting the name of a tapped slot.

## How to Build and Run the Demo

The demo application is located in the `Demos/DragonBonesAndroid` directory.

1.  **Open Project**: In Android Studio, open the `DragonBonesCPP/AndroidCompose/Demos/DragonBonesAndroid/` directory.
2.  **Gradle Sync**: Allow the project to sync with Gradle. This happens automatically on project open.
3.  **Build and Run**: Select the `app` run configuration and press the 'Run' button. This will build the C++ code, the app, and deploy it to an emulator or device.

Unlike other platform setups, there is no need to manually copy source files. The Gradle build system is configured to use the central DragonBones C++ source files from the repository root directly.

## How to Use in Your Project

### 1. Add Dependency

(This section assumes the library is published as a module or a local module dependency)

Ensure your project's `build.gradle` file includes a reference to the `dragonBones` module.

```groovy
dependencies {
    implementation(project(":dragonBones"))
    // ... other dependencies
}
```

### 2. Add Asset Files

Place your DragonBones asset files (skeleton `_ske.json`, texture atlas `_tex.json` and `_tex.png`) into your Android project's `assets` directory. For example: `src/main/assets/dragonbones/models/my_model/`.

### 3. Basic Usage

Use `DragonBonesViewCompose` in your Composable function.

```kotlin
import androidx.compose.runtime.Composable
import androidx.compose.runtime.remember
import com.dragonbones.DragonBonesModel
import com.dragonbones.DragonBonesViewCompose
import com.dragonbones.rememberDragonBonesController

@Composable
fun MyAnimationScreen() {
    // 1. Create and remember a controller instance
    val controller = rememberDragonBonesController()

    // 2. Define your DragonBones model
    //    Paths are relative to the assets directory
    val myModel = remember {
        DragonBonesModel(
            skeletonPath = "dragonbones/models/my_model/my_model_ske.json",
            textureJsonPath = "dragonbones/models/my_model/my_model_tex.json",
            textureImagePath = "dragonbones/models/my_model/my_model_tex.png"
        )
    }

    // 3. Add DragonBonesViewCompose to your UI
    DragonBonesViewCompose(
        model = myModel,
        controller = controller
    )
}
```

### 4. Controlling Animations

Use the `DragonBonesController` to control animation playback.

#### Playing and Looping Animations

The recommended method is `fadeInAnimation`, which gives you better control over blending and transitions.

```kotlin
// Play the "walk" animation on an infinite loop (loop=0)
controller.fadeInAnimation(name = "walk", layer = 0, loop = 0, fadeInTime = 0.3f)

// Play the "jump" animation once (loop=1) on a higher layer
controller.fadeInAnimation(name = "jump", layer = 1, loop = 1, fadeInTime = 0.1f)
```

#### Stopping an Animation

```kotlin
controller.stopAnimation(name = "walk")
```

#### Clearing the Animation Queue

This is an important step when switching models to prevent old animation commands from running on the new model.

```kotlin
controller.clearAnimationQueue()
currentModel = newModel
```

### 5. Listing Animations

The `DragonBonesController` automatically fetches and holds a list of available animation names. You can use this reactively in your UI.

```kotlin
val animations = controller.animationNames

// Display a row of buttons once the animation list is loaded
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
`controller.animationNames` is a `State<List<String>>` that will be updated automatically after a new model is loaded and its animations are parsed.

### 6. Switching Models

You can dynamically switch models by updating the `model` parameter passed to `DragonBonesViewCompose`.

```kotlin
var currentModel by remember { mutableStateOf<DragonBonesModel?>(bearModel) }

// ...

Button(onClick = {
    // Clear the queue before switching
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

## API Reference

### `DragonBonesViewCompose`

| Parameter | Type | Description |
| --- | --- | --- |
| `modifier` | `Modifier` | The Compose Modifier. |
| `model` | `DragonBonesModel?` | The DragonBones model to load. Passing `null` or changing this value will trigger loading/switching the model. |
| `controller` | `DragonBonesController` | The controller used to interact with the view. |
| `zOrderOnTop` | `Boolean` | Whether to place the view on top of the window. Defaults to `true`. |
| `onError` | `(String) -> Unit` | Callback for when an error occurs (e.g., model loading fails). |
| `onRenderStart`| `() -> Unit` | Callback for when the render loop starts. |
| `onRenderFrame`| `() -> Unit` | Callback for each rendered frame. |
| `onRenderEnd`  | `() -> Unit` | Callback for when the render loop ends. |

### `DragonBonesController`

#### Properties

*   `animationNames: List<String>`: (State) The list of available animation names for the current model.
*   `scale: Float`: (State) The overall scale of the armature.
*   `translationX: Float`: (State) The horizontal translation of the armature.
*   `translationY: Float`: (State) The vertical translation of the armature.
*   `onSlotTap: ((String) -> Unit)?`: Callback invoked when the user taps a slot, providing the slot's name.

#### Methods

*   `fadeInAnimation(name: String, layer: Int, loop: Int, fadeInTime: Float = 0.3f)`: Fades in and plays an animation on a specific layer.
*   `stopAnimation(name: String)`: Stops a specific animation.
*   `clearAnimationQueue()`: Clears all pending animation commands.
*   `overrideBonePosition(boneName: String, x: Float, y: Float)`: Overrides the local position of a bone.
*   `resetBone(boneName: String)`: Resets the local position of a bone.
