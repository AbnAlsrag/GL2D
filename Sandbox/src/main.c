#include <stb/stb_image.h>
#include <stdio.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <gl2d.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"

"void main() {\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    "ourColor = aColor;\n"
    "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main() {\n"
    "FragColor = texture(texture1, TexCoord);\n"
"}";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    GL2D_Renderer renderer = GL2D_CreateRenderer();

    // build and compile our shader zprogram
    // ------------------------------------
    GL2D_Shader shader = GL2D_CreateShaderFromFiles("assets/vertex.shader", "assets/fragment.shader");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
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

    // load and create a texture 
    // -------------------------
    GL2D_Texture texture = GL2D_CreateTextureFromFile("assets/i.jpg");

    GL2D_SetClearColor(GL2D_COLOR_GOODIE);

    GL2D_Vector2F pos = { 0 };
    GL2D_Matrix4 transform = { 0 };
    GL2D_Mat4InitIdentity(&transform);

    GL2D_Matrix4 mat1 = { 0 };

    mat1.data[0][0] = 4;    mat1.data[1][0] = 5;    mat1.data[2][0] = 0;    mat1.data[3][0] = 0;
    mat1.data[0][1] = 6;    mat1.data[1][1] = 7;    mat1.data[2][1] = 0;    mat1.data[3][1] = 0;
    mat1.data[0][2] = 0;    mat1.data[1][2] = 0;    mat1.data[2][2] = 0;    mat1.data[3][2] = 0;
    mat1.data[0][3] = 0;    mat1.data[1][3] = 0;    mat1.data[2][3] = 0;    mat1.data[3][3] = 0;

    GL2D_Matrix4 mat2 = { 0 };

    mat2.data[0][0] = 3;    mat2.data[1][0] = 2;    mat2.data[2][0] = 0;    mat2.data[3][0] = 0;
    mat2.data[0][1] = 1;    mat2.data[1][1] = 0;    mat2.data[2][1] = 0;    mat2.data[3][1] = 0;
    mat2.data[0][2] = 0;    mat2.data[1][2] = 0;    mat2.data[2][2] = 0;    mat2.data[3][2] = 0;
    mat2.data[0][3] = 0;    mat2.data[1][3] = 0;    mat2.data[2][3] = 0;    mat2.data[3][3] = 0;

    GL2D_Matrix4 result = GL2D_Mat4Mult(mat1, mat2);

    for (uint32_t i = 0; i < 4; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            printf("%f ", result.data[i][j]);
        }
        printf("\n");
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            pos.x += 0.001f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            pos.x -= 0.001f;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            pos.y += 0.001f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            pos.y -= 0.001f;
        }

        // render
        // ------

        // bind Texture
        GL2D_BindTexture(texture);

        // render container
        GL2D_BindShader(shader);

        GL2D_ShaderSetMat4(shader, "transform", transform);

        GL2D_BindVertexArray(VAO);

        GL2D_UpdateRenderer(renderer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    GL2D_DestroyVertexArray(VAO);
    GL2D_DestroyVertexBuffer(VBO);
    GL2D_DestroyIndexBuffer(IBO);
    GL2D_DestroyShader(shader);
    GL2D_DestroyTexture(texture);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    GL2D_SetViewport((GL2D_Rect){ 0, 0, width, height });
}