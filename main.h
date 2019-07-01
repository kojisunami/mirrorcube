#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

#include <vector>

#include "bmp.h"
#include "jpg.h"
#include "font.h"
#include "audio.h"
#include "xml.h"

#define WIDTH_SIZE  10
#define HEIGHT_SIZE 20

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000


#define GAME_TOP    1
#define GAME_INIT   2
#define GAME_START  3
#define GAME_END    4
#define GAME_LOAD   5 
#define GAME_RUN  100
#define GAME_MENU 101
#define GAME_TALK 102


using namespace std;

int game_mode = GAME_START;


float phi = M_PI*0.60, theta = -M_PI*0.75;
//int cor_u_off = 0;
//int cor_v_off = WINDOW_HEIGHT/2;
int cor_u_off = WINDOW_WIDTH/2;
int cor_v_off = WINDOW_HEIGHT/6;
int cube_size;


int score = 0;
int sys_time = 0;
int level = 1;
int num_match_total=0;

time_t t;



void keyboard( unsigned char key, int x, int y);
void keyboard_up( unsigned char key, int x, int y);
void special_key(int key ,int x , int y);
void special_key_up(int key ,int x , int y);

void display(void);
void idle();
void init();


void* gl_loop_thread(void* threadid);
void* time_watch_thread(void *threadid);
void* music_thread(void *threadid);




struct Point{
	int id;
	float x , y, z; // actual coordinate
	int u,v ; // correspond position in display

	int out_range_flag ;

	Point(float src_x, float src_y, float src_z){
		x = src_x;
		y = src_y;
		z = src_z;

		out_range_flag = 0;
	}
	Point(int src_id, float src_x, float src_y, float src_z){
		id = src_id;
		x = src_x;
		y = src_y;
		z = src_z;

		out_range_flag = 0;
	}
	Point(){
		x = 0;
		y = 0;
		z = 0;
		out_range_flag = 0;
	}

	void trans(float x_diff, float y_diff,float z_diff);
	void rot_xy( float phi);
	void rot_xz( float theta);

	void perspective();
};

class Line{
public:
	Point *pt0;
	Point *pt1;
	int id;
	int x ,y;

	Line(int src_id, Point *src_pt0, Point *src_pt1){
		id = src_id;
		pt0 = src_pt0;
		pt1 = src_pt1;
	}
	Line(Point *src_pt0, Point *src_pt1){
		pt0 = src_pt0;
		pt1 = src_pt1;
	}
	Line(){
	}

	void update();
	void draw();
};
	



#define CELL_SIZE_X 40 
#define CELL_SIZE_Y CELL_SIZE_X


void perspective_init();

class Square{
public:
	int id;
	int color;
	
	float d;
	Line *line0, *line1, *line2, *line3;

	Square( int src_id ,int src_color, Line *src_line0, Line *src_line1, Line *src_line2, Line *src_line3){
		id = src_id;
		color = src_color;
		line0 = src_line0;
		line1 = src_line1;
		line2 = src_line2;
		line3 = src_line3;
	}
	Square(){
	}
	void draw();
	float dist();

};

int square_draw_list_num = 0;
Square **square_draw_list;
//vector<Square> square_draw_list;


class Polygon{
public:

	vector<Point> pp;
	vector<Line> line;
	vector<Square> square;
	Xml_Tree *polygon_xml;

	Polygon(const char filename[]){
		load(filename);
	}
	Polygon(){
	}
	int load(const char filename[]);

	float dist();
	Point center();

	void draw();
	void draw(Point org_pt);

	void trans(float x , float y, float z);
	void rotate_theta(float theta, Point center_pt);
	void rotate_phi(float phi, Point center_pt);
	void rotate_psi(float psi, Point center_pt);
	
};

int num_cube = 0;

Xml_Tree *polygon_xml;


Point look_from;
Point look_at;

int proceed_flag = 0;
int rotation_flag_phi = 0;
int rotation_flag_theta = 0;
int lift_flag = 0;

float len;
float d;
float h;
float v=3; // real velocity is : v*1200km/h

vector<Polygon> poly;





#define time_gap( t1, t0) ( \
	(float)(t1.tv_sec - t0.tv_sec) + (float)(t1.tv_nsec - t0.tv_nsec)/1000000000 ) 

int rubik_chart[8];
int rubik_color_chart[24];
int rubik_color_test[24];

int solution_digit=0;
int solution_proposal[10];

void draw_rubik_solution();

void rubik_init();

Polygon poly_org[8];


float mx[2] = {1,2};
float my[2] = {1,2};
float mz[2] = {1,2};

float off_mx[2] = {0,1};
float off_my[2] = {0,1};
float off_mz[2] = {0,1};


