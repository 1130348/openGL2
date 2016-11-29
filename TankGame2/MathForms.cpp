#include "MathForms.h"
#include "Globals.h"


void makeRectangularPrism(float x1, float y1, float z1, float x2, float y2, float z2){
	float width = (x2-x1)/2;
	if (width < 1){
		width = -width;
	}
	float height = (y2-y1)/2;
	if (height < 1){
		height = -height;
	} 
	float depth = (z2-z1)/2;
	if (depth < 1){
		depth = -depth;
	}
	
	glBegin(GL_QUADS);
	//Front
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-width, -height, depth);
	glVertex3f(width, -height, depth);
	glVertex3f(width, height, depth);
	glVertex3f(-width, height, depth);
	
	//Right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(width, -height, -depth);
	glVertex3f(width, height, -depth);
	glVertex3f(width, height, depth);
	glVertex3f(width, -height, depth);
	
	//Back
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-width, -height, -depth);
	glVertex3f(-width, height, -depth);
	glVertex3f(width, height, -depth);
	glVertex3f(width, -height, -depth);
	
	//Left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-width, -height, -depth);
	glVertex3f(-width, -height, depth);
	glVertex3f(-width, height, depth);
	glVertex3f(-width, height, -depth);
	
	//Top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-width, height, -depth);
	glVertex3f(-width, height, depth);
	glVertex3f(width, height, depth);
	glVertex3f(width, height, -depth);
	
	//Bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-width, -height, -depth);
	glVertex3f(-width, -height, depth);
	glVertex3f(width, -height, depth);
	glVertex3f(width, -height, -depth);
	
	glEnd();
}

void normalize(float v[3])
{    
	GLfloat d = sqrt(v[1]*v[1]+v[2]*v[2]+v[3]*v[3]); 
	if (d == 0.0) {
		std::cout << "Error: Zero vector passed to function \"void normalize(float v[3])\"";
		return;
	}
	v[1] /= d;
	v[2] /= d;
	v[3] /= d; 
}

void normalizedCrossProduct(float v1[3], float v2[3], float out[3]) 
{ 	
	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];
	normalize(out); 
}


void makeGrid(float size){
	glColor4f(0.0f, 0.0f, 2.0f, 1.0f -radarVisionMagnitude);
	glBegin(GL_LINES);
	for(int i=-size;i<=size;++i) {
		glVertex3f(i,0,-size);
		glVertex3f(i,0,size);
		
		glVertex3f(size,0,i);
		glVertex3f(-size,0,i);
	}
	glEnd();
}

void desenhaChao(float size, GLuint textID){

	glBindTexture(GL_TEXTURE_2D, textID);


	GLfloat cores[][3] = { { 0.0,1.0,1.0 },
	{ 1.0,0.0,0.0 },
	{ 1.0,1.0,0.0 },
	{ 0.0,1.0,0.0 },
	{ 1.0,0.0,1.0 },
	{ 0.0,0.0,1.0 },
	{ 1.0,1.0,1.0 } };

	GLfloat vertices[][3] = { { -size,0,size },
	{ -size,0,-size },
	{ size,0,-size },
	{ size,0,size }};

	glBegin(GL_QUADS);
	
	glTexCoord2f(1, 1);
	glVertex3fv(vertices[0]);
	glTexCoord2f(0, 1);
	glVertex3fv(vertices[1]);
	glTexCoord2f(0, 0);
	glVertex3fv(vertices[2]);
	glTexCoord2f(1, 0);
	glVertex3fv(vertices[3]);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, NULL);
}

float distanceBetween(float x1, float z1, float x2, float z2){
	return sqrt((x2-x1)*(x2-x1) + (z2-z1)*(z2-z1));
}
