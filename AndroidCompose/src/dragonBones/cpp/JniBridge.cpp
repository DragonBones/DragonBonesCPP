#include "JniBridge.h"
#include <android/log.h>
#include "dragonBones/DragonBonesHeaders.h"
#include "opengl/OpenGLFactory.h"
#include "opengl/OpenGLSlot.h"
#include <GLES2/gl2.h>
#include <string>
#include <vector>
#include <cstring>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#define LOG_TAG "DragonBonesJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

namespace {
    struct JniBridgeInstance {
        dragonBones::DragonBones* dragonBones = nullptr;
        dragonBones::Armature* armature = nullptr;
        dragonBones::opengl::OpenGLFactory* factory = nullptr;
        
        // OpenGL ES 2.0 rendering variables
        GLuint programId = 0;
        GLint positionLocation = -1;
        GLint texCoordLocation = -1;
        GLint mvpMatrixLocation = -1;
        GLint textureLocation = -1;
        GLfloat projectionMatrix[16];

        // Armature transform
        float worldScale = 0.5f;
        float worldTranslateX = 0.0f;
        float worldTranslateY = 0.0f;

        // Buffers to hold data, loaded off the GL thread
        std::vector<char> dragonBonesDataBuffer;
        std::vector<char> textureJsonBuffer;
        std::vector<char> texturePngDataBuffer;
        
        // State flags to handle race condition between surface creation and data loading
        bool isDataLoaded = false;
        bool isGlReady = false;

        ~JniBridgeInstance() {
            if (dragonBones) {
                delete dragonBones;
                dragonBones = nullptr;
            }
            if (factory) {
                delete factory;
                factory = nullptr;
            }
            
            if (programId) {
                glDeleteProgram(programId);
                programId = 0;
            }
        }
    };

    JniBridgeInstance* instance = nullptr;

    JniBridgeInstance* getInstance() {
        if (!instance) {
            instance = new JniBridgeInstance();
        }
        return instance;
    }
    
    void _tryBuildArmature(JniBridgeInstance* instance); // Forward declaration

    // 辅助函数：创建和编译着色器
    GLuint compileShader(GLenum type, const char* source) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        
        // 检查编译状态
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            LOGE("Shader compilation failed: %s", infoLog);
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }
    
    // 辅助函数：创建着色器程序
    GLuint createShaderProgram() {
        // 顶点着色器
        const char* vertexShaderSource = 
            "attribute vec2 a_position;\n"
            "attribute vec2 a_texCoord;\n"
            "varying vec2 v_texCoord;\n"
            "uniform mat4 u_mvpMatrix;\n"
            "void main() {\n"
            "  gl_Position = u_mvpMatrix * vec4(a_position, 0.0, 1.0);\n"
            "  v_texCoord = a_texCoord;\n"
            "}\n";
        
        // 片段着色器
        const char* fragmentShaderSource = 
            "precision mediump float;\n"
            "varying vec2 v_texCoord;\n"
            "uniform sampler2D u_texture;\n"
            "void main() {\n"
            "  gl_FragColor = texture2D(u_texture, v_texCoord);\n"
            "}\n";
        
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        if (!vertexShader) return 0;
        
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
        if (!fragmentShader) {
            glDeleteShader(vertexShader);
            return 0;
        }
        
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
            LOGE("Shader program linking failed: %s", infoLog);
            glDeleteProgram(program);
            program = 0;
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        return program;
    }
    
    // 辅助函数：创建正交投影矩阵
    void createOrthographicMatrix(float left, float right, float bottom, float top, float near, float far, float* matrix) {
        // 列主序矩阵
        matrix[0] = 2.0f / (right - left);
        matrix[1] = 0.0f;
        matrix[2] = 0.0f;
        matrix[3] = 0.0f;
        
        matrix[4] = 0.0f;
        matrix[5] = 2.0f / (top - bottom);
        matrix[6] = 0.0f;
        matrix[7] = 0.0f;
        
        matrix[8] = 0.0f;
        matrix[9] = 0.0f;
        matrix[10] = -2.0f / (far - near);
        matrix[11] = 0.0f;
        
        matrix[12] = -(right + left) / (right - left);
        matrix[13] = -(top + bottom) / (top - bottom);
        matrix[14] = -(far + near) / (far - near);
        matrix[15] = 1.0f;
    }
    
    void createIdentityMatrix(float* matrix) {
        matrix[0] = 1.0f; matrix[4] = 0.0f; matrix[8] = 0.0f; matrix[12] = 0.0f;
        matrix[1] = 0.0f; matrix[5] = 1.0f; matrix[9] = 0.0f; matrix[13] = 0.0f;
        matrix[2] = 0.0f; matrix[6] = 0.0f; matrix[10] = 1.0f; matrix[14] = 0.0f;
        matrix[3] = 0.0f; matrix[7] = 0.0f; matrix[11] = 0.0f; matrix[15] = 1.0f;
    }

    void createTranslateMatrix(float* matrix, float tx, float ty, float tz) {
        createIdentityMatrix(matrix);
        matrix[12] = tx;
        matrix[13] = ty;
        matrix[14] = tz;
    }
    
    void createScaleMatrix(float* matrix, float sx, float sy, float sz) {
        createIdentityMatrix(matrix);
        matrix[0] = sx;
        matrix[5] = sy;
        matrix[10] = sz;
    }
    
    // 辅助函数：将 DragonBones 2D 矩阵转换为 OpenGL 4x4 矩阵
    void convertDBMatrixToGL(const dragonBones::Matrix& dbMatrix, float* glMatrix) {
        createIdentityMatrix(glMatrix);
        glMatrix[0] = dbMatrix.a;
        glMatrix[1] = dbMatrix.b;
        glMatrix[4] = dbMatrix.c;
        glMatrix[5] = dbMatrix.d;
        glMatrix[12] = dbMatrix.tx;
        glMatrix[13] = dbMatrix.ty;
    }

    // 辅助函数：矩阵乘法
    void multiplyMatrices(const float* a, const float* b, float* result) {
        float res[16];
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                res[j * 4 + i] = 0.0f;
                for (int k = 0; k < 4; k++) {
                    res[j * 4 + i] += a[k * 4 + i] * b[j * 4 + k];
                }
            }
        }
        memcpy(result, res, sizeof(res));
    }

    void _tryBuildArmature(JniBridgeInstance* instance) {
        if (!instance->isGlReady || !instance->isDataLoaded || instance->armature) {
            return;
        }

        LOGI("Creating armature on GL thread...");
        
        // The factory might have old data, clear it before parsing new data.
        instance->factory->clear();

        std::pair<void*, int> textureInfo = {instance->texturePngDataBuffer.data(), (int)instance->texturePngDataBuffer.size()};
        auto* textureAtlasData = instance->factory->parseTextureAtlasData(instance->textureJsonBuffer.data(), &textureInfo);
        if (!textureAtlasData) {
            LOGE("Failed to parse texture atlas data.");
            return;
        }
        instance->factory->addTextureAtlasData(textureAtlasData);

        auto* dragonBonesData = instance->factory->parseDragonBonesData(instance->dragonBonesDataBuffer.data());
        if (!dragonBonesData) {
            LOGE("Failed to parse DragonBones data.");
            return;
        }

        const auto& armatureNames = dragonBonesData->getArmatureNames();
        std::string armatureNameToBuild;
        if (!armatureNames.empty()) {
            bool dragonFound = false;
            for (const auto& name : armatureNames) {
                if (name == "Dragon") {
                    armatureNameToBuild = name;
                    dragonFound = true;
                    break;
                }
            }
            if (!dragonFound) {
                armatureNameToBuild = armatureNames[0]; 
            }
        }

        if (armatureNameToBuild.empty()) {
            LOGE("No armatures found in DragonBones data.");
            return;
        }

        auto* armatureObject = instance->factory->buildArmature(armatureNameToBuild, "", "", dragonBonesData->name);
        if (armatureObject)
        {
            instance->armature = armatureObject;
            LOGI("Armature '%s' built at %p, instance is %p", armatureNameToBuild.c_str(), instance->armature, instance);
            instance->dragonBones->getClock()->add(armatureObject);
            // Reset animation to force armature to setup pose.
            // This overrides any "defaultActions" in the data file (e.g., auto-playing an empty animation),
            // which can cause rendering issues for some models.
            armatureObject->getAnimation()->reset();
        } else {
            LOGE("Failed to build armature '%s'.", armatureNameToBuild.c_str());
        }
    }
}

// Global variables for DragonBones
static float viewportWidth = 0.0f;
static float viewportHeight = 0.0f;

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_init(JNIEnv *env, jclass clazz) {
    auto* instance = getInstance();
    if (!instance->factory) {
        instance->factory = new dragonBones::opengl::OpenGLFactory();
    }
    if (!instance->dragonBones) {
        instance->dragonBones = new dragonBones::DragonBones(instance->factory);
        instance->factory->setDragonBones(instance->dragonBones);
    }
    LOGI("DragonBones JNI Initialized");
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_loadDragonBones(JNIEnv *env, jclass clazz, jbyteArray skeleton_data, jbyteArray texture_json_data, jbyteArray texture_png_data) {
    auto* instance = getInstance();
    if (!instance) return;

    // Clean up previous model if it exists
    if (instance->armature) {
        LOGI("Cleaning up previous armature and factory data.");
        // 1. Dispose of the old armature object. This will queue it for cleanup on the next advanceTime().
        instance->armature->dispose();
        instance->armature = nullptr;

        // 2. Clear all parsed data (dragonbones data, texture atlas data) from the factory.
        // Recreating the factory is dangerous because the DragonBones instance holds a pointer
        // to it as an event manager. Clearing is the intended way to switch models.
        instance->factory->clear();
    }
    instance->isDataLoaded = false;
    instance->dragonBonesDataBuffer.clear();
    instance->textureJsonBuffer.clear();
    instance->texturePngDataBuffer.clear();

    LOGI("Buffering data from byte arrays...");

    // Skeleton data
    jbyte* skeleton_bytes = env->GetByteArrayElements(skeleton_data, nullptr);
    jsize skeleton_len = env->GetArrayLength(skeleton_data);
    instance->dragonBonesDataBuffer.assign(skeleton_bytes, skeleton_bytes + skeleton_len);
    env->ReleaseByteArrayElements(skeleton_data, skeleton_bytes, JNI_ABORT);
    instance->dragonBonesDataBuffer.push_back('\0'); // Null-terminate for string parsing

    // Texture JSON data
    jbyte* texture_json_bytes = env->GetByteArrayElements(texture_json_data, nullptr);
    jsize texture_json_len = env->GetArrayLength(texture_json_data);
    instance->textureJsonBuffer.assign(texture_json_bytes, texture_json_bytes + texture_json_len);
    env->ReleaseByteArrayElements(texture_json_data, texture_json_bytes, JNI_ABORT);
    instance->textureJsonBuffer.push_back('\0'); // Null-terminate for string parsing

    // Texture PNG data
    jbyte* texture_png_bytes = env->GetByteArrayElements(texture_png_data, nullptr);
    jsize texture_png_len = env->GetArrayLength(texture_png_data);
    instance->texturePngDataBuffer.assign(texture_png_bytes, texture_png_bytes + texture_png_len);
    env->ReleaseByteArrayElements(texture_png_data, texture_png_bytes, JNI_ABORT);

    if (instance->dragonBonesDataBuffer.empty() || instance->textureJsonBuffer.empty() || instance->texturePngDataBuffer.empty()) {
        LOGE("Failed to copy one or more byte arrays.");
        instance->isDataLoaded = false;
    } else {
        LOGI("Data successfully buffered.");
        instance->isDataLoaded = true;
        _tryBuildArmature(instance);
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onPause(JNIEnv *env, jclass clazz) {
    LOGI("DragonBones onPause");
    // 暂停动画和渲染
    if (getInstance()->dragonBones) {
        // 可以在这里保存状态或暂停动画
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onResume(JNIEnv *env, jclass clazz) {
    LOGI("DragonBones onResume");
    // 恢复动画和渲染
    if (getInstance()->dragonBones) {
        // 可以在这里恢复之前的状态
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onDestroy(JNIEnv *env, jclass clazz) {
    LOGI("DragonBones onDestroy");
    auto* currentInstance = getInstance();
    if (currentInstance) {
        delete currentInstance;
        instance = nullptr;
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onSurfaceCreated(JNIEnv *env, jclass clazz) {
    LOGI("DragonBones onSurfaceCreated");
    auto* instance = getInstance();
    if (!instance) return;

    // If the surface is recreated, old GL resources are invalid. Clean up the armature.
    if (instance->armature) {
        LOGI("GL context recreated. Cleaning up old armature to recreate GL resources.");
        instance->dragonBones->getClock()->remove(instance->armature);
        instance->armature = nullptr;
    }

    // 1. Setup GL State
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    // 2. Create shader program
    instance->programId = createShaderProgram();
    if (!instance->programId) {
        LOGE("Failed to create shader program");
        return;
    }
    
    // 3. Get shader locations
    instance->positionLocation = glGetAttribLocation(instance->programId, "a_position");
    instance->texCoordLocation = glGetAttribLocation(instance->programId, "a_texCoord");
    instance->mvpMatrixLocation = glGetUniformLocation(instance->programId, "u_mvpMatrix");
    instance->textureLocation = glGetUniformLocation(instance->programId, "u_texture");
    
    if (instance->positionLocation < 0 || instance->texCoordLocation < 0 || 
        instance->mvpMatrixLocation < 0 || instance->textureLocation < 0) {
        LOGE("Failed to get one or more shader variable locations.");
        return;
    }

    // 4. Set GL ready flag and attempt to build armature
    instance->isGlReady = true;
    _tryBuildArmature(instance);
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
    auto* instance = getInstance();
    if (instance) {
        glViewport(0, 0, width, height);
        viewportWidth = (float)width;
        viewportHeight = (float)height;
        // Create the projection matrix to map pixel coordinates to screen space
        createOrthographicMatrix(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f, instance->projectionMatrix);
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_onDrawFrame(JNIEnv *env, jclass clazz) {
    auto* instance = getInstance();
    if (instance && instance->dragonBones)
    {
        instance->dragonBones->advanceTime(1.0f / 60.0f);

        if (instance->armature) {
            // 1. Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 2. Setup the rendering program and global GL state
            glUseProgram(instance->programId);
            glEnableVertexAttribArray(instance->positionLocation);
            glEnableVertexAttribArray(instance->texCoordLocation);
            
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(instance->textureLocation, 0);
            
            // 3. Create a "view" matrix to scale and center the entire armature
            float viewMatrix[16], scaleM[16], transM[16];
            createScaleMatrix(scaleM, instance->worldScale, instance->worldScale, 1.0f);
            createTranslateMatrix(transM, (viewportWidth / 2.0f) + instance->worldTranslateX, (viewportHeight / 2.0f) + instance->worldTranslateY, 0.0f);
            multiplyMatrices(transM, scaleM, viewMatrix);
            
            // 4. Render each slot
            const auto& slots = instance->armature->getSlots();

            int renderedSlots = 0;
            for (const auto& slot : slots) {
                if (!slot) {
                    LOGW("onDrawFrame: Skipping null slot.");
                    continue;
                }

                if (!slot->getVisible()) {
                    LOGW("onDrawFrame: Slot '%s' is not visible.", slot->getName().c_str());
                    continue;
                }

                if (!slot->getDisplay()) {
                    // LOGW("onDrawFrame: Slot '%s' has no display object.", slot->getName().c_str());
                    continue;
                }
                
                auto* openglSlot = static_cast<dragonBones::opengl::OpenGLSlot*>(slot);
                if (!openglSlot) {
                    LOGW("onDrawFrame: Slot '%s' could not be cast to OpenGLSlot.", slot->getName().c_str());
                    continue;
                }

                if (openglSlot->vertices.empty() || openglSlot->indices.empty() || openglSlot->textureID == 0) {
                    LOGW("onDrawFrame: Skipping slot '%s' due to empty buffers or texture ID 0 (vertices: %zu, indices: %zu, textureID: %u)",
                        slot->getName().c_str(), openglSlot->vertices.size(), openglSlot->indices.size(), openglSlot->textureID);
                    continue;
                }

                // A. Get this slot's unique transformation matrix
                float slotModelMatrix[16];
                if (openglSlot->isSkinned)
                {
                    createIdentityMatrix(slotModelMatrix);
                }
                else
                {
                    convertDBMatrixToGL(slot->globalTransformMatrix, slotModelMatrix);
                }

                // B. Create the final MVP matrix: MVP = Projection * View * SlotModel
                float pvMatrix[16], mvpMatrix[16];
                multiplyMatrices(instance->projectionMatrix, viewMatrix, pvMatrix);
                multiplyMatrices(pvMatrix, slotModelMatrix, mvpMatrix);
                
                // C. Pass the final matrix to the shader
                glUniformMatrix4fv(instance->mvpMatrixLocation, 1, GL_FALSE, mvpMatrix);
                
                // D. Bind the texture and draw
                glBindTexture(GL_TEXTURE_2D, openglSlot->textureID);
                
                const GLsizei stride = 4 * sizeof(float);
                glVertexAttribPointer(instance->positionLocation, 2, GL_FLOAT, GL_FALSE, stride, openglSlot->vertices.data());
                glVertexAttribPointer(instance->texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(openglSlot->vertices.data() + 2));
                
                glDrawElements(GL_TRIANGLES, openglSlot->indices.size(), GL_UNSIGNED_SHORT, openglSlot->indices.data());
                renderedSlots++;
            }
            
            if (renderedSlots == 0 && !slots.empty()) {
                LOGW("onDrawFrame: Rendered 0 slots out of %zu.", slots.size());
            }
            
            // 5. Cleanup
            glDisableVertexAttribArray(instance->positionLocation);
            glDisableVertexAttribArray(instance->texCoordLocation);
        }
    }
}

JNIEXPORT jobjectArray JNICALL
Java_com_dragonbones_JniBridge_getAnimationNames(JNIEnv *env, jclass clazz) {
    auto* instance = getInstance();
    if (!instance || !instance->armature) {
        return nullptr;
    }

    const auto& animationNames = instance->armature->getAnimation()->getAnimationNames();
    if (animationNames.empty()) {
        return nullptr;
    }

    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray stringArray = env->NewObjectArray(animationNames.size(), stringClass, nullptr);

    for (size_t i = 0; i < animationNames.size(); ++i) {
        jstring javaString = env->NewStringUTF(animationNames[i].c_str());
        env->SetObjectArrayElement(stringArray, i, javaString);
        env->DeleteLocalRef(javaString);
    }

    return stringArray;
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_fadeInAnimation(JNIEnv *env, jclass clazz, jstring animation_name, jint layer, jint loop, jfloat fade_in_time) {
    auto* instance = getInstance();
    if (!instance || !instance->armature) {
        return;
    }

    const char* nameChars = env->GetStringUTFChars(animation_name, nullptr);
    std::string name(nameChars);
    env->ReleaseStringUTFChars(animation_name, nameChars);

    // playTimes: -1 means use default value, 0 means loop infinitely, [1~N] means repeat N times.
    // The `loop` parameter from Kotlin is: 0 for infinite, 1 for once, etc. This maps directly to playTimes.
    int playTimes = (int)loop;

    // Use fadeIn to enable blending and layering.
    // Set fadeOutMode to None to prevent animations on different layers from stopping each other.
    // Use SameLayerAndGroup if you want an animation to replace another on the same layer.
    if(instance->armature->getAnimation()->fadeIn(name, (float)fade_in_time, playTimes, (int)layer, "", dragonBones::AnimationFadeOutMode::SameLayerAndGroup)) {
        LOGI("Fading in animation: '%s' on layer %d, loop: %d, fade: %f", name.c_str(), (int)layer, playTimes, (float)fade_in_time);
    } else {
        LOGW("Animation not found: '%s'", name.c_str());
    }
}

JNIEXPORT jstring JNICALL
Java_com_dragonbones_JniBridge_containsPoint(JNIEnv *env, jclass clazz, jfloat x, jfloat y) {
    auto* instance = getInstance();
    if (!instance || !instance->armature) {
        return nullptr;
    }

    // Convert screen coordinates to armature space coordinates
    const float armatureX = (x - (viewportWidth / 2.0f) - instance->worldTranslateX) / instance->worldScale;
    const float armatureY = (y - (viewportHeight / 2.0f) - instance->worldTranslateY) / instance->worldScale;

    auto* slot = instance->armature->containsPoint(armatureX, armatureY);
    if (slot) {
        return env->NewStringUTF(slot->getName().c_str());
    }

    return nullptr;
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_setWorldScale(JNIEnv *env, jclass clazz, jfloat scale) {
    auto* instance = getInstance();
    if (instance) {
        instance->worldScale = scale;
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_setWorldTranslation(JNIEnv *env, jclass clazz, jfloat x, jfloat y) {
    auto* instance = getInstance();
    if (instance) {
        instance->worldTranslateX = x;
        instance->worldTranslateY = y;
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_overrideBonePosition(JNIEnv *env, jclass clazz, jstring bone_name, jfloat x, jfloat y) {
    auto* instance = getInstance();
    if (!instance || !instance->armature) return;

    const char* boneNameChars = env->GetStringUTFChars(bone_name, nullptr);
    auto* bone = instance->armature->getBone(boneNameChars);
    env->ReleaseStringUTFChars(bone_name, boneNameChars);

    if (bone) {
        // Convert screen coordinates to armature space
        const float armatureX = (x - (viewportWidth / 2.0f) - instance->worldTranslateX) / instance->worldScale;
        const float armatureY = (y - (viewportHeight / 2.0f) - instance->worldTranslateY) / instance->worldScale;

        bone->offsetMode = dragonBones::OffsetMode::Override;
        bone->offset.x = armatureX;
        bone->offset.y = armatureY;
        bone->invalidUpdate();
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_resetBone(JNIEnv *env, jclass clazz, jstring bone_name) {
    auto* instance = getInstance();
    if (!instance || !instance->armature) return;

    const char* boneNameChars = env->GetStringUTFChars(bone_name, nullptr);
    auto* bone = instance->armature->getBone(boneNameChars);
    env->ReleaseStringUTFChars(bone_name, boneNameChars);

    if (bone) {
        bone->offsetMode = dragonBones::OffsetMode::None;
        bone->invalidUpdate();
    }
}

JNIEXPORT void JNICALL
Java_com_dragonbones_JniBridge_stopAnimation(JNIEnv *env, jclass clazz, jstring animation_name) {
    auto* instance = getInstance();
    if (!instance || !instance->armature || !instance->armature->getAnimation()) {
        return;
    }

    const char* nameChars = env->GetStringUTFChars(animation_name, nullptr);
    std::string name(nameChars);
    env->ReleaseStringUTFChars(animation_name, nameChars);

    LOGI("Stopping animation: '%s' via JNI call.", name.c_str());
    instance->armature->getAnimation()->stop(name);
}