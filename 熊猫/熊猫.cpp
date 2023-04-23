
#include <math.h>
#include <gl/glut.h>
#include <iostream>
#include <vector>
#include<map>
#include<string>
using namespace std;



struct Point2
{
    double x;
    double y;

    Point2() { ; }
    Point2(int px, int py) { x = px; y = py; }
    void SetPoint2(int px, int py) { x = px; y = py; }

    void SOUT() {
        cout << x << " " << y << endl;
    }
};
vector<Point2> vec;
vector<vector<Point2> > save;
#define NUM_POINTS vec.size()
#define NUM_SEGMENTS (NUM_POINTS-3) 

int cnt = 0, idx = 0;


/*全局变量*/
//Point2 vec[NUM_POINTS];

bool mouseLeftDown = false;

/*绘制B样条曲线*/
void Bspline(int n)
{
    float f1, f2, f3, f4;
    float deltaT = 1.0 / n;
    float T;

    for (int i = 0; i < save.size(); i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < save[i].size() - 3; j++) {
            auto ve = save[i];
            for (int i = 0; i <= n; i++) {
                T = i * deltaT;
                f1 = (-T * T * T + 3 * T * T - 3 * T + 1) / 6.0;
                f2 = (3 * T * T * T - 6 * T * T + 4) / 6.0;
                f3 = (-3 * T * T * T + 3 * T * T + 3 * T + 1) / 6.0;
                f4 = (T * T * T) / 6.0;

                glVertex2f(f1 * ve[j].x + f2 * ve[j + 1].x + f3 * ve[j + 2].x + f4 * ve[j + 3].x,
                    f1 * ve[j].y + f2 * ve[j + 1].y + f3 * ve[j + 2].y + f4 * ve[j + 3].y);
            }
        }
        glEnd();
    }
   /* glBegin(GL_LINE_STRIP);
    for (int num = 0; num < NUM_SEGMENTS; num++)
    {
        for (int i = 0; i <= n; i++) {

            T = i * deltaT;
            f1 = (-T * T * T + 3 * T * T - 3 * T + 1) / 6.0;
            f2 = (3 * T * T * T - 6 * T * T + 4) / 6.0;
            f3 = (-3 * T * T * T + 3 * T * T + 3 * T + 1) / 6.0;
            f4 = (T * T * T) / 6.0;
            glVertex2f(f1 * vec[num].x + f2 * vec[num + 1].x + f3 * vec[num + 2].x + f4 * vec[num + 3].x,
                f1 * vec[num].y + f2 * vec[num + 1].y + f3 * vec[num + 2].y + f4 * vec[num + 3].y);
        }
    }
    glEnd();*/
}

void BsplineStrip(int n,vector<Point2> ve)
{
    float f1, f2, f3, f4;
    float deltaT = 1.0 / n;
    float T;
   
        glColor3f(0,0,0);
       
        glBegin(GL_LINE_STRIP);
     
        for (int j = 0; j < ve.size() - 3; j++) {
            for (int i = 0; i <= n; i++) {
                T = i * deltaT;
                f1 = (-T * T * T + 3 * T * T - 3 * T + 1) / 6.0;
                f2 = (3 * T * T * T - 6 * T * T + 4) / 6.0;
                f3 = (-3 * T * T * T + 3 * T * T + 3 * T + 1) / 6.0;
                f4 = (T * T * T) / 6.0;

                glVertex2f(f1 * ve[j].x + f2 * ve[j + 1].x + f3 * ve[j + 2].x + f4 * ve[j + 3].x,
                    f1 * ve[j].y + f2 * ve[j + 1].y + f3 * ve[j + 2].y + f4 * ve[j + 3].y);
            }
        }
        glEnd();
    
    
}
void BsplinePolygon(int n, vector<Point2> ve, bool Black)
{
    float f1, f2, f3, f4;
    float deltaT = 1.0 / n;
    float T;

    if (Black) {
        glColor3f(0, 0, 0);
    }
    else glColor3f(1, 1, 1);

    glBegin(GL_POLYGON);
    for (int j = 0; j < ve.size() - 3; j++) {
        for (int i = 0; i <= n; i++) {
            T = i * deltaT;
            f1 = (-T * T * T + 3 * T * T - 3 * T + 1) / 6.0;
            f2 = (3 * T * T * T - 6 * T * T + 4) / 6.0;
            f3 = (-3 * T * T * T + 3 * T * T + 3 * T + 1) / 6.0;
            f4 = (T * T * T) / 6.0;

            glVertex2f(f1 * ve[j].x + f2 * ve[j + 1].x + f3 * ve[j + 2].x + f4 * ve[j + 3].x,
                f1 * ve[j].y + f2 * ve[j + 1].y + f3 * ve[j + 2].y + f4 * ve[j + 3].y);
        }
    }
    glEnd();


}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

     glLineWidth(5.0f);
    //耳朵
    BsplinePolygon(20, save[3], true);
    glPushMatrix();
    glTranslatef(720,0,0);
    glScalef(-1,1,1);
    BsplinePolygon(20, save[3],true);
    glPopMatrix();
    //肚子
    glPushMatrix();
    glTranslatef(0,10,0);
    BsplinePolygon(20, save[6], false);
    BsplineStrip(20, save[6]);
    glPopMatrix();
    //手

    BsplinePolygon(20, save[7], true); 
    glPushMatrix();
    glTranslatef(710, 0, 0);
    glScalef(-1,1,1);
    BsplinePolygon(20, save[7], true);
    glPopMatrix();
    BsplinePolygon(20, save[8], false);
    glPushMatrix();
    glTranslatef(0,-15,0);
    BsplinePolygon(20, save[9], true);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 10, 0);
    BsplinePolygon(20, save[10], false);
    glPopMatrix();
    //头
    BsplinePolygon(20,save[0],false);
    BsplineStrip(20, save[0]);
    BsplinePolygon(20, save[11], true);
    BsplineStrip(20,save[12]);
    glPushMatrix();
    glTranslatef(715, 0, 0);
    glScalef(-1, 1, 1);    BsplineStrip(20, save[12]);
    glPopMatrix();
 
    //眼睛
    glPushMatrix();
    glTranslatef(-20,0,0);
    BsplinePolygon(20,save[2],true);
    BsplinePolygon(20, save[1], false);
    glPushMatrix();
    glScalef(0.7,0.7,1);
    glTranslatef(122,105,0);
    BsplinePolygon(20, save[1], true);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();

    glTranslatef(740, 0, 0);
    glScalef(-1, 1, 1);


    BsplinePolygon(20, save[2], true);
    BsplinePolygon(20, save[1], false);
    glPushMatrix();
    glScalef(0.7, 0.7, 1);
    glTranslatef(122, 105, 0);
    BsplinePolygon(20, save[1], true);
    glPopMatrix();
    glPopMatrix();
    //脚
    BsplinePolygon(20, save[5], true);
    BsplineStrip(20, save[5]);
    glPushMatrix();
    glTranslatef(700, 0, 0);
    glScalef(-1, 1, 1);
    BsplinePolygon(20, save[5], true);
    BsplineStrip(20, save[5]);
    glPopMatrix();
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex2f(365, 330);
    glVertex2f(357, 372);
    glEnd();
    //Bspline(20);
    glFlush();
    glutSwapBuffers();
}
void vectorInit() {
    vec.push_back(Point2(200, 400));
    vec.push_back(Point2(200, 300));
    vec.push_back(Point2(100, 300));
    vec.push_back(Point2(200, 200));
}
void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
    //vectorInit();
}


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLsizei)w, (GLsizei)h, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseLeftDown = true;
    }
  /*  if (button == GLUT_KEY_UP && state == GLUT_DOWN) {
        cout << "# 第" << cnt + 1 << "次画图" << endl;
        for (int i = 0; i < NUM_POINTS; i++)
        {

            vec[i].SOUT();

        }
        cnt++;
        save.push_back(vec);
        vec.clear();
        vectorInit();
    }*/
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        cout << "# 第" << cnt + 1 << "次画图" << endl;
        for (int i = 0; i < NUM_POINTS; i++)
        {

            vec[i].SOUT();

        }
        cnt++;
        save.push_back(vec);
        vec.clear();
        vectorInit();
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        vec.push_back(Point2(x, y));
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        mouseLeftDown = false;
    }
}

double distance(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void motion(int x, int y)
{
    if (mouseLeftDown)
    {
        for (int i = 0; i < NUM_POINTS; i++)
        {
            if (distance(vec[i].x, vec[i].y, x, y) < 20)
            {
                vec[i].SetPoint2(x, y);
                break;
            }
        }
    }

    glutPostRedisplay();
}
int StringToInt(string s)
{
    double cnt1 = 1;
    double res = 0;
    bool PointFlag = false;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != '.')
        {
            if (PointFlag) cnt1 *= 10;
            res = res * 10 + s[i] - '0';
        }
        else PointFlag = true;
    }
    return res / cnt1;
}



int main(int argc, char** argv)
{
    // freopen("in.in", "r", stdin);	//输入文件
    freopen("output.txt", "r", stdin);	//输出文件
    string strx,stry;
    bool flag = false;

    while (cin >> strx>>stry) {
        cout << strx << " " << stry << endl;
        if (strx[0] == '#') {

            if (flag)save.push_back(vec);
            else flag = true;
            vec.clear();
        }
        else {
            vec.push_back({ StringToInt(strx),StringToInt(stry)});
        }
    }
    save.push_back(vec);
    vec.clear();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("B-Spline Curve");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;

}