#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <Camera.hpp>
#include <Shader.hpp>

void windowCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

float lerp(float start, float end, float amount);
unsigned int loadTexture(char const *path);

void processInput(GLFWwindow* window);

GLFWwindow* window;
int width = 1200; int height = 800;

// Initailize Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//timing
float lastFrame = 0.0f;
float deltaTime = 0.0f;

//mouse coords
double lastX; double lastY;
bool firstMouse = true;

float vertices[] = {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float triangle[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

float plane[] = {
    10.0f, -0.5f, 10.0f, 2.0f, 0.0f,
    -10.0f, -0.5f, 10.0f, 0.0f, 0.0f,
    -10.0f, -0.5f, -10.0f, 0.0f, 2.0f,

    10.0f, -0.5f, 10.0f, 2.0f, 0.0f,
    -10.0f, -0.5f, -10.0f, 0.0f, 2.0f,
    10.0f, -0.5f, -10.0f, 2.0f, 2.0f,
};

int main(){
    if (glfwInit() != GLFW_TRUE){
        std::cout << "Failed to initailize GLFW." << std::endl;
        return -1;
    }
    window = glfwCreateWindow(width, height, "Shooter", NULL, NULL);
    if (!window){
        std::cout << "Failed to intiailzie a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set GLFW context stuff before intialising GLEW
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //Setup window for ani-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4); //4sampled poitns per fragment. More sample point smore detail

    //Function callbacks
    glfwSetFramebufferSizeCallback(window, windowCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);

    //Initialize glew
    if (glewInit() != GLEW_OK){
        std::cout << "Failed to intialize GLEW." << std::endl;
        glfwTerminate();
        return -1;
    }

    //enable depth etsting
    glEnable(GL_DEPTH_TEST);
    //Enable anti-aliasing
    glEnable(GL_MULTISAMPLE);

    // Initialize Shaders
    Shader shader("assets\\shaders\\cube.vs", "assets\\shaders\\cube.fs");

    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //position attribuite
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Texture attribuite
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    //plane atrtibs
    unsigned int planeVBO, planeVAO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    glBindVertexArray(planeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

    //position attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture attrib
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //load file
    unsigned int texture = loadTexture("assets\\textures\\crate.jpg");
    unsigned int floor = loadTexture("assets\\textures\\floor.jpg");

    shader.use();
    shader.setInt("texture", texture);
    //shader.setInt("texture1", floor);

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //claculaet delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        
        shader.use();

        //Camera model
        glm::mat4 view = glm::mat4(1.0);
        glm::mat4 projection = glm::mat4(1.0);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        //render cube
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        //render plane
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor);
        model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(window);
        glfwPollEvents();

        
    }
    glfwTerminate();
    return 0;
}

void windowCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(JUMP, deltaTime);

    //Jumping
    if (camera.jumping == true) {
        if (camera.Position.y >= 0.0f && camera.Position.y <= 1.0f && camera.jumpingDaccel == false)
            camera.Position.y += 0.8 * (camera.MovementSpeed * deltaTime);
        if (camera.Position.y >= 1.2f)
            camera.jumpingDaccel = true;
        if (camera.jumpingDaccel == true && camera.Position.y >= 0.0f)
            camera.Position.y -= 0.8f * (camera.MovementSpeed * deltaTime);
        if (camera.Position.y > 1.0f || camera.Position.y < 0.0f)
            camera.jumpingDaccel = true;
            //if (camera.Position.y <= 0.0f)
            camera.jumping = false;
    }
    else {
        camera.Position.y = 0.0f;
        camera.jumping = false; 
        camera.jumpingDaccel = false;
    }
    //std::cout << camera.jumping << std::endl;
        

        /*if (camera.Position.y >= 0.5f)
            camera.jumpingDaccel = true;
        else if (camera.jumpingDaccel == false && camera.Position.y >= 0.0f)
            camera.Position.y += 0.8 * (camera.MovementSpeed * deltaTime);
        
        if (camera.jumpingDaccel == true)
            camera.Position.y -= 0.8 * (camera.MovementSpeed * deltaTime);
            if (camera.Position.y <= 0.0f){
                camera.Position.y = 0.0f;
                camera.jumpingDaccel = false;
            }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
            camera.jumpWait = true;
        if (camera.jumpWait == true){
            if (camera.Position.y <= 0.0f){
                camera.Position.y = 0.0f;
                camera.jumpingDaccel = false;
                camera.jumping = false;
                camera.jumpWait = false;
            }
        }*/
        //camera.Position.y += 0.06;
        /*if (camera.Position.y >= 0.5f)
            camera.jumpingDaccel = true;
        else if (camera.jumpingDaccel == false && camera.Position.y >= 0.0f)
            //camera.Position.y += lerp(camera.Position.y, 1.0f, 0.3) * deltaTime;
            camera.Position.y += 0.4 * camera.MovementSpeed * deltaTime;

        if (camera.jumpingDaccel == true)
            //camera.Position.y -= lerp(camera.Position.y, 1.0f, 0.01f) * (camera.MovementSpeed * deltaTime);
            camera.Position.y -= 0.4 * camera.MovementSpeed * deltaTime;
            if (camera.Position.y <= 0.0f){
                camera.Position.y = 0.0f;
                camera.jumpingDaccel = false;
            }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
            camera.jumpWait = true;
        if (camera.jumpWait == true){
            if (camera.Position.y <= 0.0f){
                camera.Position.y = 0.0f;
                camera.jumpingDaccel = false;
                camera.jumping = false;
                camera.jumpWait = false;
            }
        }*/


}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    /*if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        camera.Position.y -= 0.5;
        
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        //camera.Zoom = 45;
        camera.Position.y += 0.5f;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        camera.Position.y += 1.0f;
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        camera.Position.y -= 1.0f;*/
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

    lastX = xpos;
    lastY = ypos;
}

float lerp(float start, float end, float amount){
    return start + amount * (end - start);
}

unsigned int loadTexture(char const *path){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data){
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        if (nrChannels == 3)
            format = GL_RGB;
        if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Failed to laod texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}