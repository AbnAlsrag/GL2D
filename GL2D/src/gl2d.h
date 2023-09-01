#ifndef _GL2D_H_
#define _GL2D_H_

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define GL2D_PLATFORM_WINDOWS
    #if defined(_WIN64)
        #define GL2D_PLATFORM_WINDOWS_64
    #else
        #define GL2D_PLATFORM_WINDOWS_32
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        #define GL2D_PLATFORM_IPHONE_SIMULATOR
    #elif TARGET_OS_MACCATALYST
        #define GL2D_PLATFORM_MACCATALYST
    #elif TARGET_OS_IPHONE
        #define GL2D_PLATFORM_IOS
    #elif TARGET_OS_MAC
        #define GL2D_PLATFORM_MACOS
    #else
        #define GL2D_PLATFORM_UNKNOWN_APPLE
        #define GL2D_PLATFORM_UNKNOWN
    #endif
#elif defined(__EMSCRIPTEN__) || defined(EMSCRIPTEN) || defined(EMSCRIPTEN_)
    #define GL2D_PLATFORM_EMSCRIPTEN
#elif defined(__ANDROID__)
    #define GL2D_PLATFORM_ANDROID
#elif defined(__linux__) || defined(__linux)
    #define GL2D_PLATFORM_LINUX
#elif defined(__FreeBSD__)
    #define GL2D_PLATFORM_FREEBSD
#elif defined(__unix__) || defined(__unix)
    #define GL2D_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
    #define GL2D_PLATFORM_POSIX
#else
    #define GL2D_PLATFORM_UNKNOWN
#endif
#if defined(__GNUC__)
    #define GL2D_PLATFORM_GCC
#elif defined(__CC_ARM)
    #define GL2D_PLATFORM_ARM
#endif

#ifdef GL2D_USE_DLL
    #ifdef GL2D_PLATFORM_WINDOWS
        #define GL2D_API __declspec(dllimport)
    #elif defined(GL2D_PLATFORM_GCC)
        #define GL2D_API
    #endif
#else
    #define GL2D_API
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct GL2D_Vector2f {
    float x;
    float y;
} GL2D_Vector2f;

typedef struct GL2D_Rect {
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t w;
} GL2D_Rect;

typedef struct GL2D_Transform {
    GL2D_Vector2f position;
    GL2D_Vector2f rotation;
    GL2D_Vector2f scale;
} GL2D_Transform;

typedef struct GL2D_Matrix4f {
    float data[4][4];
} GL2D_Matrix4f;

typedef struct GL2D_Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} GL2D_Color;

#define GL2D_COLOR_GOODIE (GL2D_Color) { 51, 76, 76, 255 }
#define GL2D_COLOR_GREY (GL2D_Color) { 192, 192, 192, 255 }
#define GL2D_COLOR_DARK_GREY (GL2D_Color) { 128, 128, 128, 255 }
#define GL2D_COLOR_VERY_DARK_GREY (GL2D_Color) { 64, 64, 64, 255 }
#define GL2D_COLOR_RED (GL2D_Color) { 255, 0, 0, 255 }
#define GL2D_COLOR_DARK_RED (GL2D_Color) { 128, 0, 0, 255 }
#define GL2D_COLOR_VERY_DARK_RED (GL2D_Color) { 64, 0, 0, 255 }
#define GL2D_COLOR_YELLOW (GL2D_Color) { 255, 255, 0, 255 }
#define GL2D_COLOR_DARK_YELLOW (GL2D_Color) { 128, 128, 0, 255 }
#define GL2D_COLOR_VERY_DARK_YELLOW (GL2D_Color) { 64, 64, 0, 255 }
#define GL2D_COLOR_GREEN (GL2D_Color) { 0, 255, 0, 255 }
#define GL2D_COLOR_DARK_GREEN (GL2D_Color) { 0, 128, 0, 255 }
#define GL2D_COLOR_VERY_DARK_GREEN (GL2D_Color) { 0, 64, 0, 255 }
#define GL2D_COLOR_CYAN (GL2D_Color) { 0, 255, 255, 255 }
#define GL2D_COLOR_DARK_CYAN (GL2D_Color) { 0, 128, 128, 255 }
#define GL2D_COLOR_VERY_DARK_CYAN (GL2D_Color) { 0, 64, 64, 255 }
#define GL2D_COLOR_BLUE (GL2D_Color) { 0, 0, 255, 255 }
#define GL2D_COLOR_DARK_BLUE (GL2D_Color) { 0, 0, 128, 255 }
#define GL2D_COLOR_VERY_DARK_BLUE (GL2D_Color) { 0, 0, 64, 255 }
#define GL2D_COLOR_MAGENTA (GL2D_Color) { 255, 0, 255, 255 }
#define GL2D_COLOR_DARK_MAGENTA (GL2D_Color) { 128, 0, 128, 255 }
#define GL2D_COLOR_VERY_DARK_MAGENTA (GL2D_Color) { 64, 0, 64, 255 }
#define GL2D_COLOR_WHITE (GL2D_Color) { 255, 255, 255, 255 }
#define GL2D_COLOR_BLACK (GL2D_Color) { 0, 0, 0, 255 }
#define GL2D_COLOR_BLANK (GL2D_Color) { 0, 0, 0, 0 }

//const static GL2D_Color
//GL2D_COLOR_GOODIE = { 51, 76, 76, 255 }, GL2D_COLOR_GREY = { 192, 192, 192, 255 },
//GL2D_COLOR_DARK_GREY = { 128, 128, 128, 255 }, GL2D_COLOR_VERY_DARK_GREY = { 64, 64, 64, 255 },
//GL2D_COLOR_RED = { 255, 0, 0, 255 }, GL2D_COLOR_DARK_RED = { 128, 0, 0, 255 },
//GL2D_COLOR_VERY_DARK_RED = { 64, 0, 0, 255 }, GL2D_COLOR_YELLOW = { 255, 255, 0, 255 },
//GL2D_COLOR_DARK_YELLOW = { 128, 128, 0, 255 }, GL2D_COLOR_VERY_DARK_YELLOW = { 64, 64, 0, 255 },
//GL2D_COLOR_GREEN = { 0, 255, 0, 255 }, GL2D_COLOR_DARK_GREEN = { 0, 128, 0, 255 },
//GL2D_COLOR_VERY_DARK_GREEN = { 0, 64, 0, 255 }, GL2D_COLOR_CYAN = { 0, 255, 255, 255 },
//GL2D_COLOR_DARK_CYAN = { 0, 128, 128, 255 }, GL2D_COLOR_VERY_DARK_CYAN = { 0, 64, 64, 255 },
//GL2D_COLOR_BLUE = { 0, 0, 255, 255 }, GL2D_COLOR_DARK_BLUE = { 0, 0, 128, 255 },
//GL2D_COLOR_VERY_DARK_BLUE = { 0, 0, 64, 255 }, GL2D_COLOR_MAGENTA = { 255, 0, 255, 255 },
//GL2D_COLOR_DARK_MAGENTA = { 128, 0, 128, 255 }, GL2D_COLOR_VERY_DARK_MAGENTA = { 64, 0, 64, 255 },
//GL2D_COLOR_WHITE = { 255, 255, 255, 255 }, GL2D_COLOR_BLACK = { 0, 0, 0, 255 },
//GL2D_COLOR_BLANK = { 0, 0, 0, 0 };

typedef struct GL2D_Renderer* GL2D_Renderer;
typedef struct GL2D_Shader* GL2D_Shader;
typedef struct GL2D_Texture* GL2D_Texture;
typedef struct GL2D_VertexArray* GL2D_VertexArray;
typedef struct GL2D_VertexBuffer* GL2D_VertexBuffer;
typedef struct GL2D_IndexBuffer* GL2D_IndexBuffer;

GL2D_API bool GL2D_VecEqual(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_API GL2D_Vector2f GL2D_VecScalarAdd(GL2D_Vector2f vector, float value);
GL2D_API GL2D_Vector2f GL2D_VecScalarSub(GL2D_Vector2f vector, float value);
GL2D_API GL2D_Vector2f GL2D_VecScalarMult(GL2D_Vector2f vector, float value);
GL2D_API GL2D_Vector2f GL2D_VecScalarDiv(GL2D_Vector2f vector, float value);
GL2D_API GL2D_Vector2f GL2D_VecPower(GL2D_Vector2f vector, float value);
GL2D_API GL2D_Vector2f GL2D_VecAdd(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_API GL2D_Vector2f GL2D_VecSub(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_API GL2D_Vector2f GL2D_VecMult(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_API GL2D_Vector2f GL2D_VecDiv(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_API float GL2D_VecDotProduct(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_API float GL2D_VecMagnitude(GL2D_Vector2f vector);
GL2D_API float GL2D_VecMagnitudeSquared(GL2D_Vector2f vector);
GL2D_API GL2D_Vector2f GL2D_VecNormalize(GL2D_Vector2f vector);
GL2D_API GL2D_Vector2f GL2D_VecRotate(GL2D_Vector2f vector, float angle);

GL2D_API GL2D_Matrix4f GL2D_Mat4InitIdentity();
GL2D_API GL2D_Matrix4f GL2D_Mat4Translate(GL2D_Matrix4f matrix, GL2D_Vector2f vector);
GL2D_API GL2D_Matrix4f GL2D_Mat4Scale(GL2D_Matrix4f matrix, GL2D_Vector2f vector);
GL2D_API GL2D_Matrix4f GL2D_Mat4Rotate(GL2D_Matrix4f matrix, float angle, GL2D_Vector2f vector);
GL2D_API GL2D_Matrix4f GL2D_Mat4Add(GL2D_Matrix4f matrix1, GL2D_Matrix4f matrix2);
GL2D_API GL2D_Matrix4f GL2D_Mat4Sub(GL2D_Matrix4f matrix1, GL2D_Matrix4f matrix2);
GL2D_API GL2D_Matrix4f GL2D_Mat4Mult(GL2D_Matrix4f matrix1,GL2D_Matrix4f matrix2);
GL2D_API GL2D_Vector2f GL2D_Mat4Vec2Mult(GL2D_Matrix4f matrix, GL2D_Vector2f vector);

GL2D_API void GL2D_SetViewport(GL2D_Rect rect);
GL2D_API void GL2D_SetClearColor(GL2D_Color color);

GL2D_API GL2D_Renderer GL2D_CreateRenderer();
GL2D_API void GL2D_UpdateRenderer(GL2D_Renderer renderer);

GL2D_API GL2D_Shader GL2D_CreateShaderFromFiles(const char* vertexPath, const char* fragmentPath);
GL2D_API void GL2D_DestroyShader(GL2D_Shader shader);
GL2D_API void GL2D_BindShader(GL2D_Shader shader);
GL2D_API void GL2D_ShaderSetBool(GL2D_Shader shader, const char* name, bool value);
GL2D_API void GL2D_ShaderSetInt(GL2D_Shader shader, const char* name, int value);
GL2D_API void GL2D_ShaderSetFloat(GL2D_Shader shader, const char* name, float value);
GL2D_API void GL2D_ShaderSetVec2(GL2D_Shader shader, const char* name, GL2D_Vector2f value);
GL2D_API void GL2D_ShaderSetMat4(GL2D_Shader shader, const char* name, GL2D_Matrix4f value);

GL2D_API GL2D_Texture GL2D_CreateTextureFromFile(const char* path);
GL2D_API void GL2D_DestroyTexture(GL2D_Texture texture);
GL2D_API void GL2D_BindTexture(GL2D_Texture texture);

GL2D_API GL2D_VertexArray GL2D_CreateVertexArray();
GL2D_API void GL2D_DestroyVertexArray(GL2D_VertexArray vao);
GL2D_API void GL2D_BindVertexArray(GL2D_VertexArray vao);

GL2D_API GL2D_VertexBuffer GL2D_CreateVertexBuffer(const float* vertices, uint64_t size);
GL2D_API void GL2D_DestroyVertexBuffer(GL2D_VertexBuffer vbo);
GL2D_API void GL2D_BindVertexBuffer(GL2D_VertexBuffer vbo);

GL2D_API GL2D_IndexBuffer GL2D_CreateIndexBuffer(const uint32_t* indices, uint64_t size);
GL2D_API void GL2D_DestroyIndexBuffer(GL2D_IndexBuffer ibo);
GL2D_API void GL2D_BindIndexBuffer(GL2D_IndexBuffer ibo);

#endif