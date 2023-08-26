#ifdef GL2D_DLL_BUILD
    #define GL2D_USE_DLL
#endif

#include "gl2d.h"

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#undef GL2D_API
#ifdef GL2D_DLL_BUILD 
    #ifdef GL2D_PLATFORM_WINDOWS
        #define GL2D_API __declspec(dllexport)
    #elif defined(GL2D_PLATFORM_GCC)
        #define GL2D_API __attribute__((visibility("default")))
    #endif
#else
    #define GL2D_API
#endif

#ifdef GL2D_PLATFORM_WINDOWS
    #pragma warning(disable:4996)
#endif

#define PI 3.14159265359

static char* ReadFile(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "Error: Couldn't open file %s: %s\n", path, strerror(errno));
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    uint64_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(fileSize + 1);

    if (buffer == NULL) {
        fprintf(stderr, "Error: Couldn't allocate buffer: %s\n", strerror(errno));
        return NULL;
    }

    buffer[fileSize] = '\0';

    size_t n = fread(buffer, sizeof(char), fileSize, file);

    if (n != fileSize) {
        fprintf(stderr, "ERROR: Couldn't read file %s: %s\n", path, strerror(errno));
        return NULL;
    }

    fclose(file);

    return buffer;
}

const GL2D_Color
GL2D_COLOR_GOODIE = { 51, 76, 76, 255 }, GL2D_COLOR_GREY = { 192, 192, 192, 255 },
GL2D_COLOR_DARK_GREY = { 128, 128, 128, 255 }, GL2D_COLOR_VERY_DARK_GREY = { 64, 64, 64, 255 },
GL2D_COLOR_RED = { 255, 0, 0, 255 }, GL2D_COLOR_DARK_RED = { 128, 0, 0, 255 },
GL2D_COLOR_VERY_DARK_RED = { 64, 0, 0, 255 }, GL2D_COLOR_YELLOW = { 255, 255, 0, 255 },
GL2D_COLOR_DARK_YELLOW = { 128, 128, 0, 255 }, GL2D_COLOR_VERY_DARK_YELLOW = { 64, 64, 0, 255 },
GL2D_COLOR_GREEN = { 0, 255, 0, 255 }, GL2D_COLOR_DARK_GREEN = { 0, 128, 0, 255 },
GL2D_COLOR_VERY_DARK_GREEN = { 0, 64, 0, 255 }, GL2D_COLOR_CYAN = { 0, 255, 255, 255 },
GL2D_COLOR_DARK_CYAN = { 0, 128, 128, 255 }, GL2D_COLOR_VERY_DARK_CYAN = { 0, 64, 64, 255 },
GL2D_COLOR_BLUE = { 0, 0, 255, 255 }, GL2D_COLOR_DARK_BLUE = { 0, 0, 128, 255 },
GL2D_COLOR_VERY_DARK_BLUE = { 0, 0, 64, 255 }, GL2D_COLOR_MAGENTA = { 255, 0, 255, 255 },
GL2D_COLOR_DARK_MAGENTA = { 128, 0, 128, 255 }, GL2D_COLOR_VERY_DARK_MAGENTA = { 64, 0, 64, 255 },
GL2D_COLOR_WHITE = { 255, 255, 255, 255 }, GL2D_COLOR_BLACK = { 0, 0, 0, 255 },
GL2D_COLOR_BLANK = { 0, 0, 0, 0 };

struct GL2D_Renderer {
    int unused;
};

struct GL2D_Shader {
    uint32_t id;
};

struct GL2D_Texture {
    uint32_t id;
    uint8_t slot;
    GL2D_Vector2F size;
};

struct GL2D_VertexArray {
    uint32_t id;
};

struct GL2D_VertexBuffer {
    uint32_t id;
};

struct GL2D_IndexBuffer {
    uint32_t id;
};

GL2D_API bool GL2D_VecEqual(GL2D_Vector2F vector1, GL2D_Vector2F vector2) {
    return (vector1.x == vector2.x && vector1.y == vector2.y);
}

GL2D_API GL2D_Vector2F GL2D_VecScalarAdd(GL2D_Vector2F vector, float value) {
    return (GL2D_Vector2F) { vector.x + value, vector.y + value };
}

GL2D_API GL2D_Vector2F GL2D_VecScalarSub(GL2D_Vector2F vector, float value) {
    return (GL2D_Vector2F) { vector.x - value, vector.y - value };
}

GL2D_API GL2D_Vector2F GL2D_VecScalarMult(GL2D_Vector2F vector, float value) {
    return (GL2D_Vector2F) { vector.x * value, vector.y * value };
}

GL2D_API GL2D_Vector2F GL2D_VecScalarDiv(GL2D_Vector2F vector, float value) {
    return (GL2D_Vector2F) { vector.x / value, vector.y / value };
}

GL2D_API GL2D_Vector2F GL2D_VecPower(GL2D_Vector2F vector, float value) {
    return (GL2D_Vector2F) { powf(vector.x, value), powf(vector.y, value) };
}

GL2D_API GL2D_Vector2F GL2D_VecAdd(GL2D_Vector2F vector1, GL2D_Vector2F vector2) {
    return (GL2D_Vector2F) { vector1.x + vector2.x, vector1.y + vector2.y };
}

GL2D_API GL2D_Vector2F GL2D_VecSub(GL2D_Vector2F vector1, GL2D_Vector2F vector2) {
    return (GL2D_Vector2F) { vector1.x - vector2.x, vector1.y - vector2.y };
}

GL2D_API GL2D_Vector2F GL2D_VecMult(GL2D_Vector2F vector1, GL2D_Vector2F vector2) {
    return (GL2D_Vector2F) { vector1.x * vector2.x, vector1.y * vector2.y };
}

GL2D_API GL2D_Vector2F GL2D_VecDiv(GL2D_Vector2F vector1, GL2D_Vector2F vector2) {
    return (GL2D_Vector2F) { vector1.x / vector2.x, vector1.y / vector2.y };
}

GL2D_API float GL2D_VecDotProduct(GL2D_Vector2F vector1, GL2D_Vector2F vector2) {
    return (vector1.x * vector2.x + vector1.y * vector2.y);
}

GL2D_API float GL2D_VecMagnitude(GL2D_Vector2F vector) {
    return sqrtf(GL2D_VecMagnitudeSquared(vector));
}

GL2D_API float GL2D_VecMagnitudeSquared(GL2D_Vector2F vector) {
    return (vector.x * vector.x + vector.y * vector.y);
}

GL2D_API GL2D_Vector2F GL2D_VecNormalize(GL2D_Vector2F vector) {
    float magnitude = GL2D_VecMagnitude(vector);

    return (GL2D_Vector2F) { vector.x / magnitude, vector.y / magnitude };
}

GL2D_API GL2D_Vector2F GL2D_VecRotate(GL2D_Vector2F vector, float angle) {
    float radians = angle * (PI / 180);
    float radCos = cosf(radians);
    float radSine = sinf(radians);

    return (GL2D_Vector2F) { vector.x * radCos - vector.y * radSine, vector.x * radSine + vector.y * radCos };
}

GL2D_API void GL2D_Mat4InitIdentity(GL2D_Matrix4* matrix) {
    matrix->data[0][0] = 1;    matrix->data[1][0] = 0;    matrix->data[2][0] = 0;    matrix->data[3][0] = 0;
    matrix->data[0][1] = 0;    matrix->data[1][1] = 1;    matrix->data[2][1] = 0;    matrix->data[3][1] = 0;
    matrix->data[0][2] = 0;    matrix->data[1][2] = 0;    matrix->data[2][2] = 1;    matrix->data[3][2] = 0;
    matrix->data[0][3] = 0;    matrix->data[1][3] = 0;    matrix->data[2][3] = 0;    matrix->data[3][3] = 1;
}

GL2D_API void GL2D_Mat4InitTranslation(GL2D_Matrix4* matrix, GL2D_Vector2F vector) {
    matrix->data[0][0] = 1;    matrix->data[1][0] = 0;    matrix->data[2][0] = 0;    matrix->data[3][0] = vector.x;
    matrix->data[0][1] = 0;    matrix->data[1][1] = 1;    matrix->data[2][1] = 0;    matrix->data[3][1] = vector.y;
    matrix->data[0][2] = 0;    matrix->data[1][2] = 0;    matrix->data[2][2] = 1;    matrix->data[3][2] = 1;
    matrix->data[0][3] = 0;    matrix->data[1][3] = 0;    matrix->data[2][3] = 0;    matrix->data[3][3] = 1;
}

GL2D_API void GL2D_Mat4InitScale(GL2D_Matrix4* matrix, GL2D_Vector2F vector) {
    matrix->data[0][0] = vector.x;    matrix->data[1][0] = 0;           matrix->data[2][0] = 0;    matrix->data[3][0] = 0;
    matrix->data[0][1] = 0;           matrix->data[1][1] = vector.y;    matrix->data[2][1] = 0;    matrix->data[3][1] = 0;
    matrix->data[0][2] = 0;           matrix->data[1][2] = 0;           matrix->data[2][2] = 1;    matrix->data[3][2] = 0;
    matrix->data[0][3] = 0;           matrix->data[1][3] = 0;           matrix->data[2][3] = 0;    matrix->data[3][3] = 1;
}

GL2D_API void GL2D_Mat4InitRotation(GL2D_Matrix4* matrix, GL2D_Vector2F vector);

GL2D_API GL2D_Matrix4 GL2D_Mat4Add(GL2D_Matrix4 matrix1, GL2D_Matrix4 matrix2) {
    GL2D_Matrix4 result = { 0 };

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            result.data[i][j] = matrix1.data[i][j] + matrix2.data[i][j];
        }
    }

    return result;
}

GL2D_API GL2D_Matrix4 GL2D_Mat4Sub(GL2D_Matrix4 matrix1, GL2D_Matrix4 matrix2) {
    GL2D_Matrix4 result = { 0 };

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            result.data[i][j] = matrix1.data[i][j] - matrix2.data[i][j];
        }
    }

    return result;
}

GL2D_API GL2D_Matrix4 GL2D_Mat4Mult(GL2D_Matrix4 matrix1, GL2D_Matrix4 matrix2) {
    GL2D_Matrix4 result = { 0 };

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            result.data[j][i] = matrix1.data[0][j] * matrix2.data[i][0] +
                                matrix1.data[1][j] * matrix2.data[i][1] +
                                matrix1.data[2][j] * matrix2.data[i][2] +
                                matrix1.data[3][j] * matrix2.data[i][3];
        }
    }

    return result;
}

GL2D_API GL2D_Vector2F GL2D_Mat4Vec2Mult(GL2D_Matrix4 matrix, GL2D_Vector2F vector) {
    float result[4] = { 0, 0, 0, 0 };
    float vec[4] = { vector.x, vector.y, 1, 1 };

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            result[i] += matrix.data[j][i] * vec[j];
        }
    }

    return (GL2D_Vector2F) { result[0], result[1] };
}

GL2D_API GL2D_Matrix4 GL2D_MatTransform(GL2D_Matrix4 matrix, GL2D_Vector2F vector);
GL2D_API GL2D_Matrix4 GL2D_MatScale(GL2D_Matrix4 matrix, GL2D_Vector2F vector);
GL2D_API GL2D_Matrix4 GL2D_MatRotate(GL2D_Matrix4 matrix, GL2D_Vector2F vector);

GL2D_API void GL2D_SetViewport(GL2D_Rect rect) {
    glViewport(rect.x, rect.y, rect.z, rect.w);
}

GL2D_API void GL2D_SetClearColor(GL2D_Color color) {
    float r = (float)color.r / 255;
    float g = (float)color.g / 255;
    float b = (float)color.b / 255;
    float a = (float)color.a / 255;

    glClearColor(r, g, b, a);
}

GL2D_API GL2D_Renderer GL2D_CreateRenderer() {
    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return NULL;
    }

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_FRAMEBUFFER_SRGB);

    GL2D_Renderer renderer = malloc(sizeof(struct GL2D_Renderer));

    if (renderer == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate renderer\n");
        return NULL;
    }

    memset(renderer, 0, sizeof(struct GL2D_Renderer));

    return renderer;
}

GL2D_API void GL2D_UpdateRenderer(GL2D_Renderer renderer) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GL2D_API GL2D_Shader GL2D_CreateShaderFromFiles(const char* vertexPath, const char* fragmentPath) {
    GL2D_Shader shader = malloc(sizeof(struct GL2D_Shader));

    if (shader == NULL) {
        printf("ERROR: can't allocate shader");
        return NULL;
    }

    memset(shader, 0, sizeof(struct GL2D_Shader));

    char* vertexSource = ReadFile(vertexPath);

    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
    }
    // fragment shader
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char* fragmentSource = ReadFile(fragmentPath);

    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
    }
    // link shaders
    shader->id = glCreateProgram();
    glAttachShader(shader->id, vertexShader);
    glAttachShader(shader->id, fragmentShader);
    glLinkProgram(shader->id);
    // check for linking errors
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->id, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader;
}

GL2D_API void GL2D_DestroyShader(GL2D_Shader shader) {
    glDeleteProgram(shader->id);
    free(shader);
}

GL2D_API void GL2D_BindShader(GL2D_Shader shader) {
    glUseProgram(shader->id);
}

GL2D_API void GL2D_ShaderSetBool(GL2D_Shader shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader->id, name), (int)value);
}

GL2D_API void GL2D_ShaderSetInt(GL2D_Shader shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

GL2D_API void GL2D_ShaderSetFloat(GL2D_Shader shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

GL2D_API void GL2D_ShaderSetVec2(GL2D_Shader shader, const char* name, GL2D_Vector2F value) {
    glUniform2f(glGetUniformLocation(shader->id, name), value.x, value.y);
}

GL2D_API void GL2D_ShaderSetMat4(GL2D_Shader shader, const char* name, GL2D_Matrix4 value) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, false, value.data);
}

GL2D_API GL2D_Texture GL2D_CreateTextureFromFile(const char* path) {
    GL2D_Texture texture = malloc(sizeof(struct GL2D_Texture));

    if (texture == NULL) {
        printf("ERROR: Couldn't allocate texture");
        return NULL;
    }

    memset(texture, 0, sizeof(struct GL2D_Texture));

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width = 0, height = 0, nrChannels = 0;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load("assets/i.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            printf("Failed to load texture\n");
        }
    }
    else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    texture->size.x = width;
    texture->size.y = height;

    return texture;
}

GL2D_API void GL2D_DestroyTexture(GL2D_Texture texture) {
    glDeleteTextures(1, &texture->id);
    free(texture);
}

GL2D_API void GL2D_BindTexture(GL2D_Texture texture) {
    glActiveTexture(GL_TEXTURE0 +  texture->slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

GL2D_API GL2D_VertexArray GL2D_CreateVertexArray() {
    GL2D_VertexArray vao = malloc(sizeof(struct GL2D_VertexArray));

    if (vao == NULL) {
        printf("ERROR: Couldn't allocate vao");
        return NULL;
    }

    memset(vao, 0, sizeof(struct GL2D_VertexArray));

    glGenVertexArrays(1, &vao->id);
    glBindVertexArray(vao->id);

    return vao;
}

GL2D_API void GL2D_DestroyVertexArray(GL2D_VertexArray vao) {
    glDeleteVertexArrays(1, &vao->id);
}

GL2D_API void GL2D_BindVertexArray(GL2D_VertexArray vao) {
    glBindVertexArray(vao->id);
}

GL2D_API GL2D_VertexBuffer GL2D_CreateVertexBuffer(const float* vertices, uint64_t size) {
    GL2D_VertexBuffer vbo = malloc(sizeof(struct GL2D_VertexBuffer));

    if (vbo == NULL) {
        printf("ERROR: Couldn't allocate vbo");
        return NULL;
    }

    memset(vbo, 0, sizeof(struct GL2D_VertexBuffer));

    glGenBuffers(1, &vbo->id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    return vbo;
}

GL2D_API void GL2D_DestroyVertexBuffer(GL2D_VertexBuffer vbo) {
    glDeleteBuffers(1, &vbo->id);
    free(vbo);
}

GL2D_API void GL2D_BindVertexBuffer(GL2D_VertexBuffer vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
}

GL2D_API GL2D_IndexBuffer GL2D_CreateIndexBuffer(const uint32_t* indices, uint64_t size) {
    GL2D_IndexBuffer ibo = malloc(sizeof(struct GL2D_IndexBuffer));

    if (ibo == NULL) {
        printf("ERROR: Couldn't allocate ibo");
        return NULL;
    }

    memset(ibo, 0, sizeof(struct GL2D_IndexBuffer));

    glGenBuffers(1, &ibo->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

    return ibo;
}

GL2D_API void GL2D_DestroyIndexBuffer(GL2D_IndexBuffer ibo) {
    glDeleteBuffers(1, &ibo->id);
    free(ibo);
}

GL2D_API void GL2D_BindIndexBuffer(GL2D_IndexBuffer ibo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
}