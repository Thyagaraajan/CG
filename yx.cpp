#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

int poly[100][2],Ymin=1000,Ymax=0,n=0,flag=0;

void init(){
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

static void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();
}

void polygonDraw(){
    glColor3d(0,1,1);

    glBegin(GL_LINES);
        for(int i=0;i<n;i++){
            int k=(i+1)%n;
            glVertex2f(poly[i][0],poly[i][1]);
            glVertex2f(poly[k][0],poly[k][1]);
        }
    glEnd();

    glFlush();
}
void yxAlgorithm(){
    for(int i=Ymin;i<=Ymax;i++){

        vector< pair<int,int> > points;

        for(int j=0;j<640;j++){

            unsigned char pixel[4];
            glReadPixels(j,i,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel);
            int R,G,B;
            R=int(pixel[0]);
            G=int(pixel[1]);
            B=int(pixel[2]);

            if(R==0 && G==255 && B==255){
                points.push_back({j,i});
            }
        }

        glColor3d(1,0,0);
        glBegin(GL_LINES);
        for(int j=0;j<points.size();j+=2){
            if(j+1>=points.size()){
                break;
            }

            glVertex2f(points[j].first,points[j].second);
            glVertex2f(points[j+1].first,points[j+1].second);
        }
        glEnd();
        glFlush();
    }

}

void mouseHandler(int button ,int state,int x,int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        poly[n][0]=x;
        poly[n][1]=480-y;
        Ymin=min(Ymin,poly[n][1]);
        Ymax=max(Ymax,poly[n][1]);
        n++;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        polygonDraw();
        yxAlgorithm();
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
