#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
bool mask[WINDOW_HEIGHT][WINDOW_WIDTH];
GLFWwindow *window;
int s = 4;

// Color structure. Can be used to define the brush and background color.
struct color { float r, g, b; };

struct color color;
struct color bg_color;

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

// Set a particular pixel of the frameBuffer to the provided color
void SetFrameBufferPixel(int x, int y, struct color lc)
{
	// Origin is upper left corner. 
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);

	frameBuffer[y][x][0] = lc.r;
	frameBuffer[y][x][1] = lc.g;
	frameBuffer[y][x][2] = lc.b;

}

void SetBackground(struct color lc) {
	bg_color = lc;
	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_HEIGHT; y++) {
			if (!mask[y][x]) {
				SetFrameBufferPixel(x, y, bg_color);
			}
		}
	}
}

void ClearDrawing() {
	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_HEIGHT; y++) {
			if (mask[y][x]) {
				SetFrameBufferPixel(x, y, bg_color);
				mask[y][x] = 0;
			}
		}
	}
}

void ClearFrameBuffer()
{
	memset(frameBuffer, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

// Display frameBuffer on screen
void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		for (int x = xpos - s; x <= xpos + s; x++) {
			for (int y = ypos - s; y <= ypos + s; y++) {
				SetFrameBufferPixel(x, y, color);
				mask[y][x] = 1;
			}
		}
		std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
	}
}

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
	{
		std::cout << "Mouse left button is pressed." << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		std::cout << "Mouse right button is pressed." << std::endl;
		ClearDrawing();
	}

}


// You can use "switch" or "if" to compare key to a specific character.
// for example,
// if (key == '0')
//     DO SOMETHING

// Keyboard callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) {
	case '-':
	case '_':
		if (s == 1) break;
		s /= 2;
		break;
	case '+':
	case '=':
		if (s == 128) break;
		s *= 2;
		break;
	case '1':
		color = { 0, 0, 1 };
		break;
	case '2':
		color = { 0, 1, 0 };
		break;
	case '3':
		color = { 0, 1, 1 };
		break;
	case '4':
		color = { 1, 0, 0 };
		break;
	case '5':
		color = { 1, 0, 1 };
		break;
	case '6':
		color = { 1, 1, 0 };
		break;
	case '7':
		color = { 1, 1, 1 };
		break;
	case '!':
		SetBackground({ 0, 0, 1 });
		break;
	case '@':
		SetBackground({ 0, 1, 0 });
		break;
	case '#':
		SetBackground({ 0, 1, 1 });
		break;
	case '$':
		SetBackground({ 1, 0, 0 });
		break;
	case '%':
		SetBackground({ 1, 0, 1 });
		break;
	case '^':
		SetBackground({ 1, 1, 0 });
		break;
	case '&':
		SetBackground({ 1, 1, 1 });
		break;
	}
	std::cout << "Key " << (char)key << " is pressed. " << std::endl;
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1 - Kyne Sun", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
	color = { 1, 1, 1 };
	bg_color = { 0, 0, 0 };
}


int main()
{	
	Init();
	while (glfwWindowShouldClose(window) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}