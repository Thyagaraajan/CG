//Active Edge

#include<windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <unistd.h>


using namespace std;

vector< pair<int,int> > points;
vector< pair<int,int> > sorted_y_points;

struct node{
    int y_max;
    float x_ymin;
    float inv_slope;
};

void drawLine(float x1,float y1,float x2,float y2){
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

void Print(vector< pair<int,int > > point){
    cout<<"\n";
    for(int i=0;i<point.size();i++){
        cout<<point[i].first<<" "<<point[i].second<<" -- ";
    }
    cout<<"\n";
}

bool sort_y_fun(const pair<int,int> &p1,const pair<int,int> &p2){
    return (p1.second<p2.second);
}

int findIndex(vector< pair<int,int> > point,pair<int,int> pr){
    for(int i=0;i<point.size();i++){
        if(point[i].first==pr.first && point[i].second==pr.second){
            return i;
        }
    }
    return -1;
}

void FillPoly(){
    copy(points.begin(), points.end(), back_inserter(sorted_y_points));
    sort(sorted_y_points.begin(),sorted_y_points.end(),sort_y_fun);

    int y_min = sorted_y_points[0].second;
    int y_max = sorted_y_points.back().second;

    vector< list<node> *> global_table(y_max-y_min+1);
    int displacement = y_min;
    set< pair<int,int> > visited;

    for(int i=0;i<global_table.size();i++){
        global_table[i] = nullptr;
    }

    for(int i=0;i<sorted_y_points.size();i++){
        int posn = findIndex(points,sorted_y_points[i]);
        int prevpos = posn-1;
        int nextpos = posn+1;
        if(posn==0){
            prevpos = points.size()-1;
        }
        else if(posn==points.size()-1){
            nextpos = 0;
        }

        if(visited.find(points[prevpos]) == visited.end()){
            int x1 = points[prevpos].first, y1 = points[prevpos].second;
            int x2 = points[posn].first, y2 = points[posn].second;
            int y_max = max(y1,y2);
            int x_ymin = 0;
            if(y1<y2){
                x_ymin = x1;
            }
            else{
                x_ymin = x2;
            }
            float inv_slope = (x2-x1)/float(y2-y1);
            node n = {y_max,x_ymin,inv_slope};
            if(global_table[y2-displacement]==nullptr){
                list<node> *l = new list<node>;
                l->push_back(n);
                global_table[y2-displacement] = l;
            }else{
                list<node> *l1 = global_table[y2-displacement];
                l1->push_back(n);
            }
        }
        if(visited.find(points[nextpos]) == visited.end()){
            int x1 = points[nextpos].first, y1 = points[nextpos].second;
            int x2 = points[posn].first, y2 = points[posn].second;
            int y_max = max(y1,y2);
            int x_ymin = 0;
            if(y1<y2){
                x_ymin = x1;
            }
            else{
                x_ymin = x2;
            }
            float inv_slope = (x2-x1)/float(y2-y1);
            node n = {y_max,x_ymin,inv_slope};
            if(global_table[y2-displacement]==nullptr){
                list<node> *l = new list<node>;
                l->push_back(n);
                global_table[y2-displacement] = l;
            }else{
                list<node> *l1 = global_table[y2-displacement];
                l1->push_back(n);
            }
        }
        visited.insert(points[posn]);
    }

    int y = y_min;
    vector< node > AET;
    while(y<=y_max){
        list<node> *l = global_table[y-displacement];
        if(l!=nullptr){
            for(auto it=l->begin();it!=l->end();it++){
                node n1 = {it->y_max,it->x_ymin,it->inv_slope};
                AET.push_back(n1);
            }
        }

        for(int i=0;i<AET.size();i++){
                //increment
                AET[i].x_ymin+=AET[i].inv_slope;
        }

        if(AET.size()>2){
            //delete
            int i=0;
            while(i<AET.size()){
                if(AET[i].y_max==y){
                    AET.erase(AET.begin() + i);
                    i--;
                }
                i++;
            }
        }

        int x1 = AET[0].x_ymin;
        int x2 = AET[1].x_ymin;
        usleep(10000);
        drawLine(x1,y,x2,y);
        y++;
    }
}

void drawPoly(){
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_POLYGON);
    for(int i=0;i<points.size();i++){
        glVertex2d(points[i].first,points[i].second);
    }
    glEnd();
    glFlush();
    FillPoly();

}

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

void MouseFn(int button,int status,int x,int y){
    if(button==GLUT_LEFT_BUTTON && status==GLUT_DOWN){
        pair<int,int> pr(x,480-y);
        points.push_back(pr);
    }
    else if(button==GLUT_RIGHT_BUTTON && status==GLUT_DOWN){
        drawPoly();
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
    glutMouseFunc(MouseFn);
    glutMainLoop();

    return EXIT_SUCCESS;
}
