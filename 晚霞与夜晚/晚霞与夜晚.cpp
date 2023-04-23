#include<GL/glut.h>
#include<math.h>
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
struct Color3ub {
public:
	int r, g, b;
	Color3ub(int r,int g,int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	Color3ub() {

	}
};
void init() {
	//Color3f temp = Color3f(252, 232, 182);
	//Color3f temp = Color3f(149, 219, 125);
	glClearColor(1.0,1.0,1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-600, 600.0, -300.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
}
struct Point
{
public:

	float x, y;
	Point(float x, float y) {
		this->x = x;
		this->y = y;
	}
	Point() {}
	void setxy(float _x, float _y) {

		x = _x;

		y = _y;

	}
	Point operator+(const Point a) {
		return Point(a.x + this->x, a.y + this->y);
	}
};
class BallonInfo {
public :
	Point pos;
	float bound;
	float x;
	float y;
	Color3ub color;
	BallonInfo(float x,float y,float  bound,Color3ub c):pos(x,y) {
		this->bound = bound;
		this->x = pos.x;
		this->y = pos.y;
		this->color = c;
	}
};
bool isBlack = false;
void setPoint(Point p) {

	glBegin(GL_POINTS);

	glVertex2f(p.x, p.y);

	glEnd();

	glFlush();

}
void setline(Point p1, Point p2 ) {

	//glBegin(GL_LINES);
	glBegin(GL_POLYGON);
	glVertex2f(p1.x,p1.y);	
	glVertex2f(-p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glVertex2f(-p2.x, p2.y);
	glVertex2f(-p1.x, p1.y);
	glVertex2f(0, p1.y);

	
	glEnd();

	//glFlush();

}
Point setBezier(Point p1, Point p2, Point p3, Point p4, double t) {

	Point p;

	double a1 = pow((1 - t), 3);

	double a2 = pow((1 - t), 2) * 3 * t;

	double a3 = 3 * t * t * (1 - t);

	double a4 = t * t * t;

	p.x = a1 * p1.x + a2 * p2.x + a3 * p3.x + a4 * p4.x;

	p.y = a1 * p1.y + a2 * p2.y + a3 * p3.y + a4 * p4.y;

	return p;

}
#define PI 3.1415926
void glArc(double x, double y, double start_angle, double end_angle, double radius)
{

	//开始绘制曲线
	//glBegin(GL_POLYGON);
	 	glBegin(GL_TRIANGLE_STRIP);
	//每次画增加的弧度
	double delta_angle = PI / 180;
	//画圆弧
	for (double i = start_angle; i <= end_angle; i += delta_angle)
	{
		//绝对定位加三角函数值
		double vx = x + radius * cos(i);
		double vy = y + radius * sin(i);
		glVertex2d(vx, vy);
	}
	//结束绘画
	glEnd();
}
void DrawCircle(GLfloat x, GLfloat y, int st,int ed,GLfloat r) {

	int n = 360;
	for (int i = st; i <= ed; i++) {
		glVertex2f(r * cos(2 * PI * i / n) + x, y + r * sin(2 * PI * i /
			n));
	}

}
void Balloon() {
	glBegin(GL_POLYGON);
	DrawCircle(0,0,0,180,40);
	DrawCircle(40,0,180,240,80);
	DrawCircle(-40,0,300,360,80);
	glEnd();
	glLineWidth(1.3);
	glBegin(GL_LINES);
	glVertex2f(0,-50);
	glVertex2f(0,-80);
	glVertex2f(0,-80);
	glVertex2f(5, -90);
	glEnd();
	glLineWidth(1);
}
void Arc(Point a,Point b,Point c,Point d) {
//	glBegin(GL_POLYGON);
	Point ps[] = {
		a,
		b,
		c,
		d
	};
	Point current = ps[0];
	for (double t = 0.0; t <= 1.0; t += 0.03)
	{
		Point p = setBezier(ps[0], ps[1], ps[2], ps[3], t);

		setline(current, p);
		current = p;
	}
	//glEnd();
}
void Tree1() {
	glPushMatrix();
	glTranslatef(0,40,0);
	Arc(Point(0, 423),
		Point(50, 420),
		Point(55, 357),
		Point(40, 345)
	);

	Arc(
		Point(40, 345),
		Point(30, 325),
		Point(30, 323),
		Point(36, 320)
	);
	Arc(
		Point(36, 320),
		Point(84, 305),
		Point(82, 251),
		Point(50, 231)
	);
	Arc(
		Point(50, 231),
		Point(45, 228),
		Point(45, 211),
		Point(50, 205)
	);
	Arc(
		Point(50, 205),
		Point(98, 175),
		Point(105, 113),
		Point(60, 100)
	);

	Arc(
		Point(60, 100),
		Point(40, 92),
		Point(40, 80),
		Point(50, 70)
	);
	Arc(
		Point(50, 72),
		Point(55, 45),
		Point(55, 15),
		Point(0, 25)
	);
	glPopMatrix();
	glLineWidth(1.5);
	glBegin(GL_LINES);
	glVertex2f(0, 80);
	glVertex2f(0, 0);
	glEnd();
	glLineWidth(1);


}
void glRec() {
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(1, 0);
	glVertex2f(1, 1);
	glVertex2f(0, 1);
	glEnd();
}
void glAng() {
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(1, 0);
	glVertex2f(0, 1);
	glEnd();
}
void glAngRec() {
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(2, 0);
	glVertex2f(2, 3);
	glVertex2f(1,4);
	glVertex2f(0,3);
	glEnd();
}
void glcolor3ub(Color3ub color) {
	glColor3ub(color.r, color.g, color.b);
}
void glPara(int multi=60) {
	float scalef = (multi-60.0) / 60.0;
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(1+scalef,0);
	glVertex2f(0+scalef,1);
	glVertex2f(-1,1);
	glEnd();
}
void HouseBase(Color3ub color3[3],int multi=60) {
	glPushMatrix();
	glScalef(60, 60, 1);
	glcolor3ub(color3[0]);
	glAngRec();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(120, 180, 0);
	glScalef(60, 60, 1);
	glcolor3ub(color3[1]);
	glPara(multi);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(120, 0, 0);
	glcolor3ub(color3[2]);
	glScalef(multi, 180, 1);
	glRec();
	glPopMatrix();
}
float Time = 1, dx = 0.0005;
int mod = 0;
int dcont = 0;
void House1(Color3ub color3[3],int multi=60) {
	HouseBase(color3,multi);

	int x = 128, y = 160;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 3; j++) {
			glPushMatrix();
		
			if (isBlack&&(i+j+10)%mod==0) {
				glColor3ub(254, 216, 119);
			}
			else glColor3f(0, 0, 0);
			glTranslatef(x+j*19,y-i*19,0);
			glScalef(10,10,1);
			glRec();
			glPopMatrix();
		}
	}
}
void House2(Color3ub color3[3], int multi=60) {
	glPushMatrix();
	glScalef(85.0/120.0, 85.0/120.0,1);
	HouseBase(color3, multi);
	glPopMatrix();
	glColor3f(0, 0, 0);
	int x = (135)* 85.0 / 120.0, y = (150)* 85.0 / 120.0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			glPushMatrix();
			if (isBlack && (i + j+1) % mod == 0) {
				glColor3ub(254, 216, 119);
			}
			else glColor3f(0, 0, 0);
			glTranslatef(x + j * 20, y - i * 20, 0);
			glScalef(10, 10, 1);
			glRec();
			glPopMatrix();
		}
	}
}
void House3(Color3ub color3[3], int multi = 60) {
	glPushMatrix();
	glScalef(50.0 / 120.0, 50.0 / 120.0, 1);
	HouseBase(color3, multi);
	glPopMatrix();
	glColor3f(0, 0, 0);
	int x = (135) * 50.0 / 120.0, y = (135) * 50.0 / 120.0;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 1; j++) {
			glPushMatrix();
			if (isBlack && (i + j+4) % mod == 0) {
				glColor3ub(254, 216, 119);
			}
			else glColor3f(0, 0, 0);
			glTranslatef(x + j * 20, y - i * 20, 0);
			glScalef(10, 10, 1);
			glRec();
			glPopMatrix();
		}
	}
	glPushMatrix();
	glTranslatef(36,0,0);
	glScalef(8,16,0);
	glRec();
	glPopMatrix();
}
void glAngRecHalf() {
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(1, 0);
	glVertex2f(1, 3);
	glVertex2f(0, 4);
	glEnd();
}
void HalfHouseBase(Color3ub color3[3], int multi = 60) {
	glPushMatrix();
	glScalef(60, 60, 1);
	glcolor3ub(color3[0]);
	glAngRecHalf();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(60, 180, 0);
	glScalef(60, 60, 1);
	glcolor3ub(color3[1]);
	glPara(multi);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(60, 0, 0);
	glcolor3ub(color3[2]);
	glScalef(multi, 180, 1);
	glRec();
	glPopMatrix();
}
void HouseGroup1() {
	Color3ub temp[3] = {
		Color3ub(255,191,91),
		Color3ub(108,114,161),
		Color3ub(251,235,222)
	};
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	House1(temp);
	temp[0] = Color3ub(255, 217, 120);
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	glPushMatrix();
	glTranslatef(-35, 0, 0);
	House2(temp, 72);
	glPopMatrix();
	temp[0] = Color3ub(255, 191, 91);
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	glPushMatrix();
	glTranslatef(-50, 0, 0);
	House3(temp);
	glPopMatrix();
}
void HouseGroup2() {
	Color3ub temp[3] = {
		Color3ub(255,191,91),
		Color3ub(108,114,161),
		Color3ub(251,235,222)
	};
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	glPushMatrix();
	glTranslatef(0, 0, 0);
	House2(temp, 72);
	glPopMatrix();
	temp[0] = Color3ub(255, 217, 120);
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	glPushMatrix();
	glTranslatef(-15, 0, 0);
	House3(temp);
	glPopMatrix();
}
void HouseGroup3() {
	Color3ub temp[3] = {
		Color3ub(108,115,161),
		Color3ub(252,234,222),
		Color3ub(215,227,253)
	};
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	glPushMatrix();
	glScalef(1,1,1);
	HouseBase(temp,70);
	glPopMatrix();
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(55, 180, 0);
	for (int i = 0; i < 7; i++) {
			glPushMatrix();
			if (isBlack && (i ) % mod == mod/2) {
				glColor3ub(254, 216, 119);
			}
			else glColor3f(0, 0, 0);
			glTranslatef(0,-i*20-10,0);
			glScalef(10,10,1);
			glRec();
			glPopMatrix();
		}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(55, 180, 0);
	for (int i = 0; i < 7; i++) {
		glPushMatrix();
		if (isBlack && (i) % mod == mod / 2) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(0, -i * 20 - 10, 0);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(150,110,0);
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		if (isBlack && (i) % mod == mod / 2) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);

		glTranslatef(-10,i*20,0);
		glScalef(10,10,1);
		glRec();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(10, i * 20, 0);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(135,0,0);
	glScalef(0.3,0.4,1);
	HalfHouseBase(temp,150);
	glPopMatrix();

	glColor3f(0,0,0);
	glPushMatrix();
	glTranslatef(170, 35, 0);
	for (int i = 0; i < 2; i++) {
		glPushMatrix();
		if (isBlack && (i) % mod == mod / 2) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(-10, i * 20, 0);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(10, i * 20, 0);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glPopMatrix();
}
void HouseGroup4() {
	Color3ub temp[3] = {
		Color3ub(255, 217, 120),
		Color3ub(108,114,161),
		Color3ub(251,235,222)
	};
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	glPushMatrix();
	glScalef(0.7,0.9,1);
	glPushMatrix();
	glScalef(0.7,1.2,1);
	HouseBase(temp,150);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(84,0,0);
	glScalef(1.0,0.6,1);
	HalfHouseBase(temp,105);
	glPopMatrix();
	glPopMatrix();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(135, 35, 0);
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		if (isBlack && (i) % mod == mod / 2) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(-13, i * 20, 0);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(13, i * 20, 0);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(92, 135, 0);
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		if (isBlack && (i) % mod == mod / 2) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(-13, i * 20, 0);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(13, i * 20, 0);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(135, 0, 0);
	glScalef(11, 22, 0);
	glRec();
	glPopMatrix();
}
void HouseGroup5() {
	Color3ub temp[3] = {
		Color3ub(108,114,161),
		Color3ub(255,191,91),
		Color3ub(251,235,222)
	};
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	glPushMatrix();
	HouseBase(temp,155);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(170,190,0);
	glcolor3ub(temp[2]);
	glScalef(40,40,1);
	glRec();
	glRotatef(-180, 0, 0, 1);
	glcolor3ub(temp[0]);
	glTranslatef(0, -1, 0);
	glAng();
	glPopMatrix();;

	glPushMatrix();
	glTranslatef(170, 190, 0);
	glColor3f(0, 0, 0);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			glPushMatrix();
			if (isBlack && (i+j) % mod == mod / 2) {
				glColor3ub(254, 216, 119);
			}
			else glColor3f(0, 0, 0);
			glTranslatef(6+j*18,6+i*18,0);
			glScalef(10,10,1);
			glRec();
			glPopMatrix();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(130,20,0);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			glPushMatrix();
			if (isBlack && (i+j) % mod == mod / 2) {
				glColor3ub(254, 216, 119);
			}
			else glColor3f(0, 0, 0);
			glTranslatef(20*j, 20*i, 1);
			glScalef(10, 10, 1);
			glRec();
			glPopMatrix();
		}
	}
	glPushMatrix();
	glTranslatef(60,-20,0);
	glPushMatrix();
	glcolor3ub(temp[0]);
	glScalef(25,180,1);
	glRec();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(7, 0, 0);
	glScalef(12, 20, 0);
	glColor3f(0,0,0);
	glRec();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50,20,0);
	for (int i = 0; i < 8; i++) {
		glPushMatrix();
		if (isBlack && (i) % mod == mod / 2) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(0, 20 * i, 1);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void glCircle() {
	glPushMatrix();
	glBegin(GL_POLYGON);

	DrawCircle(0,0,0,360,1);
	glEnd();
	glPopMatrix();
}
void HouseRoof(Color3ub temp[3]) {

	
	glPushMatrix();
	glcolor3ub(temp[2]);
	glScalef(15, 11, 1);
	glAngRec();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-180, 0, 0, 1);
	glcolor3ub(temp[0]);
	
	glScalef(30,30,1);
	glTranslatef(0, -1, 0);
	glAng();
	glPopMatrix();

	glPushMatrix();
	glScalef(20, 20, 1);
	glTranslatef(0,1.5 , 0);
	glRotatef(45,0,0,1);
	
	glColor3f(0, 0, 0);
	glPara();
	glPopMatrix();

	glPushMatrix();

	glTranslatef(10,15,0);
	glScalef(11,12,1);
	glRec();
	glPopMatrix();
}
void HouseGroup6() {

	Color3ub temp[3] = {
		Color3ub(255,191,91),
		Color3ub(108,114,161),
		Color3ub(251,235,222)
	};
	if (isBlack) {
		temp[0] = Color3ub(38, 38, 38);
		temp[1] = Color3ub(110, 110, 140);
		temp[2] = Color3ub(70, 70, 70);
	}
	

	glPushMatrix();
	glScalef(0.9,1.0,1.0);
	HouseBase(temp, 165);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(30,35,0);
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 2; j++) {
			glPushMatrix();
		
			if (isBlack && (i + j) % mod == mod / 2) {
				glColor3ub(254, 216, 119);
			}
			else glColor3f(0, 0, 0);
			glTranslatef(j*50,i*22,0);
			glScalef(6,6,1);
			glCircle();
			glPopMatrix();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(125, 26, 0);
	for (int i = 0; i < 7; i++) {
		glPushMatrix();
		if (isBlack && (i) % mod == mod-1) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(0, 22 * i, 1);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glTranslatef(22,0,0);
	for (int i = 0; i < 7; i++) {
		glPushMatrix();
		if (isBlack && (i+3) % mod == mod - 1) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(0, 22 * i, 1);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glTranslatef(60,0,0);
	for (int i = 0; i < 7; i++) {
		glPushMatrix();
		if (isBlack && (i+7) % mod == mod - 1) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(0, 22 * i, 1);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glTranslatef(22, 0, 0);
	for (int i = 0; i < 7; i++) {
		glPushMatrix();
		if (isBlack && (i+6) % mod == mod - 1) {
			glColor3ub(254, 216, 119);
		}
		else glColor3f(0, 0, 0);
		glTranslatef(0, 22 * i, 1);
		glScalef(10, 10, 1);
		glRec();
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(130,180,0);
	glScalef(0.9,0.9,1);
	HouseRoof(temp);
	glTranslatef(90,0,0);
	HouseRoof(temp);
	glPopMatrix();
}
void CreateATree(float  x, float y,float bound ,Color3ub color) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glcolor3ub(color);
	glScalef(bound,bound,1);
	Tree1();
	glPopMatrix();

}
class TreeInfo {

public:
	Point pos;
	float bound;
	float x;
	float y;
	Color3ub color;
	TreeInfo(float x, float y, float  bound, Color3ub c) :pos(x, y) {
		this->bound = bound;
		this->x = pos.x;
		this->y = pos.y;
		this->color = c;
	}
	void Create() {
		glPushMatrix();
		glTranslatef(x, y, 0);
		glcolor3ub(color);
		glScalef(bound, bound, 1);
		Tree1();
		glPopMatrix();
	}
	void CreateBlack() {
		glPushMatrix();
		glTranslatef(x, y, 0);
		glColor3ub(color.r-color.r*8/10  , color.g - color.g * 8 / 10, color.b - color.b * 8 / 10);
		glScalef(bound, bound, 1);
		Tree1();
		glPopMatrix();
	}
};
std::vector<BallonInfo> Ball_q;
std::vector<TreeInfo> Tree_q;
void CreateABalloon(float x,float y,float bound,Color3ub color) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glcolor3ub(color);
	glScalef(bound, bound, 1);
	Balloon();
	glPopMatrix();
}
void drawBackGround() {
	glBegin(GL_POLYGON);

	glColor3f(1.0, 0.8, 0.9);
	glVertex3f(-1, -1, -1);

	glColor3f(0.5, 0.5, 0.6);
	glVertex3f(1, -1, 0);

	glColor3f(0.0, 0.5, 1.0);
	glVertex3f(1, 1, 0);

	glColor3f(1.0, 0.2, 0.0);
	glVertex3f(-1, 1, 0);

	glEnd();
}
void glRoundRec(int centerX, int centerY, int width, int height, float cirR) {
	//二分之PI，一个象限的角度
	float PI_HALF = PI / 2;
	//划分程度,值越大画得越精细
	float divide = 20.0;
	//圆角矩形的坐标
	float tx, ty;
	if(isBlack)glColor3ub(23, 53, 64);
	//画封闭曲线
	glBegin(GL_POLYGON);
	//四个象限不同的操作符
	int opX[4] = { 1,-1,-1,1 };
	int opY[4] = { 1,1,-1,-1 };
	//用来计数，从第一象限到第四象限
	float x = 0;
	//x自增时加的值
	float part = 1 / divide;
	//计算内矩形宽高一半的数值
	int w = width / 2 - cirR;
	int h = height / 2 - cirR;
	//循环画线
	for (x = 0; x < 4; x += part) {
		//求出弧度
		float rad = PI_HALF * x;
		//计算坐标值
		tx = cirR * cos(rad) + opX[(int)x] * w + centerX;
		ty = cirR * sin(rad) + opY[(int)x] * h + centerY;
		//传入坐标画线
		glVertex2f(tx, ty);
	}
	//结束画线
	glEnd();
}
void Sun() {
	//glPushMatrix();
	//glColor3f(253 * 1.0 / 255.0, 226 * 1.0 / 255.0, 183 * 1.0 / 255.0);
	//glTranslatef(-600,-300,0);
	//glScalef(2400,1200,1);
	//glRec();
	//glPopMatrix();
	glColor3ub(240, 116, 3);
	glPushMatrix();
	glTranslatef(400, 200, 0);
	glScalef(50, 50, 1);
	glCircle();
	glPopMatrix();
}
void Luner() {

	//glPushMatrix();
	//glColor3f(4 * 1.0 / 255.0, 3 * 1.0 / 255.0, 62 * 1.0 / 255.0);
	//glTranslatef(-600, -300, 0);
	//glScalef(2400, 1200, 1);
	//glRec();
	//glPopMatrix();

	glColor3ub(252, 240, 172);
	glPushMatrix();
	glTranslatef(400, 200, 0);
	glScalef(50, 50, 1);
	glCircle();
	glColor3f(4 * 1.0 / 255.0, 3 * 1.0 / 255.0, 62 * 1.0 / 255.0);
	glScalef(0.6, 0.6, 1);
	glTranslatef(1, 0, 0);
	glCircle();
	glPopMatrix();
}

void myDisplay() {
	//
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();//4,3,62   240,223,152  253,226,183
	if (isBlack) { 
		glClearColor(4 * 1.0 / 255.0, 3 * 1.0 / 255.0, 62 * 1.0 / 255.0, 1.0); 
	}
	else { 
		glClearColor(253 * 1.0 / 255.0, 226 * 1.0 / 255.0, 183 * 1.0 / 255.0, 1.0); 
	}

	glPushMatrix();
	if (!isBlack) {
		Sun();
	}
	else {
		Luner();
	}
	glPopMatrix();



	


	glPushMatrix();
	glColor3ub(146, 181, 223);
	glTranslatef(400, -200, 0);
	glRotatef(45, 0, 0, 1);
	glScalef(5, 5, 1);
	glRoundRec(0, 0, 100, 100, 5);
	glPopMatrix();

	//(3)
	glPushMatrix();
	glTranslatef(130, -10, 0);
	glScalef(1, 1, 1);
	HouseGroup3();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(146, 181, 223);
	glTranslatef(-70, -100, 0);
	glRotatef(45,0,0,1);
	glScalef(5, 5, 1);
	glRoundRec(0, 0, 100, 100, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100,-80,0);

	glPopMatrix();

//(1)
	glPushMatrix();
	glTranslatef(-326,0,0);
	glScalef(1.05,1,1);
	HouseGroup1();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(146, 181, 223);
	glTranslatef(-350, -280, 0);
	glRotatef(45, 0, 0, 1);
	glScalef(5, 5, 1);
	glRoundRec(0, 0, 100, 100, 10);
	glPushMatrix();
	glTranslatef(-2,-4,0);
	glRotatef(-20,0,0,1);
	glRoundRec(0, 0, 100, 100, 6);
	glPopMatrix();
	glTranslatef(-2, -4, 0);
	glRotatef(20, 0, 0, 1);
	glRoundRec(0, 0, 100, 100, 10);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glColor3ub(146, 181, 223);
	glTranslatef(260, -280, 0);
	glRotatef(45, 0, 0, 1);
	glScalef(5, 5, 1);
	glRoundRec(0, 0, 100, 100, 13);
	glPopMatrix();

	//(2)
	glPushMatrix();
	glTranslatef(-40, 105, 0);
	glScalef(0.8,0.8,1);
	HouseGroup2();
	glPopMatrix();
	
	//(4)
	glPushMatrix();
	glTranslatef(-500,-300,0);
	glScalef(0.7,0.8,1);
	HouseGroup4();
	glPopMatrix();
	//(5)
	glPushMatrix();
	glTranslatef(-326,-300,0);
	glScalef(1.3, 1.3, 1);
	HouseGroup5();
	glPopMatrix();
	//(6)
	glPushMatrix();
	glTranslatef(240,-300,0);
	glScalef(1.2,1.2,1);
	HouseGroup6();
	glPopMatrix();

	for (int i = 0; i < Tree_q.size(); i++) {
		if (isBlack) {
			Tree_q[i].CreateBlack();
		}
		else Tree_q[i].Create();
	}
	if (!isBlack)for (int i = 0; i < Ball_q.size(); i++) {
		if (Ball_q[i].y<300) {
			Ball_q[i].y += 0.01;
			CreateABalloon(Ball_q[i].x, Ball_q[i].y , Ball_q[i].bound, Ball_q[i].color);
	
		}
	}
	//CreateATree(0,0);
	glFlush();
}

void spinDisplay() {
	if(dcont<180)dcont += dx;

	if (Time < 1) {
		Time += dx*((Tree_q.size())/5   +1);
	}
	else {
		Time = 0;
		mod = rand() % 4+2;
	}
	glutPostRedisplay();
}

float dist(Point x, Point y) {
	return sqrt(pow(x.x-y.x,2)+pow(x.y-y.y,2));
}
bool isDelete(float x,float y) {
	for (int i = 0; i < Ball_q.size(); i++) {
		if (Ball_q[i].y<300&&dist(Point(Ball_q[i].x, Ball_q[i].y), Point(x, y)) < Ball_q[i].bound * 45) {
			Ball_q[i].y = 350;
			return false;
		}
	}
	return true;
}
void mouse(int button, int state, int x, int y) {
//		std::cout << x - 600 << "," << 300 - y << std::endl;
	
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			if (isDelete(x - 600, 300 - y)) {
				float bound = (float)(rand() % 11) * 1.0 / 10.0 + 4.0 / 10.0;
				int colorBound = (rand() % 3);
			//	std::cout << colorBound << std::endl;
				Color3ub randColor[] =
				{
					Color3ub(108,115,161),
					Color3ub(158,133,178),
					Color3ub(215,227,253),
				
				};

				CreateABalloon(x - 600, 300 - y,bound,randColor[colorBound]);
				Ball_q.push_back({ (float)x - 600,(float)300 - y,bound,randColor[colorBound] });
			}

		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
		{
			isBlack = !isBlack;

		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN){
			float bound = (float)(rand() % 3) * 1.0 / 10.0 + 1.5 / 10.0;
			int colorBound = (rand() % 3);
			//std::cout << colorBound<<"tree" << std::endl;
			Color3ub randColor[] =
			{
				Color3ub(108,115,161),
				Color3ub(196,109,129),
				Color3ub(215,227,253)
			};
			//printf("%d",x-600,300-y,bound,);
			CreateATree(x - 600, 300 - y, bound, randColor[colorBound]);
			Tree_q.push_back({ (float)x - 600,(float)300 - y,bound,randColor[colorBound] });
		}
		break;
	default:
		break;
	}	
	
	//	glutIdleFunc(NULL);			//	glutIdleFunc(spinDisplay);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	//fopen("1.txt", "w");
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1200, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("第一次大作业");
	init();	
	glutMouseFunc(mouse);
	glutIdleFunc(spinDisplay);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return 0;
}