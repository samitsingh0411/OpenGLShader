#pragma once
#include <SDL2/SDL.h>

void intializer(int height, int width);
void Input();
void PreDraw();
void Draw();
void mainloop();
void cleanup();
void CreateSpecs();
GLuint CreateShaderProgram(const char* VertexShaderSource, const char* FragmentShaderSrouce);
GLuint CompileShader(GLuint ShaderType, const char* ShaderSource);
void CreateGraphicsPipeline();
std::string LoadShaderString(std::string file_path);
