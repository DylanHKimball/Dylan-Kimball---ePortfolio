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

int width, height;
double previousTime;
float aspectRatio; // Projection matrix aspect ratio
float camSpeed, yaw, pitch, lastX, lastY; // Camera location (x,y,z), speed, yaw, pitch, position
glm::vec3 cameraPos, cameraFront, cameraUp, direction; // Camera varaibles
bool firstMouse = true, orthoView = false; // Variable to check for first mouse input

unsigned int planeTex, monitorTex1, monitorTex2, waterBottleTex1, waterBottleTex2, tarantualTex1, tarantualTex2, tarantualTex3;
string vertexShaderSource, fragmentShaderSource, lightVertexShaderSource, lightFragmentShaderSource;
GLuint boxVBO, boxVAO, boxEBO, planeVBO, planeVAO, planeEBO, lightVBO, lightVAO, lightEBO, // VAO, VBO, and EBO variables
	   cylinderVBO, cylinderVAO, cylinderEBO, sphereVBO, sphereVAO, sphereEBO;            // - - - - - - - - - - - - - - 
GLuint modelLoc, viewLoc, projLoc, objectColorLoc, light1ColorLoc, light1ModelLoc, light1ViewLoc, light1ProjLoc, light1PosLoc, // Uniform reference variables
	   light2ColorLoc, light2ModelLoc, light2ViewLoc, light2ProjLoc, light2PosLoc, viewPosLoc;                                // - - - - - - - - - - - - - - 
glm::mat4 projectionMatrix, modelMatrix;  // Matricies variables
glm::mat4 viewMatrix = glm::mat4(1.0f);   // Initialize view matrix to identity matrix
GLuint shaderProgram, lightShaderProgram; // Shader program variable for complied shaders

// Define functions to process keyboard/mouse input
void processInput(GLFWwindow* window); // Processes keyboard input
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // Processes mouse button / cursor input
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
	 ||| --                                 -- |||
	 ||| -- GLSL ERROR CHECKING DEFINITIONS -- |||
	 ||| --                                 -- |||
*/
/* GLSL Error Checking Definitions */
void PrintShaderCompileError(GLuint shader) {
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


void PrintShaderLinkingError(int prog) {
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


bool IsOpenGLError() {
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
	float faceFBY, float faceFBZ, float faceTBX, float faceTBY, float faceTBZ, float faceLRX, float faceLRY, float faceLRZ, float posX, float posY, float posZ) {
	// Use loop to build Model matrix for triangle
	for (int i = 0; i < faceNumber; i++) {
		if (texture == 1 && i == 0) { // Makes it so the screen of the monitor shows a screen texture instead of the smooth plastic.
			glBindTexture(GL_TEXTURE_2D, monitorTex1); // Activate texture 2 to apply to objects
		}
		else { // Applies smooth plastic texture to entire monitor other than screen
			glBindTexture(GL_TEXTURE_2D, monitorTex2); // Activate texture 3 to apply to objects
		}
		// Apply Transform to model
		modelMatrix = glm::translate(glm::mat4(1.0f), positions[i] + glm::vec3(posX, posY, posZ)); // Position at 0,0,0
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsX[i]), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate on x axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsY[i]), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate on y axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsZ[i]), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate on z axis		
		if (i == 1 || i == 3) { // Left and Right Faces
			modelMatrix = glm::scale(modelMatrix, glm::vec3(faceLRX, faceLRY, faceLRZ)); // Change scale of object
		}
		else if (i == 4 || i == 5) { // Top and Bottom Faces
			modelMatrix = glm::scale(modelMatrix, glm::vec3(faceTBX, faceTBY, faceTBZ)); // Change scale of object
		}
		else { // Front and Back faces
			modelMatrix = glm::scale(modelMatrix, glm::vec3(faceFBX, faceFBY, faceFBZ)); // Change scale of object
		}

		// Pass transform to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
	}
}


// Function to draw cyliders and spheres (Needs position, rotation, & scale for front/back faces, top/bottom faces, & left/right faces on the X, Y, & Z axis)
void drawCylinderSphere(unsigned int texture, int rotNumber, float indices, float rotation, float rotX, float rotZ, float scaleX, float scaleY, float scaleZ, float posX, float posY, float posZ) {
	// Use loop to build Model matrix for triangle
	float currAngle = 0.0;

	glBindTexture(GL_TEXTURE_2D, texture); // Activate texture for object

	for (int i = 0; i < rotNumber; i++) {
		// Apply Transform to model
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ)); // Position at 0,0,0
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate on x axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate on z axis
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, scaleZ)); // scale object
		modelMatrix = glm::rotate(modelMatrix, glm::radians(currAngle),glm::vec3(0.0f, 1.0f, 0.0f)); // rotate on y axis

		currAngle = currAngle + rotation;

		// Pass transform to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_BYTE, nullptr);
	}
}


// Draws base of triangle
void drawPlane() {
	// Use loop to build Model matrix for triangle
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.75f, 0.0f)); // Position plane

	// Pass transform to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
}


// Function to draw primitaves (Needs position, rotation, & scale for front/back faces, top/bottom faces, & left/right faces on the X, Y, & Z axis)
void drawLight(glm::vec3 positions[], glm::float32 rotationsX[], glm::float32 rotationsY[], glm::float32 rotationsZ[], glm::vec3 lightPosition) {
	// Use loop to build Model matrix for triangle
	for (int i = 0; i < 6; i++) {
		// Apply Transform to model
		modelMatrix = glm::translate(glm::mat4(1.0f), positions[i] / glm::vec3(8., 8., 8.) + lightPosition); // Position at proper location
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsX[i]), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate on x axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsY[i]), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate on y axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationsZ[i]), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate on z axis		
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
	yaw = -90.0;
	pitch = 0.0f;
	lastX = width / 2;
	lastY = height / 2;

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

	// Define vertecies for triangles (Creates a square plane)
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
	GLfloat lightVerticies[] = {
		1.0, -1.0, 1.0,   // index 0
		-1.0, -1.0, 1.0,  // index 1
		1.0, -1.0, -1.0,  // index 2
		-1.0, -1.0, -1.0, // index 3
	};
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

	// TRIANGLE VAO START
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

	// PLANE VAO START
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

	// CYLINDER VAO START
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

	// SPHERE VAO START
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

	// LIGHT VAO START
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


	// LOAD & GENERATE TEXTURES
	// DESK TEXTURE - START
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
	// DESK TEXTURE - END
	//  - - - - - - - - - - - - - - - - - - - - - - - - - -
	// MONITOR SCREEN TEXTURE - START
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
	// MONITOR SCREEN TEXTURE - END
	//  - - - - - - - - - - - - - - - - - - - - - - - - - -
	// MONITOR STAND TEXTURE - START
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
	// MONITOR STAND TEXTURE - END
		//  - - - - - - - - - - - - - - - - - - - - - - - - - -
	// WATER BOTTLE BODY TEXTURE - START
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
	// WATER BOTTLE BODY TEXTURE - END
		//  - - - - - - - - - - - - - - - - - - - - - - - - - -
	// WATER BOTTLE TOP TEXTURE - START
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
	// WATER BOTTLE TOP TEXTURE - END
	//  - - - - - - - - - - - - - - - - - - - - - - - - - -
	// TARANTULA BLACK TEXTURE - START
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
	// TARANTULA BLACK TEXTURE - END
	//  - - - - - - - - - - - - - - - - - - - - - - - - - -
	// TARANTULA ORANGE TEXTURE - START
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
	// TARANTULA ORANGE TEXTURE - END
	// - - - - - - - - - - - - - - - - - - - - - - - - - -
	// TARANTULA BROWN TEXTURE - START
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
	// TARANTULA BROWN TEXTURE - END

	glBindTexture(GL_TEXTURE_2D, 0);
}


// Sets the projection matrix to either be orthographic or perspective based on if orthoView is true or false
void viewMode(GLFWwindow* window, double currentTime) {
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


/*
	 ||| --               -- |||
	 ||| -- MAIN FUNCTION -- |||
	 ||| --               -- |||
*/
// Main Function
int main(void) {
	initPositions();// Set up initial camera postion and rotation

	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(width, height, "6-5 Milestone - Dylan Kimball", NULL, NULL);
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

	// Create shader (program object)
	shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	// Create shader (light shader)
	lightShaderProgram = createShaderProgram(lightVertexShaderSource, lightFragmentShaderSource);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

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

		glEnable(GL_DEPTH_TEST); // Z-buffer operation
		glDepthFunc(GL_LEQUAL); // Used with Depth test
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe (Comment out for normal rendering)

		// Draw primitives
		// Plane - START
		glBindTexture(GL_TEXTURE_2D, planeTex); // Activate texture 1 to apply to objects
		glBindVertexArray(planeVAO); // Activate VAO to draw the plane(now references VBO and VAO association)
		drawPlane(); // Draws base for all other objects to sit on
		// Plane - END

		glBindVertexArray(0); //Incase different VAO wii be used
		glBindTexture(GL_TEXTURE_2D, 0); // Incase different texture will be used after

		glBindVertexArray(boxVAO); // Activate VAO to draw Monitors (now references VBO and VAO association)
		// Monitor 1 - START
		drawRectangle(1, 6, screen1Positions, monitor1RotationsAllX, monitor1RotationsAllY, monitor1RotationsAllZ, 2.5f, 1.5f, 1.0f, 0.1f, 2.5f, 1.0f, 0.1f, 1.5f, 1.0f, 0.0, 0.0, -0.18); // Draw screen1, passes plane positions, rotations, and scales
		drawRectangle(0, 6, stand1Positions, monitor1RotationsAllX, monitor1RotationsAllY, monitor1RotationsAllZ, 0.3f, 1.1f, 1.0f, 0.2f, 0.3f, 1.0f, 0.2f, 1.1f, 0.3f, 0.0, -0.4, -0.5); // Draw stand1, passes plane positions, rotations, and scales (No bottom face)
		drawRectangle(0, 4, connector1Positions, monitor1Rotations4FacesX, monitor1Rotations4FacesY, monitor1Rotations4FacesZ, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.28f, 0.1f, 1.0f, 0.0, 0.0, -0.35); // Draw connector1, passes plane positions, rotations, and scales (No front or back faces)
		drawRectangle(0, 4, stand1BasePositions1, base1Rotations1AllX, base1Rotations1AllY, base1Rotations1AllZ, 1.3f, 0.1f, 1.0f, 0.1f, 1.0f, 1.0f, 1.2f, 0.1f, 1.0f, -0.7, -1.11, -0.5); // Draw stand standleg1, passes plane positions, rotations, and scales (No front or back faces)
		drawRectangle(0, 4, stand1BasePositions1, base1Rotations2AllX, base1Rotations2AllY, base1Rotations2AllZ, 1.3f, 0.1f, 1.0f, 0.1f, 1.0f, 1.0f, 1.2f, 0.1f, 1.0f, 0.7, -1.11, -0.5); // Draw stand standleg2, passes plane positions, rotations, and scales (No front or back faces)
		drawRectangle(0, 4, stand1BasePositions2, base1Rotations3AllX, base1Rotations3AllY, base1Rotations3AllZ, 0.65f, 0.1f, 1.0f, 0.1f, 1.0f, 1.0f, 0.65f, 0.1f, 1.0f, 0.0, -1.11, -0.76); // Draw stand standleg3, passes plane positions, rotations, and scales (No front or back faces)
		// Monitor 1 - END

		// Monitor 2 (Vertical) - START
		drawRectangle(1, 6, screen2Positions, monitor2RotationsAllX, monitor2RotationsAllY, monitor2RotationsAllZ, 1.0f, 2.0f, 1.0f, 0.1f, 1.0f, 1.0f, 0.1f, 2.0f, 1.0f, -1.72, 0.0, 0.0); // Draw screen2, passes plane positions, rotations, and scales
		drawRectangle(0, 5, stand2Positions, monitor2RotationsAllX, monitor2RotationsAllY, monitor2RotationsAllZ, 0.3f, 1.35f, 1.0f, 0.1f, 0.3f, 1.0f, 0.1f, 1.35f, 0.3f, -1.72, 0.0, 0.0);// Draw stand2, passes plane positions, rotations, and scales (No bottom face)
		drawRectangle(0, 5, stand2BasePositions, monitor2RotationsAllX, monitor2RotationsAllY, monitor2RotationsAllZ, 0.7f, 0.05f, 1.0f, 0.7f, 0.7f, 1.0f, 0.7f, 0.05f, 1.0f, -1.72, 0.0, 0.0); // Draw standbase2, passes plane positions, rotations, and scales (No bottom face)
		drawRectangle(0, 4, connector2Positions, monitor2Rotations4FacesX, monitor2Rotations4FacesY, monitor2Rotations4FacesZ, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, 0.28f, 0.1f, 1.0f, -1.72, 0.0, 0.0); // Draw stand connector2, passes plane positions, rotations, and scales (No front or back faces)
		// Monitor 2 (Vertical) - END

		glBindVertexArray(0); //Incase different VAO wii be used
		glBindTexture(GL_TEXTURE_2D, 0); // Incase different texture will be used after

		glBindVertexArray(cylinderVAO); // Activate VAO to draw water bottle (now references VBO and VAO association)

		// Water bottle - START
		drawCylinderSphere(waterBottleTex1, 24, 12, 15, 0, 0, 1.0 ,1.0 ,1.0, 0.8, -0.87, 0.1); // Draws main water bottle body, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(waterBottleTex2, 24, 12, 15, 0, 0, 1.1, 0.1, 1.1, 0.8, -0.5, 0.1); // Draws upper metal piece of the water bottle, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		// Water bottle - END

		// tarantula legs - START
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 40, 0.4, 0.7, 0.4, 0.18, -1.18, 0.2); // Draws left leg 1, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 70, 0.4, 0.7, 0.4, 0.28, -1.18, 0.4); // Draws left leg 2, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 110, 0.4, 0.7, 0.4, 0.28, -1.18, 0.6); // Draws left leg 3, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 140, 0.4, 0.7, 0.4, 0.18, -1.18, 0.75); // Draws left leg 4, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 350, 0.3, 0.4, 0.3, 0.0, -1.18, 0.85); // Draws left leg 5, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z

		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 320, 0.4, 0.7, 0.4, -0.38, -1.18, 0.2); // Draws right leg 1, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 290, 0.4, 0.7, 0.4, -0.48, -1.18, 0.4); // Draws right leg 2, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 250, 0.4, 0.7, 0.4, -0.48, -1.18, 0.6); // Draws right leg 3, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 220, 0.4, 0.7, 0.4, -0.38, -1.18, 0.75); // Draws right leg 4, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex2, 24, 12, 15, 90, 10, 0.3, 0.4, 0.3, -0.2, -1.18, 0.85); // Draws right leg 5, passes texture, number of faces(rotations), indice #, rotation angle, scale on X Y Z, and position on X Y Z
		// tarantula legs - END

		glBindVertexArray(0); //Incase different VAO wii be used
		glBindTexture(GL_TEXTURE_2D, 0); // Incase different texture will be used after

		glBindVertexArray(sphereVAO); // Activate VAO to draw water bottle (now references VBO and VAO association)

		// Baseball - START
		drawCylinderSphere(waterBottleTex1, 24, 48, 15, 0, 0, 0.18, 0.18, 0.18, -1.0, -1.08, 0.4); // Draws baseball, passes texture, number of faces(rotations), indice #, rotation angle on X Y Z, scale on X Y Z, and position on X Y Z
		// Baseball - END

		// Tarantula Head & Body - START
		drawCylinderSphere(tarantualTex3, 24, 48, 15, 90, 0, 0.26, 0.3, 0.26, -0.1, -1.1, 0.2); // Draws body of tarantula, passes texture, number of faces(rotations), indice #,rotation angle on X Y Z, scale on X Y Z, and position on X Y Z
		drawCylinderSphere(tarantualTex1, 24, 48, 15, 90, 0, 0.18, 0.18, 0.18, -0.1, -1.1, 0.6); // Draws head of tarantula, passes texture, number of faces(rotations), indice #, rotation angle on X Y Z, scale on X Y Z, and position on X Y Z
		// Tarantula Head & Body - END

		glBindVertexArray(0); //Incase different VAO wii be used
		glUseProgram(0); // Incase different shader will be used
		glBindTexture(GL_TEXTURE_2D, 0); // Incase different texture will be used after


		glUseProgram(lightShaderProgram); // Call light shader program
		// Select uniform location and set matrix
		light1ModelLoc = glGetUniformLocation(lightShaderProgram, "model");
		light1ViewLoc = glGetUniformLocation(lightShaderProgram, "view");
		light1ProjLoc = glGetUniformLocation(lightShaderProgram, "projection");

		glUniformMatrix4fv(light1ViewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(light1ProjLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glBindVertexArray(lightVAO); // Activate VAO to draw verticle Monitor (now references VBO and VAO association)

		drawLight(rectanglePositions, lightRotationsX, lightRotationsY, lightRotationsZ, lightPosition1); // draws light cube 1
		drawLight(rectanglePositions, lightRotationsX, lightRotationsY, lightRotationsZ, lightPosition2); // draws light cube 2

		glBindVertexArray(0); //Incase different VAO wii be used
		glUseProgram(0); // Incase different shader will be used

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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // Exit program
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { // Move camera forward
		cameraPos += camSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { // Move camera backwards
		cameraPos -= camSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { // Move camera Left
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // Move camera right
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Move camera down
		cameraPos -= camSpeed * cameraUp;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Move camera up
		cameraPos += camSpeed * cameraUp;
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { // Change between orthographic and perspective view (P Key)

		// Checks to see if last time P was pressed is greater than 0.01 second (Makes it so the view doesn't rapidly switch with one key press)
		if (glfwGetTime() - previousTime > 0.01) {
			if (orthoView == false) { // Sets to true to switch to orthographic view
				orthoView = true;
			}
			else if (orthoView == true) { // Sets to false to switch to perspective view
				orthoView = false;
			}

		}
		previousTime = glfwGetTime(); // Sets previous time to last time P was pressed
	}
}


// Checks for mouse input
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	// Checks to see if this is the first time mouse input is received
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false; // sets to false since this is only needed for the first time
	}
	// Reset camera position, rotation, and speed to initial settings
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	// Scrolling foward increased camera speed, scrolling backwards decreases camera speed 
	camSpeed += (float)yoffset / 100; // Dividing by 100 makes it so it either increases or decreased by 0.01 instead of just 1.0

	// Constrains the camera speed so it can be to low or to high
	if (camSpeed < 0.01f)
		camSpeed = 0.01f;
	if (camSpeed > 0.5f)
		camSpeed = 0.5f;
}