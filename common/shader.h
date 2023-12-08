#ifndef COMMON_SHADER_H_
#define COMMON_SHADER_H_

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  // 程序ID
  unsigned int ID;

  // 构造器读取并构建着色器
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  // 使用/激活程序
  void use();
  // uniform工具函数
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setTransform(const std::string &name, const glm::mat4 &mat);

private:
  void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // COMMON_SHADER_H_