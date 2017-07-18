// Projection.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include<string>
#include<vector>
#include<iostream>
using namespace std;

// Windows include files 

#ifdef _WIN32
#include <windows.h>
#endif

// OpenGL include files 

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define X .525731112119133606
#define Z .850650808352039932
#define NULL 0

typedef struct Vertex
{
	float x,y,z;
}Vertex;

typedef struct Facep
{
	int num;
	int order[3];
}Facep;

GLfloat vdata[12][3]={
{-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
{0.0,Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
{Z,X,0.0},{-Z,X,0.0},{Z,-X,0.0},{-Z,-X,0.0}};


GLuint tindices[20][3]={
{1,4,0},{4,9,0},{4,5,9},{8,5,4},{1,8,4},
{1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
{3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
{10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}};

//static char *filename="m1489.off";
static char *filename="E:\\北邮\\学习\\毕设\\Sketch dataset\\SHREC13_SBR\\SHREC13_SBR_TARGET_MODELS\\SHREC13_SBR_TARGET_MODELS\\models\\m1716.off";
Vertex ver[10000];
Facep fap[20000];
int i,j,k,n_node,n_face,n_edge;
float scale=1,spin=0;
GLfloat camera[3]= {0.0,0.0,1.0};
float pivot[3];
static int window_height = 768;
static int window_width = 900;
GLdouble winx[10000];
GLdouble winy[10000];
GLdouble winz[10000];
GLint outx[10000];
GLint outy[10000];
GLint pixel[1000][1000];

void normalize(float v[3])
{
GLfloat d=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
if(d==0.0)
return ;
v[0]/=d;
v[1]/=d;
v[2]/=d;
}


void normcrossprod(float v1[],float v2[],float out[])
{
out[0]=v1[1]*v2[2]-v1[2]*v2[1];
out[1]=v1[2]*v2[0]-v1[0]*v2[2];
out[2]=v1[0]*v2[1]-v1[1]*v2[0];
normalize(out);
}


int readoff(const char* filename)
{
	FILE *fp;
	
	if(!(fp=fopen(filename,"r")))
	{
		fprintf(stderr,"Open fail");
		return 0;
	}
    char buffer[1024];
	if(fgets(buffer,1023,fp))
	{
		if(!strstr(buffer,"OFF"))
			{
				printf("It's not a OFF FILE");
				return 0;
		}
		
		if(fgets(buffer,1023,fp))
		{
			sscanf(buffer,"%d %d %d",&n_node,&n_face,&n_edge);
			
			for(i=0;i<n_node;i++)
			{
				fgets(buffer,1023,fp);
				sscanf(buffer,"%f%f%f",&ver[i].x,&ver[i].y,&ver[i].z);
			}
			for(i=0;i<n_face;i++)
			{
				fgets(buffer,1023,fp);
				int temp;
				sscanf(buffer,"%d%d%d%d",&fap[i].num,&fap[i].order[0],&fap[i].order[1],&fap[i].order[2]);
			}
		}

    }
}
void pivotPoint()
{
	float max[3]={0.0,0.0,0.0};
	float min[3]={1.0,1.0,1.0};
	for(i=0;i<n_face;i++)
	{
		for(j=0;j<3;j++){
			int count=fap[i].order[j];
			if (max[0]<ver[count].x)
				max[0]=ver[count].x;
			if (max[1]<ver[count].y)
				max[1]=ver[count].y;
			if (max[2]<ver[count].z)
				max[2]=ver[count].z;
			if (min[0]>ver[count].x)
				min[0]=ver[count].x;
			if (min[1]>ver[count].y)
				min[1]=ver[count].y;
			if (min[2]>ver[count].z)
			{min[2]=ver[count].z;}
		}
	}
	for(i=0;i<3;i++){
		pivot[i]=(max[i]+min[i])/2;
	}
}

void model()
{
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for(i=0;i<n_face;i++)
	{
		int count=fap[i].order[0];
		glVertex3f(ver[count].x,ver[count].y,ver[count].z);
		count=fap[i].order[1];
		glVertex3f(ver[count].x,ver[count].y,ver[count].z);
		count=fap[i].order[2];
		glVertex3f(ver[count].x,ver[count].y,ver[count].z);
	}
	glPopMatrix();
}

void polyhedron()
{
	GLfloat d1[3],d2[3],norm[3];
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for(i=0;i<20;i++)
	{
		for(j=0;j<3;j++)
		{
		d1[j]=vdata[tindices[i][0]][j]-vdata[tindices[i][1]][j];
		d2[j]=vdata[tindices[i][1]][j]-vdata[tindices[i][2]][j];
		}
	normcrossprod(d1,d2,norm);
	glNormal3fv(norm);
	glNormal3fv(&vdata[tindices[i][0]][0]);
	glVertex3fv(&vdata[tindices[i][0]][0]);
	glNormal3fv(&vdata[tindices[i][1]][0]);
	glVertex3fv(&vdata[tindices[i][1]][0]);
	glNormal3fv(&vdata[tindices[i][2]][0]);
	glVertex3fv(&vdata[tindices[i][2]][0]);
	}
	glPopMatrix();
}

/*void transfer()
{
	GLdouble buffer1[16];
	GLdouble buffer2[16];
	GLint buffer3[16];


	glPushMatrix();
	glGetIntegerv(GL_VIEWPORT, buffer3);
	glGetDoublev(GL_PROJECTION_MATRIX, buffer1);
	glGetDoublev(GL_MODELVIEW_MATRIX, buffer2);

	FILE *fp=fopen("out.txt","w");
	fprintf(fp,"%d %d\n",n_node,n_face);

	for(i=0;i<n_node;i++)
	{
			gluProject(ver[i].x,ver[i].y,ver[i].z,buffer2,buffer1,buffer3,&winx[i],&winy[i],&winz[i]);
			outx[i] = int(winx[i]);
			outy[i] = int(winy[i]);

			fprintf(fp,"%d %d\n",outx[i],outy[i]);
			printf("%d,%d\n",outx[i],outy[i]);
	}
	for(i=0;i<n_face;i++)
	{
		fprintf(fp,"3 %d %d %d\n",fap[i].order[0],fap[i].order[1],fap[i].order[2]);

	}
	fclose(fp);
	glPopMatrix();
}*/
void transfer()
{
	GLdouble buffer1[16];
	GLdouble buffer2[16];
	GLint buffer3[16];

	glPushMatrix();
	glGetIntegerv(GL_VIEWPORT, buffer3);
	glGetDoublev(GL_PROJECTION_MATRIX, buffer1);
	glGetDoublev(GL_MODELVIEW_MATRIX, buffer2);

	for (i=0;i<800;i++)
	{
		for(j=0;j<800;j++)
		{
			pixel[i][j]=255;
		}
	}


	FILE *fp=fopen("out.pgm","wb");
	fprintf(fp, "P2\n800 800\n255\n");

	for(i=0;i<n_node;i++)
	{
			gluProject(ver[i].x,ver[i].y,ver[i].z,buffer2,buffer1,buffer3,&winx[i],&winy[i],&winz[i]);
			outx[i] = int(winx[i]);
			outy[i] = int(winy[i]);
			pixel[outx[i]][outy[i]]=0;
			printf("%d,%d\n",outx[i],outy[i]);
	}
	
	for(i=0;i<800;i++){
		for(j=0;j<800;j++){
			fprintf(fp,"%d\n",pixel[i][j]);
		}
	}
	/*for(i=0;i<n_face;i++)
	{
		fprintf(fp,"3 %d %d %d\n",fap[i].order[0],fap[i].order[1],fap[i].order[2]);

	}*/
	fclose(fp);
	glPopMatrix();
}

void redraw(void)
{
	glClearColor(255.0, 255.0, 255.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera[0],camera[1],camera[2], 0.0,0.0,0.0, 0.0,1.0,0.0);	
	glScalef(scale,scale,scale);
	glRotatef(spin,0.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0,0.0,0.0);	
	
	glTranslatef(-pivot[0],-pivot[1],-pivot[2]);
	model();
	//glTranslatef(-pivot[0],-pivot[1],-pivot[2]);
	//polyhedron();
	//transfer();

	glEnd();
	glFlush();
	glutSwapBuffers();

}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	   

	   case '0':camera[0]=0.0;camera[1]=0.0;camera[2]=1.0;glutPostRedisplay();break;
	   case '1':for(i = 0;i<3;i++){ camera[i] = vdata[1][i];}glutPostRedisplay();break;
	   case '2':for(i = 0;i<3;i++){ camera[i] = vdata[2][i];}glutPostRedisplay();break;
	   case '3':for(i = 0;i<3;i++){ camera[i] = vdata[3][i];}glutPostRedisplay();break;
	   case '4':for(i = 0;i<3;i++){ camera[i] = vdata[4][i];}glutPostRedisplay();break;
	   case '5':for(i = 0;i<3;i++){ camera[i] = vdata[5][i];}glutPostRedisplay();break;
	   case '6':for(i = 0;i<3;i++){ camera[i] = vdata[6][i];}glutPostRedisplay();break;
	   case '7':for(i = 0;i<3;i++){ camera[i] = vdata[7][i];}glutPostRedisplay();break;
	   case '8':for(i = 0;i<3;i++){ camera[i] = vdata[8][i];}glutPostRedisplay();break;
	   case '9':for(i = 0;i<3;i++){ camera[i] = vdata[9][i];}glutPostRedisplay();break;
	   case 'q':for(i = 0;i<3;i++){ camera[i] = vdata[10][i];}glutPostRedisplay();break;
	   case 'w':for(i = 0;i<3;i++){ camera[i] = vdata[11][i];}glutPostRedisplay();break;
	   case 'e':for(i = 0;i<3;i++){ camera[i] = vdata[12][i];}glutPostRedisplay();break;

	   case 'p':glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);glutPostRedisplay();break;
       case 'l':glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);glutPostRedisplay();break;
       case 'f':glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);glutPostRedisplay();break;
	   case 'r':for(i=0;i<10;i++){spin = spin+1; glutPostRedisplay(); glFlush();};break;

	}
}
void mouse(int button,int state,int x,int y)
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
		scale*=1.1;
		glutPostRedisplay();
	}
	if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
	{
		scale*=0.9;
		glutPostRedisplay();		

	}
}

void reshape(int w, int h)
{
  glViewport (0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();  
  glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 5.0);
}

void init(int *argc, char **argv)
{
  // Open window 
  glutInit(argc, argv);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(window_width, window_height);
 
  glutCreateWindow("Projection");

  // Initialize GLUT callback functions 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  glutReshapeFunc(reshape);
  glutDisplayFunc(redraw);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //glutMotionFunc(motion);
  glutIdleFunc(0);
}


int main(int argc, char** argv)
{
	init(&argc,argv);
	readoff(filename);
	pivotPoint();
	
	glutMainLoop();
	return 0;
}