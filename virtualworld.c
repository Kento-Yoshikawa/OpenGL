//コンパイル方法 gcc -lglut -lGLU -o virtualworld virtualworld.c

//#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

GLfloat Scene[] = { (float)25 / 255,(float)25 / 255,(float)112 / 255,1.0 };//ウィンドウ全体の色 midnight blue
GLfloat Ground[] = { (float)165 / 255,(float)42 / 255, (float)42 / 255, 1.0 };//地面の色 brown
GLfloat Bamboo[] = { (float)34 / 255, (float)139 / 255, (float)34 / 255, 1.0 };//竹藪の色 forestgreen
GLfloat Tori[] = { 1.0, (float)69 / 255, 0.0, 1.0 };//鳥居の色 orangered
GLfloat BridgeColor[] = { (float)210 / 255, (float)105 / 255, (float)30 / 255, 1.0 };//橋の色 chocolate 
GLfloat RiverColor[] = { 0.0, (float)191 / 255, 1.0, 1.0 };//川の色　deepskyblue
GLfloat Light[] = { 1.0, (float)140 / 255, 0.0, 1.0 };//光源の色 darkorange
GLfloat white[] = { (float)245 / 255, 1.0,(float)250 / 255, 1.0 };

GLfloat light0pos[] = { 0.0,5.0, .0, 1.0 };
GLfloat light1pos[] = { 0.0, 15.0, -50.0, 1.0 };
GLfloat light2pos[] = { 0.0,5.0,-150.0,1.0 };

//テクスチャ関係
//川
#define TEXWIDTH1 256
#define TEXHEIGHT1 256
//橋
#define TEXWIDTH2 400
#define TEXHEIGHT2 400
//地面
#define TEXWIDTH3 256
#define TEXHEIGHT3 256

float view_x, view_y, view_z, rx, ry;//視点の座標、視線の角度
double width, height;

GLdouble normal[][3] = {
  { 0.0, 0.0,-1.0 },
  { 1.0, 0.0, 0.0 },
  { 0.0, 0.0, 1.0 },
  { -1.0, 0.0, 0.0 },
  { 0.0,-1.0, 0.0 },
  { 0.0, 1.0, 0.0 }
};

GLuint texid_1, texid_2,texid_3;
static const char texture1[] = "./river_256x256.raw";
static const char texture2[] = "./bridge_400x400.raw";
static const char texture3[] = "./ground_256x256.raw";

//テクスチャ初期化
void initTexture() {

  GLubyte texture_buf1[TEXHEIGHT1][TEXWIDTH1][4];
  GLubyte texture_buf2[TEXHEIGHT2][TEXWIDTH2][4];
  GLubyte texture_buf3[TEXHEIGHT3][TEXWIDTH3][4];

  FILE *fp;

  // テクスチャ画像(1枚目)の読み込み 
  if ((fp=fopen(texture1, "rb")) != NULL) {
    fread(texture_buf1, sizeof texture_buf1, 1, fp);
    fclose(fp);
  }
  else {
    perror(texture1);
  }
  /* テクスチャ画像(2枚目)の読み込み */
  if ((fp=fopen(texture2, "rb")) !=NULL ) {
    fread(texture_buf2, sizeof texture_buf2, 1, fp);
    fclose(fp);
  }
  else {
    perror(texture2);
  }

  /* テクスチャ画像(3枚目)の読み込み */
  if ((fp=fopen(texture3, "rb")) != NULL) {
  fread(texture_buf3, sizeof texture_buf3, 1, fp);
  fclose(fp);
  }
  else {
  perror(texture3);
  }

  ////////// 1枚目のテクスチャの読み込み ////////////
  glGenTextures(1, &texid_1);  // テクスチャIDを生成
  glBindTexture(GL_TEXTURE_2D, texid_1);  // 生成したテクスチャIDに切り替える

  /* テクスチャ画像はバイト単位に詰め込まれている */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  /* テクスチャの割り当て */
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TEXWIDTH1, TEXHEIGHT1,  // 縦横サイズはテクスチャ1用
		    GL_RGBA, GL_UNSIGNED_BYTE, texture_buf1);

  /* テクスチャを拡大・縮小する方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  /* テクスチャ環境 */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, 0);  // デフォルトにテクスチャIDに切り替える

  ////////// 2枚目のテクスチャの読み込み ////////////
  glGenTextures(1, &texid_2);  // テクスチャIDを生成
  glBindTexture(GL_TEXTURE_2D, texid_2);  // 生成したテクスチャIDに切り替える

  /* テクスチャ画像はバイト単位に詰め込まれている */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  /* テクスチャの割り当て */
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TEXWIDTH2, TEXHEIGHT2,  // 縦横サイズはテクスチャ1用
		    GL_RGBA, GL_UNSIGNED_BYTE, texture_buf2);

  /* テクスチャを拡大・縮小する方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  /* テクスチャ環境 */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, 0);  // デフォルトにテクスチャIDに切り替える

  /* テクスチャを拡大・縮小する方法の指定 */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  /* テクスチャ環境 */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, 0);  // デフォルトにテクスチャIDに切り替える

  ////////// 3枚目のテクスチャの読み込み ////////////
  glGenTextures(1, &texid_3);  // テクスチャIDを生成
  glBindTexture(GL_TEXTURE_2D, texid_3);  // 生成したテクスチャIDに切り替える

  /* テクスチャ画像はバイト単位に詰め込まれている */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  /* テクスチャの割り当て */
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TEXWIDTH3, TEXHEIGHT3,  // 縦横サイズはテクスチャ3用
  GL_RGBA, GL_UNSIGNED_BYTE, texture_buf3);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  /* テクスチャ環境 */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, 0);  // デフォルトにテクスチャIDに切り替える

  //アルファテストの比較関数
  glAlphaFunc(GL_GREATER, 0.5);
}


void texturedRiver() {
  static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };
  /* 材質の設定 */
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  /* アルファテスト開始 */
  glEnable(GL_ALPHA_TEST);
  /* テクスチャマッピング開始 */
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texid_1);  // テクスチャID=1を指定

  int i, j;
  glPushMatrix();
  glTranslated(0, 1.0, 0);
  /* １枚の４角形を描く */
  glNormal3d(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(-8.0, 0, -100.0);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(8.0, 0, -100.0);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(8.0, 0, 0.0);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-6.0, 0, 0.0);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(0, 0.0, -100.0);
  glNormal3d(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(-100.0, 0, -100.0);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(100.0, 0, -100.0);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(100.0, 0, 0.0);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-100.0, 0, 0.0);
  glEnd();
  glPopMatrix();

  /* テクスチャマッピング終了 */
  glDisable(GL_TEXTURE_2D);

  /* アルファテスト終了 */
  glDisable(GL_ALPHA_TEST);

}


void texturedBridge() {
  static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };
  // 材質の設定 
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  // アルファテスト開始 
  glEnable(GL_ALPHA_TEST);
  //テクスチャマッピング開始 
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, texid_2);  // テクスチャID=2を指定

  //橋の床面
  glPushMatrix();
  glTranslated(0, 6, 0);
  /* １枚の４角形を描く */
  glNormal3d(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(-4.5, 0, -200.0);
  glTexCoord2d(1.0, 1.0);
  glVertex3d(4.5, 0, -200.0);
  glTexCoord2d(1.0, 0.0);
  glVertex3d(4.5, 0, 100.0);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-4.5, 0, 100.0);
  glEnd();
  glPopMatrix();

  /* テクスチャマッピング終了 */
  glDisable(GL_TEXTURE_2D);

  /* アルファテスト終了 */
  glDisable(GL_ALPHA_TEST);
}


void texturedGround() {
static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };
// 材質の設定 
glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
// アルファテスト開始 
glEnable(GL_ALPHA_TEST);
//テクスチャマッピング開始 
glEnable(GL_TEXTURE_2D);

glBindTexture(GL_TEXTURE_2D, texid_3);  // テクスチャID=3を指定

glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2d(0.0, 1.0);
	glVertex3d(-100.0, 0, -100.0);
	glTexCoord2d(1.0, 1.0);	
	glVertex3d(100.0, 0, -100.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3d(100.0, 0, 100.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(-100.0, 0, 100.0);
	glEnd();
glPopMatrix();

   // テクスチャマッピング終了 
   glDisable(GL_TEXTURE_2D);

   // アルファテスト終了 
   glDisable(GL_ALPHA_TEST);
}


//度をラジアンに変換
double radians(double deg) {
  return deg * M_PI / 180;
}

//円柱の描画用関数
void cylinder(float radius, float height, int sides)
{
  int i = 0;
  //上面
  glNormal3d(0.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  for (i = 0; i < sides; i++) {
    double t = M_PI * 2 / sides * (double)i;
    glVertex3d(radius * cos(t), height, radius * sin(t));
  }
  glEnd();

  //側面
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= sides; i++) {
    double t = i * 2 * M_PI / sides;
    glNormal3f((GLfloat)cos(t), 0.0, (GLfloat)sin(t));
    glVertex3f((GLfloat)(radius*cos(t)), -height, (GLfloat)(radius*sin(t)));
    glVertex3f((GLfloat)(radius*cos(t)), height, (GLfloat)(radius*sin(t)));
  }
  glEnd();
  //下面
  glNormal3d(0.0, -1.0, 0.0);
  glBegin(GL_POLYGON);
  for (i = sides; i >= 0; --i) {
    double t = M_PI * 2 / sides * (double)i;
    glVertex3d(radius * cos(t), -height, radius * sin(t));
  }
  glEnd();
}

void drawBamboo(int x, int z) {

  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Bamboo);
  glTranslated(x, 20, z);
  cylinder(1.5, 20, 12);
  glPopMatrix();
}

void drawTori(int x, int z) {

  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Tori);
  glTranslated(x - 1, 9.5, z);
  cylinder(0.5, 3.5, 12);
  glPopMatrix();

  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Tori);
  glTranslated(x + 3, 9.5, z);
  cylinder(0.5, 3.5, 12);
  glPopMatrix();

  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Tori);
  glTranslated(x + 1, 12.7, z);
  glRotated(90, 0.0, 0.0, 1.0);
  cylinder(0.5, 3.5, 12);
  glPopMatrix();

}

void drawBridge(int x, int z) {
  //橋の左の支柱
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, BridgeColor);
  glTranslated(x - 1.4, 8, z);
  cylinder(0.2, 2, 12);
  glPopMatrix();

  //橋の右の支柱
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, BridgeColor);
  glTranslated(x + 7.4, 8, z);
  cylinder(0.2, 2, 12);
  glPopMatrix();

  //橋の左の手すり
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, BridgeColor);
  glTranslated(x - 1.5, 10, z);
  glRotated(90.0, 1.0, 0.0, 0.0);
  cylinder(0.2, 2.5, 12);
  glPopMatrix();

  //橋の右の手すり
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, BridgeColor);
  glTranslated(x + 7.5, 10, z);
  glRotated(90.0, 1.0, 0.0, 0.0);
  cylinder(0.2, 2.5, 12);
  glPopMatrix();

}

void drawLight(int x, int z) {

  //左の光源
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Light);
  glTranslated(x - 1.0, 14, z);
  glutSolidSphere(0.5, 16, 16);
  glPopMatrix();
  //右の光源
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Light);
  glTranslated(x + 6.5, 14, z);
  glutSolidSphere(0.5, 16, 16);
  glPopMatrix();

}

void display() {

  int x, z;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  gluLookAt(0.0, 8.0, 50.0, 0.0, 8.0, -200.0, 0.0, 1.0, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
  glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
  glLightfv(GL_LIGHT2, GL_POSITION, light2pos);
  glRotated(rx, 1, 0, 0);
  glRotated(ry, 0, 1, 0);
  glTranslated(-view_x, view_y, -view_z);

   //テクスチャマッピングを行った地面の描画
  texturedGround();

  for (x = -100; x <= -8; x += 4) {
    for (z = -100; z <= 0; z += 4) {
      drawBamboo(x, z);
    }
  }
  for (x = 8; x <= 100; x += 4) {
    for (z = -100; z <= 0; z += 4) {
      drawBamboo(x, z);
    }
  }
  for (z = -100; z <= 0; z += 5) {
    drawTori(-1, z);
  }
  //テクスチャマッピングを行った橋の描画
  texturedBridge();
  //橋の手すりのなどの描画
  for (z = -200; z <= 0; z += 5) {
    drawBridge(-3, z);
  }

  //テクスチャマッピングを行った川の描画
  texturedRiver();
  
  for (z = -200; z <= 0; z += 5) {
    drawLight(-3.5, z);
  }
  glutSwapBuffers();

}

void resize(int w, int h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 200.0);

  /* モデルビュー変換行列の設定 */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 8.0, 30.0, 0.0, 8.0, -200.0, 0.0, 1.0, 0.0);
}

void idle() {
  int r = 0;//回転角
  if (++r >= 360) r = 0;
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
  return (x - width / 2) / (float)width * 360 - ry_center;
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
  float step = 1;
  //q,Qを押すと終了
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':  // '\033' は ESC の ASCII コード 
    exit(0);
    break;

    //d,Dキーで下へ下降
  case 'd':
  case 'D':
    view_y += step;
    break;
    //u,Uキーで上へ上昇
  case 'u':
  case 'U':
    view_y -= step;
    break;

  default:
    break;
  }

  glutPostRedisplay();
}


void specialKey(int key, int x, int y) {
  float step = 1;

  switch (key) {
  case GLUT_KEY_UP://上カーソルキーで前へ進む
    view_z -= step*cos(radians(ry));
    view_x += step*sin(radians(ry));
    break;
  case GLUT_KEY_DOWN://下カーソルキーで後ろへ進む
    view_z += step*cos(radians(ry));
    view_x -= step*sin(radians(ry));
    break;

  case GLUT_KEY_LEFT://左カーソルキーで左へ進む
    view_z -= step*sin(radians(ry));
    view_x -= step*cos(radians(ry));
    break;
  case GLUT_KEY_RIGHT://右カーソルキーで右へ進む
    view_z += step*sin(radians(ry));
    view_x += step*cos(radians(ry));
    break;
  default:
    break;
  }

  glutPostRedisplay();
}

void init() {

  glClearColor(Scene[0], Scene[1], Scene[2], Scene[3]);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, Light);
  glLightfv(GL_LIGHT1, GL_SPECULAR, Light);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, Light);
  glLightfv(GL_LIGHT2, GL_SPECULAR, Light);

  initTexture();
}

int main(int argc, char* argv[]) {

  glutInitWindowPosition(700, 200);
  glutInitWindowSize(640, 480);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Example of Execution");

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  //うまく上を見上げることが今のところできないのでコメントアウト
  //glutPassiveMotionFunc(mouseMoved);
  //glutMotionFunc(mouseDraged);
  glutIdleFunc(idle);
  glutSpecialFunc(specialKey);
  glutReshapeFunc(resize);
  init();
  glutMainLoop();
  return 0;

}
