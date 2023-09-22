#include "gl2d.h"

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#ifdef GL2D_PLATFORM_WINDOWS
    #pragma warning(disable:4996)
#endif

#define PI 3.1415926535897932

static GL2D_Context currentContext = NULL;

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

struct GL2D_Context {
    int unused;
};

struct GL2D_Shader {
    uint32_t id;
};

struct GL2D_Texture {
    uint32_t id;
    uint8_t slot;
    GL2D_Vector2f size;
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

bool GL2D_Vec2Equal(GL2D_Vector2f vector1, GL2D_Vector2f vector2) {
    return (vector1.x == vector2.x && vector1.y == vector2.y);
}

GL2D_Vector2f GL2D_Vec2ScalarAdd(GL2D_Vector2f vector, float value) {
    return (GL2D_Vector2f) { vector.x + value, vector.y + value };
}

GL2D_Vector2f GL2D_Vec2ScalarSub(GL2D_Vector2f vector, float value) {
    return (GL2D_Vector2f) { vector.x - value, vector.y - value };
}

GL2D_Vector2f GL2D_Vec2ScalarMult(GL2D_Vector2f vector, float value) {
    return (GL2D_Vector2f) { vector.x * value, vector.y * value };
}

GL2D_Vector2f GL2D_Vec2ScalarDiv(GL2D_Vector2f vector, float value) {
    return (GL2D_Vector2f) { vector.x / value, vector.y / value };
}

GL2D_Vector2f GL2D_Vec2Power(GL2D_Vector2f vector, float value) {
    return (GL2D_Vector2f) { powf(vector.x, value), powf(vector.y, value) };
}

GL2D_Vector2f GL2D_Vec2Add(GL2D_Vector2f vector1, GL2D_Vector2f vector2) {
    return (GL2D_Vector2f) { vector1.x + vector2.x, vector1.y + vector2.y };
}

GL2D_Vector2f GL2D_Vec2Sub(GL2D_Vector2f vector1, GL2D_Vector2f vector2) {
    return (GL2D_Vector2f) { vector1.x - vector2.x, vector1.y - vector2.y };
}

GL2D_Vector2f GL2D_Vec2Mult(GL2D_Vector2f vector1, GL2D_Vector2f vector2) {
    return (GL2D_Vector2f) { vector1.x * vector2.x, vector1.y * vector2.y };
}

GL2D_Vector2f GL2D_Vec2Div(GL2D_Vector2f vector1, GL2D_Vector2f vector2) {
    return (GL2D_Vector2f) { vector1.x / vector2.x, vector1.y / vector2.y };
}

float GL2D_Vec2DotProduct(GL2D_Vector2f vector1, GL2D_Vector2f vector2) {
    return (vector1.x * vector2.x + vector1.y * vector2.y);
}

float GL2D_Vec2Magnitude(GL2D_Vector2f vector) {
    return sqrtf(GL2D_Vec2MagnitudeSquared(vector));
}

float GL2D_Vec2MagnitudeSquared(GL2D_Vector2f vector) {
    return (vector.x * vector.x + vector.y * vector.y);
}

GL2D_Vector2f GL2D_Vec2Normalize(GL2D_Vector2f vector) {
    float magnitude = GL2D_Vec2Magnitude(vector);

    return (GL2D_Vector2f) { vector.x / magnitude, vector.y / magnitude };
}

GL2D_Vector2f GL2D_Vec2Rotate(GL2D_Vector2f vector, float angle) {
    float radians = angle * (PI / 180);
    float radCos = cosf(radians);
    float radSine = sinf(radians);

    return (GL2D_Vector2f) { vector.x * radCos - vector.y * radSine, vector.x * radSine + vector.y * radCos };
}

GL2D_Matrix4f GL2D_Mat4Identity() {
    GL2D_Matrix4f matrix;

    matrix.data[0][0] = 1;    matrix.data[1][0] = 0;    matrix.data[2][0] = 0;    matrix.data[3][0] = 0;
    matrix.data[0][1] = 0;    matrix.data[1][1] = 1;    matrix.data[2][1] = 0;    matrix.data[3][1] = 0;
    matrix.data[0][2] = 0;    matrix.data[1][2] = 0;    matrix.data[2][2] = 1;    matrix.data[3][2] = 0;
    matrix.data[0][3] = 0;    matrix.data[1][3] = 0;    matrix.data[2][3] = 0;    matrix.data[3][3] = 1;

    return matrix;
}

GL2D_Matrix4f GL2D_Mat4Ortho(float left, float right, float bottom, float top, float near, float far) {
    GL2D_Matrix4f matrix;

    float m00 = 2 / (right - left);
    float m11 = 2 / (top - bottom);
    float m22 = -2 / (far - near);
    float m30 = -(right + left) / (right - left);
    float m31 = -(top + bottom) / (top - bottom);
    float m32 = -(far + near) / (far - near);

    matrix.data[0][0] = m00;    matrix.data[1][0] = 0;     matrix.data[2][0] = 0;      matrix.data[3][0] = m30;
    matrix.data[0][1] = 0;     matrix.data[1][1] = m11;    matrix.data[2][1] = 0;      matrix.data[3][1] = m31;
    matrix.data[0][2] = 0;     matrix.data[1][2] = 0;      matrix.data[2][2] = m22;    matrix.data[3][2] = m32;
    matrix.data[0][3] = 0;     matrix.data[1][3] = 0;      matrix.data[2][3] = 0;      matrix.data[3][3] = 1;

    return matrix;
}

//TODO: add perspective matrix
GL2D_Matrix4f GL2D_Mat4Perspective(float fov, float aspect, float near, float far) {

}

GL2D_Matrix4f GL2D_Mat4Translate(GL2D_Matrix4f matrix, GL2D_Vector2f vector, float z) {
    GL2D_Matrix4f result = GL2D_Mat4Identity();

    result.data[0][0];    result.data[1][0];    result.data[2][0];    result.data[3][0] = vector.x;
    result.data[0][1];    result.data[1][1];    result.data[2][1];    result.data[3][1] = vector.y;
    result.data[0][2];    result.data[1][2];    result.data[2][2];    result.data[3][2] = z;
    result.data[0][3];    result.data[1][3];    result.data[2][3];    result.data[3][3];

    return GL2D_Mat4Mult(matrix, result);
}

GL2D_Matrix4f GL2D_Mat4Scale(GL2D_Matrix4f matrix, GL2D_Vector2f vector) {
    GL2D_Matrix4f result = GL2D_Mat4Identity();

    result.data[0][0] = vector.x;    result.data[1][0];               result.data[2][0];    result.data[3][0];
    result.data[0][1];               result.data[1][1] = vector.y;    result.data[2][1];    result.data[3][1];
    result.data[0][2];               result.data[1][2];               result.data[2][2];    result.data[3][2];
    result.data[0][3];               result.data[1][3];               result.data[2][3];    result.data[3][3];

    return GL2D_Mat4Mult(matrix, result);
}

GL2D_Matrix4f GL2D_Mat4Rotate(GL2D_Matrix4f matrix, float angle) {
    GL2D_Matrix4f result = GL2D_Mat4Identity();
    float radians = angle * (PI / 180);

    float radCos = cosf(radians);
    float radSine = sinf(radians);

    result.data[0][0] = radCos;     result.data[1][0] = -radSine;    result.data[2][0];   result.data[3][0];
    result.data[0][1] = radSine;    result.data[1][1] = radCos;      result.data[2][1];   result.data[3][1];
    result.data[0][2];              result.data[1][2];               result.data[2][2];   result.data[3][2];
    result.data[0][3];              result.data[1][3];               result.data[2][3];   result.data[3][3];

    return GL2D_Mat4Mult(matrix, result);
}

GL2D_Matrix4f GL2D_Mat4Add(GL2D_Matrix4f matrix1, GL2D_Matrix4f matrix2) {
    GL2D_Matrix4f result = { 0 };

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            result.data[i][j] = matrix1.data[i][j] + matrix2.data[i][j];
        }
    }

    return result;
}

GL2D_Matrix4f GL2D_Mat4Sub(GL2D_Matrix4f matrix1, GL2D_Matrix4f matrix2) {
    GL2D_Matrix4f result = { 0 };

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            result.data[i][j] = matrix1.data[i][j] - matrix2.data[i][j];
        }
    }

    return result;
}

GL2D_Matrix4f GL2D_Mat4Mult(GL2D_Matrix4f matrix1, GL2D_Matrix4f matrix2) {
    GL2D_Matrix4f result = { 0 };

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            result.data[i][j] = matrix1.data[0][j] * matrix2.data[i][0] +
                                matrix1.data[1][j] * matrix2.data[i][1] +
                                matrix1.data[2][j] * matrix2.data[i][2] +
                                matrix1.data[3][j] * matrix2.data[i][3];
        }
    }

    return result;
}

GL2D_Vector2f GL2D_Mat4Vec2Mult(GL2D_Matrix4f matrix, GL2D_Vector2f vector) {
    float result[4] = { 0, 0, 0, 0 };
    float vec[4] = { vector.x, vector.y, 1, 1 };

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            result[i] += matrix.data[j][i] * vec[j];
        }
    }

    return (GL2D_Vector2f) { result[0], result[1] };
}

void GL2D_SetViewport(GL2D_Rectf rect) {
    glViewport(rect.x, rect.y, rect.width, rect.height);
}

void GL2D_SetClearColor(GL2D_Color color) {
    float r = (float)color.r / 255;
    float g = (float)color.g / 255;
    float b = (float)color.b / 255;
    float a = (float)color.a / 255;

    glClearColor(r, g, b, a);
}

GL2D_Context GL2D_CreateContext() {
    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return NULL;
    }

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_FRAMEBUFFER_SRGB);

    GL2D_Context context = malloc(sizeof(struct GL2D_Context));

    if (context == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate context\n");
        return NULL;
    }

    memset(context, 0, sizeof(struct GL2D_Context));

    return context;
}

void GL2D_DestroyContext(GL2D_Context context) {

}

void GL2D_SetContextCurrent(GL2D_Context context) {
    return;
}

void GL2D_ClearContext() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL2D_DrawTexture(GL2D_Texture texture, GL2D_Rectf rect) {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    char* vertexSource = "#version 330 core\n"
                         "layout(location = 0) in vec3 aPos;\n"
                         "layout(location = 1) in vec3 aColor;\n"
                         "layout(location = 2) in vec2 aTexCoord;\n"

                         "out vec3 ourColor;\n"
                         "out vec2 TexCoord;\n"

                         "uniform mat4 model;\n"
                         "uniform mat4 view;\n"
                         "uniform mat4 projection;\n"

                         "void main()\n"
                         "{\n"
                         "    gl_Position = model * vec4(aPos, 1.0);\n"
                         "    ourColor = aColor;\n"
                         "    TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                         "}\n";

    char* fragmentSource = "#version 330 core\n"
                           "out vec4 FragColor;\n"

                           "in vec3 ourColor;\n"
                           "in vec2 TexCoord;\n"

                           "// texture sampler\n"
                           "uniform sampler2D texture1;\n"

                           "void main()\n"
                           "{\n"
                           "    FragColor = texture(texture1, TexCoord) * ourColor;\n"
                           "}\n";

    float vertices[] = {
        // positions       // colors         // texture coords
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right
       -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left
       -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    GL2D_Shader shader = GL2D_CreateShaderFromStrings(vertexSource, fragmentSource);
    GL2D_VertexArray VAO = GL2D_CreateVertexArray();
    GL2D_VertexBuffer VBO = GL2D_CreateVertexBuffer(vertices, sizeof(vertices));
    GL2D_IndexBuffer IBO = GL2D_CreateIndexBuffer(indices, sizeof(indices));

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GL2D_BindShader(shader);

    GL2D_Matrix4f transform = GL2D_Mat4Identity();
    transform = GL2D_Mat4Translate(transform, GL2D_VECTOR2F(rect.x, rect.y), 0.0f);
    transform = GL2D_Mat4Scale(transform, GL2D_VECTOR2F(rect.width, rect.height));

    GL2D_ShaderSetMat4(shader, "model", transform);

    GL2D_BindTexture(texture);

    GL2D_BindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    GL2D_DestroyShader(shader);
    GL2D_DestroyVertexArray(VAO);
    GL2D_DestroyVertexBuffer(VBO);
    GL2D_DestroyIndexBuffer(IBO);
}

GL2D_Matrix4f GL2D_CamComputeView(GL2D_Camera camera) {
    GL2D_Matrix4f matrix = GL2D_Mat4Identity();

    matrix = GL2D_Mat4Translate(matrix, camera.position, 0);

    return matrix;
}

GL2D_Matrix4f GL2D_CamComputeProjection(GL2D_Camera camera) {
    GL2D_Matrix4f matrix = GL2D_Mat4Identity();

    switch (camera.projection)
    {
    case GL2D_PROJECTION_ORTHOGRAPHIC:
        matrix = GL2D_Mat4Ortho(camera.orthographic.size.x, camera.orthographic.size.y, camera.orthographic.size.width, camera.orthographic.size.height,
            camera.orthographic.near, camera.orthographic.far);
    case GL2D_PROJECTION_PERSPECTIVE:
        assert("TODO: perspective projection");
    default:
        assert("The projection enum of the camera is wrong");
        break;
    }

    return matrix;
}

GL2D_Shader GL2D_CreateShaderFromFiles(const char* vertexPath, const char* fragmentPath) {
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

GL2D_Shader GL2D_CreateShaderFromStrings(const char* vertexSource, const char* fragmentSource) {
    GL2D_Shader shader = malloc(sizeof(struct GL2D_Shader));

    if (shader == NULL) {
        printf("ERROR: can't allocate shader");
        return NULL;
    }

    memset(shader, 0, sizeof(struct GL2D_Shader));

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

void GL2D_DestroyShader(GL2D_Shader shader) {
    glDeleteProgram(shader->id);
    free(shader);
}

void GL2D_BindShader(GL2D_Shader shader) {
    glUseProgram(shader->id);
}

void GL2D_ShaderSetBool(GL2D_Shader shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader->id, name), (int)value);
}

void GL2D_ShaderSetInt(GL2D_Shader shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void GL2D_ShaderSetFloat(GL2D_Shader shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void GL2D_ShaderSetVec2(GL2D_Shader shader, const char* name, GL2D_Vector2f value) {
    glUniform2f(glGetUniformLocation(shader->id, name), value.x, value.y);
}

void GL2D_ShaderSetMat4(GL2D_Shader shader, const char* name, GL2D_Matrix4f value) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, false, value.data);
}

void GL2D_ShaderSetCamera(GL2D_Shader shader, const char* view, const char* projection, GL2D_Camera camera) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, view), 1, false, GL2D_CamComputeView(camera).data);
    glUniformMatrix4fv(glGetUniformLocation(shader->id, projection), 1, false, GL2D_CamComputeProjection(camera).data);
}

GL2D_Texture GL2D_CreateTextureFromFile(const char* path) {
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

void GL2D_DestroyTexture(GL2D_Texture texture) {
    glDeleteTextures(1, &texture->id);
    free(texture);
}

void GL2D_BindTexture(GL2D_Texture texture) {
    glActiveTexture(GL_TEXTURE0 +  texture->slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

GL2D_VertexArray GL2D_CreateVertexArray() {
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

void GL2D_DestroyVertexArray(GL2D_VertexArray vao) {
    glDeleteVertexArrays(1, &vao->id);
}

void GL2D_BindVertexArray(GL2D_VertexArray vao) {
    glBindVertexArray(vao->id);
}

GL2D_VertexBuffer GL2D_CreateVertexBuffer(const float* vertices, uint64_t size) {
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

void GL2D_DestroyVertexBuffer(GL2D_VertexBuffer vbo) {
    glDeleteBuffers(1, &vbo->id);
    free(vbo);
}

void GL2D_BindVertexBuffer(GL2D_VertexBuffer vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
}

GL2D_IndexBuffer GL2D_CreateIndexBuffer(const uint32_t* indices, uint64_t size) {
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

void GL2D_DestroyIndexBuffer(GL2D_IndexBuffer ibo) {
    glDeleteBuffers(1, &ibo->id);
    free(ibo);
}

void GL2D_BindIndexBuffer(GL2D_IndexBuffer ibo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
}