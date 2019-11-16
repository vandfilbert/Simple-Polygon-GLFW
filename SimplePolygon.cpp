#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <math.h>
#include <vector>
#include <glfw3.h>
#include "Shader.h"
#include "Shape.h"

Shape** shapes;
vector<Shape*> dot;
bool isClicked = false;
const int SHAPE_COUNT = 29;
int WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 1000;

GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
GLuint VertexArrayID;
bool tooClose(float _x, float _y) {
	bool result = false;
	for (int i = 0; i < dot.size() && !result; i++) {
		if (abs(_x - dot[i]->getPosition().x) < 0.002 && abs(_y - dot[i]->getPosition().y) < 0.002)
			result = true;
	}
	return result;
}
void mouseMoveEvent(GLFWwindow* window, double x, double y)
{
	double mod_x = (float)(x - (WINDOW_WIDTH / 2)) / (float)(WINDOW_WIDTH / 2);
	double mod_y = (float)(WINDOW_HEIGHT - y - (WINDOW_HEIGHT / 2)) / (float)(WINDOW_HEIGHT / 2);
	printf("X : %f, Y : %f\n", mod_x, mod_y);
	if (isClicked && !tooClose(mod_x, mod_y)) {
		Shape* newDot = new Circle(mod_x, mod_y, 0, 100, 0.005, 1);
		newDot->initiateBuffer();
		char dotShader[2][100] = { {"shaders/circle/vertex.shader"}, {"shaders/circle/fragment.shader"} };
		newDot->initiateShader(dotShader[0], dotShader[1]);
		dot.push_back(newDot);
	}
}

void mouseClickEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		double mod_x = (float)(x - (WINDOW_WIDTH / 2)) / (float)(WINDOW_WIDTH / 2);
		double mod_y = (float)(WINDOW_HEIGHT - y - (WINDOW_HEIGHT / 2)) / (float)(WINDOW_HEIGHT / 2);
		if (action == GLFW_PRESS)
		{
			printf("LEFT CLICK ON : (%lf, %lf)\n", mod_x, mod_y);
			isClicked = true;
		}
		else if (action == GLFW_RELEASE)
		{
			printf("LEFT RELEASE ON : (%lf, %lf)\n", mod_x, mod_y);
			isClicked = false;
		}
	}
}
void screenResizeEvent(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}
void initializeGLFW() {
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW\n");
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
}

void initializeWindow() {
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Computer Graphics", NULL, NULL);
	if (window == NULL) {
		printf("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return;
	}
	glfwSetCursorPosCallback(window, mouseMoveEvent);
	glfwSetMouseButtonCallback(window, mouseClickEvent);
	glfwSetFramebufferSizeCallback(window, screenResizeEvent);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void initializeGLEW() {
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		return;
	}
}


void initializeShapes() {
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	shapes = new Shape * [SHAPE_COUNT];
	Vertex vertex[][3] =
	{
		{ Vertex(-0.45f, 0), Vertex(-0.085f, 0.25f), Vertex(0.25f, 0) },
		{ Vertex(-0.45, 0), Vertex(0.25f, -0.45f), Vertex(0.25f, 0) },
		{ Vertex(-0.45, 0), Vertex(-0.45f, -0.45f), Vertex(0.25f, -0.45f) },
		{ Vertex(0, -0.45f), Vertex(0.15f, -0.25f), Vertex(0.15f, -0.45f) },
		{ Vertex(0, -0.45f), Vertex(0, -0.25f), Vertex(0.15f, -0.25f) },
		{ Vertex(-0.65f , -0.45f), Vertex(-0.75f , -0.1f), Vertex(-0.85f, -0.45f) },
		{ Vertex(-0.35f , -0.15f), Vertex(-0.25f , -0.05f), Vertex(-0.15f, -0.15f) },
		{ Vertex(-0.35f , -0.15f), Vertex(-0.25f , -0.25f), Vertex(-0.15f, -0.15f) },
		{ Vertex(-0.95f , -0.25f), Vertex(-0.75f , -0.1f), Vertex(-0.55f, -0.25f) },
		{ Vertex(-0.95f , -0.15f), Vertex(-0.75f , 0), Vertex(-0.55f, -0.15f) },
		{ Vertex(-0.87f , -0.05f), Vertex(-0.75f , 0.15f), Vertex(-0.65f, -0.05f) },
		{ Vertex(-1 , -0.45f), Vertex(-1 , -0.75f), Vertex(1 , -0.45f) },
		{ Vertex(1 , -0.45f), Vertex(1 , -0.75f), Vertex(-1 , -0.75f) },
		{ Vertex(0.27f , -0.35f), Vertex(0.27f, -0.2f), Vertex(0.67f , -0.35f) },
		{ Vertex(0.67f , -0.35f), Vertex(0.27f , -0.2f), Vertex(0.67f , -0.2f) },
		{ Vertex(0.35f , -0.2f), Vertex(0.51f , -0.2f), Vertex(0.37f , -0.05f) },
		{ Vertex(0.37f , -0.05f), Vertex(0.51f , -0.2f), Vertex(0.55f , -0.05f) },
		{ Vertex(0.55f , -0.05f), Vertex(0.51f , -0.2f), Vertex(0.6f, -0.2f) },
		{ Vertex(0.75f , -0.02f), Vertex(0.75f , -0.45f), Vertex(0.78f , -0.02f) },
		{ Vertex(0.75f , -0.45f), Vertex(0.78f , -0.02f), Vertex(0.78f , -0.45f) },
		{ Vertex(0.75f , -0.12f), Vertex(0.765f , -0.02f), Vertex(0.78f , -0.12f) }
	};

	int triangle_count = 15;

	for (int i = 0; i < triangle_count; i++) {
		shapes[i] = new Triangle(vertex[i], 0.5f, 0.5f);
	}

	shapes[15] = new Circle(0.65, -0.275, 0, 100, 0.077, 1);
	shapes[16] = new Circle(0.35, -0.376, 0, 100, 0.077, 1);
	shapes[17] = new Circle(0.6, -0.376, 0, 100, 0.077, 1);
	shapes[18] = new Circle(0.6, -0.376, 0, 100, 0.04, 1);
	shapes[19] = new Circle(0.35, -0.376, 0, 100, 0.04, 1);

	shapes[20] = new Triangle(vertex[15], 0.5f, 0.5f);
	shapes[21] = new Triangle(vertex[16], 0.5f, 0.5f);
	shapes[22] = new Triangle(vertex[17], 0.5f, 0.5f);
	shapes[23] = new Triangle(vertex[18], 0.5f, 0.5f);
	shapes[24] = new Triangle(vertex[19], 0.5f, 0.5f);

	shapes[25] = new Circle(0.765, -0.02, 0, 100, 0.1, 1);
	shapes[26] = new Triangle(vertex[20], 0.5f, 0.5f);

	shapes[27] = new Circle(0, 0.7, 0, 100, 0.17, 1);
	shapes[28] = new Circle(-0.1, 0.7, 0, 100, 0.17, 1);

	char vertexShader[][100] = { "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader","shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader","shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader","shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader","shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader", "shaders/triangle/vertex_1.shader" };
	char fragmentShader[][100] = { "shaders/circle/fragment.shader", "shaders/triangle/red.shader", "shaders/triangle/red.shader","shaders/triangle/blue.shader","shaders/triangle/blue.shader", "shaders/triangle/brown.shader", "shaders/triangle/yellow.shader", "shaders/triangle/yellow.shader", "shaders/triangle/green.shader", "shaders/triangle/green.shader", "shaders/triangle/green.shader", "shaders/triangle/grey.shader", "shaders/triangle/grey.shader", "shaders/circle/fragment.shader", "shaders/circle/fragment.shader", "shaders/circle/fragment.shader","shaders/triangle/grey.shader", "shaders/triangle/grey.shader", "shaders/triangle/white.shader", "shaders/triangle/white.shader", "shaders/triangle/white.shader", "shaders/triangle/white.shader", "shaders/triangle/white.shader", "shaders/triangle/brown2.shader", "shaders/triangle/brown2.shader", "shaders/triangle/white.shader", "shaders/triangle/red.shader", "shaders/triangle/yellow.shader", "shaders/triangle/black.shader" };
	char fragmentOutlineShader[][100] = { "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader","shaders/triangle/fragment_outline_2.shader","shaders/triangle/fragment_outline_2.shader","shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader","shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader", "shaders/triangle/fragment_outline_2.shader" };

	for (int i = 0; i < SHAPE_COUNT; i++)
	{
		shapes[i]->initiateBuffer();
		shapes[i]->initiateShader(vertexShader[i], fragmentShader[i]);
		shapes[i]->initiateOutlineShader(vertexShader[i], fragmentOutlineShader[i]);
	}
}

void render() {
	glEnableVertexAttribArray(0);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < SHAPE_COUNT; i++) {
			shapes[i]->drawPolygon();
			shapes[i]->drawPolyline();
		}
		for (int i = 0; i < dot.size(); i++) {
			dot[i]->drawPolygon();
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	glDisableVertexAttribArray(0);
}

int main()
{
	initializeGLFW();
	initializeWindow();
	initializeGLEW();
	initializeShapes();
	render();
}