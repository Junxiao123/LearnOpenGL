#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glog/logging.h>
#include <linux/limits.h>
#include "common/shader.h"
#include "common/env.h"
#include "common/window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <test/test.h>

#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"

int main(int argc, char const* argv[]) {
  google::InitGoogleLogging("argv[0]");

  FLAGS_stderrthreshold = google::INFO;
  FLAGS_colorlogtostderr = true;
  test();

  LOG(INFO) << "==================Hello start===================";
  LOG(INFO) << "GLM Version: " << GLM_VERSION_MAJOR << "." << GLM_VERSION_MINOR << "."
            << GLM_VERSION_PATCH << std::endl;

  Window window;
  window.Init();

  /**************************************修改部分*****************************************/

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      // positions          // colors       // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
  };
  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  std::string project_dir = std::string(PROJECT_PATH) + "/Transformations";
  std::string shader_dir = project_dir + "/shader/";
  Shader ourShader(shader_dir + "vertex.vs", shader_dir + "fragment.fs");

  // create vertex buffer
  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Tell opengl how to parse these vertex datas
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // position attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  std::string image_dir = project_dir + "/image/";
  std::string img_path = std::string(image_dir + "img1.jpg");
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(img_path.c_str(), &width, &height, &nrChannels, 0);
  LOG(INFO) << "image_path==== " << img_path.c_str();

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    LOG(ERROR) << "Load img error, path: " << img_path;
    return -1;
  }

  // Test translation vec (1, 0, 0) translate to (1, 1, 0)
  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans = glm::mat4(1.0f);  // 4x4 unit matrix
  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
  vec = trans * vec;
  LOG(INFO) << "after translate: (" << vec.x << ", " << vec.y << ", " << vec.z << ")";

  // ourShader.use();
  // // rotate 90 deg by z aixs, then scale 0.5 xyz
  // glm::mat4 trans1 = glm::mat4(1.0f);
  // trans1 = glm::rotate(trans1, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  // trans1 = glm::scale(trans1, glm::vec3(0.5f, 0.5f, 0.5f));

  // unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
  // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans1));

  // render loop
  // -----------
  while (!window.ShouldClose()) {
    window.ProcessInput();

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // create transformations
    glm::mat4 transform =
        glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    // get matrix's uniform location and set matrix
    ourShader.use();
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // Draw triangle
    ourShader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // 交换缓冲并查询IO事件
    window.SwapAndPoll();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  /**************************************修改部分*****************************************/

  window.Terminate();
  return 0;
}
