#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stb/stb_image.h>
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

float RandomRange(float min, float max) {
    float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min);      /* [min, max] */
}

int main() {
    // glfw: initialize and configure
    // ------------------------------
    srand((uint32_t)time(NULL));

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
    GL2D_Context renderer = GL2D_CreateContext();
    //GL2D_MakeContextCurrent(renderer);

    // build and compile our shader zprogram
    // ------------------------------------
 
    // load and create a texture s
    // -------------------------
    GL2D_Texture texture = GL2D_CreateTextureFromFile("assets/i.jpg");

    GL2D_SetClearColor(GL2D_COLOR_GOODIE);
    
    GL2D_Camera camera = { .position = GL2D_VECTOR2F(0, 0),
        .projection = GL2D_PROJECTION_ORTHOGRAPHIC,
        .orthographic = GL2D_ORTHOGRAPHIC_SPECS(GL2D_RECTF(-1.0f, 1.0f, -1.0f, 1.0f), 0.0f, 100.0f) };

    GL2D_Rectf rect = GL2D_RECTF(0, -0.9999f, 0.3f, 0.3f);
    GL2D_Rectf enemy = GL2D_RECTF(0, 1.0f, 0.4f, 0.4f);

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
            rect.x += 0.005f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            rect.x -= 0.005f;
        }
        // render
        // ------

        // bind Texture

        // render container

        //GL2D_ShaderSetCamera(shader, "view", "projection", camera);

        if (enemy.y <= -0.9999f) {
            enemy = GL2D_RECTF(RandomRange(-1, 1), 1.0f, 0.4f, 0.4f);
        }

        enemy.y -= 0.004f;

        GL2D_ClearContext();

        GL2D_DrawTexture(texture, rect);
        GL2D_DrawTexture(texture, enemy);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    GL2D_DestroyTexture(texture);
    GL2D_DestroyContext(renderer);

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
    GL2D_SetViewport(GL2D_RECTF(0, 0, (float)width, (float)height));
}