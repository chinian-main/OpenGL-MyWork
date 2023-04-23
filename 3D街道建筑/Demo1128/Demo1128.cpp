#include<GL/freeglut.h>
#include<iostream>
#include<vector>
#include<map>
using namespace std;
struct vec3 {
    float x, y, z;
};
struct vec2 {
    float x, y;
};

#define _CRT_SECURE_NO_WARNINGS
#define mi 19.69
GLuint texGround;
GLuint texWall;

#define BMP_Header_Length 54  //图像数据在内存块中的偏移量
static GLfloat angle = 220.0f;   //旋转角度

// 函数power_of_two用于判断一个整数是不是2的整数次幂
int power_of_two(int n)
{
    if (n <= 0)
        return 0;
    return (n & (n - 1)) == 0;
}
/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint last_texture_ID = 0, texture_ID = 0;

    // 打开文件，如果失败，返回
    FILE* pFile = fopen(file_name, "rb");
    if (pFile == 0)
        return 0;
    // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);
    printf("w=%d h=%d\n", width, height);
    // 计算每行像素所占字节数，并根据此数据计算总像素字节数
    {
        GLint line_bytes = width * 3;
        while (line_bytes % 4 != 0)
            ++line_bytes;
        total_bytes = line_bytes * height;
    }
    // 根据总像素字节数分配内存
    pixels = (GLubyte*)malloc(total_bytes);
    if (pixels == 0)
    {
        cout << "pixels" << total_bytes << endl;
        fclose(pFile);
        return 0;
    }
    // 读取像素数据
    if (fread(pixels, total_bytes, 1, pFile) <= 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    // 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
    // 若图像宽高超过了OpenGL规定的最大值，也缩放
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if (!power_of_two(width)
            || !power_of_two(height)
            || width > max
            || height > max)
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;

            // 计算每行需要的字节数和总字节数
            new_line_bytes = new_width * 3;
            while (new_line_bytes % 4 != 0)
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;

            // 分配内存
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if (new_pixels == 0)
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }

            // 进行像素缩放
            gluScaleImage(GL_RGB,
                width, height, GL_UNSIGNED_BYTE, pixels,
                new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }

    // 分配一个新的纹理编号
    glGenTextures(1, &texture_ID);
    if (texture_ID == 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // 绑定新的纹理，载入纹理并设置纹理参数
    // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
    GLint lastTextureID = last_texture_ID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //********1.请输入代码设置纹理映射控制函数********//
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    //**********************************************//
    //*********2.请输入代码设置纹理的定义函数*********//
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    //**********************************************//
    //********3.请输入代码进行纹理映射方式设置*********//

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    //**********************************************//
    glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
    free(pixels);
    return texture_ID;
}
bool loadOBJ(
    const char* path,
    std::vector<vec3>& out_vertices,
    std::vector<vec2>& out_uvs,
    std::vector<vec3>& out_normals
){
    printf("Loading OBJ file %s...\n", path);

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<vec3> temp_vertices;
    std::vector<vec2> temp_uvs;
    std::vector<vec3> temp_normals;


    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        return false;
    }

    while (1) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

    // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0) {
           // cout << "Get v" << endl;
            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
            //cout << temp_vertices.size() << endl;
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            //cout << "Get vt" << endl;
            vec2 uv;
            float t;
            fscanf(file, "%f %f %f\n", &uv.x, &uv.y,&t);
            uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
           // cout << "Get vn" << endl;
            vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else 
            if (strcmp(lineHeader, "f") == 0) {
            //cout << "Get f" << endl;
            std::string vertex1, vertex2, vertex3;
            int vertexIndex[3], uvIndex[3], normalIndex[3];
            
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0],&normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
          //  cout << (char*)(file) << endl;
          //  printf("%d/%d/%d %d/%d/%d %d/%d/%d\n", vertexIndex[0], uvIndex[0], normalIndex[0], vertexIndex[1], uvIndex[1], normalIndex[1], vertexIndex[2], uvIndex[2], normalIndex[2]);
            
            if (matches != 9) {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
        }
        else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }


    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {

        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int normalIndex = normalIndices[i];
        unsigned int uvIndex = uvIndices[i];
        // Get the attributes thanks to the index
        vec3 vertex = temp_vertices[vertexIndex - 1];
        vec3 normal = temp_normals[normalIndex - 1];
        vec2 uv = temp_uvs[uvIndex - 1];
        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_normals.push_back(normal);
        out_uvs.push_back(uv);

    }
    return true;
}
GLfloat points1[4][3] = { {-1,0,1},{1,0,1},{0,0,-0.7},{0,1.7,0} };
GLfloat Colors1[4][3] = { {0,1,0},{1,0,0},{1,1,0},{0,0,1} };
int vertice1[4][3] = { {0,1,2},{1,2,3},{0,2,3},{0,1,3} };
struct Model {
    std::vector<vec3> vertices;//点坐标
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
};
double rotate_x = 0.0;
double rotate_y = 0.0;
Model cube;
/*观察坐标系参数设置*/
GLfloat x0 = 40.5+20, yy = 126, z0 = 70.0;	    //设置观察坐标系原点 
GLfloat xref = 0.0, yref = 0.0, zref = 0.0;	//设置观察坐标系参考点（视点） 
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;	    //设置观察坐标系向上向量（y轴） 
GLfloat white[] = { 1.0,1.0,1.0,1.0 };
GLfloat light_pos[] = { 0,300,300,1 };
float fovy = 40+12; 
void DrawInit(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, 1, 1, 10000);
    glMatrixMode(GL_MODELVIEW); //指定当前操作模型视图矩阵堆栈
    glLoadIdentity();
    gluLookAt(x0, yy, z0, xref, yref, zref, Vx, Vy, Vz);        //指定三维观察参数
}
//void Init()
//{
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//GLfloat noLight[4] = { 0.0f,0.0f,0.0f,1.0f };
//GLfloat ambientLight[4] = { 0.3f,0.3f,0.3f,0.2f };
//GLfloat brightLight[4] = { 1.0f,1.0f,1.0f,0.3f };
//
//static GLfloat lightPos[] = { 5.0f,5.0f,5.0f,1.0f };
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_DEPTH_TEST);
//
//    glEnable(GL_LIGHTING);
//    glLigntModelfv(GL_LIGHT_MODEL_AMBIENT, noLight);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, brightLight);
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//
//    glEnable(GL_COLOR_MATERIAL);
//    glColorMaterialf(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, brightLight);
//
//    glMateriali(GL_FRONT, GL_SHININESS, 30);
//
//    
//}
//void MaterialSetting() {
//    glMaterialfv(GL_FRONT, GL_AMBIENT, mid_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, mid_diffuse);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mid_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, mid_shininess);
//    glMaterialfv(GL_FRONT, GL_EMISSION, mid_emission);
//
//    //glMaterialfv(GL_FRONT, GL_AMBIENT, left_ambient);
//    //glMaterialfv(GL_FRONT, GL_DIFFUSE, left_diffuse);
//    //glMaterialfv(GL_FRONT, GL_SPECULAR, left_specular);
//    //glMaterialfv(GL_FRONT, GL_EMISSION, left_emission);
//}
const int AXES_LEN = 300;
const int ARROW_LEN = 100;
const int ARROW_RADIUS = 30;
void InitTex();
void LightSetting() {
    GLfloat white_light[] = { 100.0, 100.0, 100.0, 100.0 };
    GLfloat light_ambient[] = { 0.2 , 0.2 , 0.2 , 1.0 };
    GLfloat brightLight[4] = { 1.0f,1.0f,1.0f,0.3f };
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);   
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, brightLight);
    glMaterialfv(GL_BACK, GL_SPECULAR, brightLight);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    glMaterialf(GL_BACK, GL_SHININESS,100);
}
void GLDrawSpaceAxes(void)
{
    GLUquadricObj* objCylinder = gluNewQuadric();
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(0.25, 6, 6);
    glColor3f(0.0f, 0.0f, 1.0f);
    gluCylinder(objCylinder, 0.1, 0.1, AXES_LEN, 10, 5);
    glTranslatef(0, 0, AXES_LEN);
    gluCylinder(objCylinder, 0.3, 0.0, 0.6, 10, 5);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(objCylinder, 0.1, 0.1, AXES_LEN, 10, 5);
    glTranslatef(0, 0, AXES_LEN);
    gluCylinder(objCylinder, 0.3, 0.0, 0.6, 10, 5);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glRotatef(90, 0.0, 1.0, 0.0);
    gluCylinder(objCylinder, 0.1, 0.1, AXES_LEN, 10, 5);
    glTranslatef(0, 0, AXES_LEN);
    gluCylinder(objCylinder, 0.3, 0.0, 0.6, 10, 5);
    glPopMatrix();
}
map<string, GLint> texs;
map<string, Model > models;
void CreateModel(string model_str ,string tex_str) {
    Model model = models[model_str];
    GLint tex = texs[tex_str];

    glBindTexture(GL_TEXTURE_2D, tex);
    //MaterialSetting();
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < model.normals.size(); i++)
    {
        glTexCoord2f(model.uvs[i].x, model.uvs[i].y);
        glNormal3f(model.normals[i].x, model.normals[i].y, model.normals[i].z);
        glVertex3f(model.vertices[i].x, model.vertices[i].y, model.vertices[i].z);
    }
    glEnd();
}
void Car(int i=0) {
    glPushMatrix();
    glTranslatef(-mi,0,0.55*mi);
    glScalef(0.25,0.25,0.35);
    glRotatef(90,0,0,1);
    if(i==0)  CreateModel("car1", "zise");
    else if(i==1) CreateModel("car1", "red");
    else CreateModel("car1", "black");
    CreateModel("car1", "black");
   
    
    CreateModel("car3", "window"); 
    CreateModel("car2", "yellow");
    CreateModel("car4", "zise");
    glPopMatrix();
}
void ReCar(int i = 0) {
    glPushMatrix();
    glTranslatef( mi, 0, 0.55 * mi);
    glScalef(0.25, 0.25, 0.35);
    glRotatef(-90, 0, 0, 1);
    if (i == 0)  CreateModel("car1", "zise");
    else if (i == 1) CreateModel("car1", "red");
    else CreateModel("car1", "black");
    CreateModel("car1", "black");
    CreateModel("car2", "yellow");
    CreateModel("car3", "window");
    CreateModel("car4", "zise");
    glPopMatrix();
}
void Bench() {
    glPushMatrix();
   // glRotatef(rotate_x, 0, 0, 1);
    CreateModel("bench", "bench");
    glPopMatrix();
}
void Tree() {
    glPushMatrix();
  //  glRotatef(rotate_x, 0, 0, 1);
    glScalef(0.6,0.6,0.6);

    CreateModel("leaf", "leaf");
    glScalef(1.5,1.5,1.5);
    CreateModel("treepi", "treepi");
    glPopMatrix();
}
void GrassLand() {
    glPushMatrix();
    CreateModel("grass","grassland");
    glPopMatrix();
}
void Road() {
    glPushMatrix();
    glTranslatef(0,0,0.1f);
    glScalef(0.5,1,1);
    CreateModel("grass", "road");
    glPopMatrix();
}
void WhiteLine() {
    glPushMatrix();
   // glRotatef(90,0,0,1);
    glTranslatef(0, -2.5*mi, 0.2f);
    glScalef(0.03 ,0.30, 1);
    CreateModel("grass", "whiteline");
    glPopMatrix();
    glPushMatrix();
    // glRotatef(90,0,0,1);
    glTranslatef(0, 2.5 * mi, 0.2f);
    glScalef(0.03, 0.30, 1);
    CreateModel("grass", "whiteline");
    glPopMatrix();
}
void RoadGroup() {
    Road();
    WhiteLine();
}
void Cube() {
    CreateModel("cube", "wall");
}
void RoadOb() {
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glScalef(0.25, 0.25, 1);
    Cube();
    glPopMatrix();
}
void Display() {
    DrawInit();
    glRotatef(90, 1, 0, 0);
    glRotatef(180, 0, 1, 0);
    glRotatef(120, 0, 0, 1);
      //glColorMaterial(GL_FRONT, GL_DIFFUSE);
      //glEnable(GL_COLOR_MATERIAL);
    //begin 
   // GLDrawSpaceAxes();
   // LightSetting();
    glPushMatrix(); 

    glScalef(0.25,0.25,0.25);

    //树和路障
    {
    for (int i = -5; i < 5; i++) {
            for (int j = -1; j < 2; j++) {
                if (j == 0)continue;
                glPushMatrix();
            
                glTranslatef(j*mi*5,i*mi*5,0);
                glRotatef((i+5)*10-((j==-1?-1:0)*30),0,0,1);
                Tree();
                glPopMatrix();
            }
             for (int j = -4; j < 5; j++) {
                 for (int k = -1; k < 2; k++) {
                     if (k == 0)continue;
                  glPushMatrix();
                  glTranslatef(k*mi*2.5,j*mi+i*mi*10,0);
                  RoadOb();
                  glPopMatrix();
                 }
           
            }     
        }
    }
    //椅子
    {
    for (int i = -5; i < 5; i++) {
            for (int j = -1; j < 2; j++) {
                if (j == 0)continue;
                glPushMatrix();
                glTranslatef(-1*j*mi*3.5,i*mi*10,0);
                glRotatef(j*90,0,0,1);
                glScalef(0.7,0.6,0.7);
                Bench();
                glPopMatrix();
           
            }
        } 
    }
    //草地面
    { glPushMatrix();
    glTranslatef(-1*mi,0,0);
    for (int i = -5; i < 5; i++) {
        for (int j = -4; j < 4; j++) {
            glPushMatrix();
            glTranslatef(j*mi*10, i * mi * 10,0);
            GrassLand();

            glPopMatrix();
        }
    }
    glPopMatrix();}
    // 道路
    {
     glPushMatrix();
        for(int i = -5; i < 5; i++) {
                glPushMatrix();
                glTranslatef(0, i * mi * 10, 0);
                RoadGroup();
                glPopMatrix();
        }
        glPopMatrix();
    }
    //大厦
    {
    glPushMatrix();
    glTranslatef(-10*mi,0,0);
    glScalef(1.7, 1.7, 1.7);
    CreateModel("stand","dasa");
    glPushMatrix();
    CreateModel("roof", "roof");
    glPopMatrix();
    glPopMatrix();
    }
    //肯德基
    {
        glPushMatrix();
        glTranslatef(-10*mi,8*mi,0);
        glScalef(2.5,5,1.5);
        CreateModel("stand","kfc");
        glPushMatrix();
        glTranslatef(0,0,-mi);
        CreateModel("roof", "roof");
        glPopMatrix();
        glPopMatrix();
    }
    //sam
    {
        glPushMatrix();
        glTranslatef(-11 * mi, -10 * mi, 0);
        glScalef(0.5f,3,0.5);
        glScalef(4,2,2);
        glRotatef(180,1,0,0);
        glRotatef(180,0, 1, 0);
        CreateModel("stand","sam");
        glPushMatrix();
        CreateModel("roof","roof");
        glPopMatrix();
        glPopMatrix();
    }
    //钟楼
    {
        glPushMatrix();
        glTranslatef(-9*mi,-19*mi,0);
        glPushMatrix();
        glTranslatef(0,0,mi*5);
        glScalef(1.3,1.3,0.5);
        CreateModel("stand","zhongUp");
        glTranslatef(0,mi*0.25,mi*5.5);
        glScalef(1,1,2);
        CreateModel("sileng","zhongRed");
        glPopMatrix();
        glPushMatrix();
        CreateModel("stand","zhongDown");
        glPopMatrix();
        glPopMatrix();
    }
    //car
    {
        for (int i = -5; i < 5; i++) {
         glPushMatrix();
         glTranslatef(0,i*mi*5,0);
         glTranslatef(0,rotate_x * 3,0);
         Car((i+100)%3);
         glPopMatrix();       
        }
        for (int i = -5; i < 5; i++) {
            glPushMatrix();
            glTranslatef(0, i * mi * 5, 0);
            glTranslatef(0, -rotate_x*3, 0);
            ReCar((i+80) % 3);
            glPopMatrix();
        }
        
    }
 
    glPopMatrix();
    //end

    glFlush(); 
    glutSwapBuffers();
}
void reshape(int width, int height)
{

/* 
    if(height == 0)
    height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
*/


    if (0 == height)
        height = 1;
    glViewport(0, 0, width, height); //设置视区尺寸
    glMatrixMode(GL_PROJECTION); //指定当前操作投影矩阵堆栈
    glLoadIdentity(); //重置投影矩阵
    //指定透视投影的观察空间
    gluPerspective(45.0f, (float)width / (float)width, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void spin( ) {
    rotate_x +=0.05;
    glutPostRedisplay();
}
void specialkeys(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT)
    {
        fovy -= 1;
    }
    else if (key == GLUT_KEY_LEFT) {
        fovy += 1;
    }
       
    else if (key == GLUT_KEY_UP) {
        yy += 1;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        yy-=1;
    }
    //cout << fovy << " " << yy << endl;
    glutPostRedisplay();
}
void myKeyboard(unsigned char key, int x, int y)
{
 /*   glMatrixMode(GL_MODELVIEW);
    glMatrixMode(GL_PROJECTION);*/
    //switch (key)
    //{
    //case 'a': case 'A': glTranslated(-10, 0, 0); break;     //向左平移
    //case 'd': case 'D': glTranslated(10, 0, 0); break;     //向右平移
    //case 'w': case 'W': glTranslated(0, 10, 0); break;     //向上平移
    //case 's': case 'S': glTranslated(0, -10, 0); break;     //向下平移
    //case 'q': case 'Q': glTranslated(0, 0, 100); break;     //向外平移
    //case 'e': case 'E': glTranslated(0, 0, -100); break;     //向里平移
    //case 'j': case 'J': glRotated(10, 0, 1, 0); break;      //绕y轴旋转10度
    //case 'l': case 'L': glRotated(10, 0, -1, 0); break;
    //case 'i': case 'I': glRotated(10, 1, 0, 0); break;      //绕x轴旋转10度
    //case 'k': case 'K': glRotated(10, -1, 0, 0); break;
    //case 'u': case 'U': glRotated(10, 0, 0, 1); break;      //绕z轴旋转10度
    //case 'o': case 'O': glRotated(10, 0, 0, -1); break;
    //case 'z': case 'Z': glScalef(1.5, 1.5, 1.5); break;         //放大
    //case 'x': case 'X': glScalef(0.5, 0.5, 0.5); break;         //缩小
    //default: break;
    //}

    if (key == 'z' || key == 'Z') {
        rotate_x += 10;
    }
    glutPostRedisplay();      //重新绘制
}
void InitOBJ() {
  //  models["cube"] = Model();
    loadOBJ("Models/cube115.obj", models["cube"].vertices, models["cube"].uvs, models["cube"].normals);
    loadOBJ("Models/treepi.obj", models["treepi"].vertices, models["treepi"].uvs, models["treepi"].normals);
    loadOBJ("Models/leaf.obj", models["leaf"].vertices, models["leaf"].uvs, models["leaf"].normals);
    loadOBJ("Models/car1.obj", models["car1"].vertices, models["car1"].uvs, models["car1"].normals);
    loadOBJ("Models/car2.obj", models["car2"].vertices, models["car2"].uvs, models["car2"].normals);
    loadOBJ("Models/car3.obj", models["car3"].vertices, models["car3"].uvs, models["car3"].normals);
    loadOBJ("Models/car4.obj", models["car4"].vertices, models["car4"].uvs, models["car4"].normals);
    loadOBJ("Models/muyi.obj", models["bench"].vertices, models["bench"].uvs, models["bench"].normals);
    loadOBJ("Models/grass.obj", models["grass"].vertices, models["grass"].uvs, models["grass"].normals);
    loadOBJ("Models/stand.obj", models["stand"].vertices, models["stand"].uvs, models["stand"].normals);
    loadOBJ("Models/faceStand.obj", models["faceStand"].vertices, models["faceStand"].uvs, models["faceStand"].normals);
    loadOBJ("Models/sileng.obj", models["sileng"].vertices, models["sileng"].uvs, models["sileng"].normals);
    loadOBJ("Models/roof.obj", models["roof"].vertices, models["roof"].uvs, models["roof"].normals);
   // loadOBJ("Models/treepi1.obj", models["treepi1"].vertices, models["treepi1"].uvs, models["treepi1"].normals);
}
void InitTex() {
    glEnable(GL_TEXTURE_2D);    // 启用纹理
    texs["ground"] = load_texture("BMP/ground1.bmp");  //加载纹理
    texs["wall"] = load_texture("BMP/wall1.bmp");
    texs["treepi"] = load_texture("BMP/treepi.bmp");
    texs["leaf"] = load_texture("BMP/leaf.bmp");
    texs["red"] = load_texture("BMP/redred.bmp");
    texs["yellow"] = load_texture("BMP/yellow.bmp");
    texs["window"] = load_texture("BMP/window.bmp");
    texs["zise"] = load_texture("BMP/zise.bmp");
    texs["bench"] = load_texture("BMP/mutou.bmp");
    texs["grassland"]= load_texture("BMP/caodi.bmp");
    texs["road"] = load_texture("BMP/road.bmp");
    texs["whiteline"] = load_texture("BMP/whiteLine.bmp");
    texs["yanshi"] = load_texture("BMP/yanshi.bmp");
    texs["dasa"] = load_texture("BMP/dasa.bmp");
    texs["kfc"] = load_texture("BMP/kfc.bmp");
    texs["zhongUp"] = load_texture("BMP/zhong_up.bmp");
    texs["hncj"] = load_texture("BMP/hncj.bmp");
    texs["zhongDown"] = load_texture("BMP/zhong_down.bmp");
    texs["zhongRed"] = load_texture("BMP/zhongRed.bmp");
    texs["black"] = load_texture("BMP/black.bmp");
    texs["roof"] = load_texture("BMP/roof.bmp");
    texs["sam"] = load_texture("BMP/sam.bmp");
}
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("3D大作业");
    DrawInit();
    LightSetting();
    InitTex();
    InitOBJ();
    //glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(specialkeys);
    glutDisplayFunc(Display);
    glutIdleFunc(spin);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}