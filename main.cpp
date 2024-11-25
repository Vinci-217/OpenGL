#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>

// 粒子结构体
struct Particle {
    float position[3];
    float velocity[3];
    float color[4];
    float life; // 粒子生命周期
};

std::vector<Particle> particles; // 粒子容器
const size_t maxParticles = 100000; // 最大粒子数量
const float emitRate = 2000.0f;   // 每秒生成粒子数
float lastFrameTime = 0.0f;      // 上一帧时间

// 初始化粒子
void initParticle(Particle& particle) {
    particle.position[0] = -0.8f; // 中心底部
    particle.position[1] = 0.8f;
    particle.position[2] = 0.0f;

    // 初始速度，随机向上
    float angle = ((rand() % 100) / 100.0f) * 3.14f / 2.0f - 3.14f / 4.0f; // ±45度范围内
    float speed = ((rand() % 50) / 100.0f) + 0.5f;                         // 随机速度0.5-1.0
    particle.velocity[0] = speed * cos(angle);
    particle.velocity[1] = speed * sin(angle);
    particle.velocity[2] = 0.0f;

    // 颜色随机
    particle.color[0] = 0.5f + ((rand() % 50) / 100.0f);
    particle.color[1] = 0.5f + ((rand() % 50) / 100.0f);
    particle.color[2] = 1.0f;
    particle.color[3] = 1.0f; // 初始透明度

    particle.life = 1.0f; // 生命周期
}

// 更新粒子
void updateParticles(float deltaTime) {
    size_t newParticles = static_cast<size_t>(emitRate * deltaTime);
    for (size_t i = 0; i < newParticles && particles.size() < maxParticles; ++i) {
        Particle newParticle;
        initParticle(newParticle);
        particles.push_back(newParticle);
    }

    // 更新已有粒子
    for (size_t i = 0; i < particles.size(); ++i) {
        Particle& p = particles[i];
        if (p.life > 0.0f) {
            // 更新位置
            p.position[0] += p.velocity[0] * deltaTime;
            p.position[1] += p.velocity[1] * deltaTime;
            p.position[2] += p.velocity[2] * deltaTime;

            // 重力影响
            p.velocity[1] -= 1.5f * deltaTime;

            // 减少生命周期
            p.life -= deltaTime * 0.5f;
            p.color[3] = p.life; // 渐变透明度
        } else {
            // 粒子失效，重新初始化
            initParticle(p);
        }
    }
}

// 渲染粒子
void renderParticles() {
    glBegin(GL_POINTS);
    for (const Particle& p : particles) {
        if (p.life > 0.0f) {
            glColor4fv(p.color);
            glVertex3fv(p.position);
        }
    }
    glEnd();
}

// 初始化OpenGL
void setupOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(5.0f);
}

// 渲染循环
void renderLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        if (deltaTime > 0.1f) deltaTime = 0.1f; // 限制deltaTime最大值，防止低帧率时粒子异常
        lastFrameTime = currentFrameTime;

        glClear(GL_COLOR_BUFFER_BIT);

        updateParticles(deltaTime); // 更新粒子
        renderParticles();          // 渲染粒子

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// 主函数
int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Particle System", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    setupOpenGL();

    lastFrameTime = static_cast<float>(glfwGetTime());
    renderLoop(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
