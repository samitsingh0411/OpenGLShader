#pragma once
#include <SDL2/SDL.h>

void intializer(int height, int width);
void Input();
void PreDraw();
void Draw();
void mainloop();
void cleanup();
void CreateVertexSpecs();
GLuint CreateShaderProgram(const char* VertexShaderSource, const char* FragmentShaderSrouce);
void CreateGraphicsPipeline();
