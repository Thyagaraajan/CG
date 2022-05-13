#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

int n=0, win[4][2], poly[20][2];
int clipper_size=4,poly_size;

void init(){
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFlush();
}

int x_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4){
	int num = (x1*y2 - y1*x2) * (x3-x4) -
			(x1-x2) * (x3*y4 - y3*x4);
	int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
	return num/den;
}

int y_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4){
	int num = (x1*y2 - y1*x2) * (y3-y4) -
			(y1-y2) * (x3*y4 - y3*x4);
	int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
	return num/den;
}

void polygonDraw(int c){
    if(c==0){
        glColor3d(1,0,0);
    }
    else if(c==1){glBegin(GL_POLYGON);
        for(int i=0;i<n-2;i++){
            glVertex2f(poly[i][0],poly[i][1]);
        }
    glEnd();
        glColor3d(0,1,0);
    }

    glBegin(GL_POLYGON);
        for(int i=0;i<n-2;i++){
            glVertex2f(poly[i][0],poly[i][1]);
        }
    glEnd();

    glFlush();
}

void windowDraw(){
    glColor3d(0,1,1);

    glBegin(GL_LINES);
        glVertex2f(win[0][0],win[0][1]);
        glVertex2f(win[1][0],win[1][1]);

        glVertex2f(win[1][0],win[1][1]);
        glVertex2f(win[2][0],win[2][1]);

        glVertex2f(win[2][0],win[2][1]);
        glVertex2f(win[3][0],win[3][1]);

        glVertex2f(win[3][0],win[3][1]);
        glVertex2f(win[0][0],win[0][1]);
    glEnd();

    glFlush();
}

void clip(int x1, int y1, int x2, int y2){

    int poly_size=n-2;
    int new_points[20][2],new_poly_size=0;
    for(int i=0;i<poly_size;i++)
    {
        int k=(i+1)%poly_size;
        int ix=poly[i][0],iy=poly[i][1];
        int kx=poly[k][0],ky=poly[k][1];

        int ipos= (x2-x1)*(iy-y1) - (y2-y1)*(ix-x1);
        int kpos= (x2-x1)*(ky-y1) - (y2-y1)*(kx-x1);
        //IN - IN
        if(ipos<0 && kpos<0)
        {
            new_points[new_poly_size][0]=kx;
            new_points[new_poly_size][1]=ky;
            new_poly_size++;
        }
        //OUT -IN
        else if(ipos>=0 && kpos<0)
        {
            new_points[new_poly_size][0]=x_intersect(x1,y1,x2,y2,ix,iy,kx,ky);
            new_points[new_poly_size][1]=y_intersect(x1,y1,x2,y2,ix,iy,kx,ky);
            new_poly_size++;

            new_points[new_poly_size][0]=kx;
            new_points[new_poly_size][1]=ky;
            new_poly_size++;

        }
        //IN- OUT
        else if(ipos<0 && kpos>=0)
        {
            new_points[new_poly_size][0]=x_intersect(x1,y1,x2,y2,ix,iy,kx,ky);
            new_points[new_poly_size][1]=y_intersect(x1,y1,x2,y2,ix,iy,kx,ky);
            new_poly_size++;
        }

    }
    poly_size=new_poly_size;
    n=new_poly_size+2;
    for(int i=0;i<poly_size;i++)
    {
        poly[i][0] = new_points[i][0];
		poly[i][1] = new_points[i][1];
    }
}

void suthHodgClip(){

    for(int i=0;i<clipper_size;i++)
    {
        int k=(i+1)%clipper_size;
        clip(win[i][0],win[i][1],win[k][0],win[k][1]);
    }
    polygonDraw(1);
}

void mouseHandler(int button,int state,int x,int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(n<2){
            if(n==0){
                win[n][0]=x;
                win[n][1]=480-y;
            }
            if(n==1){
                win[2][0]=x;
                win[2][1]=480-y;

                win[1][0]=win[0][0];
                win[1][1]=win[2][1];

                win[3][0]=win[2][0];
                win[3][1]=win[0][1];
            }
        }
        else{
            poly[n-2][0]=x;
            poly[n-2][1]=480-y;
        }
        n++;
    }
    if(n==2){
        windowDraw();
    }
    if(n>2 && button == GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
        polygonDraw(0);
        suthHodgClip();
        n=2;
    }
}

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("GLUT Shapes");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    glutMainLoop();

    return EXIT_SUCCESS;
}
