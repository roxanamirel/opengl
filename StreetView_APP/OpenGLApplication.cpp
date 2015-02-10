// OpenGLApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "glut.h"
#include <gl/gl.h>
#include "glm.h"
#include "tga.h"
#include "glui.h"
int screen_width=940;
int screen_height=780;
int main_window;
GLfloat deplasare = -3;
GLfloat directionAngle = 0;
GLfloat fTreeSize;
GLfloat fSkyboxSizeX, fSkyboxSizeY, fSkyboxSizeZ; //skybox size on X, Y and Z axes

GLMmodel *car,*street,*tree,*bench,*mail;
GLMmodel *plan,*artar,*lamp,*house,*house2;

GLuint roadTexture,lampTexture,mailTexture,houseTexture,house2Texture,sunTexture;
GLuint carTexture,car2Texture;
GLuint treeTexture,benchTexture,artariTexture,tree2Texture;
GLuint skyboxTexture[6]; //variable used for the IDs of the six textures that form the skybox

GLfloat fGlobalAngleX, fGlobalAngleY, fGlobalAngleZ; //global rotation angles


GLUI_RadioGroup *radio;
int   obj_type = 1;
int   wireframe = 1;
int   segments = 8;
//Define a constant that identify the radio buttons group
#define RADIO_BUTTONS_GROUP_ID 200

float myarray[16] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0 , 0.0, 0.0, 0.0, 1.0 };



//16.01
static GLint fogMode;
GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};

GLfloat light_ambient[]   = { 2.0, 2.0, 2.0, 1.0 };
GLfloat light_specular[]  = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_diffuse[]   = { 5.0, 5.0, 5.0, 1.0 };
GLfloat light_position[]  = { -2.0, 0.0, 0.0, 1.0 };

GLfloat light_ambient1[]  = { 3.0, 3.0, 3.0, 1.0 };
GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_diffuse1[]  = { 3.0, 3.0, 3.0, 1.0 };
GLfloat light_position1[] = { 0.0, 2.55, 0.0, 1.0 };
GLfloat light_emission[] = {1.0, 1.0, 0.0, 1.0};

GLfloat light_ambient2[]  = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_diffuse2[]  = { 9.0, 9.0, 9.0, 1.0 };
GLfloat light_position2[] = { -3.5,-2 ,-2, 1.0 };

GLfloat material_3_ambient[] = {0.0, 0.0, 0.0, 1.0};
GLfloat material_3_diffuse[] = {0.0, 0.0, 0.0, 1.0};
GLfloat material_3_specular[] = {0.0, 0.0, 0.0, 1.0};
GLfloat material_3_shininess[] = {128.0};
GLfloat material_3_emission[] = {1.0, 1.0, 0.0, 1.0};
GLfloat material_no_emission[] = {0.0, 0.0, 0.0, 1.0};


//Projection shadow 
enum {
	X, Y, Z, W
};
int shadow1 = 0;
enum {
	A, B, C, D
};
GLfloat floorshadow[4][4];
GLfloat v0[3], v1[3], v2[3];
GLfloat plane[4];

int fogen   = 0,enlight = 1, objtype=1,objlook,
	light1  = 0, 
	light2  = 0, 
	light3  = 0;

//Shadow volume
#define SHADOWVOL 1
typedef struct 
{
	GLfloat *vertices;
	GLfloat *normal;
	int n;                /* number of vertices */
} ShadObj;

GLfloat shadVerts[] =  
{
	-1.082f, -0.177f, -1.06f,
	-1.082f, -0.83f, -1.06f,
	-1.315f, -0.83f, -0.94f,
	-1.315f, -0.177f, -0.94f
};
GLfloat shadNormal[] = {1.f, 1.f, 1.f};
ShadObj shadower;
enum { J, K, L };


void ExtendVertex(GLfloat extended[3], GLfloat light[3], GLfloat vertex[3], GLfloat magnitude)
{
	GLfloat delta[3];

	delta[J] = vertex[J] - light[J];
	delta[K] = vertex[K] - light[K];
	delta[L] = vertex[L] - light[L];

	extended[J] = light[J] + delta[J] * magnitude;
	extended[K] = light[K] + delta[K] * magnitude;
	extended[L] = light[L] + delta[L] * magnitude;
}

void render(ShadObj * obj)
{
	GLfloat *v;           
	int i;

	// plane - textured
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, roadTexture);
	glPushMatrix();
		glRotatef(180,1,0,0);
		glBegin(GL_QUADS);
			glNormal3f(0.f, 1.f, 0.f);
			glTexCoord2i(0, 1);
			glVertex3f(-1.82f, 0.823f, -1.78f);
			glTexCoord2i(0, 0);
			glVertex3f(0.82f, 0.823f, -1.78f);
			glTexCoord2i(1, 0);
			glVertex3f(0.82f, 0.823f, 1.78f);
			glTexCoord2i(1, 1);
			glVertex3f(-1.82f, 0.823f, 1.78f);
		glEnd();
	glPopMatrix();
	
	//vaza
	glPushMatrix();
		glTranslatef(-0.5,-0.33,0);
		glScalef(0.5,0.5,0.5);
		glRotatef(-90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D,sunTexture);
		glmDraw(lamp, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	
	glBegin(GL_POLYGON);
		glNormal3fv(obj->normal);
		for (v = obj->vertices, i = 0; i < obj->n; i++) 
		{
			glVertex3fv(v);
			v += 3;
		}
	glEnd();
}



void PlaneEq(GLfloat plane[4], GLfloat p0[3], GLfloat p1[3], GLfloat p2[3])
{
	GLfloat vec0[3], vec1[3];

	/* need 2 vectors to find cross product */
	vec0[X] = p1[X] - p0[X];
	vec0[Y] = p1[Y] - p0[Y];
	vec0[Z] = p1[Z] - p0[Z];

	vec1[X] = p2[X] - p0[X];
	vec1[Y] = p2[Y] - p0[Y];
	vec1[Z] = p2[Z] - p0[Z];

	/* find cross product to get A, B, and C of plane equation */
	plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
	plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

	plane[D] = -(plane[A] * p0[X] + plane[B] * p0[Y] + plane[C] * p0[Z]);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//																									//
//	Name: void ComputeShadowMatrix(GLfloat shadowMat[16], GLfloat plane[4], GLfloat lightPos[4])	//
//																									//
//	Description: Computes the matrix used to project the shadow on a plane							//
//																									//
//	Input: GLfloat plane[4] -> array containing the A, B, C and D coefficients for the equation		//
//							   of the plane on which the shadow will be cast						//
//		   GLfloat lightPos[4] -> array containing the X, Y, Z and W coordinates of the light		//
//								  source															//
//																									//
//	Outputs: Glfloat shadowMat[16] -> array containing the column-major matrix used to project		//
//									  the shadow on the given plane									//
//																									//
//////////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeShadowMatrix(GLfloat shadowMat[4][4],  GLfloat groundplane[4],  GLfloat lightpos[4])
{
	GLfloat dot;
	/* find dot product between light position vector and ground plane normal */
	dot = groundplane[X] * lightpos[X] + 
		  groundplane[Y] * lightpos[Y] + 
		  groundplane[Z] * lightpos[Z] + 
		  groundplane[W] * lightpos[W];

	shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
	shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
	shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
	shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

	shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
	shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
	shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
	shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

	shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
	shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
	shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
	shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

	shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
	shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
	shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
	shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}

void MakeShadowVolume(ShadObj * shadower, GLfloat light[3], GLfloat t, GLint dlist)
{
	int i;
	GLfloat newv[3];

	glNewList(1, GL_COMPILE);
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
			//set shadow colume color
			glColor3f(.2f, .8f, .4f);
			//traverse the list of edges
			for (i = 0; i < shadower->n; i++) 
			{
				glVertex3fv(&shadower->vertices[i * 3]);
				ExtendVertex(newv, light, &shadower->vertices[i * 3], t);
				glVertex3fv(newv);
				ExtendVertex(newv, light, &shadower->vertices[((i + 1) % shadower->n) * 3],  t);
				glVertex3fv(newv);
				glVertex3fv(&shadower->vertices[((i + 1) % shadower->n) * 3]);
			}
		glEnd();
		glEnable(GL_LIGHTING);
	glEndList();
}

void InitHouse2(char*filename)
{
    if(!house2)
{
    house2=glmReadOBJ(filename);   
        if(!house2)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(house2);
//Generate facet normal vectors for model 
glmFacetNormals(house2);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(house2,90.0);
    }
}
void InitHouse(char*filename)
{
    if(!house)
{
    house=glmReadOBJ(filename);   
        if(!house)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(house);
//Generate facet normal vectors for model 
glmFacetNormals(house);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(house,90.0);
    }
}
void InitStreet(char*filename)
{
    if(!street)
{
    street=glmReadOBJ(filename);   
        if(!street)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(street);
//Generate facet normal vectors for model 
glmFacetNormals(street);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(street,90.0);
    }
}
void InitBench(char*filename)
{
    if(!bench)
{
    bench=glmReadOBJ(filename);   
        if(!bench)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(bench);
//Generate facet normal vectors for model 
glmFacetNormals(bench);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(bench,90.0);
    }
}
void InitPlan(char*filename)
{
    if(!plan)
{
   plan=glmReadOBJ(filename);   
        if(!plan)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(plan);
//Generate facet normal vectors for model 
glmFacetNormals(plan);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(plan,90.0);
    }
}
void InitCar(char*filename)
{
    if(!car)
{
    car=glmReadOBJ(filename);   
        if(!car)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(car);
//Generate facet normal vectors for model 
glmFacetNormals(car);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(car,90.0);
    }
}
void InitTree(char*filename)
{
    if(!tree)
{
    tree=glmReadOBJ(filename);   
        if(!tree)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(tree);
//Generate facet normal vectors for model 
glmFacetNormals(tree);
//Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(tree,90.0);


    }
}
void InitArtari(char*filename)
{
    if(!artar)
{
    artar=glmReadOBJ(filename);   
        if(!artar)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(artar);
//Generate facet normal vectors for model 
glmFacetNormals(artar);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(artar,90.0);
    }
}
void InitLamp(char*filename)
{
    if(!lamp)
{
    lamp=glmReadOBJ(filename);   
        if(!lamp)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(lamp);
//Generate facet normal vectors for model 
glmFacetNormals(lamp);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(lamp,90.0);
    }
}
void InitMail(char*filename)
{
    if(!mail)
{
    mail=glmReadOBJ(filename);   
        if(!mail)
			 exit(0); 
        //Fit the original model inside a unit cube
glmUnitize(mail);
//Generate facet normal vectors for model 
glmFacetNormals(mail);
        //Generate vertex normal vectors (called only after generating facet normals) 
glmVertexNormals(mail,90.0);
    }
}

void renderSpacedBitmapString(float x, float y, int spacing, void *font, char *string)
{
char *c;
int x1=x;
for (c=string; *c != '\0'; c++) {
glRasterPos2f(x1,y);
glutBitmapCharacter(font, *c);
x1 = x1 + glutBitmapWidth(font,*c) + spacing;
}
}

void drawText()
{
glViewport (0, 0, screen_width, screen_height);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0, 640, 0, 480);
glScalef(1, -1, 1);
glTranslatef(0, -480, 0);
glMatrixMode(GL_MODELVIEW);

glColor3f(1.0f,0.0f,0.0f);
glLoadIdentity();

renderSpacedBitmapString(5, 15, 5, GLUT_BITMAP_HELVETICA_12, " ®Roxana Roman");

glColor3f(1.0f,1.0f,1.0f);
glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen_width/(GLfloat)screen_height, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}



void initOpenGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glViewport(3, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen_width/(GLfloat)screen_height, 1.0f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);

	//OBJECTS
    InitCar("Objects\\H2.obj");
	InitBench("Objects\\bench.obj");
	InitPlan("Objects\\plan4.obj");
	InitTree("Objects\\copacClau.obj");
	InitLamp("Objects\\mylamp.obj");
	InitMail("Objects\\mailbox.obj");
	InitHouse("Objects\\bloc.obj");
	InitHouse2("Objects\\myhouse.obj");
	//TEXTURES
    glGenTextures(1,&carTexture);
	loadTGA("Textures\\H2.tga",carTexture);
	glGenTextures(1,&car2Texture);
    loadTGA("Textures\\cube.tga",car2Texture);

	 glGenTextures(1,&roadTexture);
	 loadTGA("Textures\\plan.tga",roadTexture);
	 
	 
	glGenTextures(1,&benchTexture);
	loadTGA("Textures\\Old_lantern_UV_Diffuse.tga",benchTexture);

	glGenTextures(1,&tree2Texture);
	loadTGA("Textures\\copacClau.tga",tree2Texture);

	glGenTextures(1,&treeTexture); 
	loadTGA("Textures\\tree.tga",treeTexture);		
	
	glGenTextures(1,&lampTexture); 
	loadTGA("Textures\\bench.tga",lampTexture);	

	glGenTextures(1,&mailTexture); 
	loadTGA("Textures\\mail.tga",mailTexture);	
	
	glGenTextures(1,&houseTexture); 
	loadTGA("Textures\\bloc.tga",houseTexture);

	glGenTextures(1,&house2Texture); 
	loadTGA("Textures\\house.tga",house2Texture);
	
	glGenTextures(1,&sunTexture); 
	loadTGA("Textures\\fur.tga",sunTexture);

	glGenTextures(6,skyboxTexture);
	loadTGA("Textures\\greenhill_negative_x.tga",skyboxTexture[0]);	
	loadTGA("Textures\\greenhill_negative_y.tga",skyboxTexture[1]);	
	loadTGA("Textures\\greenhill_negative_z.tga",skyboxTexture[2]);	
	loadTGA("Textures\\greenhill_positive_x.tga",skyboxTexture[3]);	
	loadTGA("Textures\\greenhill_positive_y.tga",skyboxTexture[4]);	
	loadTGA("Textures\\greenhill_positive_z.tga",skyboxTexture[5]);	

	//set skybox size
	fSkyboxSizeX =20.0;
	fSkyboxSizeY =3.0;
	fSkyboxSizeZ =20.0;

	//set tree size
	fTreeSize = 1.0;


	
	//SMOKE
	{
	  glFogi (GL_FOG_MODE, GL_EXP);
      glFogfv (GL_FOG_COLOR, fogColor);
	  glFogf (GL_FOG_DENSITY, 0.25);
      glHint (GL_FOG_HINT, GL_DONT_CARE);
      glFogf (GL_FOG_START, 1.0);
      glFogf (GL_FOG_END, 2.0);
	}


}

void DrawSingleTreeTexture(GLfloat size)
{
	glEnable(GL_TEXTURE_2D); //enable 2D texturing		
	glBindTexture(GL_TEXTURE_2D, treeTexture); //select the current texture

	glEnable(GL_ALPHA_TEST); //enable alpha testing
	glAlphaFunc(GL_GREATER, 0.1f); //select the alpha testing function
									//GL_GREATER function passes if the incoming alpha value is greater than the reference value
		
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(size, size, 0.0); //assign each corner of the texture to a 3D vertex in the OpenGL scene
		glTexCoord2f(0, 1);glVertex3f(-size, size, 0.0); //(0,0) is the left lower corner, while (1,1) is the right upper corner of the texture
		glTexCoord2f(0, 0);glVertex3f(-size, -size, 0.0);
		glTexCoord2f(1, 0);glVertex3f(size, -size, 0.0);
	glEnd();

	glDisable(GL_ALPHA_TEST); //disable alpha testing
	glDisable(GL_TEXTURE_2D); //disable 2D texturing
}

void DrawSkybox (GLfloat sizeX, GLfloat sizeY, GLfloat sizeZ)
{
	glEnable(GL_TEXTURE_2D); //enable 2D texturing

	
	//////////////////////////////////////////////////////////////////
	// please consult the orientation convention for this skybox    //
	// ("orientation_convention.png" file in the "Textures" folder) //
	//////////////////////////////////////////////////////////////////


	//negative x plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]); //select the current texture
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(-sizeX, sizeY, -sizeZ); //assign each corner of the texture to a 3D vertex in the OpenGL scene
		glTexCoord2f(0, 1);glVertex3f(-sizeX, sizeY, sizeZ); //(0,0) is the left lower corner, while (1,1) is the right upper corner of the texture
		glTexCoord2f(0, 0);glVertex3f(-sizeX, -sizeY, sizeZ);
		glTexCoord2f(1, 0);glVertex3f(-sizeX, -sizeY, -sizeZ);
	glEnd();

	//negative y plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(sizeX, -sizeY, -sizeZ);
		glTexCoord2f(0, 1);glVertex3f(-sizeX, -sizeY, -sizeZ);
		glTexCoord2f(0, 0);glVertex3f(-sizeX, -sizeY, sizeZ);
		glTexCoord2f(1, 0);glVertex3f(sizeX, -sizeY, sizeZ);
	glEnd();
	
	//negative z plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[2]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(-sizeX, sizeY, sizeZ);
		glTexCoord2f(0, 1);glVertex3f(sizeX, sizeY, sizeZ);
		glTexCoord2f(0, 0);glVertex3f(sizeX, -sizeY, sizeZ);
		glTexCoord2f(1, 0);glVertex3f(-sizeX, -sizeY, sizeZ);
	glEnd();
	
	//positive x plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(sizeX, sizeY, sizeZ);
		glTexCoord2f(0, 1);glVertex3f(sizeX, sizeY, -sizeZ);
		glTexCoord2f(0, 0);glVertex3f(sizeX, -sizeY, -sizeZ);
		glTexCoord2f(1, 0);glVertex3f(sizeX, -sizeY, sizeZ);
	glEnd();
	
	//positive y plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[4]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(sizeX, sizeY, sizeZ);
		glTexCoord2f(0, 1);glVertex3f(-sizeX, sizeY, sizeZ);
		glTexCoord2f(0, 0);glVertex3f(-sizeX, sizeY, -sizeZ);
		glTexCoord2f(1, 0);glVertex3f(sizeX, sizeY, -sizeZ);
	glEnd();

	//positive z plane
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1);glVertex3f(sizeX, sizeY, -sizeZ);
		glTexCoord2f(0, 1);glVertex3f(-sizeX, sizeY, -sizeZ);
		glTexCoord2f(0, 0);glVertex3f(-sizeX, -sizeY, -sizeZ);
		glTexCoord2f(1, 0);glVertex3f(sizeX, -sizeY, -sizeZ);
	glEnd();

	glDisable(GL_TEXTURE_2D); //disable 2D texuring
}




void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	

	//place the camera 1.2 units above the negative Y plane of the skybox
	gluLookAt(-0.3, -fSkyboxSizeY + 1.2 , 7-deplasare, 0.0, -fSkyboxSizeY + 1.2, -10.0, 0.0, 1.0, 0.0);
	 //Enable 2D texturing
	glEnable(GL_TEXTURE_2D);

	//set global rotation on the X,Y and Z axes
	glRotatef(fGlobalAngleX, 1.0, 0.0, 0.0);
	glRotatef(fGlobalAngleY, 0.0, 1.0, 0.0);
	glRotatef(fGlobalAngleZ, 0.0, 0.0, 1.0);


	
	
	//GALBEN
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_3_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_3_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_3_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_3_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_3_emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_no_emission);//CA SA NU SE FACA GALBENE TOATE*/


		//SOARE
		glEnable(GL_LIGHT0);
	glPushMatrix();
	    glTranslatef( 0,-0.6 ,-17);
		glutSolidSphere(0.3,20,10);
	glPopMatrix();

    //soare
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		//lampa
		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

		glClear(GL_STENCIL_BUFFER_BIT);
		
		glBindTexture(GL_TEXTURE_2D, sunTexture);

	
	if (obj_type == 0) { 
     //Select current texture
	glBindTexture(GL_TEXTURE_2D,car2Texture);
	
	
    }
    else if (obj_type == 1) {
		//Select current texture
	glBindTexture(GL_TEXTURE_2D,carTexture);  
    }
	
	
	glMultMatrixf(myarray);
	
	//DRAW

	//place the tree on the negative Y plane of the skybox
	glTranslatef(0.0, -fSkyboxSizeY + fTreeSize, 0.0);

	//car
	glPushMatrix();
		glTranslatef( -0.3,-2.5 , -2-deplasare);
		glRotatef(directionAngle,0,1,0);
		glRotatef(-270,1,0,0);
		glRotatef(180,0,1,0);
		glmDraw(car, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	
	//plan+street
	glPushMatrix();	
		glTranslatef(0.5,-2.8,-3);
		glScalef(20, 5, 17);
		glRotatef(180,0,0,1);
		glRotatef(180,1,0,0);
		glRotatef(90,0,0,1);
		glRotatef(90,1,0,0);
		glRotatef(180,1,0,0);
		glRotatef(90,0,0,1);
		glRotatef(90,0,0,1);
		glRotatef(180,1,0,0);
		glRotatef(90,0,1,0);
		glRotatef(180,0,1,0);
		glRotatef(180,0,0,1);
		glBindTexture(GL_TEXTURE_2D,roadTexture);
		glmDraw(plan, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	/*//plan de shadow		
		glPushMatrix();
			glRotatef(90,0,1,0);
			glScalef(1.35,1,1.25);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex3f(-6.f, -2.99f, -4.f);
				glTexCoord2f(0,1);
				glVertex3f(-6.f, -2.99f, 4.f);
				glTexCoord2f(1,1);
				glVertex3f(6.f, -2.99f, 4.f);
				glTexCoord2f(1,0);
				glVertex3f(6.f, -2.99f, -4.f);
			glEnd();
		glPopMatrix();*/
		if (shadow1 == 1) {
			//aceleasi coordonate ca la podeaua de mai sus
			v0[X] = -6.f;
			v0[Y] = -2.99f;
			v0[Z] = -4.f;

			v1[X] = -6.f;
			v1[Y] = -2.99f;
			v1[Z] = 4.f;

			v2[X] = 6.f;
			v2[Y] = -2.99f;
			v2[Z] = 4.f;

			PlaneEq(plane, v0, v1, v2);
			ComputeShadowMatrix(floorshadow, plane, light_position);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			glColor3f(0.f, 0.f, 0.f); 

			glPushMatrix();
				//proiectam masina pe podea
				glMultMatrixf((GLfloat *) floorshadow);
			
					glTranslatef( -0.3,-2.5 , -2-deplasare);
					glRotatef(directionAngle,0,1,0);
					glRotatef(-270,1,0,0);
					glRotatef(180,0,1,0);
					glmDraw(car, GLM_SMOOTH | GLM_TEXTURE);

			glPopMatrix();

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
		}
			
		glEnable(GL_TEXTURE_2D);
	
	
	//house
	
	glPushMatrix();	
		glTranslatef(1,1,-17);
		glScalef(2, -3, 2);
		glRotatef(180,0,1,0);
		glRotatef(90,1,0,0);
		glBindTexture(GL_TEXTURE_2D,houseTexture);
		glmDraw(house, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

		//house 2
	glPushMatrix();	
		glTranslatef(-5,1,-17);
		glScalef(2, -3, 2);
		glRotatef(180,0,1,0);
		glRotatef(90,1,0,0);
		glBindTexture(GL_TEXTURE_2D,house2Texture);
		glmDraw(house, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	/*//house2
	glPushMatrix();	
	glTranslatef(-2,-1.5,-10);
	//glScalef(2, -5, 2);
	glRotatef(180,0,1,0);
	glRotatef(90,1,0,0);
	glBindTexture(GL_TEXTURE_2D,house2Texture);
    glmDraw(house2, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();*/
    
		
	
	//banca
	glPushMatrix();	
		glTranslatef(0,-2.5,-19.0);
		glRotatef(90,0,0,1);
		glRotatef(90,0,1,0);
		glBindTexture(GL_TEXTURE_2D,benchTexture);
		glmDraw(bench, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();
	
	//lampa
	glPushMatrix();	
		glTranslatef(-3.5,-2 , -2);
		glScalef(7, 7, 7);
		glRotatef(270,1,0,0);
		glBindTexture(GL_TEXTURE_2D,lampTexture);
		glmDraw(lamp, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	//lampa1
	glPushMatrix();	
		glTranslatef(0.9,-2 , -4);
		glScalef(7, 7, 7);
		glRotatef(270,1,0,0);
		glBindTexture(GL_TEXTURE_2D,lampTexture);
		glmDraw(lamp, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	//lampa2
	glPushMatrix();	
		glTranslatef(5,-2 , -16);
		glScalef(7, 7, 7);
		glRotatef(270,1,0,0);
		glBindTexture(GL_TEXTURE_2D,lampTexture);
		glmDraw(lamp, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	//lampa3
	glPushMatrix();	
		glTranslatef(-7,-2 , -16);
		glScalef(7, 7, 7);
		glRotatef(270,1,0,0);
		glBindTexture(GL_TEXTURE_2D,lampTexture);
		glmDraw(lamp, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	//mailbox
	glPushMatrix();	
		glTranslatef(3,-2.5 , -18);
		glRotatef(90,0,0,1);
		glRotatef(90,0,1,0);
		glBindTexture(GL_TEXTURE_2D,mailTexture);
		glmDraw(mail, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	
    //copac2
	glPushMatrix();	
	glTranslatef(-7.0,-1.8,-10.0);
	glBindTexture(GL_TEXTURE_2D,tree2Texture);
    glmDraw(tree, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	  //copac3
	glPushMatrix();	
	glTranslatef(-5.0,-1.8,-10.0);
	glBindTexture(GL_TEXTURE_2D,tree2Texture);
    glmDraw(tree, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	//copac4
	glPushMatrix();	
	glTranslatef(5.0,-1.8,-10.0);
	glBindTexture(GL_TEXTURE_2D,tree2Texture);
    glmDraw(tree, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

		//copac5
	glPushMatrix();	
	glTranslatef(7.0,-1.8,-10.0);
	glBindTexture(GL_TEXTURE_2D,tree2Texture);
    glmDraw(tree, GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();

	
	
    
	//draw skybox
	glPushMatrix();		
		DrawSkybox(fSkyboxSizeX, fSkyboxSizeY, fSkyboxSizeZ);
	glPopMatrix();

	//draw the tree slice
	glPushMatrix();
	    glTranslatef(-3.2,-1.8,-17.0);
		DrawSingleTreeTexture(fTreeSize);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1,-1,-13.0);
		DrawSingleTreeTexture(fTreeSize);
	glPopMatrix();

	//Disable 2D Texturing
	glDisable(GL_TEXTURE_2D);
	drawText();


	//--------------------------------------------

	//obiectele wireframe sau solid
	if (objtype == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//obiectele smooth sau polygonal
	if (objlook == 0)
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);
	
	if (fogen == 1)
		glEnable(GL_FOG);
	else
		glDisable(GL_FOG);


	if(enlight == 0)
	{
		glDisable(GL_LIGHTING);
	}
	else
	{
		glEnable(GL_LIGHTING);
		if(light1==1)//lumina soarelui
			glEnable(GL_LIGHT0);
		else 
			glDisable(GL_LIGHT0);
		
		if(light3==1)//lumina de la lampa
			glEnable(GL_LIGHT2);
		else 
			glDisable(GL_LIGHT2);

	}
	/*if(shadow1==1){
		glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);
			glEnable(GL_CULL_FACE);

			glCullFace(GL_BACK);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			shadower.vertices = shadVerts;
			shadower.normal   = shadNormal;
			shadower.n        = sizeof(shadVerts) / (3 * sizeof(GLfloat));
    		MakeShadowVolume(&shadower, light_position2, 1.18f, SHADOWVOL);
			
			//make the color buffer read-only
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			render(&shadower);  // render scene in depth buffer 

			glEnable(GL_STENCIL_TEST);
			glDepthMask(GL_FALSE);
			glStencilFunc(GL_ALWAYS, 0, 0);

			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_BACK);  // increment using front face of shadow volume 
			glCallList(SHADOWVOL);

			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glCullFace(GL_FRONT);  // increment using front face of shadow volume 
			glCallList(SHADOWVOL);

			glDepthMask(GL_TRUE);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glCullFace(GL_BACK);
			glDepthFunc(GL_LEQUAL);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			glStencilFunc(GL_EQUAL, 1, 1);  // draw shadowed part 
			glDisable(GL_LIGHT2);
			render(&shadower);

			glStencilFunc(GL_EQUAL, 0, 1);  // draw lit part 
			glEnable(GL_LIGHT2);
			render(&shadower);

			glDepthFunc(GL_LESS);
			glDisable(GL_STENCIL_TEST);
		}*/
	    glEnable(GL_TEXTURE_2D);








glutSwapBuffers();


}


void changeSize(int w, int h)
{
	screen_width=w;
	screen_height=h;

	if(h == 0)
		h = 1;

	float ratio = 1.0*w/h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 50.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
}

void processNormalKeys(unsigned char key, int x, int y)
{
	
	switch(key)
	{
		case 't':
			//process
			glutPostRedisplay();
			break;
		case 'w':
			deplasare= deplasare+0.30;
			glutPostRedisplay();
			break;
		case 's':
			deplasare= deplasare-0.30;
			glutPostRedisplay();
			break;

		case 'd':
			directionAngle -= 1.3;
			break;

		case 'a':
			directionAngle += 1.3;
			break;

		case 27: //esc
			exit(1);
			break;
		//control the global Y rotation angle using 'a' and 'd'
		case 'f': 
			fGlobalAngleY += 1;
			if (fGlobalAngleY >= 360) //clamp the rotation angle in the [0,360) interval
				fGlobalAngleY = (GLint)fGlobalAngleY % 360;
			break;
		case 'g':
			fGlobalAngleY -= 1;
			if (fGlobalAngleY <= -360) //clamp the rotation angle in the [0,360) interval
				fGlobalAngleY = (GLint)fGlobalAngleY % 360;			
			break;
		//control the global X rotation angle using 'w' and 's'
		case 'h':
			fGlobalAngleX += 1;
			if (fGlobalAngleX >= 360) //clamp the rotation angle in the [0,360) interval
				fGlobalAngleX = (GLint)fGlobalAngleX % 360;			
			break;
		case 'j':
			fGlobalAngleX -= 1;
			if (fGlobalAngleX <= -360) //clamp the rotation angle in the [0,360) interval
				fGlobalAngleX = (GLint)fGlobalAngleX % 360;	
			break;
		//control the global Z rotation angle using 'q' and 'e'
		case 'k':
			fGlobalAngleZ += 1;
			if (fGlobalAngleZ >= 360) //clamp the rotation angle in the [0,360) interval
				fGlobalAngleZ = (GLint)fGlobalAngleZ % 360;	
			break;

        //fog and stuff
		case 'l':
			fGlobalAngleZ -= 1;
			if (fGlobalAngleZ <= -360) //clamp the rotation angle in the [0,360) interval
				fGlobalAngleZ = (GLint)fGlobalAngleZ % 360;	
			break;

       case '1': //enable fog
			fogen=1;
			glutPostRedisplay();
			break;
		case '2': //disable fog
			fogen=0;
			glutPostRedisplay();
			break;
		case '3': //disable lights
			enlight=0;
			glutPostRedisplay();
			break;
		case '4': //enable lights
			enlight=1;
			glutPostRedisplay();
			break;	
		case '5': //dezactivez lumina soare
			light1=0;
			glutPostRedisplay();
			break;
		case '6': //activez lumina soare
			light1=1;
			glutPostRedisplay();
			break;
       case '7': //dezactivez lumina lampa
			light3=0;
			glutPostRedisplay();
			break;
		case '8': //activez lumina lampa
			light3=1;
			glutPostRedisplay();
			break;
		case '<': //activez projection shadow
			shadow1 = 1;
			glutPostRedisplay();
			break;
	    case '>': //dezctivez projection shadow
			shadow1 = 0;
			glutPostRedisplay();
			break;

		case '?': //desenez obiectele wireframe
			objtype=0;
			glutPostRedisplay();
			break;
       case '!': //desenez obiectele wireframe
			objlook=0;
			glutPostRedisplay();
			break;
	}
}



void myGlutIdleFunction( void )
{
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  
  glutPostRedisplay();
}

void main(int argc, char* argv[])
{
  //Initialize GLUT and create window
glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
glutInitWindowPosition( 50, 50 );
glutInitWindowSize( 900, 900 );
  //Initialize GLUI and create window
main_window = glutCreateWindow("GLUI tutorial");
glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);
  GLUI *glui;
glui = GLUI_Master.create_glui("GLUI", 0, 400, 50);

//Define the “Type” panel described in Figure 1
GLUI_Panel *type_panel = glui->add_panel("Type");
  radio = glui->add_radiogroup_to_panel(type_panel, &obj_type, RADIO_BUTTONS_GROUP_ID);
  glui->add_radiobutton_to_group(radio, "Texture 1");
glui->add_radiobutton_to_group(radio, "Texture 2");
  
glui->add_separator_to_panel(type_panel);
  //Add the rotate control

glui->add_rotation_to_panel( type_panel,"Rotate",myarray, -1,NULL);

//Keyboard  callback function
glutKeyboardFunc(processNormalKeys);

  GLUI_Master.set_glutIdleFunc(myGlutIdleFunction); 
  
  initOpenGL();

  glutMainLoop();



}

