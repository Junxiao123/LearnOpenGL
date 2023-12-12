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
      // positions          // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // top left
  };
  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // load and create a texture
  // -------------------------
  unsigned int texture1, texture2;
  // texture 1
  // ---------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);  // tell stb_image.h to flip loaded texture's on the y-axis.

  std::string project_dir = std::string(PROJECT_PATH) + "/CoordinateSystems";
  std::string shader_dir = project_dir + "/shader/";
  Shader ourShader(shader_dir + "vertex.vs", shader_dir + "fragment.fs");

  std::string image_dir = project_dir + "/image/";
  std::string img_path = std::string(image_dir + "img1.jpg");

  unsigned char* data = stbi_load(img_path.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    LOG(ERROR) << "Failed to load texture1";
  }
  stbi_image_free(data);
  // texture 2
  // ---------
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps

  img_path = std::string(image_dir + "monado.png");
  data = stbi_load(img_path.c_str(), &width, &height, &nrChannels, 0);

  if (data) {
    LOG(INFO) << "===WIDTH:" << width << " height: " << height;
    // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to
    // tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    LOG(ERROR) << "Failed to load texture2";
  }
  stbi_image_free(data);

  // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
  // -------------------------------------------------------------------------------------------
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);
  glm::mat4 projection(1.0f);

  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

  ourShader.setMat4("model", model);
  ourShader.setMat4("view", view);
  ourShader.setMat4("projection", projection);

  // render loop
  // -----------
  while (!window.ShouldClose()) {
    window.ProcessInput();

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

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
