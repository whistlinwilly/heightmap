/**
 * @file project.cpp
 * @brief OpenGL project
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "opengl/project.hpp"

// use this header to include the OpenGL headers
// DO NOT include gl.h or glu.h directly; it will not compile correctly.
#include "application/opengl.hpp"
#include <stdio.h>
#include <string.h>

// A namespace declaration. All proejct files use this namespace.
// Add this declration (and its closing) to all source/headers you create.
// Note that all #includes should be BEFORE the namespace declaration.
namespace _462 {

// definitions of functions for the OpenglProject class

// constructor, invoked when object is created
OpenglProject::OpenglProject()
{
    // TODO any basic construction or initialization of members
    // Warning: Although members' constructors are automatically called,
    // ints, floats, pointers, and classes with empty contructors all
    // will have uninitialized data!	
	

}

// destructor, invoked when object is destroyed
OpenglProject::~OpenglProject()
{
    // TODO any final cleanup of members
    // Warning: Do not throw exceptions or call virtual functions from deconstructors!
    // They will cause undefined behavior (probably a crash, but perhaps worse).
	
}

/**
 * Initialize the project, doing any necessary opengl initialization.
 * @param camera An already-initialized camera.
 * @param scene The scene to render.
 * @return true on success, false on error.
 */
bool OpenglProject::initialize( Camera* camera, Scene* scene )
{

    // copy scene
	this->scene = *scene;
	
	int numVertex = this->scene.mesh.num_vertices;
	normalList = (Vector3*) malloc(sizeof(Vector3) * numVertex);
	memset(normalList, 0, sizeof(Vector3) * numVertex);

GLfloat mat_specular[] = { 0.5, 0.5, 1.0, 0.1 };
   GLfloat mat_shininess[] = { 20.0 };
   GLfloat light_position[] = { 10.0, 7.0, 15.0, 0.0 };
   GLfloat redDiffuseMaterial[] = {0.5, 0.0, 0.0}; 
   GLfloat greenEmissiveMaterial[] = {0.0, 0.1, 0.0};
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   
   GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 0.0 };
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  // glMaterialfv(GL_FRONT, GL_DIFFUSE, redDiffuseMaterial);
  // glMaterialfv(GL_FRONT, GL_EMISSION, greenEmissiveMaterial);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);


	Vector3 camPos = (*camera).get_position();
	Vector3 camDir = (*camera).get_direction();
	Vector3 camUp = (*camera).get_up();


	Vector3 newPos, temp;
	newPos.x = 3;
	newPos.y = 0;
	newPos.z = 0;

	int indA, indB, indC;
	Vector3 vertA, vertB, vertC, vertD, vectorU, vectorV, normal;
	int i;

	
//for each triangle get surface normal
//index into list by vertex and reaverage normal
numTri = this->scene.mesh.num_triangles;

meshVertices = (float*) malloc(sizeof(float) * numTri * 9);
meshNormals = (float*) malloc(sizeof(float) * numTri * 9);
meshIndices = (int*) malloc(sizeof(int) * numTri * 3);

	for(i=0;i<numTri;i++){
		//get vertex index
		indA = this->scene.mesh.triangles[i].vertices[0]; 
		indB = this->scene.mesh.triangles[i].vertices[1]; 
		indC = this->scene.mesh.triangles[i].vertices[2];

		//set index in array
		meshIndices[3*i] = indA;
		meshIndices[3*i + 1] = indB;
		meshIndices[3*i + 2] = indC;

		//get vertex
		vertA = this->scene.mesh.vertices[indA];
		vertB = this->scene.mesh.vertices[indB];
		vertC = this->scene.mesh.vertices[indC];

		meshVertices[9*indA] = vertA.x;
		meshVertices[9*indA + 1] = vertA.y;
		meshVertices[9*indA + 2] = vertA.z;

		meshVertices[9*indB] = vertB.x;
		meshVertices[9*indB + 1] = vertB.y;
		meshVertices[9*indB + 2] = vertB.z;

		meshVertices[9*indC] = vertC.x;
		meshVertices[9*indC + 1] = vertC.y;
		meshVertices[9*indC + 2] = vertC.z;

	//compute vectors
	vectorU = vertB - vertA;
	vectorV = vertC - vertA;

	//cross product
	normal.x = vectorU.y*vectorV.z - vectorU.z*vectorV.y;
	normal.y = vectorU.z*vectorV.x - vectorU.x*vectorV.z;
	normal.z = vectorU.x*vectorV.y - vectorU.y*vectorV.x;

	//set vertex a
	temp.x = meshNormals[3*indA];
	temp.y = meshNormals[3*indA + 1];
	temp.z = meshNormals[3*indA + 2];
	temp += normalize(normal);
	temp = normalize(temp);
	meshNormals[3*indA] = temp.x;
	meshNormals[3*indA + 1] = temp.y;
	meshNormals[3*indA + 2] = temp.z;

	//set vertex b
	temp.x = meshNormals[3*indB];
	temp.y = meshNormals[3*indB + 1];
	temp.z = meshNormals[3*indB + 2];
	temp += normalize(normal);
	temp = normalize(temp);
	meshNormals[3*indB] = temp.x;
	meshNormals[3*indB + 1] = temp.y;
	meshNormals[3*indB + 2] = temp.z;

	//set vertex c
	temp.x = meshNormals[3*indC];
	temp.y = meshNormals[3*indC + 1];
	temp.z = meshNormals[3*indC + 2];
	temp += normalize(normal);
	temp = normalize(temp);
	meshNormals[3*indC] = temp.x;
	meshNormals[3*indC + 1] = temp.y;
	meshNormals[3*indC + 2] = temp.z;
}

//	zWater = (float*) malloc (sizeof(float) * WATER_ARRAY_SIZE_X * WATER_ARRAY_SIZE_Y);
	waterNormals = (float*) malloc (sizeof(float) * WATER_ARRAY_SIZE_X * WATER_ARRAY_SIZE_Y * 3);
	waterVertices = (float*) malloc (sizeof(float) * WATER_ARRAY_SIZE_X * WATER_ARRAY_SIZE_Y * 3);

	numIndices = (WATER_ARRAY_SIZE_X - 1) * (WATER_ARRAY_SIZE_Y - 1) * 6;
	indexArray = (int*) malloc (sizeof(int) * numIndices);

	int n = 0;

	for(int y = 0; y < WATER_ARRAY_SIZE_Y - 1; y++)
		for(int x = 0; x < WATER_ARRAY_SIZE_X-1; x++){
			int index = y*WATER_ARRAY_SIZE_X + x;
			indexArray[n] = index;
			indexArray[n+1] = index + WATER_ARRAY_SIZE_X;
			indexArray[n+2] = index + 1;
			indexArray[n+3] = index + 1;
			indexArray[n+4] = index + WATER_ARRAY_SIZE_X;
			indexArray[n+5] = index + WATER_ARRAY_SIZE_X + 1;
			n+=6;
		}

	n = 0;
	for(int z = 0; z < WATER_ARRAY_SIZE_Y; z++)
		for(int x = 0; x < WATER_ARRAY_SIZE_X; x++){
			float verX = -1.0 + (2.0/WATER_ARRAY_SIZE_X)*x;
			float verZ = -1.0 + (2.0/WATER_ARRAY_SIZE_Y)*z;
			float verY = this->scene.heightmap->compute_height(Vector2(verX,verZ));
			waterVertices[n] = verX;
			waterVertices[n+1] = verY;
			waterVertices[n+2] = verZ;
			n+=3;
		}


	//loop through vertices by triangle
	for(int m=0; m < numIndices; m+=3){

		//get vertex indices
		int aVertIndex = indexArray[m];
		int bVertIndex = indexArray[m+1];
		int cVertIndex = indexArray[m+2];

		//get vertices
		vertA.x = waterVertices[3*aVertIndex];
		vertA.y = waterVertices[3*aVertIndex + 1];
		vertA.z = waterVertices[3*aVertIndex + 2];

		vertB.x = waterVertices[3*bVertIndex];
		vertB.y = waterVertices[3*bVertIndex + 1];
		vertB.z = waterVertices[3*bVertIndex + 2];

		vertC.x = waterVertices[3*cVertIndex];
		vertC.y = waterVertices[3*cVertIndex + 1];
		vertC.z = waterVertices[3*cVertIndex + 2];

		//compute vectors for cross product
		vectorU = vertB - vertA;
		vectorV = vertC - vertA;

		//compute cross product
		normal.x = vectorU.y*vectorV.z - vectorU.z*vectorV.y;
		normal.y = vectorU.z*vectorV.x - vectorU.x*vectorV.z;
		normal.z = vectorU.x*vectorV.y - vectorU.y*vectorV.x;

		//set vertex a
		temp.x = waterNormals[3*aVertIndex];
		temp.y = waterNormals[3*aVertIndex + 1];
		temp.z = waterNormals[3*aVertIndex + 2];
		temp += normalize(normal);
		temp = normalize(temp);
		waterNormals[3*aVertIndex] = temp.x;
		waterNormals[3*aVertIndex + 1] = temp.y;
		waterNormals[3*aVertIndex + 2] = temp.z;

		//set vertex b
		temp.x = waterNormals[3*bVertIndex];
		temp.y = waterNormals[3*bVertIndex + 1];
		temp.z = waterNormals[3*bVertIndex + 2];
		temp += normalize(normal);
		temp = normalize(temp);
		waterNormals[3*bVertIndex] = temp.x;
		waterNormals[3*bVertIndex + 1] = temp.y;
		waterNormals[3*bVertIndex + 2] = temp.z;

		//set vertex c
		temp.x = waterNormals[3*cVertIndex];
		temp.y = waterNormals[3*cVertIndex + 1];
		temp.z = waterNormals[3*cVertIndex + 2];
		temp += normalize(normal);
		temp = normalize(temp);
		waterNormals[3*cVertIndex] = temp.x;
		waterNormals[3*cVertIndex + 1] = temp.y;
		waterNormals[3*cVertIndex + 2] = temp.z;
	}
	
	return true;
}

/**
 * Clean up the project. Free any memory, etc.
 */
void OpenglProject::destroy()
{
    // TODO any cleanup code, e.g., freeing memory
//	free(this->nList);
}

/**
 * Perform an update step. This happens on a regular interval.
 * @param dt The time difference from the previous frame to the current.
 */
void OpenglProject::update( real_t dt )
{
	
	int n = 0;
	Vector3 vertA, vertB, vertC, vertD, normal, temp, vectorU, vectorV;

    // update our heightmap
    scene.heightmap->update( dt );

	for(int z = 0; z < WATER_ARRAY_SIZE_Y; z++)
		for(int x = 0; x < WATER_ARRAY_SIZE_X; x++){
			float verX = -1.0 + (2.0/WATER_ARRAY_SIZE_X)*x;
			float verZ = -1.0 + (2.0/WATER_ARRAY_SIZE_Y)*z;
			float verY = this->scene.heightmap->compute_height(Vector2(verX,verZ));
			waterVertices[n+1] = verY;
			n+=3;
		}

	//loop through vertices by triangle
	for(int m=0; m < numIndices; m+=3){

		//get vertex indices
		int aVertIndex = indexArray[m];
		int bVertIndex = indexArray[m+1];
		int cVertIndex = indexArray[m+2];

		//get vertices
		vertA.x = waterVertices[3*aVertIndex];
		vertA.y = waterVertices[3*aVertIndex + 1];
		vertA.z = waterVertices[3*aVertIndex + 2];

		vertB.x = waterVertices[3*bVertIndex];
		vertB.y = waterVertices[3*bVertIndex + 1];
		vertB.z = waterVertices[3*bVertIndex + 2];

		vertC.x = waterVertices[3*cVertIndex];
		vertC.y = waterVertices[3*cVertIndex + 1];
		vertC.z = waterVertices[3*cVertIndex + 2];

		//compute vectors for cross product
		vectorU = vertB - vertA;
		vectorV = vertC - vertA;

		//compute cross product
		normal.x = vectorU.y*vectorV.z - vectorU.z*vectorV.y;
		normal.y = vectorU.z*vectorV.x - vectorU.x*vectorV.z;
		normal.z = vectorU.x*vectorV.y - vectorU.y*vectorV.x;

		//set vertex a
		temp.x = waterNormals[3*aVertIndex];
		temp.y = waterNormals[3*aVertIndex + 1];
		temp.z = waterNormals[3*aVertIndex + 2];
		temp += normalize(normal);
		temp = normalize(temp);
		waterNormals[3*aVertIndex] = temp.x;
		waterNormals[3*aVertIndex + 1] = temp.y;
		waterNormals[3*aVertIndex + 2] = temp.z;

		//set vertex b
		temp.x = waterNormals[3*bVertIndex];
		temp.y = waterNormals[3*bVertIndex + 1];
		temp.z = waterNormals[3*bVertIndex + 2];
		temp += normalize(normal);
		temp = normalize(temp);
		waterNormals[3*bVertIndex] = temp.x;
		waterNormals[3*bVertIndex + 1] = temp.y;
		waterNormals[3*bVertIndex + 2] = temp.z;

		//set vertex c
		temp.x = waterNormals[3*cVertIndex];
		temp.y = waterNormals[3*cVertIndex + 1];
		temp.z = waterNormals[3*cVertIndex + 2];
		temp += normalize(normal);
		temp = normalize(temp);
		waterNormals[3*cVertIndex] = temp.x;
		waterNormals[3*cVertIndex + 1] = temp.y;
		waterNormals[3*cVertIndex + 2] = temp.z;
	}


}

void OpenglProject::myWaterDraw(){

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

glVertexPointer(3, GL_FLOAT, 0, waterVertices);
glNormalPointer(GL_FLOAT, 0, waterNormals);

glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indexArray);

// deactivate vertex arrays after drawing
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_NORMAL_ARRAY);

}

void OpenglProject::myDraw(){
/*
	int cinCrap;
	Vector3 normal, vertA, vertB, vertC;
	int i, hash, indA, indB, indC;
	NormalList temp;

	glBegin( GL_TRIANGLES); 
glColor3f( 0.0f, 0.0f, 1.0f ); // sets color to blue 
//memset( &nList, 0, sizeof( nList ) );
int numTri = this->scene.mesh.num_triangles;

	

	
glEnd(); 
glFlush();
*/

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_NORMAL_ARRAY);

glVertexPointer(3, GL_FLOAT, 0, meshVertices);
glNormalPointer(GL_FLOAT, 0, meshNormals);

glDrawElements(GL_TRIANGLES, numTri*3, GL_UNSIGNED_INT, meshIndices);

// deactivate vertex arrays after drawing
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_NORMAL_ARRAY);


}

/**
 * Clear the screen, then render the mesh using the given camera.
 * @param camera The logical camera to use.
 * @see math/camera.hpp
 */
void OpenglProject::render( const Camera* camera )
{
	Vector3 vPos, vAt, vCen, vUp;
	
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_PROJECTION ); // set current matrix 
	glLoadIdentity(); // Clears the matrix 
	gluPerspective(90,1,2,20);
		vPos = (*camera).get_position();
	vAt = (*camera).get_direction();
	vCen = vPos + vAt;
	vUp = (*camera).get_up();
	gluLookAt(vPos.x, vPos.y, vPos.z, vCen.x, vCen.y, vCen.z, vUp.x, vUp.y,  vUp.z);


    glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity();
	

   glEnable(GL_NORMALIZE);

	glPushMatrix();
	Vector3 meshTrans = this->scene.mesh_position.position;
	glTranslatef(meshTrans.x, meshTrans.y, meshTrans.z);
	Quaternion meshRot = this->scene.mesh_position.orientation;
	glRotatef(meshRot.w / (2 * PI) * 360.0, meshRot.x, meshRot.y, meshRot.z);
	Vector3 meshSc = this->scene.mesh_position.scale;
    	glScalef(meshSc.x, meshSc.y, meshSc.z);
	//myDraw();
	glPopMatrix();

	glPushMatrix();	
	Vector3 heightTrans = this->scene.heightmap_position.position;
	glTranslatef(heightTrans.x, heightTrans.y, heightTrans.z);
	Quaternion heightRot = this->scene.heightmap_position.orientation;
	glRotatef(heightRot.w / (2 * PI) * 360.0, heightRot.x, heightRot.y, heightRot.z);
	Vector3 heightSc = this->scene.heightmap_position.scale;
	glScalef(heightSc.x, heightSc.y, heightSc.z);
	myWaterDraw();
	glPopMatrix();

	glDisable(GL_NORMALIZE);

	

    // TODO render code
	




}



} /* _462 */

