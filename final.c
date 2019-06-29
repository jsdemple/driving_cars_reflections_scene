/*
 *  Joey Demple - hw2
 *
 *  Sports Car Texture and Lighting Scene with Skybox
 *
 *  Key bindings:
 *  +/-        Increase/decrease texture tiling repetition on the ground
 *  l          Toggle lighting on/off
 *  t          Change textures
 *  m          Toggles texture mode modulate/replace
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */

#include "CSCIx229.h"
int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=0;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=20;         //  Elevation of view angle
int light=1;      //  Lighting
int rep=10;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=4.1;   //  Size of world  1.3
const double PI = 3.1415927;
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0.5;  // Elevation of light
int side      =   0;  // Two sided lighting mode
//  Textures
unsigned int texture[2]; // Texture names
unsigned int car_texture[8];
unsigned int building_texture[4];
int box=1;	//  Draw sky
int sky[2];	//  Sky textures
//  Movement
double movement_x=0;
double tire_rot=0;


/*
 *  Draw Car
 *  	at (x,y,z)
 *  	dimensions (dx, dy, dz)
 *  	rotated th about the y axis
 */
static void car(double x, double y, double z,
             	    double dx, double dy, double dz,
		    double th,
		    double tire_rotation,
		    int paint_R, int paint_G, int paint_B)
{
   /*  Car Dimensions and Characteristics  */
   //  Body
   const double body_rear_height=6;
   const double body_front_height=3;
   const double body_len=15;
   const double body_width=7;
   //  Top
   const double top_height=6.5;
   const double top_width=6.5;
   const double back_windsh_base=15;
   const double back_windsh_top=7;
   const double front_windsh_base=8;
   const double front_windsh_top=1;
   float windsh_tint[] = {0.25,0.25,0.31};
   //  Wheels/Tires
   const double tire_radius=2.5;
   const double tire_width=1.0;
   const double axel_height=(tire_radius/2.0) + 1.3;
   const double rear_axel_pos=-10;
   const double front_axel_pos=11;
   const double tire_protrusion=-0.5;  //  How far the wheels stick out from the body
   const double tire_pos[4][3]= {
				      {front_axel_pos, axel_height, -body_width-tire_protrusion},  //  driver front
				      {rear_axel_pos,  axel_height, body_width+tire_protrusion},  //  passenger rear
				      {front_axel_pos, axel_height, body_width+tire_protrusion},  //  passenger front
				      {rear_axel_pos,  axel_height, -body_width-tire_protrusion},  //  driver rear
	                             };
   /*  Begin Drawing Car  */
   //  Save Transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx, dy, dz);
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   //  Draw Body
   glPushMatrix();  // textured faces
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   //glBindTexture(GL_TEXTURE_2D,texture[0]);
   //  Passenger
   glColor3ub(paint_R, paint_G, paint_B);
   glBindTexture(GL_TEXTURE_2D,car_texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-body_len,0+axel_height, body_width);
   glTexCoord2f(1,0); glVertex3f(+body_len,0+axel_height, body_width);
   glTexCoord2f(1,1); glVertex3f(+body_len,body_front_height+axel_height, body_width);
   glTexCoord2f(0,1); glVertex3f(-body_len,body_rear_height+axel_height, body_width);
   glEnd();
   //  Driver
   glColor3ub(paint_R, paint_G, paint_B);
   glBindTexture(GL_TEXTURE_2D,car_texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(1,0); glVertex3f(-body_len,0+axel_height,-body_width); //1
   glTexCoord2f(0,0); glVertex3f(+body_len,0+axel_height,-body_width); //2
   glTexCoord2f(0,1); glVertex3f(+body_len,body_front_height+axel_height,-body_width); //3
   glTexCoord2f(1,1); glVertex3f(-body_len,body_rear_height+axel_height,-body_width); //4
   glEnd();
   //  Front
   glColor3ub(paint_R, paint_G, paint_B);
   glBindTexture(GL_TEXTURE_2D,car_texture[1]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+body_len,0+axel_height,+body_width);
   glTexCoord2f(1,0); glVertex3f(+body_len,0+axel_height,-body_width);
   glTexCoord2f(1,1); glVertex3f(+body_len,body_front_height+axel_height,-body_width);
   glTexCoord2f(0,1); glVertex3f(+body_len,body_front_height+axel_height,+body_width);
   glEnd();
   //  Rear
   glColor3ub(paint_R, paint_G, paint_B);
   glBindTexture(GL_TEXTURE_2D,car_texture[2]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-body_len,0+axel_height,-body_width);
   glTexCoord2f(1,0); glVertex3f(-body_len,0+axel_height,+body_width);
   glTexCoord2f(1,1); glVertex3f(-body_len,body_rear_height+axel_height,+body_width);
   glTexCoord2f(0,1); glVertex3f(-body_len,body_rear_height+axel_height,-body_width);
   glEnd();
   //  Top
   glColor3ub(paint_R, paint_G, paint_B);
   glBindTexture(GL_TEXTURE_2D,car_texture[4]);
   glBegin(GL_QUADS);
   glNormal3f(body_rear_height-body_front_height,2*body_len, 0);  //  should be around (3,30,0) if body_len is 15 and front and rear h are 3,6
   glTexCoord2f(0,0); glVertex3f(-body_len,body_rear_height+axel_height,+body_width);
   glTexCoord2f(1,0); glVertex3f(+body_len,body_front_height+axel_height,+body_width);
   glTexCoord2f(1,1); glVertex3f(+body_len,+body_front_height+axel_height,-body_width);
   glTexCoord2f(0,1); glVertex3f(-body_len,+body_rear_height+axel_height,-body_width);
   glEnd();
   //  Bottom
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,car_texture[3]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-body_len,axel_height,-body_width);
   glTexCoord2f(1,0); glVertex3f(+body_len,axel_height,-body_width);
   glTexCoord2f(1,1); glVertex3f(+body_len,axel_height,+body_width);
   glTexCoord2f(0,1); glVertex3f(-body_len,axel_height,+body_width);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();

   //  Draw Top
   glPushMatrix();
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   //  Passenger
   glColor3f(windsh_tint[0], windsh_tint[1], windsh_tint[2]);
   glBindTexture(GL_TEXTURE_2D,car_texture[5]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-back_windsh_base, body_front_height+axel_height, top_width);
   glTexCoord2f(1,0); glVertex3f(+front_windsh_base,body_front_height+axel_height, top_width);
   glTexCoord2f(1,1); glVertex3f(+front_windsh_top,top_height+axel_height, top_width);
   glTexCoord2f(0,1); glVertex3f(-back_windsh_top, top_height+axel_height, top_width);
   glEnd();
   //  Driver
   glColor3f(windsh_tint[0], windsh_tint[1], windsh_tint[2]);
   glBindTexture(GL_TEXTURE_2D,car_texture[5]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(1,0); glVertex3f(-back_windsh_base,body_front_height+axel_height,-top_width); //1
   glTexCoord2f(0,0); glVertex3f(+front_windsh_base,body_front_height+axel_height,-top_width); //2
   glTexCoord2f(0,1); glVertex3f(+front_windsh_top,top_height+axel_height,-top_width); //3
   glTexCoord2f(1,1); glVertex3f(-back_windsh_top,top_height+axel_height,-top_width); //4
   glEnd();
   //  Front -- windshield
   glColor3f(windsh_tint[0], windsh_tint[1], windsh_tint[2]);
   glBindTexture(GL_TEXTURE_2D,car_texture[5]);
   glBegin(GL_QUADS);
   glNormal3f(top_height, (front_windsh_base - front_windsh_top), 0);
   glTexCoord2f(0,0); glVertex3f(+front_windsh_base,body_front_height+axel_height,+top_width);
   glTexCoord2f(1,0); glVertex3f(+front_windsh_base,body_front_height+axel_height,-top_width);
   glTexCoord2f(1,1); glVertex3f(+front_windsh_top,top_height+axel_height,-top_width);
   glTexCoord2f(0,1); glVertex3f(+front_windsh_top,top_height+axel_height,+top_width);
   glEnd();
   //  Rear -- rear windshield
   glColor3f(windsh_tint[0], windsh_tint[1], windsh_tint[2]);
   glBindTexture(GL_TEXTURE_2D,car_texture[5]);
   glBegin(GL_QUADS);
   glNormal3f(-top_height, (back_windsh_base - back_windsh_top), 0);
   glTexCoord2f(0,0); glVertex3f(-back_windsh_base,body_front_height+axel_height,-top_width);
   glTexCoord2f(1,0); glVertex3f(-back_windsh_base,body_front_height+axel_height,+top_width);
   glTexCoord2f(1,1); glVertex3f(-back_windsh_top,top_height+axel_height,+top_width);
   glTexCoord2f(0,1); glVertex3f(-back_windsh_top,top_height+axel_height,-top_width);
   glEnd();
   //  Top
   glColor3ub(paint_R, paint_G, paint_B);
   glDisable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0); glVertex3f(-back_windsh_top,  top_height+axel_height, +top_width);
   glTexCoord2f(1,0); glVertex3f(+front_windsh_top, top_height+axel_height, +top_width);
   glTexCoord2f(1,1); glVertex3f(+front_windsh_top, top_height+axel_height, -top_width);
   glTexCoord2f(0,1); glVertex3f(-back_windsh_top,  top_height+axel_height, -top_width);
   glEnd();
   //  Bottom
   glColor3ub(paint_R, paint_G, paint_B);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-back_windsh_base, body_front_height+axel_height,-top_width);
   glTexCoord2f(1,0); glVertex3f(+front_windsh_base,body_front_height+axel_height,-top_width);
   glTexCoord2f(1,1); glVertex3f(+front_windsh_base,body_front_height+axel_height,+top_width);
   glTexCoord2f(0,1); glVertex3f(-back_windsh_base, body_front_height+axel_height,+top_width);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   //  Tires
   int i,k;
   double Tx;
   double Ty;
   double Tz;

   //  Tire: Driver Front
   i = 0;
   Tx=tire_pos[i][0];
   Ty=tire_pos[i][1];
   Tz=tire_pos[i][2];
   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(Tx,Ty,Tz);
   glRotated(-tire_rot,0,0,1);
   glScaled(tire_radius,tire_radius,tire_width);
   //  Faces
   glColor3f(1,1,1);
   for (i=1;i>=-1;i-=2)
   {
   glBindTexture(GL_TEXTURE_2D,car_texture[6]);
   glNormal3f(0,0,i);
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0,0,i);
   for (k=0;k<=360;k+=10)
   {
   glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
   glVertex3f(i*Cos(k),Sin(k),i);
   }
   glEnd();
   }
   //  Edge
   glBindTexture(GL_TEXTURE_2D,car_texture[7]);
   glColor3f(1,1,1);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
   glNormal3f(Cos(k),Sin(k),0);
   glTexCoord2f(0,0.05*k); glVertex3f(Cos(k),Sin(k),+1);
   glTexCoord2f(1,0.05*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   //  Tire: Passenger Rear
   i = 1;
   Tx=tire_pos[i][0];
   Ty=tire_pos[i][1];
   Tz=tire_pos[i][2];
   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(Tx,Ty,Tz);
   glRotated(-tire_rot,0,0,1);
   glScaled(tire_radius,tire_radius,tire_width);
   //  Faces
   glColor3f(1,1,1);
   for (i=1;i>=-1;i-=2)
   {
   glBindTexture(GL_TEXTURE_2D,car_texture[6]);
   glNormal3f(0,0,i);
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0,0,i);
   for (k=0;k<=360;k+=10)
   {
   glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
   glVertex3f(i*Cos(k),Sin(k),i);
   }
   glEnd();
   }
   //  Edge
   glBindTexture(GL_TEXTURE_2D,car_texture[7]);
   glColor3f(1,1,1);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
   glNormal3f(Cos(k),Sin(k),0);
   glTexCoord2f(0,0.05*k); glVertex3f(Cos(k),Sin(k),+1);
   glTexCoord2f(1,0.05*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   //  Tire: Passenger Front
   i = 2;
   Tx=tire_pos[i][0];
   Ty=tire_pos[i][1];
   Tz=tire_pos[i][2];
   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(Tx,Ty,Tz);
   glRotated(-tire_rot,0,0,1);
   glScaled(tire_radius,tire_radius,tire_width);
   //  Faces
   glColor3f(1,1,1);
   for (i=1;i>=-1;i-=2)
   {
   glBindTexture(GL_TEXTURE_2D,car_texture[6]);
   glNormal3f(0,0,i);
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0,0,i);
   for (k=0;k<=360;k+=10)
   {
   glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
   glVertex3f(i*Cos(k),Sin(k),i);
   }
   glEnd();
   }
   //  Edge
   glBindTexture(GL_TEXTURE_2D,car_texture[7]);
   glColor3f(1,1,1);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
   glNormal3f(Cos(k),Sin(k),0);
   glTexCoord2f(0,0.05*k); glVertex3f(Cos(k),Sin(k),+1);
   glTexCoord2f(1,0.05*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   //  Tire: Driver Rear
   i = 3;
   Tx=tire_pos[i][0];
   Ty=tire_pos[i][1];
   Tz=tire_pos[i][2];
   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(Tx,Ty,Tz);
   glRotated(-tire_rot,0,0,1);
   glScaled(tire_radius,tire_radius,tire_width);
   //  Faces
   glColor3f(1,1,1);
   for (i=1;i>=-1;i-=2)
   {
   glBindTexture(GL_TEXTURE_2D,car_texture[6]);
   glNormal3f(0,0,i);
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0,0,i);
   for (k=0;k<=360;k+=10)
   {
   glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
   glVertex3f(i*Cos(k),Sin(k),i);
   }
   glEnd();
   }
   //  Edge
   glBindTexture(GL_TEXTURE_2D,car_texture[7]);
   glColor3f(1,1,1);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
   glNormal3f(Cos(k),Sin(k),0);
   glTexCoord2f(0,0.05*k); glVertex3f(Cos(k),Sin(k),+1);
   glTexCoord2f(1,0.05*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();
}

/*
 *  Draw Car and reflected car
 *  	at (x,y,z)
 *  	dimensions (dx, dy, dz)
 *  	rotated th about the y axis
 */
static void car_with_reflection(double x, double y, double z,
             	    double dx, double dy, double dz,
		    double th,
		    double tire_rotation,
		    int paint_R, int paint_G, int paint_B)
{
   car(x,y,z, dx,dy,dz,  th, tire_rotation, paint_R,paint_G,paint_B);  //  car
   car(x,y,z, dx,-dy,dz, th, tire_rotation, paint_R,paint_G,paint_B);  //  reflection of car
}


/*
 *  Draw sidewalk
 *     at (x,y,z)
 *     dimensions (dx,dy)
 *     rotated th about the y axis
 *     texture repeater variable
 */
static void sidewalk(double x,double y,double z,
                 double dx,double dy,
                 double th,
		 double rep)
{
   //  Default Road Dimensions
   double roadLen=4;
   double roadWidth=1.3;
   double curbHeight=0.03;

   //  Draw Sidewalk
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   //  Offset, scale and rotate
   glTranslated(x,y+curbHeight,z);
   glRotated(90,1,0,0);
   glRotated(th,0,1,0);
   glScaled(dx,dy,1);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   //  Draw road
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,  0);   glVertex2f(-roadLen,-roadWidth);
   glTexCoord2f(2*rep,0);   glVertex2f(+roadLen,-roadWidth);
   glTexCoord2f(2*rep,rep); glVertex2f(+roadLen,+roadWidth);
   glTexCoord2f(0,  rep); glVertex2f(-roadLen,+roadWidth);
   glEnd();
   //  Undo transformations and textures
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   //  Draw Curb
   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   //  Offset, scale and rotate
   glTranslated(x,y,z-roadWidth);
   glRotated(th,0,1,0);
   glScaled(dx,dy,1);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   //  Draw road
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0,0,+1);
   glTexCoord2f(0,  0);   glVertex2f(-roadLen,-curbHeight);
   glTexCoord2f(2*rep,0);   glVertex2f(+roadLen,-curbHeight);
   glTexCoord2f(2*rep,rep); glVertex2f(+roadLen,+curbHeight);
   glTexCoord2f(0,  rep); glVertex2f(-roadLen,+curbHeight);
   glEnd();
   //  Undo transformations and textures
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   //  Draw Curb
   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   //  Offset, scale and rotate
   glTranslated(x,y,z+roadWidth);
   glRotated(th,0,1,0);
   glScaled(dx,dy,1);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   //  Draw road
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0,0,+1);
   glTexCoord2f(0,  0);   glVertex2f(-roadLen,-curbHeight);
   glTexCoord2f(2*rep,0);   glVertex2f(+roadLen,-curbHeight);
   glTexCoord2f(2*rep,rep); glVertex2f(+roadLen,+curbHeight);
   glTexCoord2f(0,  rep); glVertex2f(-roadLen,+curbHeight);
   glEnd();
   //  Undo transformations and textures
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a 2D road
 *     at (x,y,z)
 *     dimensions (dx,dy)
 *     rotated th about the y axis
 *     texture repeater variable
 */
static void road(double x,double y,double z,
                 double dx,double dy,
                 double th,
		 double rep)
{
   //  Default Road Dimensions
   double roadLen=4;
   double roadWidth=1.3;
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(90,1,0,0);
   glRotated(th,0,1,0);
   glScaled(dx,dy,1);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   //  Draw road
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,  0);   glVertex2f(-roadLen,-roadWidth);
   glTexCoord2f(2*rep,0);   glVertex2f(+roadLen,-roadWidth);
   glTexCoord2f(2*rep,rep); glVertex2f(+roadLen,+roadWidth);
   glTexCoord2f(0,  rep); glVertex2f(-roadLen,+roadWidth);
   glEnd();
   //  Undo transformations and textures
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   //  Draw Yellow lines
   glPushMatrix();
   glColor3f(0.9,0.9,0);
   glTranslated(x,y,z-0.03);
   glRotated(90,1,0,0);
   glRotated(th,1,0,0);
   glScaled(dx,dy,1);
   glBegin(GL_QUADS);
   glVertex2f(+roadLen,roadWidth*+0.01);
   glVertex2f(-roadLen,roadWidth*+0.01);
   glVertex2f(-roadLen,roadWidth*-0.01);
   glVertex2f(+roadLen,roadWidth*-0.01);
   glEnd();
   glPopMatrix();
   glPushMatrix();
   glColor3f(0.9,0.9,0);
   glTranslated(x,y,z+0.03);
   glRotated(90,1,0,0);
   glRotated(th,1,0,0);
   glScaled(dx,dy,1);
   glBegin(GL_QUADS);
   glVertex2f(+roadLen,roadWidth*+0.01);
   glVertex2f(-roadLen,roadWidth*+0.01);
   glVertex2f(-roadLen,roadWidth*-0.01);
   glVertex2f(+roadLen,roadWidth*-0.01);
   glEnd();
   glPopMatrix();
 }


/*
 *  Draw water
 *     at (x,y,z)
 *     dimensions (dx,dy)
 *     rotated th about the y axis
 *     texture repeater variable
 */
static void water(double x,double y,double z,
                 double dx,double dy,
                 double th,
		 double rep)
{
   //  Default Road Dimensions
   double Len=4;
   double Width=1.3;
   //  Save transformation
   glPushMatrix();
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glColor4f(0.0, 0.0, 0.9, 0.3);
   glColor4f(1.0, 1.0, 1.0, 0.3);
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(90,1,0,0);
   glRotated(th,0,1,0);
   glScaled(dx,dy,1);
   //  Draw water
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glVertex2f(-Len,-Width);
   glVertex2f(+Len,-Width);
   glVertex2f(+Len,+Width);
   glVertex2f(-Len,+Width);
   glEnd();
   //  Undo transformations and textures
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
}


/*
 *  Draw a building
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void building(int building_texture_index,
                     double x,double y,double z,
                     double dx,double dy,double dz,
                     double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   //glBindTexture(GL_TEXTURE_2D,building_texture[building_texture_index]);
   //  Front
   glBindTexture(GL_TEXTURE_2D,building_texture[building_texture_index]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1, 0, 1);
   glTexCoord2f(1,0); glVertex3f(+1, 0, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+2, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+2, 1);
   glEnd();
   //  Back
   glBindTexture(GL_TEXTURE_2D,building_texture[building_texture_index]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1, 0,-1);
   glTexCoord2f(1,0); glVertex3f(-1, 0,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+2,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+2,-1);
   glEnd();
   //  Right
   glBindTexture(GL_TEXTURE_2D,building_texture[building_texture_index]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1, 0,+1);
   glTexCoord2f(1,0); glVertex3f(+1, 0,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+2,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+2,+1);
   glEnd();
   //  Left
   glBindTexture(GL_TEXTURE_2D,building_texture[building_texture_index]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1, 0,-1);
   glTexCoord2f(1,0); glVertex3f(-1, 0,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+2,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+2,-1);
   glEnd();
   //  Top
   glColor3f(0.7,0.7,0.7);
   glBindTexture(GL_TEXTURE_2D,texture[1]);  //  Use sidewalk texture for roof
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+2,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+2,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+2,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+2,-1);
   glEnd();
   //  Bottom
   glBindTexture(GL_TEXTURE_2D,texture[1]);  //  Use sidewalk texture for under building
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1, 0,-1);
   glTexCoord2f(1,0); glVertex3f(+1, 0,-1);
   glTexCoord2f(1,1); glVertex3f(+1, 0,+1);
   glTexCoord2f(0,1); glVertex3f(-1, 0,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a building
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void building_with_reflection(int building_texture_index,
                     double x,double y,double z,
                     double dx,double dy,double dz,
                     double th)
{
   building(building_texture_index, x,y,z, dx,dy,dz,  th);
   building(building_texture_index, x,y,z, dx,-dy,dz, th);
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a street light
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
 static void street_light(double x, double y, double z,
                          double r, double d,
                          double th)
{
   int i,k;

   //  DRAW BASE
   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(90,1,0,0);
   glScaled(r,r,d);
   //  Head & Tail
   glColor3f(1,1,1);
   for (i=1;i>=-1;i-=2)
   {
      glBindTexture(GL_TEXTURE_2D,texture[0]);
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }
   //  Edge
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glColor3f(1,1,1);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);


   //  DRAW HORIZONTAL ARM
   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y+d,z+(d/2.0));
   glScaled(r,r,d/2.0);
   //  Head & Tail
   glColor3f(1,1,1);
   for (i=1;i>=-1;i-=2)
   {
      glBindTexture(GL_TEXTURE_2D,texture[0]);
      glNormal3f(0,0,i);
      glBegin(GL_TRIANGLE_FAN);
      glTexCoord2f(0.5,0.5);
      glVertex3f(0,0,i);
      for (k=0;k<=360;k+=10)
      {
         glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
         glVertex3f(i*Cos(k),Sin(k),i);
      }
      glEnd();
   }
   //  Edge
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glColor3f(1,1,1);
   glBegin(GL_QUAD_STRIP);
   for (k=0;k<=360;k+=10)
   {
      glNormal3f(Cos(k),Sin(k),0);
      glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),+1);
      glTexCoord2f(1,0.5*k); glVertex3f(Cos(k),Sin(k),-1);
   }
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);


   //  DRAW LIGHT FIXTURE
   glEnable(GL_TEXTURE_2D);
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y+d-(r),z+(d));
   glScaled(r*2.0,r,r*2.0);
   //  Head & Tail
   glColor3f(1,1,1);

   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1, 0, 1);
   glTexCoord2f(1,0); glVertex3f(+1, 0, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+2, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+2, 1);
   glEnd();
   //  Back
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1, 0,-1);
   glTexCoord2f(1,0); glVertex3f(-1, 0,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+2,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+2,-1);
   glEnd();
   //  Right
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1, 0,+1);
   glTexCoord2f(1,0); glVertex3f(+1, 0,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+2,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+2,+1);
   glEnd();
   //  Left
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1, 0,-1);
   glTexCoord2f(1,0); glVertex3f(-1, 0,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+2,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+2,-1);
   glEnd();
   //  Top
   glColor3f(0.7,0.7,0.7);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+2,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+2,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+2,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+2,-1);
   glEnd();
   //  Bottom
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1, 0,-1);
   glTexCoord2f(1,0); glVertex3f(+1, 0,-1);
   glTexCoord2f(1,1); glVertex3f(+1, 0,+1);
   glTexCoord2f(0,1); glVertex3f(-1, 0,+1);
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);


//   //  DRAW GROUND FIXTURE
//   //  Save transformation
//   glPushMatrix();
//   //  Offset, scale and rotate
//   glTranslated(x,y-d,z);
//   glScaled(r*2.0,r,r*2.0);
//   //  White ball
//   glColor3f(0.1,0.1,0.1);
//   glutSolidSphere(1.0,16,16);
//   //  Undo transofrmations
//   glPopMatrix();

//   int ball_th,ball_ph;
//   int inc=10;
//   float yellow[] = {1.0,1.0,0.0,1.0};
//   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
//   //  Save transformation
//   glPushMatrix();
//   //  Offset, scale and rotate
//   glTranslated(x,y-d,z);
//   glScaled(r*2.0,r*2.0,r*2.0);
//   //  White ball
//   glColor3f(1,1,1);
//   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
//   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
//   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
//   //  Bands of latitude
//   for (ball_ph=-90;ball_ph<90;ball_ph+=inc)
//   {
//      glBegin(GL_QUAD_STRIP);
//      for (ball_th=0;ball_th<=360;ball_th+=2*inc)
//      {
//         Vertex(ball_th,ball_ph);
//         Vertex(ball_th,ball_ph+inc);
//      }
//      glEnd();
//   }

   
   int ball_th,ball_ph;
  /* glPushMatrix();
   glTranslated(x,y-d,z);
   glScaled(r*2.0,r*2.0,r*2.0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   //  Latitude bands
   glColor3f(1,1,1);
   for (ball_ph=-90;ball_ph<90;ball_ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (ball_th=0;ball_th<=360;ball_th+=5)
      {
         Vertex(ball_th,ball_ph);
         Vertex(ball_th,ball_ph+5);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
   */


   //  DRAW LIGHT
   int inc=10;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y+d-r,z+(d));
   glScaled(r*2.0,r,r*2.0);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ball_ph=-90;ball_ph<90;ball_ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (ball_th=0;ball_th<=360;ball_th+=2*inc)
      {
         Vertex(ball_th,ball_ph);
         Vertex(ball_th,ball_ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw sky box
 */
static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,sky[1]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

static void spotlight(double x, double y, double z)
{
   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   //  Light direction
   float Position[]  = {x,y,z,1};
   //  Draw light position as ball (still no lighting here)
   glColor3f(1,1,1);
   ball(Position[0],Position[1],Position[2] , 0.1);
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Two sided lighting on or off
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Length of axes
   const double len=2.0;
   //  Eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   //  Draw skybox
   if (box) Sky(3.5*dim);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Two sided lighting on or off
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
   //  Draw scene
   car_with_reflection(-5.7+movement_x,0,0.7 , 0.05,0.05,0.05, 0  , tire_rot, 205,150,10);  // yellow car
   car_with_reflection(+5.7-movement_x,0,-0.7, 0.05,0.05,0.05, 180, tire_rot, 205,15,10);  // red car

   building_with_reflection(3, -6.7,0,-4.2, 2.1,2.6,2.1, 0);    //  building 3 concrete/glass
   building_with_reflection(0, -2.7,0,-2.7, 1,1,1, 0);          //  building 0 brick storefront
   building_with_reflection(1, 0,0,-3.2, 1.3,1.3,1.3, 0);       //  building 1 yellow building glass storefront
   building_with_reflection(2, 2.7,0,-2.7, 1.3,1.3,1.3, 0);     //  building 2 old white/yellow/brown building
   building_with_reflection(1, 5.7,0,-3.5, 1.3,1.3,1.3, 0);       //  building 1 yellow building glass storefront
   building_with_reflection(3, 9.7,0,-4.2, 2.1,2.6,2.1, 0);    //  building 3 concrete/glass

   road(    0,0 ,0  , 1,1, 0, rep);
   water(0,0, 2.6, 1,3, 0, rep);
   sidewalk(0,0,-2.6, 1,1, 0, rep);

   road(    -8,0 ,0  , 1,1, 0, rep);
   water(-8,0, 2.6, 1,3, 0, rep);
   sidewalk(-8,0,-2.6, 1,1, 0, rep);

   road(    8,0 ,0  , 1,1, 0, rep);
   water(8,0, 2.6, 1,3, 0, rep);
   sidewalk(8,0,-2.6, 1,1, 0, rep);

   street_light(0,1,0, 0.05,1, 0);
   //spotlight(0,0,0);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Light=%s Movement=%.1f",th,ph,dim,light?"On":"Off",movement_x);
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   movement_x = t*0.55;
   tire_rot = t*200;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Two sided mode
   else if (key == GLUT_KEY_F3)
      side = 1-side;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 25;
   //  Toggle texture mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(45,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(1200,600);
   glutCreateWindow("Joey Demple - hw3");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[0] = LoadTexBMP("asphalt.bmp");
   texture[1] = LoadTexBMP("sidewalk.bmp");
   car_texture[0] = LoadTexBMP("car_side.bmp");
   car_texture[1] = LoadTexBMP("car_front.bmp");
   car_texture[2] = LoadTexBMP("car_rear.bmp");
   car_texture[3] = LoadTexBMP("car_underside.bmp");
   car_texture[4] = LoadTexBMP("car_top.bmp");
   car_texture[5] = LoadTexBMP("car_glass.bmp");
   car_texture[6] = LoadTexBMP("car_wheel.bmp");
   car_texture[7] = LoadTexBMP("car_tread.bmp");
   building_texture[0] = LoadTexBMP("building0.bmp");
   building_texture[1] = LoadTexBMP("building1.bmp");
   building_texture[2] = LoadTexBMP("building2.bmp");
   building_texture[3] = LoadTexBMP("building3.bmp");
   sky[0] = LoadTexBMP("sky0.bmp");
   sky[1] = LoadTexBMP("sky1.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
