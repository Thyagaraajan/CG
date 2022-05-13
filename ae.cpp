#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

vector< pair<int,int> > p;
vector< pair<int,int> > new_y;

struct node
{
    int ymax;
    float xymin;
    float slope;
};

void drawLine(float x1,float y1,float x2,float y2)
{
    //glColor3b(1,0,0);
    glBegin(GL_LINES);
    glVertex2d(x1,y1);
    glVertex2d(x2,y2);
    glEnd();
    glFlush();
}

static void display(void)
{
    glFlush();
}

bool sort_y_fun(const pair<int,int> &p1,const pair<int,int> &p2)
{
    return (p1.second<p2.second);
}

int findIndex(vector< pair<int,int> > point,pair<int,int> pr)
{
    for(int i=0;i<point.size();i++)
    {
        if(point[i].first==pr.first && point[i].second==pr.second)
        {
            return i;
        }
    }
    return -1;
}

void FillPoly()
{
    copy(p.begin(), p.end(), back_inserter(new_y));
    sort(new_y.begin(),new_y.end(),sort_y_fun);

    int y_min = new_y[0].second;
    int ymax = new_y.back().second;

    vector< list<node> *> g_table(ymax-y_min+1);
    int displacement = y_min;
    set< pair<int,int> > mp;

    for(int i=0;i<g_table.size();i++)
    {
        g_table[i] = nullptr;
    }

    for(int i=0;i<new_y.size();i++)
    {
        int posn = findIndex(p,new_y[i]);
        int prevpos = posn-1;
        int nextpos = posn+1;
        if(posn==0)
        {
            prevpos = p.size()-1;
        }
        else if(posn==p.size()-1)
        {
            nextpos = 0;
        }

        if(mp.find(p[prevpos]) == mp.end())
        {
            int x1 = p[prevpos].first, y1 = p[prevpos].second;
            int x2 = p[posn].first, y2 = p[posn].second;
            int ymax = max(y1,y2);
            int xymin = 0;
            if(y1<y2)
            {
                xymin = x1;
            }
            else
            {
                xymin = x2;
            }
            float slope = (x2-x1)/float(y2-y1);
            node n = {ymax,xymin,slope};
            if(g_table[y2-displacement]==nullptr)
            {
                list<node> *l = new list<node>;
                l->push_back(n);
                g_table[y2-displacement] = l;
            }
            else
            {
                list<node> *l1 = g_table[y2-displacement];
                l1->push_back(n);
            }
        }
        if(mp.find(p[nextpos]) == mp.end())
        {
            int x1 = p[nextpos].first, y1 = p[nextpos].second;
            int x2 = p[posn].first, y2 = p[posn].second;
            int ymax = max(y1,y2);
            int xymin = 0;
            if(y1<y2)
            {
                xymin = x1;
            }
            else
            {
                xymin = x2;
            }
            float slope = (x2-x1)/float(y2-y1);
            node n = {ymax,xymin,slope};
            if(g_table[y2-displacement]==nullptr)
            {
                list<node> *l = new list<node>;
                l->push_back(n);
                g_table[y2-displacement] = l;
            }else
            {
                list<node> *l1 = g_table[y2-displacement];
                l1->push_back(n);
            }
        }
        mp.insert(p[posn]);
    }

    int y = y_min;
    vector< node > a;
    while(y<=ymax)
    {
        list<node> *l = g_table[y-displacement];
        if(l!=nullptr)
        {
            for(auto it=l->begin();it!=l->end();it++)
            {
                node n1 = {it->ymax,it->xymin,it->slope};
                a.push_back(n1);
            }
        }
        for(int i=0;i<a.size();i++)
        {
                a[i].xymin+=a[i].slope;
        }

        if(a.size()>2)
        {
            int i=0;
            while(i<a.size())
            {
                if(a[i].ymax==y)
                {
                    a.erase(a.begin() + i);
                    i--;
                }
                i++;
            }
        }
        int x1 = a[0].xymin;
        int x2 = a[1].xymin;
        usleep(10000);
        drawLine(x1,y,x2,y);
        y++;
    }
}

void polygondraw()
{
    glColor3d(1,1,0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_POLYGON);
    for(int i=0;i<p.size();i++)
    {
        glVertex2d(p[i].first,p[i].second);
    }
    glEnd();
    glFlush();
    FillPoly();

}

void init()
{
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

void mousefunc(int button,int status,int x,int y)
{
    if(button==GLUT_LEFT_BUTTON && status==GLUT_DOWN)
        {
        pair<int,int> pt(x,480-y);
        p.push_back(pt);
    }
    else if(button==GLUT_RIGHT_BUTTON && status==GLUT_DOWN)
    {
        polygondraw();
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB);

    glutCreateWindow("GLUT Shapes");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mousefunc);
    glutMainLoop();

    return EXIT_SUCCESS;
}
