/*
     ██████╗ ██╗     ██████╗ ██████╗
    ██╔════╝ ██║     ╚════██╗██╔══██╗
    ██║  ███╗██║      █████╔╝██║  ██║
    ██║   ██║██║     ██╔═══╝ ██║  ██║
    ╚██████╔╝███████╗███████╗██████╔╝
     ╚═════╝ ╚══════╝╚══════╝╚═════╝

    This Library Was Made by ABN ALSRAG
    GL2D:

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

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

#include <stdint.h>
#include <stdbool.h>

#define GL2D_ARRAY(type, ...) ((type[]) { ##__VA_ARGS__ }), (sizeof((type[]){0, ##__VA_ARGS__})/sizeof(type)-1)

typedef struct GL2D_Vector2f {
    float x;
    float y;
} GL2D_Vector2f;

#define GL2D_VECTOR2F(x, y) (GL2D_Vector2f) { x, y }

typedef struct GL2D_Rectf {
    float x;
    float y;
    float width;
    float height;
} GL2D_Rectf;

#define GL2D_RECTF(x, y, width, height) (GL2D_Rectf) { x, y, width, height }

typedef struct GL2D_Transform {
    GL2D_Vector2f position;
    float rotation;
    GL2D_Vector2f scale;
} GL2D_Transform;

#define GL2D_TRANSFORM(p, r, s) (GL2D_Transform) { .position = p, .rotation = r, .scale = s }

typedef struct GL2D_Matrix4f {
    float data[4][4];
} GL2D_Matrix4f;

typedef struct GL2D_Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} GL2D_Color;

#define GL2D_COLOR(r, g, b, a) (GL2D_Color) { r, g, b, a }

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

typedef enum GL2D_TextureFileType {
    GL2D_FILE_TGA,
} GL2D_TextureFileType;

typedef struct GL2D_Context* GL2D_Context;

typedef enum GL2D_Projection {
    GL2D_PROJECTION_ORTHOGRAPHIC,
    GL2D_PROJECTION_PERSPECTIVE,
} GL2D_Projection;

typedef struct GL2D_OrthographicSpecs {
    GL2D_Rectf size;
    float near;
    float far;
} GL2D_OrthographicSpecs;

#define GL2D_ORTHOGRAPHIC_SPECS(s, n, f) (GL2D_OrthographicSpecs) { .size = s, .near = n, .far = f }

typedef struct GL2D_PerspectiveSpecs {
    float fov;
    float aspect;
    float near;
    float far;
} GL2D_PerspectiveSpecs;

#define GL2D_PERSPECTIVE_SPECS(fo, a, n, f) (GL2D_PerspectiveSpecs) { .fov = fo, .aspect = a, .near = n, .far = f }

typedef struct GL2D_Camera {
    GL2D_Vector2f position;
    GL2D_Projection projection;
    union {
        GL2D_OrthographicSpecs orthographic;
        GL2D_PerspectiveSpecs perspective;
    };
} GL2D_Camera;

#define GL2D_ORTHOGRAPHIC_CAMERA(pos, ortho) (GL2D_Camera) { .position = pos,                           \
                                                             .projection = GL2D_PROJECTION_ORTHOGRAPHIC,\
                                                             .orthographic = ortho }

#define GL2D_PERSPECTIVE_CAMERA(pos, pers) (GL2D_Camera) { .position = pos,                           \
                                                           .projection = GL2D_PROJECTION_PERSPECTIVE, \
                                                           .perspective = pers }

typedef struct GL2D_Shader* GL2D_Shader;
typedef struct GL2D_Texture* GL2D_Texture;
typedef struct GL2D_VertexArray* GL2D_VertexArray;
typedef struct GL2D_VertexBuffer* GL2D_VertexBuffer;
typedef struct GL2D_IndexBuffer* GL2D_IndexBuffer;

bool GL2D_Vec2Equal(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_Vector2f GL2D_Vec2ScalarAdd(GL2D_Vector2f vector, float value);
GL2D_Vector2f GL2D_Vec2ScalarSub(GL2D_Vector2f vector, float value);
GL2D_Vector2f GL2D_Vec2ScalarMult(GL2D_Vector2f vector, float value);
GL2D_Vector2f GL2D_Vec2ScalarDiv(GL2D_Vector2f vector, float value);
GL2D_Vector2f GL2D_Vec2Power(GL2D_Vector2f vector, float value);
GL2D_Vector2f GL2D_Vec2Add(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_Vector2f GL2D_Vec2Sub(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_Vector2f GL2D_Vec2Mult(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
GL2D_Vector2f GL2D_Vec2Div(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
float GL2D_Vec2DotProduct(GL2D_Vector2f vector1, GL2D_Vector2f vector2);
float GL2D_Vec2Magnitude(GL2D_Vector2f vector);
float GL2D_Vec2MagnitudeSquared(GL2D_Vector2f vector);
GL2D_Vector2f GL2D_Vec2Normalize(GL2D_Vector2f vector);
GL2D_Vector2f GL2D_Vec2Rotate(GL2D_Vector2f vector, float angle);

GL2D_Matrix4f GL2D_Mat4Identity();
GL2D_Matrix4f GL2D_Mat4Ortho(float left, float right, float bottom, float top, float near, float far);
GL2D_Matrix4f GL2D_Mat4Perspective(float fov, float aspect, float near, float far);
GL2D_Matrix4f GL2D_Mat4Translate(GL2D_Matrix4f matrix, GL2D_Vector2f vector, float z);
GL2D_Matrix4f GL2D_Mat4Scale(GL2D_Matrix4f matrix, GL2D_Vector2f vector);
GL2D_Matrix4f GL2D_Mat4Rotate(GL2D_Matrix4f matrix, float angle);
GL2D_Matrix4f GL2D_Mat4Add(GL2D_Matrix4f matrix1, GL2D_Matrix4f matrix2);
GL2D_Matrix4f GL2D_Mat4Sub(GL2D_Matrix4f matrix1, GL2D_Matrix4f matrix2);
GL2D_Matrix4f GL2D_Mat4Mult(GL2D_Matrix4f matrix1,GL2D_Matrix4f matrix2);
GL2D_Vector2f GL2D_Mat4Vec2Mult(GL2D_Matrix4f matrix, GL2D_Vector2f vector);

void GL2D_SetViewport(GL2D_Rectf rect);
void GL2D_SetClearColor(GL2D_Color color);

GL2D_Context GL2D_CreateContext();
void GL2D_DestroyContext(GL2D_Context context);
void GL2D_SetContextCurrent(GL2D_Context context);
void GL2D_ClearContext();

void GL2D_DrawTexture(GL2D_Texture texture, GL2D_Rectf rect);

GL2D_Matrix4f GL2D_CamComputeView(GL2D_Camera camera);
GL2D_Matrix4f GL2D_CamComputeProjection(GL2D_Camera camera);

GL2D_Shader GL2D_CreateShaderFromFiles(const char* vertexPath, const char* fragmentPath);
GL2D_Shader GL2D_CreateShaderFromStrings(const char* vertexSource, const char* fragmentSource);
void GL2D_DestroyShader(GL2D_Shader shader);
void GL2D_BindShader(GL2D_Shader shader);
void GL2D_ShaderSetBool(GL2D_Shader shader, const char* name, bool value);
void GL2D_ShaderSetInt(GL2D_Shader shader, const char* name, int value);
void GL2D_ShaderSetFloat(GL2D_Shader shader, const char* name, float value);
void GL2D_ShaderSetVec2(GL2D_Shader shader, const char* name, GL2D_Vector2f value);
void GL2D_ShaderSetMat4(GL2D_Shader shader, const char* name, GL2D_Matrix4f value);
void GL2D_ShaderSetCamera(GL2D_Shader shader, const char* view, const char* projection, GL2D_Camera camera);

GL2D_Texture GL2D_CreateTextureFromFile(const char* path);
void GL2D_DestroyTexture(GL2D_Texture texture);
void GL2D_BindTexture(GL2D_Texture texture);

GL2D_VertexArray GL2D_CreateVertexArray();
void GL2D_DestroyVertexArray(GL2D_VertexArray vao);
void GL2D_BindVertexArray(GL2D_VertexArray vao);

GL2D_VertexBuffer GL2D_CreateVertexBuffer(const float* vertices, uint64_t size);
void GL2D_DestroyVertexBuffer(GL2D_VertexBuffer vbo);
void GL2D_BindVertexBuffer(GL2D_VertexBuffer vbo);

GL2D_IndexBuffer GL2D_CreateIndexBuffer(const uint32_t* indices, uint64_t size);
void GL2D_DestroyIndexBuffer(GL2D_IndexBuffer ibo);
void GL2D_BindIndexBuffer(GL2D_IndexBuffer ibo);

#endif