// SrenderEngine.h
#ifndef SRENDERENGINE_H
#define SRENDERENGINE_H

#include "dependency.h"
#include "Swindow.h"
#include "SsignalEngine.h"
// ��Ⱦ������
class SrenderEngine {
private:
    bool isInit = false;
    Swindow* renderWindow;
public:
    SrenderEngine(const Swindow* window)
        : window(nullptr), shaderProgram(0), VAO(0), VBO(0) {
        if (window) {
            this->vertices=window->getVertices();
            this->renderWindow = static_cast<Swindow*>(const_cast<Swindow*>(window));
        }
    }

    ~SrenderEngine() {
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    bool init() {
        isInit = true;

        // �� Scolor ת��Ϊ RGBA ��ʽ�� float
        auto [r, g, b, a] = colorToFloat(this->renderWindow->getBackgroundColor());

        // ��ʼ��GLFW
        if (!glfwInit()) {
            std::cerr << "[SIMPLE-UI:ERROR] GLFW initialization failed!" << std::endl;
            return false;
        }

        this->window = glfwCreateWindow(this->renderWindow->getWidth(), this->renderWindow->getHeight(), this->renderWindow->getTitle().c_str(), nullptr, nullptr);
        glfwSetWindowPos(window, renderWindow->getX(), renderWindow->getY());
        // ����GLFW����

        // ������ֱͬ��
        glfwSwapInterval(1);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        if (!window) {
            std::cerr << "[SIMPLE-UI:ERROR] Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            return false;
        }

        // ����OpenGL������
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "[SIMPLE-UI:ERROR] Failed to initialize GLAD!" << std::endl;
            return false;
        }

        // ������ɫ������
        shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
        if (shaderProgram == 0) {
            return false;
        }

        // ���ô��ڱ�����ɫ
        glClearColor(r, g, b, a);

        // ׼����������
        prepareVertices();

        return true;
    }

    void setWindow(const Swindow* window) {
        if (window) {
            this->vertices = window->getVertices();
            this->renderWindow = static_cast<Swindow*>(const_cast<Swindow*>(window));
            this->window = toGLFWwindow(this->renderWindow);
        }
    }

    void render() {
        if (!isInit) {
            std::cerr << "[SIMPLE-UI:ERROR] Render engine is not initialized!" << std::endl;
            return;
        }
        // �����Ļ
        glClear(GL_COLOR_BUFFER_BIT);
        // ʹ����ɫ������
        glUseProgram(shaderProgram);
        // ����
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);
        glBindVertexArray(0);
        // ����������
        glfwSwapBuffers(window);
        // �����¼�
        glfwPollEvents();
    }

    bool shouldClose() const {
        return glfwWindowShouldClose(window);
    }

private:
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 aPosition;
        layout(location = 1) in vec4 aColor;
        out vec4 fragColor;
        void main() {
            gl_Position = vec4(aPosition, 0.0, 1.0);
            fragColor = aColor;
        }
    )";

    // Ƭ����ɫ��Դ��
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec4 fragColor;
        out vec4 FragColor;

        void main() {
            FragColor = fragColor;
        }
    )";

    std::vector<Vertex> vertices;
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint VAO, VBO;

    // ������ɫ������
    GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);

        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "Vertex Shader Compilation Failed: " << infoLog << std::endl;
            return 0;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "Fragment Shader Compilation Failed: " << infoLog << std::endl;
            return 0;
        }

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cerr << "Shader Program Linking Failed: " << infoLog << std::endl;
            return 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }

    void prepareVertices() {
        std::vector<float> vertexData;

        for (const auto& vertex : vertices) {
            for (int i = 0; i < 3; ++i) {
                vertexData.push_back(static_cast<float>(vertex.position[0])); // x
                vertexData.push_back(static_cast<float>(vertex.position[1])); // y

                auto [r, g, b, a] = colorToFloat(vertex.color[i]);
                vertexData.push_back(r);
                vertexData.push_back(g);
                vertexData.push_back(b);
                vertexData.push_back(a);
            }
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 修改为 2 个分量
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float))); // 调整偏移量
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};
#endif // SRENDERENGINE_H