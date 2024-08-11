/*
*    || -- Graphics Rendering Engine (Utilizing OpenGL) -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 7/24/2024
*    |  Version: 2.0
* 
*    This project was created to render simple and complex objects in real time while allowing the user to freely move around the scene.
*    This project works as a simple graphics engine which can render simple and complex objects as rectangles, spheres, cylindrs, and planes.
*	 The code below creates shaders, lighting, objects, textures, and renders it all on the users screen.
*    Currently, the code renders a simple image of a desk with two monitors, a baseball, a waterball, and a stuffed animal tarantual.
*	 
*    CONTROLS:
*       - Use mouse to look around                             | - 1 = Select light 1
*       - Use mouse scroll wheel to change movement speed      | - 2 = Select light 2
*       - W = Move Forward                                     | - L = Moves selected light to current camera position
*       - A = Move Left                                        | - ↑ = Move selected light Forward
*       - S = Move Backwards                                   | - ← = Move selected light Left
*       - D = Move Right                                       | - ↓ = Move selected light Backwards
*       - Q = Move Up                                          | - → = Move selected light Right
*       - E = Move Down                                        | - + = move selected light Up
*       - P = Change between perspective and orthographic view | - - = move selected light Down
* 
*    ENHANCEMENTS:
*		- User can manipulate lights and move them around the scene
*       - Tarantual legs are now drawn through one function call instead of 10 seperate calls
*       - Animation added to make baseball increase in size and jump up and down across the desk
* 
*		- NOT IMPLEMENTED: Pressing F will cause selected light to orbit around the scene
*              - Currently working on implementing this feature
*/

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
// GLM library
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
// Soil2 library
#include <SOIL2/stb_image.h>

using namespace std;

int width, height, currentLight = 1, ballCount = 0;
double previousTime;
float aspectRatio; //                                             // Projection matrix aspect ratio
float camSpeed, yaw, pitch, lastX, lastY, lightMoveSpeed = 0.03f; // Camera location (x,y,z), speed, yaw, pitch, position
glm::vec3 cameraPos, cameraFront, cameraUp, direction;            // Camera varaibles
bool firstMouse = true, orthoView = false; //                     // Variable to check for first mouse input

// Variables to control ball animation
float ballScale = 0.18f, ballPositionY = -1.08f, ballPositionZ = 0.4f;
bool ballIncrease = false, ballForward = true; 

// Variables to control tarantual animation
float legRotation = 0.0f, legPosY = -1.18f, pedipalPosY = -1.18f, bodyHeadPosY = -1.1f;
bool tarantualStandUp = true; 

// Variables to control light rectangluar orbit
bool light1Orbit = false, light2Orbit = false;
bool light1Forward = false, light1Right = false;
bool light2Forward = false, light2Right = false;
float temp = 0.0f;

// Texture variables
unsigned int planeTex;
unsigned int monitorTex1, monitorTex2;
unsigned int waterBottleTex1, waterBottleTex2;
unsigned int tarantualTex1, tarantualTex2, tarantualTex3;

// Shader variables & shander program variables for compiled shaders
string vertexShaderSource, fragmentShaderSource, lightVertexShaderSource, lightFragmentShaderSource;
GLuint shaderProgram, lightShaderProgram;

// VAO, VBO, and EBO variables
GLuint boxVBO, boxVAO, boxEBO;
GLuint planeVBO, planeVAO, planeEBO;
GLuint lightVBO, lightVAO, lightEBO;
GLuint cylinderVBO, cylinderVAO, cylinderEBO;
GLuint sphereVBO, sphereVAO, sphereEBO;

// Uniform Reference Variables
GLuint modelLoc, viewLoc, projLoc, objectColorLoc, viewPosLoc;
GLuint light1ColorLoc, light1ModelLoc, light1ViewLoc, light1ProjLoc, light1PosLoc;
GLuint light2ColorLoc, light2ModelLoc, light2ViewLoc, light2ProjLoc, light2PosLoc;                    

// Matricies variables & initling the view matrix to identy matrix
glm::mat4 projectionMatrix, modelMatrix;
glm::mat4 viewMatrix = glm::mat4(1.0f);

// Define functions to process keyboard/mouse input
void processInput(GLFWwindow* window); //                                 // Processes keyboard input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);        // Processes mouse button / cursor input
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); // Processes scroll wheel input


/*
	 ||| --                                 -- |||
	 ||| -- POSITION / ROTATIONS OF OBJECTS -- |||
	 ||| --                                 -- |||
*/
// MONITOR 1 - START
glm::vec3 screen1Positions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),     // Position 1 (Front Face)
	glm::vec3(1.25f, 0.0f, -0.05f),  // Position 2 (Right Face)
	glm::vec3(0.0f, 0.0f, -0.1f),    // Position 3 (back Face)
	glm::vec3(-1.25f, 0.0f, -0.05f), // Position 4 (Left Face)
	glm::vec3(0.0f, 0.75f, -0.05f),  // Position 5 (Top Face)
	glm::vec3(0.0f, -0.75f, -0.05f)  // Position 6 (Bottom Face)
};
// Plane positions (Screen to stand connectorb 4 faces - no front or back)
glm::vec3 connector1Positions[] = {
	glm::vec3(0.14f, 0.0f, 0.0f),  // Position 1 (Right Face)
	glm::vec3(0.0f, 0.05f, 0.0f),  // Position 2 (Top Face)
	glm::vec3(-0.14f, 0.0f, 0.0f), // Position 3 (Left Face)
	glm::vec3(0.0f, -0.05f, 0.0f), // Position 4 (Bottom Face)
};
// Plane positions (stand 5 faces - no bottom)
glm::vec3 stand1Positions[] = {
	glm::vec3(0.0f, 0.0f, 0.1f),   // Position 1 (Front Face)
	glm::vec3(0.15f, 0.0f, 0.0f),  // Position 2 (Right Face)
	glm::vec3(0.0f, 0.0f, -0.1f),  // Position 3 (back Face)
	glm::vec3(-0.15f, 0.0f, 0.0f), // Position 4 (Left Face)
	glm::vec3(0.0f, 0.55f, 0.0f),  // Position 5 (Top Face)
	glm::vec3(0.0f, -0.55f, 0.0f)  // Position 6 (Bottom Face)
};
// Plane positions (Stand Base, 4 faces - no bottom)
glm::vec3 stand1BasePositions1[] = {
	glm::vec3(0.0f, 0.0f, 0.05f),   // Position 1 (Front Face)
	glm::vec3(0.0f, 0.053f, 0.0f),  // Position 2 (Top Face)
	glm::vec3(0.0f, 0.0f, -0.05f),  // Position 3 (back Face)
	glm::vec3(0.0f, -0.053f, 0.0f), // Position 4 (bottom Face)
};
// Plane positions (Stand Base, 4 faces - no bottom)
glm::vec3 stand1BasePositions2[] = {
	glm::vec3(0.05f, 0.0f, 0.0f),   // Position 1 (Right Face)
	glm::vec3(0.0f, 0.077f, 0.0f),  // Position 2 (Top Face)
	glm::vec3(-0.05f, 0.0f, 0.0f),  // Position 3 (Left Face)
	glm::vec3(0.0f, -0.077f, 0.0f), // Position 4 (bottom Face)
};
// Plane rotations X, Y, & Z axis - all faces of a cube
glm::float32 monitor1RotationsAllX[] = {
	0.0f, 0.0f, 0.0f, 0.0f, 270.0f,90.0f }; // F, R, Ba, L, T, Bo
glm::float32 monitor1RotationsAllY[] = {
	0.0f, 90.0f, 180.0f, 270.0f, 0.0f, 0.0f }; // F, R, Ba, L, T, Bo
glm::float32 monitor1RotationsAllZ[] = {
	0.0f, 0.0f, 0.0f, 0.0f, 90.0f, 90.0f }; // F, R, Ba, L, T, Bo
// Plane rotations X, Y, & Z axis - all faces of a cube
glm::float32 base1Rotations1AllX[] = {
	0.0f, 270.0f, 180.0f, 90.0f }; // F, T, Ba, Bo
glm::float32 base1Rotations1AllY[] = {
	0.0f, 340.0f, 0.0f, 20.0f }; // F, T, Ba, Bo
glm::float32 base1Rotations1AllZ[] = {
	20.0f, 0.0f, 340.0f, 0.0f }; // F, T, Ba, Bo
glm::float32 base1Rotations2AllX[] = {
	0.0f, 270.0f, 180.0f, 90.0f }; // F, T, Ba, Bo
glm::float32 base1Rotations2AllY[] = {
	0.0f, 20.0f, 0.0f, 340.0f }; // F, T, Ba, Bo
glm::float32 base1Rotations2AllZ[] = {
	340.0f, 0.0f, 20.0f, 0.0f }; // F, T, Ba, Bo
glm::float32 base1Rotations3AllX[] = {
	0.0f, 220.0f, 180.0f, 40.0f }; // L, T, R, Bo
glm::float32 base1Rotations3AllY[] = {
	90.0f, 0.0f, 270.0f, 0.0f }; // L, T, R, Bo
glm::float32 base1Rotations3AllZ[] = {
	310.0f, 90.0f, 50.0f, 90.0f }; // L, T, R, Bo
// Plane rotations X, Y, & Z axis - Left, right, top, & bottom faces of a cube (no front or back face)
glm::float32 monitor1Rotations4FacesX[] = {
	0.0f, 270.0f, 0.0f, 90.0f }; // R, T, L, Bo
glm::float32 monitor1Rotations4FacesY[] = {
	90.0f, 0.0f, 270.0f, 0.0f }; // R, T, L, Bo
glm::float32 monitor1Rotations4FacesZ[] = {
	0.0f, 0.0f, 0.0f, 0.0f }; // R, T, L, Bo
// MONITOR 1 - END

// MONITOR 2 - START
// Plane positions (Screen of monitor, 6 faces - all faces)
glm::vec3 screen2Positions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),        // Position 1 (Front Face)
	glm::vec3(0.4524f, 0.0f, -0.2181f), // Position 2 (Right Face)
	glm::vec3(-0.034f, 0.0f, -0.094f),  // Position 3 (back Face)
	glm::vec3(-0.4865f, 0.0f, 0.124f),  // Position 4 (Left Face)
	glm::vec3(-0.017f, 1.0f, -0.047f),  // Position 5 (Top Face)
	glm::vec3(-0.017f, -1.0f, -0.047f)  // Position 6 (Bottom Face)
};
// Plane positions (Screen to stand connectorb 4 faces - no front or back)
glm::vec3 connector2Positions[] = {
	glm::vec3(0.086f, 0.0f, -0.188f),   // Position 1 (Right Face)
	glm::vec3(-0.045f, 0.05f, -0.14f),  // Position 2 (Top Face)
	glm::vec3(-0.176f, 0.0f, -0.092f),  // Position 3 (Left Face)
	glm::vec3(-0.045f, -0.05f, -0.14f), // Position 4 (Bottom Face)
};
// Plane positions (stand 5 faces - no bottom)
glm::vec3 stand2Positions[] = {
	glm::vec3(-0.06f, -0.54f, -0.18f),    // Position 1 (Front Face)
	glm::vec3(0.063f, -0.54f, -0.2782f),  // Position 2 (Right Face)
	glm::vec3(-0.095f, -0.54f, -0.2738f), // Position 3 (back Face)
	glm::vec3(-0.218f, -0.54f, -0.1759f), // Position 4 (Left Face)
	glm::vec3(-0.0775f, 0.135f, -0.226f), // Position 5 (Top Face)
};
// Plane positions (Stand Base, 5 faces - no bottom)
glm::vec3 stand2BasePositions[] = {
	glm::vec3(0.08f, -1.22f, 0.1785f),    // Position 1 (Front Face)
	glm::vec3(0.2889f, -1.22f, -0.27f),   // Position 2 (Right Face)
	glm::vec3(-0.1595f, -1.22f, -0.478f), // Position 3 (back Face)
	glm::vec3(-0.3685f, -1.22f, -0.03f),  // Position 4 (Left Face)
	glm::vec3(-0.04f, -1.195f, -0.15f),   // Position 5 (Top Face)
};
// Plane rotations X, Y, & Z axis - all faces of a cube
glm::float32 monitor2RotationsAllX[] = {
	0.0f, 0.0f, 0.0f, 0.0f, 270.0f,90.0f }; // F, R, Ba, L, T, Bo
glm::float32 monitor2RotationsAllY[] = {
	20.0f, 110.0f, 200.0f, 290.0f, 0.0f, 0.0f }; // F, R, Ba, L, T, Bo
glm::float32 monitor2RotationsAllZ[] = {
	0.0f, 0.0f, 0.0f, 0.0f, 110.0f, 70.0f }; // F, R, Ba, L, T, Bo
// Plane rotations X, Y, & Z axis - Left, right, top, & bottom faces of a cube (no front or back face)
glm::float32 monitor2Rotations4FacesX[] = {
	0.0f, 270.0f, 0.0f, 90.0f }; // R, T, L, Bo
glm::float32 monitor2Rotations4FacesY[] = {
	110.0f, 0.0f, 290.0f, 0.0f }; // R, T, L, Bo
glm::float32 monitor2Rotations4FacesZ[] = {
	0.0f, 20.0f, 0.0f, 340.0f }; // R, T, L, Bo
// MONITOR 2 - END

glm::vec3 rectanglePositions[] = {
	glm::vec3(0.0f, -1.0f, 2.0f), // Position 3 (front face)
	glm::vec3(0.0f, -1.0f, 0.0f), // Position 4 (back Face)
	glm::vec3(-2.0f, -1.0f, 0.0f), // Position 5 (left Face)
	glm::vec3(0.0f, -1.0f, 0.0f), // Position 5 (right Face)
	glm::vec3(0.0f, 1.0f, 0.0f), // Position 5 (top Face)
	glm::vec3(0.0f, -1.0f, 0.0f), // Position 5 (bottom Face)
};
// Light positions & rotations
glm::vec3 lightPosition1(0.0f, 6.0f, 8.0f);
glm::vec3 lightPosition2(8.0f, 4.0f, 2.0f);
// rectangle rotations X, Y, & Z
glm::float32 lightRotationsX[] = {
	90.0f, 90.0f, 90.0f, 90.0f, 0.0f, 0.0f };
glm::float32 lightRotationsY[] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
glm::float32 lightRotationsZ[] = {
	0.0f, 0.0f, 90.0f, 90.0f, 0.0f, 0.0f };
/*
	 ||| --                                          -- |||
	 ||| -- POSITION / ROTATIONS OF OBJECTS END HERE -- |||
	 ||| --                                          -- |||
*/


/*
	 ||| --                                 -- |||
	 ||| -- GLSL ERROR CHECKING DEFINITIONS -- |||
	 ||| --                                 -- |||
*/
/* GLSL Error Checking Definitions */
void PrintShaderCompileError(GLuint shader) 
{
	int len = 0;
	int chWritten = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWritten, log);
		cout << "Shader Compile Error: " << log << endl;
		free(log);
	}
}


void PrintShaderLinkingError(int prog) 
{
	int len = 0;
	int chWritten = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(prog, len, &chWritten, log);
		cout << "Shader Linking Error: " << log << endl;
		free(log);
	}
}


bool IsOpenGLError() 
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}
/* GLSL Error Checking Definitions End Here */
/*
	 ||| --                                          -- |||
	 ||| -- GLSL ERROR CHECKING DEFINITIONS END HERE -- |||
	 ||| --                                          -- |||
*/


/*
	 ||| --                           -- |||
	 ||| -- FUNCTIONS TO DRAW OBJECTS -- |||
	 ||| --                           -- |||
*/
// Function to draw rectangles (Needs position, rotation, & scale for front/back faces, top/bottom faces, & left/right faces on the X, Y, & Z axis)
void drawRectangle(int texture, int faceNumber, glm::vec3 positions[], glm::float32 rotationsX[], glm::float32 rotationsY[], glm::float32 rotationsZ[], float faceFBX,
	float faceFBY, float faceFBZ, float faceTBX, float faceTBY, float faceTBZ, float faceLRX, float faceLRY, float faceLRZ, float posX, float posY, float posZ) 
{
	// Use loop to build model matrix for rectangle
	for (int i = 0; i < faceNumber; i++) {

		// Makes it so the screen of the monitor shows a screen texture instead of the smooth plastic.
		if (texture == 1 && i == 0) 
		{
			glBindTexture(GL_TEXTURE_2D, monitorTex1);
		}
		// Applies smooth plastic texture to entire monitor other than screen
		else 
		{
			glBindTexture(GL_TEXTURE_2D, monitorTex2);
		}

		// Apply Transform to model
		modelMatrix = glm::translate(glm::mat4(1.0f), positions[i] + glm::vec3(posX, posY, posZ));        // Position at 0,0,0
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsX[i]), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate on x axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsY[i]), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate on y axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsZ[i]), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate on z axis	

		if (i == 1 || i == 3) // Left and Right Faces (Change scale of object)
		{ 
			modelMatrix = glm::scale(modelMatrix, glm::vec3(faceLRX, faceLRY, faceLRZ));
		}
		else if (i == 4 || i == 5) // Top and Bottom Faces (Change scale of object)
		{ 
			modelMatrix = glm::scale(modelMatrix, glm::vec3(faceTBX, faceTBY, faceTBZ));
		}
		else // Front and Back faces (Change scale of object)
		{ 
			modelMatrix = glm::scale(modelMatrix, glm::vec3(faceFBX, faceFBY, faceFBZ));
		}

		// Pass transform to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
	}
}

// - ENHANCED AND REWRITTEN TO CREATE REUSIBLE FUNCITON THAT COULD DRAW MULTIPLE OBJECTS WHEN NEEDED - //
// Function to draw cyliders (Needs position, rotation, & scale for front/back faces, top/bottom faces, & left/right faces on the X, Y, & Z axis)
void drawCylinderSphere(unsigned int texture, int loopCount, int rotNumber, float indices, float rotation, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, float posX, float posY, float posZ)
{
	float currAngle = 0.0;
	bool reverse = false;

	glBindTexture(GL_TEXTURE_2D, texture); // Activate texture for object

	for (int i = 0; i < loopCount; ++i)
	{
		// Use loop to draw model matrix for cylinder / sphere
		for (int j = 0; j < rotNumber; j++) {
			// Apply Transform to model
			modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));  // Position at 0,0,0
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));     // rotate on x axis
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));     // rotate on x axis
			modelMatrix = glm::rotate(modelMatrix, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));     // rotate on z axis
			modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, scaleZ));                    // scale object
			modelMatrix = glm::rotate(modelMatrix, glm::radians(currAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate on y axis

			currAngle = currAngle + rotation;

			// Pass transform to shader
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_BYTE, nullptr);
		}
		// If loop count is greater than 1 than the tarantula legs are being drawn and the below modifications will take place to properly draw the tarantuals legs
		if (loopCount > 1)
		{
			// If drawing the 8 main legs
			if (i < 7) {
				if (i == 0 || i == 6) { // Moves middle left and right legs out slightly
					posX += 0.1f;
				}
				else if (i == 2 || i == 4) { // Moves back legs back in to match front legs
					posX -= 0.1f;
				}

				// Increment rotation of each leg, and the z position
				if (i != 3) {
					rotZ += 30.0f;
					if (reverse) {
						posZ -= 0.2f;
					}
					else {
						posZ += 0.2f;
					}
				}
				// If the four left legs have been drawn, set up variabls to draw four right legs properly
				else {
					rotZ += 100.0f;
					rotY -= 360.0f;
					if (rotY <= 0)
					{
						rotY = 360 - rotY;
					}
					reverse = true;
					posX -= 0.5f;
				}
			}
			// Else drawing right pedipalp 
			else if (i == 7) {
				scaleX = 0.3f;
				scaleY = 0.4f;
				scaleZ = 0.3f;
				posX += 0.15f;
				posZ += 0.6f;
				posY = pedipalPosY;
				rotY = 0.0f;
				rotZ = 10.0f;
			}
			// Else drawing left pedipalp 
			else {
				posX += 0.2f;
				rotZ = 350.0f;
			}
		}
	}
}


// Function to draw a plane
void drawPlane() 
{
	// Build model matrix for plane
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.75f, 0.0f)); // Position plane

	// Pass transform to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
}


// Function to draw lights (Needs position, rotation, & scale for front/back faces, top/bottom faces, & left/right faces on the X, Y, & Z axis)
void drawLight(glm::vec3 positions[], glm::float32 rotationsX[], glm::float32 rotationsY[], glm::float32 rotationsZ[], glm::vec3 lightPosition) {

	// Use loop to build Model matrix for cube
	for (int i = 0; i < 6; i++) {
		// Apply Transform to model
		modelMatrix = glm::translate(glm::mat4(1.0f), positions[i] / glm::vec3(8.0f, 8.0f, 8.0f) + lightPosition); // Position at proper location
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsX[i]), glm::vec3(1.0f, 0.0f, 0.0f));          // Rotate on x axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsY[i]), glm::vec3(0.0f, 1.0f, 0.0f));          // Rotate on y axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsZ[i]), glm::vec3(0.0f, 0.0f, 1.0f));          // Rotate on z axis		
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.125f, 0.125f, 0.125f));

		// Pass transform to shader
		glUniformMatrix4fv(light1ModelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
	}
}


/*
	 ||| --                            -- |||
	 ||| -- FUNCTIONS CREATING SHADERS -- |||
	 ||| --                            -- |||
*/
// Set up initial camera positions and rotation
void initPositions() {
	width = 1500; height = 1000; // width and height for window
	camSpeed = 0.05f;
	yaw = -90.0f;
	pitch = 0.0f;
	lastX = width / 2.0f;
	lastY = height / 2.0f;

	cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}


// Create and compile shaders
static GLuint ShaderCompiler(const string& source, GLuint shaderType) {
	// Creates shader object
	GLuint shaderID = glCreateShader(shaderType);
	const char* src = source.c_str();

	glShaderSource(shaderID, 1, &src, nullptr); // Attaches source code to shader object

	glCompileShader(shaderID); // Compile shader

	/* Shader Compliation Error Check */
	GLint shaderCompiled;
	IsOpenGLError();
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != 1) {
		cout << "Shader Compilation Failed!" << endl;
		PrintShaderCompileError(shaderID);
	}
	/* End here */

	return shaderID; // Returns compiled shader ID
}


// Create program object
static GLuint createShaderProgram(const string& vertexShader, const string& fragmentShader) {

	GLuint vertexShaderComp = ShaderCompiler(vertexShader, GL_VERTEX_SHADER); // Compile vertex shader

	GLuint fragmentShaderComp = ShaderCompiler(fragmentShader, GL_FRAGMENT_SHADER); // Compile fragment shader

	GLuint shaderProgram = glCreateProgram(); // Create program object

	// Attached vertex and fragment shaders to program object
	glAttachShader(shaderProgram, vertexShaderComp);
	glAttachShader(shaderProgram, fragmentShaderComp);

	glLinkProgram(shaderProgram); // Link shaders to create executable

	/* Shader Linking Error Check */
	GLint linked;
	IsOpenGLError();
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader Linking Failed!" << endl;
		PrintShaderLinkingError(shaderProgram);
	}
	/* End here */

	glValidateProgram(shaderProgram);

	// Delete vertex and fragment shaders
	glDeleteShader(vertexShaderComp);
	glDeleteShader(fragmentShaderComp);

	return shaderProgram; // Returns shader program
}


// Function to create shader source code
void ShaderSourceCode() {
	// Write vertex shader source code
	vertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;"
		"layout(location = 1) in vec2 texCoord;"
		"layout(location = 2) in vec3 normal;"

		"out vec2 o_texCoord;"
		"out vec3 o_normal;"
		"out vec3 fragPos;"

		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"

		"void main()\n"
		"{\n"
		"gl_Position = projection * view * model * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);"
		"o_texCoord = texCoord;"
		"o_normal = mat3(transpose(inverse(model))) * normal;"
		"fragPos = vec3(model * vec4(vPosition, 1.0f));"
		"}\n";
	// Write fragment shader source code
	fragmentShaderSource =
		"#version 330 core\n"
		"in vec2 o_texCoord;"
		"in vec3 o_normal;"
		"in vec3 fragPos;"

		"out vec4 fragColor;"

		"uniform sampler2D myTexture;"
		"uniform vec3 objectColor;"
		"uniform vec3 lightColor1;"
		"uniform vec3 lightPos1;"
		"uniform vec3 lightColor2;"
		"uniform vec3 lightPos2;"
		"uniform vec3 viewPos;"

		"void main()\n"
		"{\n"
		"//AMBINET LIGHT\n"
		"float ambientStrength = 0.2f;"
		"vec3 ambient = ambientStrength * lightColor1;"

		"//DIFFUSE LIGHT 1\n"
		"vec3 norm = normalize(o_normal);"
		"vec3 lightDir1 = normalize(lightPos1 - fragPos);"
		"float diff1 = max(dot(norm, lightDir1), 0.0);"
		"vec3 diffuse1 = diff1 * lightColor1;"

		"//DIFFUSE LIGHT 2\n"
		"vec3 lightDir2 = normalize(lightPos2 - fragPos);"
		"float diff2 = max(dot(norm, lightDir2), 0.0);"
		"vec3 diffuse2 = diff2 * lightColor2;"

		"//SPECULAR LIGHT 1\n"
		"float specularStrength1 = 2f;"
		"vec3 viewDir1 = normalize(viewPos - fragPos);"
		"vec3 reflectDir1 = reflect(-lightDir1, norm);"
		"float spec1 = pow(max(dot(viewDir1, reflectDir1), 0.0), 50);"
		"vec3 specular1 = specularStrength1 * spec1 * lightColor1;"

		"//SPECULAR LIGHT 2\n"
		"float specularStrength2 = 2.5f;"
		"vec3 viewDir2 = normalize(viewPos - fragPos);"
		"vec3 reflectDir2 = reflect(-lightDir2, norm);"
		"float spec2 = pow(max(dot(viewDir2, reflectDir2), 0.0), 100);"
		"vec3 specular2 = specularStrength2 * spec2 * lightColor2;"

		"vec3 result = (ambient + diffuse1 + diffuse2 + specular1 + specular2) * objectColor;"
		"fragColor = texture(myTexture, o_texCoord) * vec4(result, 1.0f);"
		"}\n";

	// Write lamp vertex shader source code
	lightVertexShaderSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vPosition;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main()\n"
		"{\n"
		"gl_Position = projection * view * model * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);"
		"}\n";
	// Write lamp fragment shader source code
	lightFragmentShaderSource =
		"#version 330 core\n"
		"out vec4 fragColor;"
		"void main()\n"
		"{\n"
		"fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
		"}\n";
}


/*
	 ||| --                                                -- |||
	 ||| -- FUNCTION SETTING VERTICIES, ARRAYS, & TEXTURES -- |||
	 ||| --                                                -- |||
*/
// Generate vertices, indicies, create VAO, create buffers, associate VAO with VBO, load vertices to VBOs, associate VBOs with VAs (Vertex Attributes)
void CreateVertices() {

	// Define vertecies for a plane
	GLfloat planeVertices[] = {
		4.0f, -5.0f, -2.0f, // Vert 0 (Back left of plane)
		0.0f, 1.0f,        // (UV) Top left
		0.0f, 0.0f, 1.0f, // Normal - positive Z

		4.0f, -5.0f, 2.0f,  // Vert 1 (Back right of plane)
		1.0f, 1.0f,        // (UV) Top right
		0.0f, 0.0f, 1.0f, // Normal - positive Z

		-4.0f, -5.0f, -2.0f, // Vert 3 (Front left of plane)
		0.0f, 0.0f,         // (UV) Bottom left
		0.0f, 0.0f, 1.0f,  // Normal - positive Z

		-4.0f, -5.0f, 2.0f, // Vert 2 (Front right of plane)
		1.0f, 0.0f,        // (UV) Bottom right
		0.0f, 0.0f, 1.0f  // Normal - positive Z
	};
	// Define vertecies for a box
	GLfloat boxVerticies[] = {
		0.5f, 0.5f, 0.0f,   // vert 0 (top left)
		0.0f, 1.0f,        // (UV) Top left
		0.0f, 0.0f, 1.0f, // Normal - positive Z

		-0.5f, 0.5f, 0.0f,  // vert 1 (top right)
		1.0f, 1.0f,        // (UV) Top right
		0.0f, 0.0f, 1.0f, // Normal - positive Z

		0.5f, -0.5f, 0.0f,  // vert 2 (bottom left)
		0.0f, 0.0f,        // (UV) Bottom left
		0.0f, 0.0f, 1.0f, // Normal - positive Z

		-0.5f, -0.5f, 0.0f, // vert 3 (bottom right)
		1.0f, 0.0f,        // (UV) Bottom right
		0.0f, 0.0f, 1.0f  // Normal - positive Z
	};
	// Define vertecies for a cylinder
	GLfloat cylinderVerticies[] = { //18 degrees
		0.0f, 0.38f, 0.0f,   // vert 0 (top mid)
		0.5f, 0.5f,        // (UV) Top Mid
		0.0f, 1.0f, 0.0f, // Normal - positive Y

		0.02369f, 0.38f, 0.18f, // vert 1 (top right)
		1.0f, 1.0f,             // (UV) Top right
		0.0f, 0.0f, 1.0f,      // Normal - positive Z
	
		-0.02369f, 0.38f, 0.18f, // vert 2 (top left)
		0.0f, 1.0f,              // (UV) Top left
		0.0f, 0.0f, 1.0f,       // Normal - positive Z

		0.02106f, -0.38f, 0.16f, // vert 3 (bottom right)
		1.0f, 0.0f,             // (UV) Bottom right
		0.0f, 0.0f, 1.0f,      // Normal - positive Z

		-0.02106f, -0.38f, 0.16f, // vert 4 (bottom left)
		0.0f, 0.0f,              // (UV) Bottom left
		0.0f, 0.0f, 1.0f,       // Normal - positive Z

		0.0f, -0.38f, 0.0f, // vert 5 (bottom mid)
		0.5f, 0.5f,       // (UV) Bottom mid
		0.0f, -1.0f, 0.0f // Normal - negative Y
	};
	// Define vertecies for a sphere
	GLfloat sphereVerticies[] = {
		0.0f, 1.0f, 0.0f,   // vert 0 (top)
		0.5f, 1.0f,        // (UV) Top left
		0.0f, 1.0f, 0.0f, // Normal - positive Y
		// TOP VERTICES
		0.3827f, 0.9239f, 0.0504f, // vert 1 (top 1 right)
		0.6f, 0.9f,               // (UV) Top left
		0.0f, 0.2f, 0.8f,        // Normal - positive Z

		0.3827f, 0.9239f, -0.0504f, // vert 2 (top 1 left)
		0.4f, 0.9f,                // (UV) Top left
		0.0f, 0.2f, 0.8f,         // Normal - positive Z

		0.7071f, 0.7071f, 0.0931f, // vert 3 (top mid right)
		0.7f, 0.8f,               // (UV) Top left
		0.0f, 0.4f, 0.6f,        // Normal - positive Z

		0.7071f, 0.7071f, -0.0931f, // vert 4 (top mid left)
		0.3f, 0.8f,                // (UV) Top left
		0.0f, 0.4f, 0.6f,         // Normal - positive Z

		0.9239f, 0.3827f, 0.1217f, // vert 5 (top 2 right)
		0.8f, 0.7f,               // (UV) Top left
		0.0f, 0.6f, 0.4f,        // Normal - positive Z

		0.9239f, 0.3827f, -0.1217f, // vert 6 (top 2 left)
		0.2f, 0.7f,                // (UV) Top left
		0.0f, 0.6f, 0.4f,         // Normal - positive Z
		// MID VERTICES - START
		1.0f, 0.05f, 0.1317f, // vert 7 (mid right)
		1.0f, 0.6f,          // (UV) Top left
		0.0f, 0.0f, 1.0f,   // Normal - positive Z

		1.0f, 0.05f, -0.1317f, // vert 8 (mid left)
		0.0f, 0.6f,           // (UV) Top left
		0.0f, 0.0f, 1.0f,    // Normal - positive Z

		1.0f, -0.05f, 0.1317f, // vert 9 (mid right)
		1.0f, 0.04f,          // (UV) Top left
		0.0f, 0.0f, 1.0f,    // Normal - positive Z

		1.0f, -0.05f, -0.1317f, // vert 10 (mid left)
		0.0f, 0.4f,            // (UV) Top left
		0.0f, 0.0f, 1.0f,     // Normal - positive Z
		// MID VERTICES - END
		0.9239f, -0.3827f, 0.1217f, // vert 11 (bottom 2 right)
		0.8f, 0.3f,              // (UV) Top left
		0.0f, -0.6f, 0.4f,     // Normal - positive Z

		0.9239f, -0.3827f, -0.1217f, // vert 12 (bottom 2 left)
		0.2f, 0.3f,               // (UV) Top left
		0.0f, -0.6f, 0.4f,      // Normal - positive Z

		0.7071f, -0.7071f, 0.0931f, // vert 13 (bottom mid right)
		0.7f, 0.2f,                // (UV) Top left
		0.0f, -0.4f, 0.6f,       // Normal - positive Z

		0.7071f, -0.7071f, -0.0931f, // vert 14 (bottom mid left)
		0.3f, 0.2f,                 // (UV) Top left
		0.0f, -0.4f, 0.6f,        // Normal - positive Z

		0.3827f, -0.9239f, 0.0504f, // vert 15 (bottom 1 right)
		0.6f, 0.1f,                // (UV) Top left
		0.0f, -0.2f, 0.8f,       // Normal - positive Z

		0.3827f, -0.9239f, -0.0504f, // vert 16 (bottom 1 left)
		0.4f, 0.1f,                 // (UV) Top left
		0.0f, -0.2f, 0.8f,        // Normal - positive Z
		// BOTTOM VERTICES
		0.0f, -1.0f, 0.0f,   // vert 17 (top)
		0.5f, 0.0f,         // (UV) Top left
		0.0f, -1.0f, 0.0f, // Normal - negative Y
	};
	// Define vertecies for a box (light)
	GLfloat lightVerticies[] = {
		1.0, -1.0, 1.0,   // index 0
		-1.0, -1.0, 1.0,  // index 1
		1.0, -1.0, -1.0,  // index 2
		-1.0, -1.0, -1.0, // index 3
	};
	// Define indicies
	GLubyte indices[] = {
	0, 1, 2,
	1, 2, 3
	};
	GLubyte cylinderIndices[] = {
	0, 1, 2,
	1, 2, 3,
	2, 3, 4,
	3, 4, 5
	};
	GLubyte sphereIndices[] = {
	0, 1, 2,
	1, 2, 3,
	2, 3, 4,
	3, 4, 5,
	4, 5, 6,
	5, 6, 7,
	6, 7, 8,
	7, 8, 9,
	8, 9, 10,
	9, 10, 11,
	10, 11, 12,
	11, 12, 13,
	12, 13, 14,
	13, 14, 15,
	14, 15, 16,
	15, 16, 17
	};

	// *TRIANGLE VAO - START* //
	glGenVertexArrays(1, &planeVAO); // Create triangle VAO
	glGenBuffers(1, &planeVBO);     // Create buffer object triangle VBO
	glGenBuffers(1, &planeEBO);    // Create buffer object triangle EBO

	glBindVertexArray(planeVAO);  // Activate triangle VAO for triangle VBO association
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO); // Enable triangle VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO); // Enable triangle EBO

	// Load data for verticies & indicies
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW); // Loads vertex attributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Loads indices

	// Specify position attribute location and layout for GPU
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Specify texture attribute location and layout for GPU
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Specify normal attribute location and layout for GPU
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VOA or close off (Must call VOA explicitly in loop)
	// *TRIANGLE VAO - END* //

	// *PLANE VAO - START* //
	glGenVertexArrays(1, &boxVAO); // Create plane VAO
	glGenBuffers(1, &boxVBO);     // Create buffer object plane VBO
	glGenBuffers(1, &boxEBO);    // Create buffer object plane EBO

	glBindVertexArray(boxVAO);   // Activate plane VAO for plane VBO association
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO); // Enable plane VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxEBO); // Enable plane EBO

	// Load data for verticies & indicies
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVerticies), boxVerticies, GL_STATIC_DRAW); // Loads vertex attributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Loads indices

	// Specify position attribute location and layout for GPU
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Specify texture attribute location and layout for GPU
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Specify normal attribute location and layout for GPU
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VOA or close off (Must call VOA explicitly in loop)
	// *PLANE VAO - END* //

	// *CYLINDER VAO - START* //
	glGenVertexArrays(1, &cylinderVAO); // Create cylinder VAO
	glGenBuffers(1, &cylinderVBO);     // Create buffer object cylinder VBO
	glGenBuffers(1, &cylinderEBO);    // Create buffer object cylinder EBO

	glBindVertexArray(cylinderVAO);   // Activate plane VAO for cylinder VBO association
	glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO); // Enable cylinder VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO); // Enable cylinder EBO

	// Load data for verticies & indicies
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderVerticies), cylinderVerticies, GL_STATIC_DRAW); // Loads vertex attributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinderIndices), cylinderIndices, GL_STATIC_DRAW); // Loads indices

	// Specify position attribute location and layout for GPU
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Specify texture attribute location and layout for GPU
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Specify normal attribute location and layout for GPU
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VOA or close off (Must call VOA explicitly in loop)
	// *CYLINDER VAO - END* //

	// *SPHERE VAO - START* //
	glGenVertexArrays(1, &sphereVAO); // Create sphere VAO
	glGenBuffers(1, &sphereVBO);     // Create buffer object sphere VBO
	glGenBuffers(1, &sphereEBO);    // Create buffer object sphere EBO

	glBindVertexArray(sphereVAO);   // Activate plane VAO for sphere VBO association
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO); // Enable sphere VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO); // Enable sphere EBO

	// Load data for verticies & indicies
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVerticies), sphereVerticies, GL_STATIC_DRAW); // Loads vertex attributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW); // Loads indices

	// Specify position attribute location and layout for GPU
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Specify texture attribute location and layout for GPU
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Specify normal attribute location and layout for GPU
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VOA or close off (Must call VOA explicitly in loop)
	// *SPHERE VAO - END* //

	// *LIGHT VAO - START* //
	glGenVertexArrays(1, &lightVAO); // Create light VAA
	glGenBuffers(1, &lightVBO); //Create VBO
	glGenBuffers(1, &lightEBO); //Create EBO

	glBindVertexArray(lightVAO);  // Activate VAO for triangle VBO association
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO); // Enable VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO); // Enable EBO

	// Load data for verticies & indicies
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVerticies), lightVerticies, GL_STATIC_DRAW); // Loads vertex attributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Loads indices

	// Specify position attribute location and layout for GPU
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VOA or close off (Must call VOA explicitly 
	// *LIGHT VAO - END* //

	// LOAD & GENERATE TEXTURES
	// *DESK TEXTURE - START* //
	glGenTextures(1, &planeTex); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, planeTex); // Activate texture object
	// Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Sets the testure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int imageWidth, imageHeight, nrChannels;
	unsigned char* imageData;
	//stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load("DeskTop.jpg", &imageWidth, &imageHeight, &nrChannels, 0); // Load Image

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Handles resolution of texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageData); // Free texture 
	// *DESK TEXTURE - END* //

	// *MONITOR SCREEN TEXTURE - START* //
	glGenTextures(1, &monitorTex1); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, monitorTex1); // Activate texture object
	// Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Sets the testure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load("NewScreen.jpg", &imageWidth, &imageHeight, &nrChannels, 0); // Load Image

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Handles resolution of texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageData); // Free texture 
	// *MONITOR SCREEN TEXTURE - END* //

	// *MONITOR STAND TEXTURE - START* //
	glGenTextures(1, &monitorTex2); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, monitorTex2); // Activate texture object
	// Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Sets the testure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load("StandTexture.jpg", &imageWidth, &imageHeight, &nrChannels, 0); // Load Image

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Handles resolution of texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageData); // Free texture 
	// *MONITOR STAND TEXTURE - END* //

	// *WATER BOTTLE BODY TEXTURE - START* //
	glGenTextures(1, &waterBottleTex1); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, waterBottleTex1); // Activate texture object
	// Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Sets the testure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load("WaterBottleBody.jpg", &imageWidth, &imageHeight, &nrChannels, 0); // Load Image

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Handles resolution of texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageData); // Free texture 
	// *WATER BOTTLE BODY TEXTURE - END* //

	// *WATER BOTTLE TOP TEXTURE - START* //
	glGenTextures(1, &waterBottleTex2); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, waterBottleTex2); // Activate texture object
	// Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Sets the testure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load("Metal.jpg", &imageWidth, &imageHeight, &nrChannels, 0); // Load Image

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Handles resolution of texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageData); // Free texture 
	// *WATER BOTTLE TOP TEXTURE - END* //

	// *TARANTULA BLACK TEXTURE - START* //
	glGenTextures(1, &tarantualTex1); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, tarantualTex1); // Activate texture object
	// Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Sets the testure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load("BlackFuz.jpg", &imageWidth, &imageHeight, &nrChannels, 0); // Load Image

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Handles resolution of texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageData); // Free texture 
	// *TARANTULA BLACK TEXTURE - END* //
	//  - - - - - - - - - - - - - - - - - - - - - - - - - -
	// *TARANTULA ORANGE TEXTURE - START* //
	glGenTextures(1, &tarantualTex2); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, tarantualTex2); // Activate texture object
	// Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Sets the testure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load("OrangeFuz.jpg", &imageWidth, &imageHeight, &nrChannels, 0); // Load Image

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Handles resolution of texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageData); // Free texture 
	// *TARANTULA ORANGE TEXTURE - END* //

	// *TARANTULA BROWN TEXTURE - START* //
	glGenTextures(1, &tarantualTex3); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, tarantualTex3); // Activate texture object
	// Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Sets the testure filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load("BrownFuz.jpg", &imageWidth, &imageHeight, &nrChannels, 0); // Load Image

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Handles resolution of texture
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(imageData); // Free texture 
	// *TARANTULA BROWN TEXTURE - END* //

	glBindTexture(GL_TEXTURE_2D, 0);
}


// Sets the projection matrix to either be orthographic or perspective based on if orthoView is true or false
void viewMode(GLFWwindow* window, double currentTime) 
{
	// Resize window and graphics simultaneously
	glViewport(0, 0, width, height);
	glfwGetFramebufferSize(window, &width, &height);

	// Build projectMatrix in orthographic view
	if (orthoView == true) {
		projectionMatrix = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, -5.0f, 5.0f);
	}
	// Build projectionMatrix in perspective view
	else if (orthoView == false) {
		aspectRatio = (float)width / (float)height;
		projectionMatrix = glm::perspective(1.0f, aspectRatio, 0.1f, 1000.0f);
	}
}

// - ENHANCED BY ADDING ANIMATION FUNCTIONS BELOW FOR BALL AND TARANTUAL - //
/*
	 ||| --                             -- |||
	 ||| -- ANIMATION FUNCTIONS - START -- |||
	 ||| --                             -- |||
*/
// Function to handle the baseballs scale up and move animation
void ballAnimation()
{
	// If ball is set to increase, then increment the scale and position of the ball
	if (ballIncrease)
	{
		ballScale += 0.001f;
		ballPositionY += 0.002f;
		// If the balls scale reaches the max point set then set the ball to decrease
		if (ballScale >= 0.6f)
		{
			ballIncrease = false;
		}
	}
	// If ball is set to decrease, then decrement the scale and position of the ball
	else
	{
		ballScale -= 0.001f;
		ballPositionY -= 0.002f;
		// If the balls scale reaches the min point set then set the ball to increase
		if (ballScale <= 0.18f)
		{
			// Every time the ball is at its normal size the direction it moves will be flipped
			ballIncrease = true;
			if (ballForward)
			{
				ballForward = false;
			}
			else
			{
				ballForward = true;
			}
		}
	}
	// If the ball is moving forward, increment the z axis movement, if moving backwards decrement the z axis movement
	if (ballForward)
	{
		ballPositionZ -= 0.001f;
	}
	else
	{
		ballPositionZ += 0.001f;
	}
}

// Function to handle the tarantulas stand up animation
void tarantualAnimation()
{

	if (tarantualStandUp)
	{
		legRotation -= 0.1f;
		legPosY += 0.0001f;
		pedipalPosY += 0.0004f;
		bodyHeadPosY += 0.0004f;

		if (legRotation <= -50.0f)
		{
			tarantualStandUp = false;
		}
	}
	else
	{
		legRotation += 0.1f;
		legPosY -= 0.0001f;
		pedipalPosY -= 0.0004f;
		bodyHeadPosY -= 0.0004f;

		if (legRotation >= 0.0f)
		{
			tarantualStandUp = true;
		}
	}
}
/*
	 ||| --                            -- |||
	 ||| -- ANIMATION FUNCTIONS - END -- |||
	 ||| --                            -- |||
*/

// - ENHANCED BY ADDING FUNCITON TO HANDLE ORBITING LIGHT - //
// Function to handle the rectangular orbit of the lights around the scene
void lightOrbit()
{
	// If light one is set to orbit then it checks starts moving around the scene
	if (light1Orbit == true) {
		// If else statment will change the left and right direction of the light
		// based on if the light moves past -5 or 5 on the x axis.
		if (light1Right == true) {
			lightPosition1.x += 0.01f;
			if (lightPosition1.x >= 5.0f) {
				light1Right = false;
			}
		}
		else {
			lightPosition1.x -= 0.01f;
			if (lightPosition1.x <= -5.0f) {
				light1Right = true;
			}
		}
		// If else statment will change the forward and backwards direction of the light
		// based on if the light moves past -5 or 5 on the z axis.
		if (light1Forward == true) {
			lightPosition1.z += 0.01f;
			if (lightPosition1.z >= 5.0f) {
				light1Forward = false;
			}
		}
		else {
			lightPosition1.z -= 0.01f;
			if (lightPosition1.z <= -5.0f) {
				light1Forward = true;
			}
		}
	}

	// If light two is set to orbit then it checks starts moving around the scene
	if (light2Orbit == true) {
		// If else statment will change the left and right direction of the light
		// based on if the light moves past -5 or 5 on the x axis.
		if (light2Right == true) {
			lightPosition2.x += 0.01f;
			if (lightPosition2.x >= 5.0f) {
				light2Right = false;
			}
		}
		else {
			lightPosition2.x -= 0.01f;
			if (lightPosition2.x <= -5.0f) {
				light2Right = true;
			}
		}
		// If else statment will change the forward and backwards direction of the light
		// based on if the light moves past -5 or 5 on the z axis.
		if (light2Forward == true) {
			lightPosition2.z += 0.01f;
			if (lightPosition2.z >= 5.0f) {
				light2Forward = false;
			}
		}
		else {
			lightPosition2.z -= 0.01f;
			if (lightPosition2.z <= -5.0f) {
				light2Forward = true;
			}
		}
	}
}

/*
	 ||| --               -- |||
	 ||| -- MAIN FUNCTION -- |||
	 ||| --               -- |||
*/
// Main Function
int main(void) 
{
	initPositions();// Set up initial camera postion and rotation

	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(width, height, "7-1 Project - Dylan Kimball", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Set input callback funcitons
	glfwSetCursorPosCallback(window, mouse_callback); // Tell GLFW to use mouseCallBack funciton when mouse moves
	glfwSetScrollCallback(window, scroll_callback); // Tell GLFW to use cursorPositoinCallback funciton when scroll wheel moves
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Tell GLFW to capture the mouse

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
		cout << "Error!" << endl;

	CreateVertices(); // Generate vertices, indicies, create VAO, create buffers, associate VAO with VBO, load vertices to VBOs, associate VBOs with VAs (Vertex Attributes)

	ShaderSourceCode(); // Gets shader source code

	shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource); // Create shader (program object)
	lightShaderProgram = createShaderProgram(lightVertexShaderSource, lightFragmentShaderSource); // Create shader (light shader)

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) 
	{
		processInput(window); // Checks for keyboard input each frame

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Z - buffer operation(hsr removal) & remove animation trails

		glUseProgram(shaderProgram); // Call shader per-frame when updating attributes

		// Select uniform location and set matrix
		modelLoc = glGetUniformLocation(shaderProgram, "model");
		viewLoc = glGetUniformLocation(shaderProgram, "view");
		projLoc = glGetUniformLocation(shaderProgram, "projection");

		// Select uniform location for light 1 & 2, object color, and light 1 & 2 position location
		objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
		light1ColorLoc = glGetUniformLocation(shaderProgram, "lightColor1");
		light1PosLoc = glGetUniformLocation(shaderProgram, "lightPos1");
		light2ColorLoc = glGetUniformLocation(shaderProgram, "lightColor2");
		light2PosLoc = glGetUniformLocation(shaderProgram, "lightPos2");
		viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

		// Assign light and object colors
		glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f); // object color
		glUniform3f(light1ColorLoc, 1.0f, 1.0f, 0.9f); // light 1 color  (slight yellow)
		glUniform3f(light2ColorLoc, 1.0f, 1.0f, 1.0f); // light 2 color  (white)

		// Set light 1 & 2 position
		glUniform3f(light1PosLoc, lightPosition1.x, lightPosition1.y, lightPosition1.z);
		glUniform3f(light2PosLoc, lightPosition2.x, lightPosition2.y, lightPosition2.z);

		// Specify camera position
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		viewMode(window, glfwGetTime()); // Changes projection matrix each frame to either orthographic or perspective based on user input.

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		// - ENHANCED TO ADD ANIMATION & ORBIT FUNCTIONS INTO THE MAIN LOOP - //
		// ANIMATION & ORBIT FUNCTIONS
		ballAnimation();      // Call function to change the baseballs parametrs for the frame
		tarantualAnimation(); // Call function to change tarantuals paramaters for the frame
		lightOrbit();         // Call function to have light orbit the scene in a rectangular manner

		glEnable(GL_DEPTH_TEST); // Z-buffer operation
		glDepthFunc(GL_LEQUAL); // Used with Depth test
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe (Comment out for normal rendering)

		// Draw primitives
		// *Plane - START* //
		glBindTexture(GL_TEXTURE_2D, planeTex); // Activate texture 1 to apply to objects
		glBindVertexArray(planeVAO); // Activate VAO to draw the plane(now references VBO and VAO association)
		drawPlane(); // Draws base for all other objects to sit on

		// Incase different VAO and texture will be used (unbind current VAO and texture)
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		// *Plane - END* //

		glBindVertexArray(boxVAO); // Activate VAO to draw Monitors (now references VBO and VAO association)
		// *Monitor 1 - START* //
		// Passes plane positions, rotations, and scales
		drawRectangle(1, 6, screen1Positions, monitor1RotationsAllX, monitor1RotationsAllY, monitor1RotationsAllZ, 2.5f, 1.5f, 1.0f, 0.1f, 2.5f, 1.0f, 0.1f, 1.5f, 1.0f, 0.0f, 0.0f, -0.18f);              // Draw screen1
		drawRectangle(0, 6, stand1Positions, monitor1RotationsAllX, monitor1RotationsAllY, monitor1RotationsAllZ, 0.3f, 1.1f, 1.0f, 0.2f, 0.3f, 1.0f, 0.2f, 1.1f, 0.3f, 0.0f, -0.4f, -0.5f);               // Draw stand1 (No bottom face)
		drawRectangle(0, 4, connector1Positions, monitor1Rotations4FacesX, monitor1Rotations4FacesY, monitor1Rotations4FacesZ, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.28f, 0.1f, 1.0f, 0.0f, 0.0f, -0.35f); // Draw connector1 (No front or back faces)
		drawRectangle(0, 4, stand1BasePositions1, base1Rotations1AllX, base1Rotations1AllY, base1Rotations1AllZ, 1.3f, 0.1f, 1.0f, 0.1f, 1.0f, 1.0f, 1.2f, 0.1f, 1.0f, -0.7f, -1.11f, -0.5f);              // Draw stand standleg1 (No front or back faces)
		drawRectangle(0, 4, stand1BasePositions1, base1Rotations2AllX, base1Rotations2AllY, base1Rotations2AllZ, 1.3f, 0.1f, 1.0f, 0.1f, 1.0f, 1.0f, 1.2f, 0.1f, 1.0f, 0.7f, -1.11f, -0.5f);               // Draw stand standleg2 (No front or back faces)
		drawRectangle(0, 4, stand1BasePositions2, base1Rotations3AllX, base1Rotations3AllY, base1Rotations3AllZ, 0.65f, 0.1f, 1.0f, 0.1f, 1.0f, 1.0f, 0.65f, 0.1f, 1.0f, 0.0f, -1.11f, -0.76f);            // Draw stand standleg3 (No front or back faces)
		// *Monitor 1 - END* //

		// *Monitor 2 (Vertical) - START* //
		// Passes plane positions, rotations, and scales
		drawRectangle(1, 6, screen2Positions, monitor2RotationsAllX, monitor2RotationsAllY, monitor2RotationsAllZ, 1.0f, 2.0f, 1.0f, 0.1f, 1.0f, 1.0f, 0.1f, 2.0f, 1.0f, -1.72f, 0.0f, 0.0f);              // Draw screen2
		drawRectangle(0, 5, stand2Positions, monitor2RotationsAllX, monitor2RotationsAllY, monitor2RotationsAllZ, 0.3f, 1.35f, 1.0f, 0.1f, 0.3f, 1.0f, 0.1f, 1.35f, 0.3f, -1.72f, 0.0f, 0.0f);             // Draw stand2 (No bottom face)
		drawRectangle(0, 5, stand2BasePositions, monitor2RotationsAllX, monitor2RotationsAllY, monitor2RotationsAllZ, 0.7f, 0.05f, 1.0f, 0.7f, 0.7f, 1.0f, 0.7f, 0.05f, 1.0f, -1.72f, 0.0f, 0.0f);         // Draw standbase2 (No bottom face)
		drawRectangle(0, 4, connector2Positions, monitor2Rotations4FacesX, monitor2Rotations4FacesY, monitor2Rotations4FacesZ, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.28f, 0.1f, 1.0f, -1.72f, 0.0f, 0.0f); // Draw stand connector2 (No front or back faces)

		// Incase different VAO and texture will be used (unbind current VAO and texture)
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// *Monitor 2 (Vertical) - END* //

		glBindVertexArray(cylinderVAO); // Activate VAO to draw water bottle (now references VBO and VAO association)

		// *Water bottle - START* //
		// Passes texture, number of faces(rotations), indice #, rotation angle, rotation on X Y Z, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(waterBottleTex1, 1, 24, 12, 15.0f, 0.0f, 0.0f, 0.0f, 1.0f ,1.0f ,1.0f, 0.8f, -0.87f, 0.1f); // Draws main water bottle body
		drawCylinderSphere(waterBottleTex2, 1, 24, 12, 15.0f, 0.0f, 0.0f, 0.0f, 1.1f, 0.1f, 1.1f, 0.8f, -0.5f, 0.1f);  // Draws upper metal piece of the water bottle
		// *Water bottle - END* //

		// - ENHANCED BY ONLY REQUIRING ONE CALL TO DRAW ALL 10 TARANTULA LEGS AND TAKING VARIABLES TO ANIMATE LEGS - //
		// *Tarantula legs - START* //
		// Passes texture, number of faces(rotations), indice #, rotation angle, rotation on X Y Z, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 10, 24, 12, 15.0f, 90.0f, legRotation, 40.0f, 0.4f, 0.7f, 0.4f, 0.14f, legPosY, 0.2f); // Draws all 10 tarantaula legs

		// Incase different VAO and texture will be used (unbind current VAO and texture)
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		// *Tarantula legs - END* //

		glBindVertexArray(sphereVAO); // Activate VAO to draw water bottle (now references VBO and VAO association)

		// - ENHANCED BY MODIFYING FUNCTION TO ACCEPT VARIABLES THAT ANIMATE THE BALL - //
		// *Baseball - START* //
		// Passes texture, number of faces(rotations), indice #, rotation angle, rotation on X Y Z, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(waterBottleTex1, 1, 24, 48, 15.0f, 0.0f, 0.0f, 0.0f, ballScale, ballScale, ballScale, -1.0f, ballPositionY, ballPositionZ); // Draws baseball
		// *Baseball - END* //
		
		// - ENHANCED BY MODIFYING FUNCTION TO ACCEPT VARIABLES THAT ANIMATE THE HEAD AND BODY - //
		// *Tarantula Head & Body - START* //
		// Passes texture, number of faces(rotations), indice #, rotation angle, rotation on X Y Z, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex3, 1, 24, 48, 15.0f, 90.0f, 0.0f, 0.0f, 0.26f, 0.3f, 0.26f, -0.11f, bodyHeadPosY, 0.2f);  // Draws body of tarantula
		drawCylinderSphere(tarantualTex1, 1, 24, 48, 15.0f, 90.0f, 0.0f, 0.0f, 0.18f, 0.18f, 0.18f, -0.11f, bodyHeadPosY, 0.6f); // Draws head of tarantula

		// Incase different VAO, Shader, and texture will be used (unbind current VAO, shader and texture)
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		// *Tarantula Head & Body - END* //

		glUseProgram(lightShaderProgram); // Call light shader program

		// Select uniform location and set matrix
		light1ModelLoc = glGetUniformLocation(lightShaderProgram, "model");
		light1ViewLoc = glGetUniformLocation(lightShaderProgram, "view");
		light1ProjLoc = glGetUniformLocation(lightShaderProgram, "projection");

		glUniformMatrix4fv(light1ViewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(light1ProjLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glBindVertexArray(lightVAO); // Activate VAO to draw verticle Monitor (now references VBO and VAO association)
		
		// *Draw Lights - Start* //
		drawLight(rectanglePositions, lightRotationsX, lightRotationsY, lightRotationsZ, lightPosition1); // draws light cube 1
		drawLight(rectanglePositions, lightRotationsX, lightRotationsY, lightRotationsZ, lightPosition2); // draws light cube 2

		// Incase different VAO and texture will be used (unbind current VAO and texture)
		glBindVertexArray(0);
		glUseProgram(0);
		// *Draw Lights - END* //

		/* Swap front and back buffers */
		glfwSwapBuffers(window); // VSync operation

		/* Poll for and process events */
		glfwPollEvents(); // Detect keyboard and mouse input
	}

	glfwTerminate();
	return 0;
}


/*
	 ||| --                    -- |||
	 ||| -- CALLBACK FUNCTIONS -- |||
	 ||| --                    -- |||
*/
// DEFINE CALLBACK FUNCTIONS
// Checks for keys pressed
void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  // Exit program
	{
		glfwSetWindowShouldClose(window, true);
	}

	// *CAMERA MOVEMENT KEY BINDS* //
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // Move camera forward
	{
		cameraPos += camSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // Move camera backwards
	{
		cameraPos -= camSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // Move camera Left
	{ 
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // Move camera right
	{ 
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Move camera down
	{ 
		cameraPos -= camSpeed * cameraUp;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Move camera up
	{ 
		cameraPos += camSpeed * cameraUp;
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // Change between orthographic and perspective view
	{
		// Checks to see if last time P was pressed is greater than 0.01 second (Makes it so the view doesn't rapidly switch with one key press)
		if (glfwGetTime() - previousTime > 0.01) {
			if (orthoView == false) // Sets to true to switch to orthographic view
			{
				orthoView = true;
			}
			else if (orthoView == true) // Sets to false to switch to perspective view
			{
				orthoView = false;
			}
		}
		previousTime = glfwGetTime(); // Sets previous time to last time P was pressed
	}

	// - ENHANCED BY ADDING THE BELOW KEYBINDS TO MANIUPULATE LIGHT MOVEMENT - //
	// *LIGHT MANIPULATION KEY BINDS* //
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // Selects light 1 to move around
	{
		currentLight = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // Selects light 2 to move around
	{
		currentLight = 2;
	}
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // Moves either light 1 or 2 to the current cameras position
	{
		if (currentLight == 1)
		{
			lightPosition1 = cameraPos;
		}
		else
		{
			lightPosition2 = cameraPos;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // Moves either light 1 or 2 forwards
	{
		if (currentLight == 1)
		{
			lightPosition1.z -= lightMoveSpeed;
		}
		else
		{
			lightPosition2.z -= lightMoveSpeed;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // Moves either light 1 or 2 backwards
	{
		if (currentLight == 1)
		{
			lightPosition1.z += lightMoveSpeed;
		}
		else
		{
			lightPosition2.z += lightMoveSpeed;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // Moves either light 1 or 2 left
	{
		if (currentLight == 1)
		{
			lightPosition1.x -= lightMoveSpeed;
		}
		else
		{
			lightPosition2.x -= lightMoveSpeed;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Moves either light 1 or 2 right
	{
		if (currentLight == 1)
		{
			lightPosition1.x += lightMoveSpeed;
		}
		else
		{
			lightPosition2.x += lightMoveSpeed;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) // Moves either light 1 or 2 up
	{
		if (currentLight == 1)
		{
			lightPosition1.y += lightMoveSpeed;
		}
		else
		{
			lightPosition2.y += lightMoveSpeed;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) // Moves either light 1 or 2 down
	{
		if (currentLight == 1)
		{
			lightPosition1.y -= lightMoveSpeed;
		}
		else
		{
			lightPosition2.y -= lightMoveSpeed;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) // Sets orbit to true for either light 1 or 2
	{
		// Checks to see if last time F was pressed is greater than 0.01 second (Makes it so the view doesn't rapidly switch with one key press)
		if (glfwGetTime() - previousTime > 0.01) {
			// If light 1 or light 2 is selected then set light positino to orbit starting point and set orbit to true or false
			// depending on whether it is currently true or false
			if (currentLight == 1)
			{
				lightPosition1 = glm::vec3(0.0f, 4.0f, 5.0f);
				if (light1Orbit == false) {
					light1Orbit = true;
					cout << light1Orbit;
				}
				else {
					light1Orbit = false;
					light1Forward = false;
					light1Right = false;
				}
			}
			else
			{
				lightPosition2 = glm::vec3(0.0f, 4.0f, 5.0f);
				if (light2Orbit == false) {
					light2Orbit = true;
				}
				else {
					light2Orbit = false;
					light2Forward = false;
					light2Right = false;
				}
			}
		}
		previousTime = glfwGetTime(); // Sets previous time to last time P was pressed
	}
}


// Checks for mouse input
void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	// Checks to see if this is the first time mouse input is received
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false; // sets to false since this is only needed for the first time
	}

	// Reset camera position, rotation, and speed to initial settings
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
	{
		camSpeed = 0.05f;
		yaw = -90.0;
		pitch = 0.0f;
		lastX = width / 2;
		lastY = height / 2;

		cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	// Calculates the offset for the mouse movement between the last and current frame
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // y-coordinates range from bottom to top so it is reversed
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f; // Sets sensitivity for speed of looking around
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// Add offsets to the yaw and pitch
	yaw += xoffset;
	pitch += yoffset;

	// Constrains the camera so it can't go around and flip the perspective on the Y axis
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Calculates the direction using the pitch and yaw
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}


// Checks for scroll wheel movement
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
	// Scrolling foward increased camera speed, scrolling backwards decreases camera speed 
	camSpeed += (float)yoffset / 100; // Dividing by 100 makes it so it either increases or decreased by 0.01 instead of just 1.0

	// Constrains the camera speed so it can be to low or to high
	if (camSpeed < 0.01f)
		camSpeed = 0.01f;
	if (camSpeed > 0.5f)
		camSpeed = 0.5f;
}