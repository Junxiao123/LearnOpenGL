#ifndef COMMON_WINDOW_H_
#define COMMON_WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

class Window {
public:
  void Init();
  void Terminate();
  void SwapAndPoll();
  void ProcessInput();
  bool ShouldClose();

private:
  GLFWwindow *window = nullptr;
};

#endif // COMMON_WINDOW_H_