
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"
#define PIE 3.14159265359

float color = 0.1f;
float changeX = 0.1f;
float changeY = 0.1f;
float changeZ = 0.1f;

int questionToShow;

float objectColorR = 1.0f, objectColorG = 1.0f, objectColorB = 1.0f; 
float starX = 0.0f, starY = 0.0f, radius = 0.5f;
float red = 1.0f, green = 0.0f, blue = 0.0f;
float redStep = 0.01f, greenStep = 0.01f, blueStep = 0.01f;
float speedMultiplier = 1.0f;
float angle = 0.0f;   
float radius2 = 0.8f;
float x = radius2 * cos(angle);
float y = radius2 * sin(angle);
float boxScale = 0.1f; 
float expansionRate = 0.005f; 
float maxBoxScale = 1.0f;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
				PostQuitMessage(0);
				break;
		case VK_UP:
			changeY += 0.1f;
				break;
			case VK_DOWN:
				changeY -= 0.1f;
				break;
			case VK_LEFT:
				changeX -= 0.1f;
				break;
			case VK_RIGHT:
				changeX += 0.1f;
				break;
			case 'R': 
				objectColorR = 1.0f;
				objectColorG = 0.0f;
				objectColorB = 0.0f;
				break;
			case 'G': 
				objectColorR = 0.0f;
				objectColorG = 1.0f;
				objectColorB = 0.0f;
				break;
			case 'B': 
				objectColorR = 0.0f;
				objectColorG = 0.0f;
				objectColorB = 1.0f;
				break;
			case VK_SPACE: 
				objectColorR = 1.0f;
				objectColorG = 1.0f;
				objectColorB = 1.0f;
				changeX = 0.0f;              
				changeY = 0.0f;              
				changeZ = 0.0f;
				break;
			case 'W': // for adjust speed of colour chanhing
				speedMultiplier += 0.1f;
				if (speedMultiplier > 5.0f) speedMultiplier = 5.0f; 
				break;
			case 'S': 
				speedMultiplier -= 0.1f;
				if (speedMultiplier < 0.1f) speedMultiplier = 0.1f; 
				break;
			case 0x31:
				questionToShow = 1;
				break;
			case 0x32:
				questionToShow = 2;
				break;
			case 0x33:
				questionToShow = 3;
				break;
			case 0x34:
				questionToShow = 4;
				break;
			case 0x35:
				questionToShow = 5;
				break;
			case 0x36:
				questionToShow = 6;
				break;
			case 0x37:
				questionToShow = 7;
				break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	switch (questionToShow) {
	case 1: // change colour and move
		glClearColor(color, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(changeX, changeY, changeZ);
		glBegin(GL_TRIANGLES),
			glLineWidth(5.0f);
		// we need some point here	
		glColor3f(objectColorR, objectColorG, objectColorB); 
		glVertex2f(0, 0.5);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0.5, -0.5);
		glEnd();
		break;
	case 2:
		// Update colors for gradient with speed multiplier
		red += redStep * speedMultiplier;
		green += greenStep * speedMultiplier;
		blue += blueStep * speedMultiplier;

		// Reverse direction if colors exceed bounds
		if (red > 1.0f || red < 0.0f) redStep = -redStep;
		if (green > 1.0f || green < 0.0f) greenStep = -greenStep;
		if (blue > 1.0f || blue < 0.0f) blueStep = -blueStep;

		// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		// Draw the star
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 5; i++) {
			float angle1 = i * 2 * PIE / 5;
			float angle2 = (i + 2) * 2 * PIE / 5;

			// Set dynamic color
			glColor3f(red, green, blue);
			glVertex2f(starX, starY);
			glColor3f((red + 0.5f), (green + 0.5f), (blue + 0.5f)); // Gradient effect
			glVertex2f(starX + radius * cos(angle1), starY + radius * sin(angle1));
			glColor3f((red + 0.25f), (green + 0.25f), (blue + 0.25f)); // Gradient effect
			glVertex2f(starX + radius * cos(angle2), starY + radius * sin(angle2));
		}
		glEnd();


		break;
	case 3:
		// Update angle for anti-clockwise movement
		angle += 0.01f; // Adjust the increment to control speed
		if (angle > 2 * PIE) angle -= 2 * PIE;

		// Calculate new position
		x = radius2 * cos(angle);
		y = radius2 * sin(angle);

		// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		// Draw the rotating quad
		glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f, 0.0f); // Red color for the quad
		glVertex2f(x - 0.1f, y - 0.1f);
		glVertex2f(x + 0.1f, y - 0.1f);
		glVertex2f(x + 0.1f, y + 0.1f);
		glVertex2f(x - 0.1f, y + 0.1f);
		glEnd();

		break;

	case 4:
		boxScale += expansionRate;

		// Reset the size if it exceeds the maximum scale
		if (boxScale > maxBoxScale) {
			boxScale = 0.1f; // Reset to the initial size
		}

		// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		// Draw the expandable red box
		glColor3f(1.0f, 0.0f, 0.0f); // Red color
		glBegin(GL_QUADS);
		glVertex2f(-boxScale, -boxScale); // Bottom-left
		glVertex2f(boxScale, -boxScale);  // Bottom-right
		glVertex2f(boxScale, boxScale);   // Top-right
		glVertex2f(-boxScale, boxScale);  // Top-left
		glEnd();
		break;
	case 5:
		
		break;
	case 6:
		break;
	}
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	

	

	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

// hinstance hinst is something like provide an ID, like maybe i need to open 5 times the program, but they are in same program, then each of the program has an id
// histance hprevinst rarely use it, it is for security purpose
// lpstr cmdlineparam is command line parameter,OS pass command to program
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdlineparam, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX)); //zeromemory find address, get the size, and fill zero 

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL); //or put hInst, same result
	wc.lpfnWndProc = WindowProcedure; //window procedure incharge on capture input and process it, example is when user wan to close the program, then it will capture it and process
	wc.lpszClassName = WINDOW_TITLE; //classname, can put whatever u wan
	wc.style = CS_HREDRAW | CS_VREDRAW; // horizontal size and vertical size

	if (!RegisterClassEx(&wc)) return false; //define the class, check whether it is correct created or not, the above one

	HWND hWnd = CreateWindow(WINDOW_TITLE, // this one is ur window class name,
		WINDOW_TITLE, // this one is your window title, at left corner
		WS_OVERLAPPEDWINDOW, //style
		CW_USEDEFAULT, //size of the window location
		CW_USEDEFAULT, //size of the window location
		800, //width
		800, //height
		NULL,
		NULL, //menu paramenter, something like this vs 2022 top one punya menu, file,edit.....
		wc.hInstance, //the id
		NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true) // infinite loop, keep track user event
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // check if got message or not,
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------