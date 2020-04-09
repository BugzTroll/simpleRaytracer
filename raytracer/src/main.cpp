#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include <algorithm> 

#include <glad/glad.h>

#include "vec3.h"
#include "vec3.cpp"
#include "ray.h"
#include "hitableList.h"
#include "sphere.h"
#include "camera.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
#include <thread>

using namespace std;

vec3 color(const ray& r, hitable* world, int depth)
{
	hitRecord rec;
	if (world->hit(r, 0.001, numeric_limits<float>::max(), rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unitDirection = unitVector(r.direction());
		float t = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"layout(location = 0) in vec3 aPos;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = aColor;\n"
"	TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"	FragColor = texture(ourTexture, TexCoord);\n"
"}\0";

unsigned int generateTexture(GLubyte* data, size_t width, size_t height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to set texture" << std::endl;
	}

	return texture;
}

void setTextureBytes(GLubyte* data, size_t width, size_t height)
{
	glBindTexture(GL_TEXTURE_2D, 1);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to set texture" << std::endl;
	}
}

unsigned int loadTexture(std::string filepath)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("../outputImages/outputImage.pnm", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//glUseProgram(shaderProgram);

	return texture;
}


unsigned int generateTexture()
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("../outputImages/outputImage.pnm", &width, &height, &nrChannels, 0);


	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//glUseProgram(shaderProgram);

	return texture;
}

int buildShaders()
{
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

unsigned int bindVerticeBuffers()
{
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return VAO;
}

void refresh(GLFWwindow* window, unsigned int VAO, int shaderProgram)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindTexture(GL_TEXTURE_2D, 1);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
	/* Poll for and process events */
	glfwPollEvents();
}

int startRenderLoop(GLFWwindow* window)
{
	// build shaders
	int shaderProgram = buildShaders();

	// bind buffers
	unsigned int VAO = bindVerticeBuffers();

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture);
		glBindTexture(GL_TEXTURE_2D, 1);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;

}

GLFWwindow* createWindow(int width, int height)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	// set gl viewport
	glViewport(0, 0, width, height);
	return window;
}

hitable* randomScene() {
	int n = 500;
	hitable** list = new hitable * [n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float chooseMat = utilities::randomDouble();
			vec3 center(a + 0.9 * utilities::randomDouble(), 0.2, b + 0.9 * utilities::randomDouble());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (chooseMat < 0.8) {  // diffuse
					list[i++] = new sphere(
						center, 0.2,
						new lambertian(vec3(utilities::randomDouble() * utilities::randomDouble(),
							utilities::randomDouble() * utilities::randomDouble(),
							utilities::randomDouble() * utilities::randomDouble()))
						);
				}
				else if (chooseMat < 0.95) { // metal
					list[i++] = new sphere(
						center, 0.2,
						new metal(vec3(0.5 * (1 + utilities::randomDouble()),
							0.5 * (1 + utilities::randomDouble()),
							0.5 * (1 + utilities::randomDouble())),
							0.5 * utilities::randomDouble())
						);
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitableList(list, i);
}

void raytraceScene(size_t width, size_t height)
{
	std::cout << "Writing to file" << endl;

	// open image file
	ofstream myfile("../outputImages/outputImage.pnm", std::ios::binary);

	int nx = width;
	int ny = height;
	int ns = 1;

	std::vector<GLubyte> imagePixels(nx * ny * 3);

	vec3 lookFrom(13, 2, 3);
	vec3 lookAt(0, 0, 0);
	float distToFocus = 10.0f;
	float aperture = 0.1f;

	camera cam(lookFrom, lookAt, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, distToFocus);
	float R = cos(M_PI_4);

	new lambertian(vec3());

	hitable* world = randomScene();
	myfile << "P6\n" << nx << " " << ny << "\n255\n";

	// Send rays for every pixels
	int index = 0;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {

			// Average the color to get antialiased image
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = (float(i) + (rand() / double(RAND_MAX))) / float(nx);
				float v = (float(j) + (rand() / double(RAND_MAX))) / float(ny);

				ray r = cam.getRay(u, v);
				vec3 p = r.pointAtParameter(2.0);
				col += color(r, world, 0);
			}

			col /= float(ns);

			//gamma correction
			col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

			unsigned char r = static_cast<unsigned char>(std::min(1.f, col.r()) * 255);
			unsigned char g = static_cast<unsigned char>(std::min(1.f, col.g()) * 255);
			unsigned char b = static_cast<unsigned char>(std::min(1.f, col.b()) * 255);
			myfile << r << g << b;

			imagePixels[index] = (GLubyte)(std::min(1.f, col.r()) * 255);
			imagePixels[index + 1] = (GLubyte)(std::min(1.f, col.g()) * 255);
			imagePixels[index + 2] = (GLubyte)(std::min(1.f, col.b()) * 255);
			index += 3;
		}
	}

	myfile.close();

	std::cout << "done" << endl;
}

int main()
{
	std::cout << "Writing to file" << endl;

	// open image file
	ofstream myfile("../outputImages/outputImage.pnm", std::ios::binary);

	int nx = 400;
	int ny = 200;
	int ns = 10;

	GLFWwindow* window = createWindow(nx, ny);
	std::vector<GLubyte> imagePixels(nx * ny * 3);

	for (size_t i = 0; i < imagePixels.size(); i += 3)
	{
		imagePixels[i] = (GLubyte)0;
		imagePixels[i + 1] = (GLubyte)0;
		imagePixels[i + 2] = (GLubyte)0;
	}

	generateTexture(imagePixels.data(), nx, ny);

	// build shaders
	int shaderProgram = buildShaders();

	// bind buffers
	unsigned int VAO = bindVerticeBuffers();

	refresh(window, VAO, shaderProgram);

	std::thread first(raytraceScene, nx, ny);

	//vec3 lookFrom(13, 2, 3);
	//vec3 lookAt(0, 0, 0);
	//float distToFocus = 10.0f;
	//float aperture = 0.1f;

	//camera cam(lookFrom, lookAt, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, distToFocus);
	//float R = cos(M_PI_4);

	//new lambertian(vec3());

	//hitable* world = randomScene();
	//myfile << "P6\n" << nx << " " << ny << "\n255\n";

	//// Send rays for every pixels
	//int index = 0;
	//for (int j = ny - 1; j >= 0; j--) {
	//	for (int i = 0; i < nx; i++) {

	//		// Average the color to get antialiased image
	//		vec3 col(0, 0, 0);
	//		for (int s = 0; s < ns; s++)
	//		{
	//			float u = (float(i) + (rand() / double(RAND_MAX))) / float(nx);
	//			float v = (float(j) + (rand() / double(RAND_MAX))) / float(ny);

	//			ray r = cam.getRay(u, v);
	//			vec3 p = r.pointAtParameter(2.0);
	//			col += color(r, world, 0);
	//		}

	//		col /= float(ns);

	//		//gamma correction
	//		col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

	//		unsigned char r = static_cast<unsigned char>(std::min(1.f, col.r()) * 255);
	//		unsigned char g = static_cast<unsigned char>(std::min(1.f, col.g()) * 255);
	//		unsigned char b = static_cast<unsigned char>(std::min(1.f, col.b()) * 255);
	//		myfile << r << g << b;

	//		imagePixels[index] = (GLubyte)(std::min(1.f, col.r()) * 255);
	//		imagePixels[index + 1] = (GLubyte)(std::min(1.f, col.g()) * 255);
	//		imagePixels[index + 2] = (GLubyte)(std::min(1.f, col.b()) * 255);
	//		index += 3;
	//	}

	//	setTextureBytes(imagePixels.data(), nx, ny);
	//	refresh(window, VAO, shaderProgram);
	//}

	//myfile.close();

	//std::cout << "done" << endl;

	setTextureBytes(imagePixels.data(), nx, ny);
	refresh(window, VAO, shaderProgram);

	startRenderLoop(window);

	//std::thread first(startRenderLoop, window);

	//loadTexture("../outputImages/outputImage.pnm");
	//startRenderLoop(window);

	//createWindow(nx, ny);

	std::cin >> nx;
	return 0;
}