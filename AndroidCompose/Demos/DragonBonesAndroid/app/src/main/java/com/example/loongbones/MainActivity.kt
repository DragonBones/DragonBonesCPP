package com.example.loongbones

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import com.dragonbones.DragonBonesModel
import com.dragonbones.DragonBonesViewCompose
import com.dragonbones.rememberDragonBonesController
import com.example.loongbones.ui.theme.DragonBonesCPPTheme
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.lazy.LazyRow
import androidx.compose.foundation.lazy.items

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            DragonBonesCPPTheme {
                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    DragonBonesAndroidDemo()
                }
            }
        }
    }
}

@Composable
fun DragonBonesAndroidDemo() {
    val controller = rememberDragonBonesController()

    val bearModel = remember {
        DragonBonesModel(
            skeletonPath = "dragonbones/models/bear/bear.json",
            textureJsonPath = "dragonbones/models/bear/bear_tex.json",
            textureImagePath = "dragonbones/models/bear/bear_tex.png"
        )
    }

    val loongModel = remember {
        DragonBonesModel(
            skeletonPath = "dragonbones/models/loong/loongbones-web.json",
            textureJsonPath = "dragonbones/models/loong/loongbones-web_tex.json",
            textureImagePath = "dragonbones/models/loong/loongbones-web_tex.png"
        )
    }

    var currentModel by remember { mutableStateOf<DragonBonesModel?>(null) }
    var animationsLoaded by remember { mutableStateOf(false) }

    // Start with the bear model
    LaunchedEffect(Unit) {
        currentModel = bearModel
    }

    // Effect to play the default animation when the model changes and animation names become available.
    LaunchedEffect(currentModel, controller.animationNames) {
        val anims = controller.animationNames
        if (anims.isEmpty()) {
            animationsLoaded = false
            return@LaunchedEffect
        }

        animationsLoaded = true
        
        val animToPlay = when {
            "walk" in anims -> "walk"
            "stand" in anims -> "stand"
            else -> anims.firstOrNull()
        }

        animToPlay?.let {
            // controller.fadeInAnimation(name = it, layer = 0, loop = 0, fadeInTime = 0.3f)
        }
    }

    Box(modifier = Modifier.fillMaxSize()) {
        DragonBonesViewCompose(
            modifier = Modifier.fillMaxSize(),
            model = currentModel,
            controller = controller,
            onError = { error ->
                // Handle errors, e.g., show a toast or a snackbar
                println("DragonBones Error: $error")
            }
        )

        Column(
            modifier = Modifier
                .align(Alignment.BottomCenter)
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Button(onClick = {
                // 切换模型前清除动画队列，避免旧动画命令应用到新模型上
                controller.clearAnimationQueue()
                animationsLoaded = false
                currentModel = if (currentModel == bearModel) loongModel else bearModel
            }) {
                Text("Switch Model")
            }
            
            if (animationsLoaded) {
                Text(
                    text = "Available Animations:",
                    modifier = Modifier.padding(vertical = 8.dp)
                )
                LazyRow(
                    horizontalArrangement = Arrangement.spacedBy(8.dp),
                    verticalAlignment = Alignment.CenterVertically
                ) {
                    items(controller.animationNames) { animName ->
                        Button(onClick = {
                            controller.fadeInAnimation(
                                name = animName,
                                layer = 2, // Use a single, dedicated layer for all one-shot animations
                                loop = 1,  // Play once
                                fadeInTime = 0.1f
                            )
                        }) {
                            Text(text = animName)
                        }
                    }
                }
            }
        }
    }
} 