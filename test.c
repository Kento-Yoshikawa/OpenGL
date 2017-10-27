//テスト
//床面,鳥居,竹の描画
//これを調整
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include <math.h>

GLfloat Ground[] = { 0.8, 0.3, 0.2, 1.0 };
GLfloat Bamboo[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat Tori[]={ 1.0, 0.2, 0.1, 1.0};

GLfloat light0pos[] = { 20.0, 0.0, 5.0, 1.0 };
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };


int r=0;//回転角
float view_x,view_y,view_z,rx,ry;//視点の座標、視線の角度
double width, height;

GLdouble normal[][3] = {
  { 0.0, 0.0,-1.0 },
  { 1.0, 0.0, 0.0 },
  { 0.0, 0.0, 1.0 },
  {-1.0, 0.0, 0.0 },
  { 0.0,-1.0, 0.0 },
  { 0.0, 1.0, 0.0 }
};

//度をラジアンに変換
double radians(double deg){
	return deg * M_PI / 180 ;
}

void cylinder(float radius,float height,int sides)
{
	int i=0;
	double pi = 3.1415;
	
       //上面
	glNormal3d(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for(i = 0; i < sides; i++) {
		double t = pi*2/sides * (double)i;
		glVertex3d(radius * cos(t), height, radius * sin(t));
	}
	glEnd();

        //側面
	glBegin(GL_QUAD_STRIP);
	for(i=0;i<=sides;i++){
		double t = i*2*pi/sides;
		glNormal3f((GLfloat)cos(t),0.0,(GLfloat)sin(t));
		glVertex3f((GLfloat)(radius*cos(t)),-height,(GLfloat)(radius*sin(t)));
		glVertex3f((GLfloat)(radius*cos(t)),height,(GLfloat)	(radius*sin(t)));
 }
	glEnd();
        //下面
	glNormal3d(0.0, -1.0, 0.0);
	glBegin(GL_POLYGON);
	for(i = sides; i >= 0; --i) {
	double t = pi*2/sides * (double)i;
	glVertex3d(radius * cos(t), -height, radius * sin(t));
       }
      glEnd();
}

void drawBamboo(int x,int z){

 glPushMatrix();
 	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,Bamboo);
	glTranslated(x,3,z);
 	cylinder(0.5,3,12);
 glPopMatrix();
}

void drawTori(int x,int z){

   glPushMatrix();
 	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,Tori);
	glTranslated(x,2,z);
 	cylinder(0.2,2,12);
 glPopMatrix();

 glPushMatrix();
 	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,Tori);
	glTranslated(x+3,2,z);
 	cylinder(0.2,2,12);
 glPopMatrix();

  glPushMatrix();
 	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,Tori);
	glTranslated(x+1.5,4,z);
	glRotated(90,0.0,0.0,1.0);
 	cylinder(0.2,2,12);
 glPopMatrix();

}

void display(){

  int x,z;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
  glLoadIdentity();
  
  gluLookAt(0.0, 5.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glRotated(rx,1,0,0); 
  glRotated(ry,0,1,0);
  glTranslated(-view_x,view_y,-view_z);


  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,Ground);

  glBegin(GL_POLYGON);
  	glNormal3f(0,1,0);
  	glVertex3d(-8.0,0, -100.0);
  	glVertex3d(8.0,0, -100.0);
  	glVertex3d(8.0,0, 100.0);
  	glVertex3d(-8.0,0, 100.0);
  glEnd();
  
  for(z=-100;z<=8;z++){
    	drawBamboo(-8,z);
   }
  for(z=-100;z<=8;z++){
	drawBamboo(8,z);
  }

   for(z=-100;z<=6;z++){
	drawTori(-1,z);
  }

   glutSwapBuffers();
}

void resize(int w, int h)
{ 
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
  
/* モデルビュー変換行列の設定 */
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
  gluLookAt(0.0, 5.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void idle(){

	glutPostRedisplay();
}

//視線の中心
int ry_center;
//ドラッグされているか
int draged;

//マウスがドラッグされたら呼ばれる
void mouseDraged(int x, int y) {
	draged = 1;

}
//マウスのx座標から、横方向の回転に変換する
double xRotatey(int x) {
	return (x - width / 2) / (float)width * 180 - ry_center;
}
//マウスのy座標から、縦方向の回転に変換する
double yRotatex(int y) {
	return (y - height / 2) / (float)height * 90;
}

//マウスが動いたら
void mouseMoved(int x, int y) {
	ry = xRotatey(x);
	rx = yRotatex(y);

}

void keyboard(unsigned char key, int x, int y)
{
  float step=1;
  //q,Qを押すと終了
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':  // '¥033' は ESC の ASCII コード 
    exit(0);
    break;

  case 'r':
  case 'R':
     view_y += step;
     break;

  case 'f':
  case 'F':
      view_y -= step;
	break;

  default:
    break;
  }

  glutPostRedisplay();
}


void specialKey(int key, int x, int y){
	float step=1;
	
       switch(key){
	 case GLUT_KEY_UP:
	 	 view_z -= step*cos(radians(ry));
        	 view_x += step*sin(radians(ry));
	  break;
	 case GLUT_KEY_DOWN:
 		 view_z += step*cos(radians(ry));
		 view_x -= step*sin(radians(ry));
	  break;
	 default:
	   break;
	} 

 glutPostRedisplay();
}

void init(){

	glClearColor(1.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

int main(int argc, char* argv[]){

	glutInitWindowPosition(700,200);
	glutInitWindowSize(640,480);
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("test");
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMoved);	
	glutMotionFunc(mouseDraged);
	glutIdleFunc(idle);
	glutSpecialFunc(specialKey);
	glutReshapeFunc(resize); 
	init();
	glutMainLoop();
	return 0;

}
