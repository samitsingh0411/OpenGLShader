#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include "project/fucntions.h"
#include <vector>
#include <fstream>
#include <sstream>

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

// WIndow and Context
extern SDL_Window* MyWindow;
extern SDL_GLContext MyContext;
extern bool gQuit;

// VBO and VAO
extern GLuint gVertexArrayObject;
extern GLuint gVertexBufferObject;

// Graphics Pipeline
extern GLuint gGraphicsPipelineProgram;

extern std::string gVertexShaderSource;

extern std::string gFragmentShaderSource;


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//


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


//###################################################################################################//


// Creating Pipeline

std::string LoadShaderString(std::string file_path) {

	std::string Line = "";
	std::string SourceCode = "";

	std::fstream ShaderFile(file_path.c_str());

	if (!ShaderFile) {
		std::cout << "Failed to open file " << std::endl;
		exit(-1);
		return nullptr;
	}

	while (std::getline(ShaderFile, Line)) {
		SourceCode += Line + "\n";
	}

	return SourceCode;

}

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
	GLuint VertexShaderObject = CompileShader(GL_VERTEX_SHADER, gVertexShaderSource.c_str());
	std::cout << "Created Fragment Shader Object\n";
	GLuint FragemtnShaderObject = CompileShader(GL_FRAGMENT_SHADER, gFragmentShaderSource.c_str());

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

void CreateGraphicsPipeline() {

	gVertexShaderSource = LoadShaderString("C:\\Users\\samit\\source\\repos\\OpenGL Shader\\Shaders\\vertex.glsl");
	gFragmentShaderSource = LoadShaderString("C:\\Users\\samit\\source\\repos\\OpenGL Shader\\Shaders\\frag.glsl");

	std::cout << "Vertex Shader: \n" << gVertexShaderSource << "\n\n\n\n";
	std::cout << "Fragment Shader: \n" << gFragmentShaderSource << std::endl;

	gGraphicsPipelineProgram = CreateShaderProgram(gVertexShaderSource.c_str(), gFragmentShaderSource.c_str());
	std::cout << "Creation of Graphics Pipeline Complete" << std::endl;
}


//###################################################################################################//


// Creating Vertex Specs	
void CreateSpecs() {

	// Declaring our vertex data
	const std::vector<GLfloat> VertexDataList = {
		//x	 //y   //z
		-0.5f, -0.5f,  0.0f,
		1.0f,  0.0f,  0.0f, // Color of vertex 1 
	    
		0.5f, -0.5f,  0.0f,
		0.0f,  1.0f,  0.0f, // Color of vertex 2

		-0.5f,  0.5f,  0.0f,
		0.0f,  0.0f,  1.0f // Color of vertex 3
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
	glBufferData(GL_ARRAY_BUFFER, VertexDataList.size() * sizeof(GLfloat), VertexDataList.data(), GL_STATIC_DRAW); 
	std::cout << "put data into the vertex buffer object";
	
		// Setting VAO
	
			// Setting Position
	glEnableVertexAttribArray(0); // This enables vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (void*)0);
	std::cout << "Set position attrib\n";

			// Setting Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6, (GLvoid*)(sizeof(GLfloat) * 3));
	std::cout << "Set color attrib\n";

	// Unbinding and Diabling stuff so that we don't write to them again
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	std::cout << "Unbound vertex array and vertex buffer object" << std::endl;
}


//###################################################################################################//


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

//###################################################################################################//

// Cleanup Functions
void cleanup() {
	std::cout << "Clearned Up!" << std::endl;
	SDL_DestroyWindow(MyWindow);
	SDL_Quit();
}
