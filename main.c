#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include "shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(){
	if(!glfwInit()){
		printf("Failed to init GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 700, "Voxel Engine", NULL, NULL); 
	if (window == NULL){
		printf("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){	
		printf("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int vertex_shader;
	char* vertex_shader_source = read_file("vertex.glsl");
	if(compile_shader(&vertex_shader, GL_VERTEX_SHADER, vertex_shader_source) != 0){
		return -1;
	}

	unsigned int fragment_shader;
	char* fragment_shader_source = read_file("fragment.glsl");
	if(compile_shader(&fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_source) != 0){
		return -1;
	}
	
	int success;
	char infoLog[512];

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragment_shader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADERPROGRAM::LINKING_FAILED\n%s", infoLog);
	}

	float vertices[] = {
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	float texCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(1);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);



	stbi_image_free(data);
	
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	unsigned int VBO, VAO, EBO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(shaderProgram);
	
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);


		
	while(!glfwWindowShouldClose(window)){

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue)/ 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glActiveTexture(GL_TEXTURE0);	
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);



		glfwSwapBuffers(window); 
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE)){
		glfwSetWindowShouldClose(window, 1);
	}
}
