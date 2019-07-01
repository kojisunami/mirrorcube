#include "main.h"


void Point::trans(float x_diff, float y_diff,float z_diff){
	x += x_diff;
	y += y_diff;
	z += z_diff;
}

void Point::rot_xy( float phi){
	float tmp0 = x * cos(phi) - y * sin(phi);
	float tmp1 = x * sin(phi) + y * cos(phi);
	x = tmp0;
	y = tmp1;
}

void Point::rot_xz( float theta){
	float tmp0 = x * cos(theta) - z * sin(theta);
	float tmp1 = z * sin(theta) + z * cos(theta);
	x = tmp0;
	z = tmp1;
}

void Point::perspective(){
		// we add the perspective
		// we measure the distance.
		float p,q,r;
		p = (look_at.x -look_from.x)/
			sqrt( pow(look_at.x-look_from.x,2)+ pow(look_at.y-look_from.y,2)) ;
		q = (look_at.y -look_from.y)/
			sqrt( pow(look_at.x-look_from.x,2)+ pow(look_at.y-look_from.y,2)) ;
		r = 0;
	
		float tmp;
		tmp = p;
		p = q;
		q =  -tmp;

		float s;
		float a, b,c;
		s =(p * (look_from.x-look_at.x) + 
			q * (look_from.y-look_at.y) +
			r * (look_from.z-look_at.z))/ 
				( pow(look_from.x-look_at.x,2) + 
					pow(look_from.y-look_at.y,2) +
					pow(look_from.z-look_at.z,2));
		a = p - s * (look_from.x-look_at.x);
		b = q - s * (look_from.y-look_at.y);
		c = r - s * (look_from.z-look_at.z);
	
		float t;
		float d,e,f;
		t =((x-look_at.x) * (look_from.x-look_at.x) + 
			(y-look_at.y) * (look_from.y-look_at.y) +
			(z-look_at.z) * (look_from.z-look_at.z))/ 
				( pow(look_from.x-look_at.x,2) + 
					pow(look_from.y-look_at.y,2) +
					pow(look_from.z-look_at.z,2));
		d = (x-look_at.x) - t * (look_from.x-look_at.x);
		e = (y-look_at.y) - t * (look_from.y-look_at.y);
		f = (z-look_at.z) - t * (look_from.z-look_at.z);
		float dist;
		dist = sqrt(( pow(d,2) + pow(e,2) + pow(f,2)));
	
		float cos_arg ; // cos
		cos_arg = (a * d + b *e + c *f)/
			(sqrt(pow(a,2)+pow(b,2)+pow(c,2))*sqrt(pow(d,2)+pow(e,2)+pow(f,2)));


	// calculation of sine is a bit complicated
	// we need to make sure if it is positive or not.
	// calculation of X origin(perpendicular to Y and perspective vector(at-from))
		float g,h,j;
		float tmp_arg;
		g = c*(look_from.y-look_at.y)- b*(look_from.z-look_at.z);
		h = a*(look_from.z-look_at.z)- c*(look_from.x-look_at.x);
		j = b*(look_from.x-look_at.x)- a*(look_from.y-look_at.y);

		tmp_arg = (g * d + h *e + j *f)/
			(sqrt(pow(g,2)+pow(h,2)+pow(j,2))*sqrt(pow(d,2)+pow(e,2)+pow(f,2)));

		float sin_arg;
		if( tmp_arg > 0)
			sin_arg = sqrt(1- pow(cos_arg,2));	
		else
			sin_arg = - sqrt(1- pow(cos_arg,2));	


		if( ((look_from.x - d)/(look_from.x-look_at.x) < 0)||
			((look_from.y - e)/(look_from.y-look_at.y) < 0)||
			((look_from.z - f)/(look_from.z-look_at.z) < 0)){
			// this is outrange
			out_range_flag = 1;
		}else{
			out_range_flag = 0;
		}

		float l;
		l = sqrt(pow(look_from.x-x,2)+ pow(look_from.y-y,2)+ pow(look_from.z-z,2));
		dist = 40 * (dist / sqrt(pow(l,2)-pow(dist,2)));
		int cube_size = 40;

		u = (int)(dist * cos_arg * cube_size) + cor_u_off;// + cor_x * cube_size;
		v = (int)(dist * sin_arg * cube_size) + cor_v_off;// + cor_x * cube_size;
}



void Line::update(){
	pt0->perspective();
	pt1->perspective();
}

void Line::draw(){

	update();

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex2i(pt0->u, pt0->v);
	glVertex2i(pt1->u, pt1->v);
	glEnd();
}

float Square::dist(){
	d=0;
	d += sqrt(pow(line0->pt0->x-look_from.x,2) +  pow(line0->pt0->y-look_from.y,2) + 
		pow(line0->pt0->z-look_from.z,2));
	d += sqrt(pow(line0->pt1->x-look_from.x,2) +  pow(line0->pt1->y-look_from.y,2) +
		pow(line0->pt0->z-look_from.z,2));
	d += sqrt(pow(line1->pt0->x-look_from.x,2) +  pow(line1->pt0->y-look_from.y,2) +
		pow(line0->pt0->z-look_from.z,2));
	d += sqrt(pow(line1->pt1->x-look_from.x,2) +  pow(line1->pt1->y-look_from.y,2) +
		pow(line0->pt0->z-look_from.z,2));
	d += sqrt(pow(line2->pt0->x-look_from.x,2) +  pow(line2->pt0->y-look_from.y,2) +
		pow(line0->pt0->z-look_from.z,2));
	d += sqrt(pow(line2->pt1->x-look_from.x,2) +  pow(line2->pt1->y-look_from.y,2) +
		pow(line0->pt0->z-look_from.z,2));
	d += sqrt(pow(line3->pt0->x-look_from.x,2) +  pow(line3->pt0->y-look_from.y,2) +
		pow(line0->pt0->z-look_from.z,2));
	d += sqrt(pow(line3->pt1->x-look_from.x,2) +  pow(line3->pt1->y-look_from.y,2) +
		pow(line0->pt0->z-look_from.z,2));
}



void Square::draw(){

	//square_draw_list[square_draw_list_num] = this;
	//square_draw_list_num++;
	switch(color){
		case 0:
			glColor3d( 1.0, 0.0, 0.0); break;
		case 1:
			glColor3d( 0.0, 0.0, 1.0); break;
		case 2:
			glColor3d( 1.0, 1.0, 0.0); break;
		case 3:
			glColor3d( 1.0, 1.0, 1.0); break;
		case 4:
			glColor3d( 0.0, 1.0, 0.0); break;
		case 5:
			glColor3d( 1.0, 0.5, 0.0); break;
	}
	line0->update();
	line1->update();
	line2->update();
	line3->update();

	glBegin(GL_POLYGON);
	glVertex2i(line0->pt1->u, line0->pt1->v);

	if( ( line1->pt0 == line0->pt0) || ( line1->pt0 == line0->pt1) ){
		glVertex2i(line1->pt0->u, line1->pt0->v);
		glVertex2i(line1->pt1->u, line1->pt1->v);
	}
	else{
		glVertex2i(line1->pt1->u, line1->pt1->v);
		glVertex2i(line1->pt0->u, line1->pt0->v);
	}

	if( ( line2->pt0 == line1->pt0) || ( line2->pt0 == line1->pt1) ){
		glVertex2i(line2->pt0->u, line2->pt0->v);
		glVertex2i(line2->pt1->u, line2->pt1->v);
	}else{
		glVertex2i(line2->pt1->u, line2->pt1->v);
		glVertex2i(line2->pt0->u, line2->pt0->v);
		}
	
	if( ( line3->pt0 == line2->pt0) || ( line3->pt0 == line2->pt1) ){
		glVertex2i(line3->pt0->u, line3->pt0->v);
		glVertex2i(line3->pt1->u, line3->pt1->v);
	}else{
		glVertex2i(line3->pt1->u, line3->pt1->v);
		glVertex2i(line3->pt0->u, line3->pt0->v);
		}
	glEnd();

	glColor3d( 0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(line0->pt1->u, line0->pt1->v);

	if( ( line1->pt0 == line0->pt0) || ( line1->pt0 == line0->pt1) ){
		glVertex2i(line1->pt0->u, line1->pt0->v);
		glVertex2i(line1->pt1->u, line1->pt1->v);
	}
	else{
		glVertex2i(line1->pt1->u, line1->pt1->v);
		glVertex2i(line1->pt0->u, line1->pt0->v);
	}

	if( ( line2->pt0 == line1->pt0) || ( line2->pt0 == line1->pt1) ){
		glVertex2i(line2->pt0->u, line2->pt0->v);
		glVertex2i(line2->pt1->u, line2->pt1->v);
	}else{
		glVertex2i(line2->pt1->u, line2->pt1->v);
		glVertex2i(line2->pt0->u, line2->pt0->v);
		}
	
	if( ( line3->pt0 == line2->pt0) || ( line3->pt0 == line2->pt1) ){
		glVertex2i(line3->pt0->u, line3->pt0->v);
		glVertex2i(line3->pt1->u, line3->pt1->v);
	}else{
		glVertex2i(line3->pt1->u, line3->pt1->v);
		glVertex2i(line3->pt0->u, line3->pt0->v);
		}

	glEnd();
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT);

	struct timespec newtime, fps_oldtime;


// showing fps
	static int count =0;
	static int fps;

	if(count == 0)
		clock_gettime(CLOCK_REALTIME, &fps_oldtime);
	clock_gettime(CLOCK_REALTIME, &newtime);

	if( time_gap(newtime, fps_oldtime) > 1){
		fps = count;
		count =0;
		fps_oldtime = newtime;
	}
	count ++;

	char str[64];
	int len;

	static int start_flag = 0;
	struct  timespec start_oldtime;


	len = sprintf(str, "Copyright(C) 2019 Koji Sunami");
	put_str(str, len, 1, 230, 40);

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex2i(700,0);
	glVertex2i(700,1000);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i( cor_u_off-30, cor_v_off+30);
	glVertex2i( cor_u_off+30, cor_v_off-30);
	glVertex2i( cor_u_off-30, cor_v_off-30);
	glVertex2i( cor_u_off+30, cor_v_off+30);
	glEnd();

	len = sprintf(str, "%dfps", fps);
	put_str(str, len, 2, 740, 200);

	len = sprintf(str, "Score: %d\nTime: %d", score, sys_time);
	put_str(str, len, 2, 740, 300);

	len = sprintf(str, "x: %f\ny: %f\nz:%f", look_from.x, look_from.y, look_from.z);
	put_str(str, len, 2, 740, 600);

/*
	len = sprintf(str, "x: %f\ny: %f\nz:%f", look_at.x, look_at.y, look_at.z);
	put_str(str, len, 2, 740, 300);
	*/

	put_str(str, len, 2, 740, 800);

	len = sprintf(str, "alt %dm", (int)round(look_from.z*1000/3));
	put_str(str, len, 2, 740, 700);


// square drawing

	for(int i =0;i<poly.size();i++){
		for(int j =0;j<poly[i].square.size();j++){
			square_draw_list[square_draw_list_num++] = &poly[i].square[j];
	}}

	for(int i =0;i < square_draw_list_num;i++)
		square_draw_list[i]->dist();


	Square *tmp;
	for(int j = 0;j<square_draw_list_num;j++){
		for(int i =0;i < square_draw_list_num-1;i++){
			if ( square_draw_list[i]->d < square_draw_list[i+1]->d ){
				tmp = square_draw_list[i];
				square_draw_list[i] =  square_draw_list[i+1];
				square_draw_list[i+1] = tmp;
			}
		}
	}

	for(int i =0;i < square_draw_list_num;i++)
		square_draw_list[i]->draw();

	square_draw_list_num = 0;

	if(game_mode == GAME_LOAD){
		len = sprintf(str, "caculating...");
		put_str(str, len, 5, 300, 200);
	}else{
		draw_rubik_solution();
	}

	glutSwapBuffers();
	glFlush();
}


void perspective_init(){
	look_from.x = -8.0;
	look_from.y = -10.0;
	look_from.z = 10.0;

	look_at.x = -.1;
	look_at.y = -.1;
	look_at.z = 0.1;
}


int Polygon::load(const char filename[]){
	polygon_xml = new Xml_Tree(filename);

	int pt_count=0;
	for(int j =0;j<polygon_xml->children.size();j++){
		// get vertecies
		if(str_match(polygon_xml->children[j].xml_list[0].arg[0], (char*)"vertex", 6) == 1){

			int id = atoi(polygon_xml->get_data_by_id((char*)"vertex", pt_count, (char*)"id=" ) );
			if(id != -1){
				int x = 
					atoi(polygon_xml->get_data_by_id( (char*)"vertex", pt_count, (char*)"x=") );
				int y = 
					atoi(polygon_xml->get_data_by_id( (char*)"vertex", pt_count, (char*)"y=") );
				int z = 
				atoi(polygon_xml->get_data_by_id( (char*)"vertex", pt_count, (char*)"z=") );

				pp.push_back(Point(id, x,y,z)); // actually float
				pt_count++;
			}
		}
	}

	int line_count=0;
	for(int j =0;j<polygon_xml->children.size();j++){
		// get lines
		if(str_match(polygon_xml->children[j].xml_list[0].arg[0], (char*)"line", 4) == 1){
			int id = atoi(polygon_xml->get_data_by_id((char*)"line", line_count, (char*)"id=" ) );
			if(id != -1){
				int x = 
					atoi(polygon_xml->get_data_by_id( (char*)"line", line_count, (char*)"x=") );
				int y = 
					atoi(polygon_xml->get_data_by_id( (char*)"line", line_count, (char*)"y=") );
				line.push_back( Line( id,  &pp[x], &pp[y]));
				line_count++;
			}
		}
	}
	int square_count=0;
	for(int j =0;j<polygon_xml->children.size();j++){
		// get lines
		if(str_match(polygon_xml->children[j].xml_list[0].arg[0], (char*)"square", 4) == 1){
			int id = atoi(polygon_xml->get_data_by_id((char*)"square", square_count, (char*)"id=" ) );
			if(id != -1){
				int x = 
					atoi(polygon_xml->get_data_by_id( (char*)"square", square_count, (char*)"x=") );
				int y = 
					atoi(polygon_xml->get_data_by_id( (char*)"square", square_count, (char*)"y=") );
				int z = 
					atoi(polygon_xml->get_data_by_id( (char*)"square", square_count, (char*)"z=") );
				int w = 
					atoi(polygon_xml->get_data_by_id( (char*)"square", square_count, (char*)"w=") );
				int color = 
					atoi(polygon_xml->get_data_by_id( (char*)"square", square_count, (char*)"color=") );
				square.push_back( Square( id,  color, &line[x], &line[y], &line[z], &line[w]));
				square_count++;
			}
		}
	}
	return 0;
}


float Polygon::dist(){
	float sum=0;
	for(int i =0;i<pp.size();i++)
		sum +=  pow(pp[i].x-look_from.x,2) + pow(pp[i].y-look_from.y,2) + pow(pp[i].z-look_from.z,2);
		//sum += sqrt( pow(pp[i].x-look_from.x,2) + 
			//pow(pp[i].y-look_from.y,2) + pow(pp[i].z-look_from.z,2));

	// warning: it returns square of its value to reduce the amount of calculation.
	return (sum / pp.size());
}

Point Polygon::center(){
	Point pt;
	float sum_x = 0;
	float sum_y = 0;
	float sum_z = 0;
	for(int i =0;i<pp.size();i++){
		sum_x += pp[i].x;
		sum_y += pp[i].y;
		sum_z += pp[i].z;
	}
	pt.x = sum_x/pp.size();
	pt.y = sum_y/pp.size();
	pt.z = sum_z/pp.size();
	return pt;
}

void Polygon::trans( float x ,float y, float z){
	for(int i =0;i<pp.size();i++){
		pp[i].x += x;	
		pp[i].y += y;	
		pp[i].z += z;
	}
}


static int rotate_theta_flag = 0;
static int rotate_phi_flag = 0;
static int rotate_psi_flag = 0;

void Polygon::rotate_theta(float theta, Point center_pt){

	for(int i =0;i<pp.size();i++){
		float x = (pp[i].x - center_pt.x);
		float y = (pp[i].y - center_pt.y);
		float l = sqrt( pow( x, 2) + pow( y, 2));
		if( l == 0)
			continue;
		float th = acos(x/l);
		if( asin(y/l) < 0)
			th = -th;	
		pp[i].x = l * cos( th + theta) + center_pt.x;
		pp[i].y = l * sin( th + theta) + center_pt.y;
	}
}

void Polygon::rotate_phi(float phi, Point center_pt){
	for(int i =0;i<pp.size();i++){
		float x = (pp[i].x - center_pt.x);
		float z = (pp[i].z - center_pt.z);
		float l = sqrt( pow( x, 2) + pow( z, 2));
		if( l == 0)
			continue;
		float th = acos(x/l);
		if( asin(z/l) < 0)
			th = -th;	
		pp[i].x = l * cos( th + phi) + center_pt.x;
		pp[i].z = l * sin( th + phi) + center_pt.z;
	}
}

void Polygon::rotate_psi(float psi, Point center_pt){
	for(int i =0;i<pp.size();i++){
		float y = (pp[i].y - center_pt.y);
		float z = (pp[i].z - center_pt.z);
		float l = sqrt( pow( y, 2) + pow( z, 2));
		if( l == 0)
			continue;
		float th = acos(y/l);
		if( asin(z/l) < 0)
			th = -th;	
		pp[i].y = l * cos( th + psi) + center_pt.y;
		pp[i].z = l * sin( th + psi) + center_pt.z;
	}
}

void Polygon::draw( Point org_pt){
	
	Point pt = center();
	char buf[64];
	int len ;
	float d = dist();
	len = sprintf(buf, "%fkm\n",d/3);
	put_str(buf, len, 1, pt.u, pt.v);

	
	for(int i =0;i<pp.size();i++)
		pp[i].trans(org_pt.x, org_pt.y, org_pt.z);

	for(int i =0;i<line.size();i++){
		line[i].draw();
	}
	for(int i =0;i<square.size();i++){
		square[i].draw();
	}
	for(int i =0;i<pp.size();i++)
		pp[i].trans(-org_pt.x, -org_pt.y, -org_pt.z);
}


void Polygon::draw(){
	
	Point pt = center();
	pt.perspective();
	char buf[64];
	int len ;
	float d = dist();
	len = sprintf(buf, "%fkm\n",d/3);
	put_str(buf, len, 1, pt.u, pt.v);

	for(int i =0;i<line.size();i++){
		line[i].draw();
	}
	for(int i =0;i<square.size();i++)
		square[i].draw();
}


void rubik_init(){
	for(int i =0;i<8;i++)
		rubik_chart[i] = i;
	
	for(int i =0;i<24;i++)
		rubik_color_chart[i] = i;

}

void init(){

	square_draw_list = (Square **) malloc(sizeof(Square ) * 100);
	perspective_init();

	
	for(int i =0;i<8;i++){
		poly.push_back(Polygon("resources/polygon.xml"));
		for(int j =0;j<poly[i].pp.size();j++){
			poly[i].pp[j] = Point( 
				poly[i].pp[j].x*mx[i%2],
				poly[i].pp[j].y*my[(i/2)%2],
				poly[i].pp[j].z*mz[i/4]);
		}
		poly[i].trans( off_mx[i%2], off_my[((i/2)%2)], off_mz[(i/4)]);
	}

	

	printf("x y z is look_From %f %f %f\n", look_from.x, look_from.y, look_from.z);

	rubik_init();

	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
}



void* gl_loop_thread(void* threadid){
	glutMainLoop();
	return NULL;
}



void trans(int a, int b, int c, int d, int *arr){	
	// "abcd" will be "bcda"
	int tmp  = arr[a];
	arr[a] = arr[b];
	arr[b] = arr[c];
	arr[c] = arr[d];
	arr[d] = tmp;
}


void rubik_rottheta(float t){
	Point origin_pt( 1.0, 1.0, 1.0);
	poly[rubik_chart[0]].rotate_theta(t, origin_pt);
	poly[rubik_chart[1]].rotate_theta(t, origin_pt);
	poly[rubik_chart[2]].rotate_theta(t, origin_pt);
	poly[rubik_chart[3]].rotate_theta(t, origin_pt);
	int tmp = rubik_chart[0];
	rubik_chart[0] = rubik_chart[2];
	rubik_chart[2] = rubik_chart[3];
	rubik_chart[3] = rubik_chart[1];
	rubik_chart[1] = tmp;

	trans(12,14,16,18, rubik_color_chart);
	trans(13,15,17,19, rubik_color_chart);
	trans(20,21,22,23, rubik_color_chart);
	
}

void rubik_rotphi(float t){
	Point origin_pt( 1.0, 1.0, 1.0);
	poly[rubik_chart[0]].rotate_phi(t, origin_pt);
	poly[rubik_chart[1]].rotate_phi(t, origin_pt);
	poly[rubik_chart[4]].rotate_phi(t, origin_pt);
	poly[rubik_chart[5]].rotate_phi(t, origin_pt);
	int tmp = rubik_chart[0];
	rubik_chart[0] = rubik_chart[4];
	rubik_chart[4] = rubik_chart[5];
	rubik_chart[5] = rubik_chart[1];
	rubik_chart[1] = tmp;

	trans(15, 3,10,23, rubik_color_chart);
	trans( 7, 1,18,21, rubik_color_chart);
	trans( 8, 9,17,16, rubik_color_chart);
}

void rubik_rotpsi(float t){
	Point origin_pt( 1.0, 1.0, 1.0);
	poly[rubik_chart[0]].rotate_psi(t, origin_pt);
	poly[rubik_chart[2]].rotate_psi(t, origin_pt);
	poly[rubik_chart[6]].rotate_psi(t, origin_pt);
	poly[rubik_chart[4]].rotate_psi(t, origin_pt);
	int tmp = rubik_chart[0];
	rubik_chart[0] = rubik_chart[4];
	rubik_chart[4] = rubik_chart[6];
	rubik_chart[6] = rubik_chart[2];
	rubik_chart[2] = tmp;

	trans( 5, 3,16,20, rubik_color_chart);
	trans(13, 2, 8,21, rubik_color_chart);
	trans( 6, 7,14,15, rubik_color_chart);
}



void rubik_get_solution(){

	solution_digit = 0;
	long c =0;

	while(1){
		// create solution table 
		for(int i =0;i<solution_digit;i++)
			solution_proposal[i] = ( c /(long)pow(9,i))%9;
		c ++;

		for(int i =0;i<24;i++)
			rubik_color_test[i] = rubik_color_chart[i];
	
		// calculate the permutation of rubiks cube following the solution proposal
		for(int j =0;j<solution_digit;j++){
			switch(solution_proposal[j]%3){
			case 0:{
				for(int i =0;i<=solution_proposal[j]/3;i++){
					trans(12,14,16,18, rubik_color_test);
					trans(13,15,17,19, rubik_color_test);
					trans(20,21,22,23, rubik_color_test);
				}
				break;
			}
			case 1:{
				for(int i =0;i<=solution_proposal[j]/3;i++){
					trans(15, 3,10,23, rubik_color_test);
					trans( 7, 1,18,21, rubik_color_test);
					trans( 8, 9,17,16, rubik_color_test);
				}
				break;
			}
			case 2:{
				for(int i =0;i<=solution_proposal[j]/3;i++){
					trans( 5, 3,16,20, rubik_color_test);
					trans(13, 2, 8,21, rubik_color_test);
					trans( 6, 7,14,15, rubik_color_test);
				}
				break;
			}
			}
		}
	
		// if given transformation is the solution or not.
		for(int i = 0;i<24;i++){
			if( rubik_color_test[i] != i){
				break;
			}
			if(i == 24-1){
				game_mode = GAME_RUN;
				return ;
			}
		}
	
		// check if there is any keyboard interrupt
		if( game_mode == GAME_RUN){
			solution_digit = 0;
			return ;
		}

		// check digits
		if(solution_digit == 0){
			solution_digit++;
			continue;
		}
		for(int i =0;i<solution_digit;i++){
			if(solution_proposal[i] != 9-1 )
				break;
			if (i == solution_digit - 1){
				if( solution_digit < 10){
					solution_digit++;
					printf("entering solution with %d digits...\n", solution_digit );
					continue;
				}
				else{
					printf("error, no solution\n");
					game_mode = GAME_RUN;
					return ;
				}
			}
		}
	}
}

void draw_rubik_solution(){
	int len = 0;
	char str[64];

	for(int i =0;i<64;i++)
		str[i] = 0;
	
	for(int i =0;i<solution_digit;i++){
		switch(solution_proposal[i]%3){
			case 0:
				len = sprintf(str, "%s a%d ", str, solution_proposal[i]/3+1); break;
			case 1:
				len = sprintf(str, "%s s%d ", str, solution_proposal[i]/3+1); break;
			case 2:
				len = sprintf(str, "%s d%d ", str, solution_proposal[i]/3+1); break;
		}
	}
	put_str( str,  len , 2, 100,100);
}



void *rubik_calculation_thread(void *threadid){
	while(1){
		if(game_mode == GAME_LOAD){
			rubik_get_solution();
			game_mode = GAME_RUN;
		}
	}
	return NULL;
}


void* length_calc_thread (void *threadid){
	while(1){
		d = sqrt(pow(look_at.x - look_from.x, 2) + pow(look_at.y - look_from.y ,2));
	}
	return NULL;
}

void* time_watch_thread(void *threadid){
	struct timespec oldtime, newtime, middletime;

	while(1){ 

		clock_gettime(CLOCK_REALTIME, &newtime);
		
		static int first_time_flag = 0;
		if ( first_time_flag == 0){
			clock_gettime(CLOCK_REALTIME, &middletime);	
			clock_gettime(CLOCK_REALTIME, &oldtime);	
			first_time_flag ++;
		}
	
		float t = time_gap(newtime,middletime);
		middletime = newtime;
	
	
		float x = look_at.x - look_from.x;
		float y = look_at.y - look_from.y;
		float diff = (1/d) * t * v;
		float dx = x * diff;
		float dy = y * diff;
	
		if( lift_flag == 1)
			look_from.z += t;
		else if( lift_flag == -1)
			look_from.z -= t;
	
		if( proceed_flag == 1 ){
			look_from.x += dx;
			look_from.y += dy;
			look_at.x += dx;
			look_at.y += dy;
		}

		if(proceed_flag ==1)
			//v+=t*0.3;
			v = 3;
		else
			 v= 0;
	
		if( rotation_flag_theta == 2){
			look_at.x += t * y;
			look_at.y -= t * x;
		}
		else if( rotation_flag_theta == -2){
			look_at.x -= t * y;
			look_at.y += t * x;
		}
		else if( rotation_flag_theta == 1){
			look_at.x += t * y/32;
			look_at.y -= t * x/32;
		}
		else if( rotation_flag_theta == -1){
			look_at.x -= t * y/32;
			look_at.y += t * x/32;
		}

		if( rotation_flag_phi == 1){
			look_at.x += x * t;
			look_at.y += y * t;
		}
		else if( rotation_flag_phi == -1){
			look_at.x -= x * t;
			look_at.y -= y * t;
		}

		usleep(200);
	}

	return NULL;
}

void* music_thread(void *threadid){
	while(1){
		//music_play("resources/tetris.wav");
		sleep(1);
	}
	return NULL;
}


void keyboard( unsigned char key, int x, int y){

	switch(key){
		case 27:	
			exit(0);
		case 'a':
			rubik_rottheta(M_PI/2); break;
			//if((rotate_phi_flag == 1) || (rotate_psi_flag == 1)) break;
			///rotate_theta_flag = 1; break;
		case 's':
			rubik_rotphi(M_PI/2); break;
			//if((rotate_theta_flag == 1) || (rotate_psi_flag == 1)) break;
			//rotate_phi_flag = 1; break;
		case 'd':
			rubik_rotpsi(M_PI/2); break;
			//if((rotate_theta_flag == 1) || (rotate_phi_flag == 1)) break;
			//rotate_psi_flag = 1; break;
		case 'r':{
			lift_flag = -1; break;
		}
		case 't':{
			lift_flag = 1; break;
		}
		case 'f':{
			proceed_flag = 1; break;
		case ' ':{
			break;
		}
		case 'j':{
			rotation_flag_theta = -1; break;
		}
		case 'l':{
			rotation_flag_theta = 1; break;
		}

		case 'z':{
			game_mode = GAME_LOAD; break;
		}
		case 'x':{
			game_mode = GAME_RUN; break;
		}
		}
	}
}

void keyboard_up( unsigned char key, int x, int y){
	switch(key) {
		case 'r':{
			lift_flag = 0; break;
		}
		case 't':{
			lift_flag = 0; break;
		}
		case 'f':{
			proceed_flag = 0; break;
		}
		case 'j':{
			rotation_flag_theta = 0; break;
		}
		case 'l':{
			rotation_flag_theta = 0; break;
		}
	}
}

void special_key(int key ,int x , int y){


	switch(key){
		case GLUT_KEY_UP:
			rotation_flag_phi = 1; break;

		case GLUT_KEY_DOWN:
			rotation_flag_phi = -1; break;

		case GLUT_KEY_RIGHT:
			rotation_flag_theta = 2; break;

		case GLUT_KEY_LEFT:
			rotation_flag_theta = -2; break;
	}
}

void special_key_up(int key ,int x , int y){
	switch(key){
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
			rotation_flag_phi = 0;
			break;
		case GLUT_KEY_RIGHT:
		case GLUT_KEY_LEFT:
			rotation_flag_theta = 0;
			break;
	}
}


void idle() {
	glutPostRedisplay();
}



int main(int argc, char *argv[]) {

	srand( (unsigned int) time(&t));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowPosition(0,0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);


	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialFunc(special_key);
	glutSpecialUpFunc(special_key_up);
	glutIdleFunc(idle);

	init();

	pthread_t threads[4];
	int rc;

	//rc = pthread_create(&threads[0], NULL, gl_loop_thread, (void*) 0);
	rc= pthread_create(&threads[0], NULL, length_calc_thread, (void*) 0);
	rc= pthread_create(&threads[1], NULL, time_watch_thread, (void*) 1);
	rc= pthread_create(&threads[2], NULL, music_thread, (void*) 2);
	rc= pthread_create(&threads[3], NULL, rubik_calculation_thread, (void*) 2);
	if(rc){
		printf("failed to launch pthread\n");
	}

	glutMainLoop();

	return 0;
}



