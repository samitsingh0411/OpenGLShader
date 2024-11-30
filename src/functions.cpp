#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include "project/fucntions.h"
#include <vector>

// WIndow and Context
extern SDL_Window* MyWindow;
extern SDL_GLContext MyContext;
extern bool gQuit;

// VBO and VAO
extern GLuint gVertexArrayObject;
extern GLuint gVertexBufferObject;

// Graphics Pipeline
extern GLuint gGraphicsPipelineProgram;

const char* gVertexShaderSource =
"#version 460 core\n"
"in vec4 position;\n"
"void main() {\n"
"	gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
"}\n";

const char* gFragmentShaderSource =
"#version 460 core\n"
"out vec4 color;\n"
"void main() {\n"
"	color = vec4(0.361f, 0.204f, 0.0f, 1.f);\n"
"}\n";

// Initializer Functions
void intializer(int height, int width) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Failed to initialize SDL" << std::endl;
		exit(-1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	MyWindow = SDL_CreateWindow("Hello World!", 100, 100, height, width, SDL_WINDOW_OPENGL);
	if (!MyWindow) {
		std::cout << "Window Creation Failed" << std::endl;
		exit(-1);
	}


	MyContext = SDL_GL_CreateContext(MyWindow);
	if (!MyContext) {
		std::cout << "Context creation failed" << std::endl;
		SDL_DestroyWindow(MyWindow);
		exit(-1);
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cout << "Glad failed to initialize" << std::endl;
	}

	std::cout << "Initialized SDL and OpenGL Context and assgined context to window\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

}

// Creating Pipeline
GLuint CompileShader(GLuint ShaderType, const char* ShaderSource) {

	GLuint ShaderObject = 0;
	
	if (ShaderType == GL_VERTEX_SHADER) {
		ShaderObject = glCreateShader(ShaderType);
	}

	else if (ShaderType == GL_FRAGMENT_SHADER) {
		ShaderObject = glCreateShader(ShaderType);
	}

	glShaderSource(ShaderObject, 1, &ShaderSource, nullptr);
	glCompileShader(ShaderObject);

	GLint isCompiled = 0;
	glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled) {
		char infoLog[512];
		glGetShaderInfoLog(ShaderObject, 512, nullptr, infoLog);
		std::cout << "Shader Compilation Failed: " << infoLog << std::endl;
		exit(-1);
	}
	
	std::cout << "Compiled and attached shaders source with shader object\n";

	return ShaderObject;
}

GLuint CreateShaderProgram(const char* VertexShaderSource, const char* FragmentShaderSrouce) {

	// Creating an program object which is going to hold the program
	GLuint ProgramObject = glCreateProgram();
	std::cout << "Created ProgramObject\n";
	// Creating a vertex and fragment shader object

	std::cout << "Created Vertex Shader Object\n";
	GLuint VertexShaderObject = CompileShader(GL_VERTEX_SHADER, gVertexShaderSource);
	std::cout << "Created Fragment Shader Object\n";
	GLuint FragemtnShaderObject = CompileShader(GL_FRAGMENT_SHADER, gFragmentShaderSource);

	glAttachShader(ProgramObject, VertexShaderObject);
	glAttachShader(ProgramObject, FragemtnShaderObject);

	glLinkProgram(ProgramObject);
	GLint isLinked;
	glGetProgramiv(ProgramObject, GL_LINK_STATUS, &isLinked);

	if (!isLinked) {
		char infoLog[512];
		glGetProgramInfoLog(ProgramObject, 512, nullptr, infoLog);
		std::cout << "Program Linking Failed: " << infoLog << std::endl;
		exit(-1);
	}

	else {
		std::cout << "Attached Shader to program object\n";
	}

	// Verifying 
	glValidateProgram(ProgramObject);
	std::cout << "Returning Program Object\n";
	return ProgramObject;

}

// Creating the graphics pipleline
void CreateGraphicsPipeline() {
	gGraphicsPipelineProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
	std::cout << "Creation of Graphics Pipeline Complete" << std::endl;
}

// Creating Vertex Specs	
void CreateVertexSpecs() {

	// Declaring our vertex data
	const std::vector<GLfloat> VertexPositionList = {
		//x	 //y   //z
		0.8f,  0.0f,  0.0f,
		-0.8f, 0.0f,  0.0f,
		0.0f,  0.8f,  0.0f
	};

	// Generating and selecting vertex array object, this determines how we move through VBO
	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);
	std::cout << "Created and Binded Vertex Array Object\n";
	
	// Generating and selecting vertex buffer object, this is what will hold our data
	glGenBuffers(1, &gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	std::cout << "Created and Binded Vertex Buffer Object\n";

	// Setting VBO and VAO 
		// Setting VBO
	glBufferData(GL_ARRAY_BUFFER, VertexPositionList.size() * sizeof(GLfloat), VertexPositionList.data(), GL_STATIC_DRAW); 
	std::cout << "put data into the vertex buffer object";
		// Setting VAO
	glEnableVertexAttribArray(0); // This enables vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	
	std::cout << "enabled set the vertex array obejct specs\n";

	// Unbinding and Diabling stuff so that we don't write to them again
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	std::cout << "Unbound vertex array and vertex buffer object" << std::endl;
}


// Main loop functions
void mainloop() {
	while (!gQuit) {

		Input();

		PreDraw();

		Draw();

		// Swaps Buffer Window with displaying window
		SDL_GL_SwapWindow(MyWindow);
		std::cout << "Swapped The Buffer\n";

	}

}

void Input() {
	SDL_Event e;
	// We put events which we want to happen every frame regardless of user input here too.
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			std::cout << "GoodBye" << std::endl;
			gQuit = true;
		}
	}
	std::cout << "Looked for input\n";
}

void PreDraw() {

	std::cout << "PreDraw Function Called\n";

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glViewport(0, 0, 640, 480);
	glClearColor(0.020f, 0.271f, 0.267f, 0.75f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(gGraphicsPipelineProgram);
}

void Draw() {

	std::cout << "Draw Functoin Called\n";

	glBindVertexArray(gVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Cleanup Functions
void cleanup() {
	std::cout << "Clearned Up!" << std::endl;
	SDL_DestroyWindow(MyWindow);
	SDL_Quit();
}