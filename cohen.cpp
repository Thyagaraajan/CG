#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

struct Point
{
    int x;
    int y;
};
Point pwmin, pwmax;
int cnt=0;
int xp[50],yp[50];
const int inside = 0; // 0000
const int left1 = 1; // 0001
const int right1 = 2; // 0010
const int bottom1 = 4; // 0100
const int top1 = 8; // 1000
void init()
{
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}
int rCode(int x, int y)
{
	int code = inside;
	if (x < pwmin.x)
    {
        code |= left1;   //xxx0 -> xxx1
    }
	else if (x > pwmax.x)
    {
        code |= right1;  //xx0x -> xx1x
    }
	if (y < pwmin.y)
    {
        code |= bottom1; //x0xx -> x1xx
    }
	else if (y > pwmax.y)
    {
        code |= top1;    //0xxx -> 1xxx
    }
	return code;
}
void draw_line(int x1,int y1, int x2, int y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}
void clip(int x1, int y1,int x2, int y2)
{
	int code1 = rCode(x1, y1);
	int code2 = rCode(x2, y2);
	while (true)
    {
		if (!(code1 | code2))   //trivial accept
        {
			glColor3d(0,1,0);   //green
			draw_line(x1,y1,x2,y2);
			break;
		}
		else if (code1 & code2) //trivial reject
        {
			glColor3d(1,0,0);   //red
			draw_line(x1,y1,x2,y2);
			break;
		}
		else
        {
			int pt;
			int x,y;
			if (code1!=0)
            {
				pt=code1;
            }
			else
            {
				pt=code2;
            }
			if(pt & top1)
            {
				x=x1+(x2-x1)*(pwmax.y-y1)/(y2-y1);
				y=pwmax.y;
			}
			else if(pt & bottom1)
			{
				x=x1+(x2-x1)*(pwmin.y-y1)/(y2-y1);
				y=pwmin.y;
			}
			else if(pt & right1)
			{
				y=y1+(y2-y1)*(pwmax.x-x1)/(x2-x1);
				x=pwmax.x;
			}
			else if(pt & left1)
			{
				y=y1+(y2-y1)*(pwmin.x-x1)/(x2-x1);
				x=pwmin.x;
			}
			if(pt==code1)
            {
				x1=x;
				y1=y;
				code1=rCode(x1,y1);
			}
			else
			{
				x2=x;
				y2=y;
				code2=rCode(x1,y1);
			}
		}
	}
}
void draw_boxes()
{
    glColor3d(1,0,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(pwmin.x,pwmin.y);
    glVertex2f(pwmax.x,pwmin.y);
    glVertex2f(pwmax.x,pwmax.y);
    glVertex2f(pwmin.x,pwmax.y);
    glEnd();
}
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}
static void mouseHandling(int button, int state, int x ,int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    xp[cnt] = x;
    yp[cnt] = 480 - y;
    cnt++;
    if(cnt == 2)
    {
        pwmin.x=xp[0];
        pwmin.y=yp[0];
        pwmax.x=xp[1];
        pwmax.y=yp[1];
        draw_boxes();
    }
    if(cnt % 2 == 0 && cnt != 2)
    {
        clip(xp[cnt-2],yp[cnt-2],xp[cnt-1],yp[cnt-1]);
    }
    glFlush();
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
    glutMouseFunc(mouseHandling);
    glClearColor(0,0,0,0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glutMainLoop();

    return EXIT_SUCCESS;
}
