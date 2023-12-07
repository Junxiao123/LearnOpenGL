#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glog/logging.h>
#include <linux/limits.h>
#include "common/shader.h"
#include "common/env.h"
#include "common/window.h"
#include <cmath>

#include <iostream>

int main(int argc, char const *argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_stderrthreshold = google::INFO;
  FLAGS_colorlogtostderr = true;

  LOG(INFO) << "Hello Triangle start.";

  Window window;
  window.Init();

  /**************************************修改部分*****************************************/

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      // positions         // colors
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
  };

  std::string project_dir = std::string(PROJECT_PATH) + "/HelloShader";
  std::string shader_dir = project_dir + "/shader/";
  Shader ourShader(shader_dir + "vertex.vs", shader_dir + "fragment_uniform.fs");

  // create vertex buffer
  unsigned int VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Tell opengl how to parse these vertex datas
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // render loop
  // -----------
  while (!window.ShouldClose()) {
    window.ProcessInput();

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();

    // 更新uniform颜色
    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(ourShader.ID, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // Draw triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 交换缓冲并查询IO事件
    window.SwapAndPoll();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  /**************************************修改部分*****************************************/

  window.Terminate();
  return 0;
}
