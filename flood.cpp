#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

int n=0,ok=0,arrX[100],arrY[100];

void init(){
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();
}

void drawPixel(int x,int y){
    glColor3d(1,0,0);

    glBegin(GL_POINTS);
        glVertex2f(x,y);
    glEnd();

    glFlush();
}

void polygonDraw(){
    glColor3d(0,1,1);

    glBegin(GL_LINES);
    for(int i=0;i<n;i++){
        int k=(i+1)%n;
        glVertex2f(arrX[i],arrY[i]);
        glVertex2f(arrX[k],arrY[k]);
    }
    glEnd();

    glFlush();
}

void floodFill(int x,int y,int R,int G,int B){
    if(x<0 || x>=640 || y<0 || y>=480){
        return ;
    }

    int nR,nG,nB;
    unsigned char pixel[4];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    nR=(int)pixel[0];
    nG=(int)pixel[1];
    nB=(int)pixel[2];

    if(nR!=R || nG!=G || nB!=B){
        return ;
    }

    drawPixel(x,y);
    floodFill(x+1,y,nR,nG,nB);
    floodFill(x-1,y,nR,nG,nB);
    floodFill(x,y+1,nR,nG,nB);
    floodFill(x,y-1,nR,nG,nB);

    return ;
}

void mouseHandler(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        arrX[n]=x;
        arrY[n]=480-y;
        n++;
    }
    if(ok==0 && button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
        polygonDraw();
        n=0;
        ok=1;
    }
    else if(ok==1 && button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
        unsigned char pixel[4];
        glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        int R,G,B;
        R=(int)pixel[0];
        G=(int)pixel[1];
        B=(int)pixel[2];
        floodFill(x,480-y,R,G,B);
        n=0;
        ok=0;
    }
}

int main(int argc, char *argv[])
{
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
