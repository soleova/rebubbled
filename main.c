#include <GL/glut.h>  
#include <math.h>     
#include <stdio.h>

static int animation_active;
static int arrow_animation;
static float arrow_speed = 0.05; 
static float arrow_x = 0.0;
static float arrow_y = -0.7;

const char title[] = "Rebbubled";  
int window_width  = 500;   
int window_height = 500;    
int window_pos_x   = 50;      
int window_pos_y   = 50;      

int flag;
float player_x = 0.0;
float player_y = -0.95;

float ball_radius = 0.09; 
float smaller_ball_radius = 0.06;  

float ball_x = 0.0;         
float ball_y = 0.0;

float ball2_x = 0.2;
float ball2_y = 0.0;

float ball_x_max, ball_x_min, ball_y_max, ball_x_min;

float x_speed = 0.01;     
float y_speed = 0.05;

int refresh = 30;  
double clip_area_x_left, clip_area_x_right, clip_area_y_bottom, clip_area_y_top;
 
/* Deklaracije callback funkcija */
static void keyboard(unsigned char key, int x, int y);
static void reshape(int width, int height);
static void display(void);
static void timer(int value);

/* Funkcija koja proverava granice odskakanja lopte. */
static void bounce();
/* Funkcija koja proverava granice strele. */
static void shoot(void);
/* Funkcija koja proverava granice odskakanja manjih lopti. */
static void bounce2(void);

void display(void) {

    /* Postavlja se boja svih piksela na zadatu boju pozadine. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
    glMatrixMode(GL_MODELVIEW);   
    glLoadIdentity();           
 
   /* Parametri svetla */
    GLfloat light_ambient[] = { 0, 0, 0, 1 };
    GLfloat light_diffuse[] = { 1, 1, 0.9, 1 };
    GLfloat light_specular[] = { 1, 1, 1, 1 };
    GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };
   
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    /* Pozicija svetla. */
    GLfloat light_position[] = { 1, 1, 1, 0 };

    /* Nulti koeficijenti refleksije materijala. */
    GLfloat no_material[] = { 0, 0, 0, 1 };

    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat material_ambient[] = { 0.7, 0.7, 0.7, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat material_diffuse[] = { 0.1, 0.5, 0.8, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat material_specular[] = { 1, 1, 1, 1 };

    /* Koeficijent spekularne refleksije za slucaj kada nema spekularne refleksije. */
    GLfloat no_shininess[] = { 0 };

    /* Koeficijent spekularne refleksije za slucaj male vrednosti koeficijenta. */
    GLfloat low_shininess[] = { 5 };

    /* Koeficijent spekularne refleksije za slucaj velike vrednosti koeficijenta. */
    // GLfloat high_shininess[] = { 100 };

    /* Emisiona boja materijala. */
    GLfloat material_emission[] = { 0.3, 0.2, 0.2, 0 };

    /* Pozicionira se svetlo. */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 

    /* Crtamo igraca. */
    glPushMatrix();
      glTranslatef(player_x, player_y, 0);
      glutSolidCube(0.1);
    glPopMatrix();

    if(flag == 0){
    /* Crtamo loptu */
    bounce();
    glPushMatrix();
      glTranslatef(ball_x, ball_y, 0.0f);
      glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
      glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
      glutSolidSphere(ball_radius, 15, 15);
    glPopMatrix();

   }
    /* Crtamo dve manje loptice kada dodje do kolizije */ 
    if(flag==3){
    bounce2();
    glPushMatrix();  
      glTranslatef(ball2_x, ball2_y, 0);
      glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
      glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
      glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
      glutSolidSphere(smaller_ball_radius, 15, 15);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(ball2_x/2, ball2_y, 0);
      glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
      glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
      glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
      glutSolidSphere(smaller_ball_radius, 15, 15);
    glPopMatrix();
   }
 
   /* Ako je pogodjen igrac, izlazimo iz programa. */
    if(flag == 1 || flag == 2){
       exit(1);
   }
    
   /* Iskljucujemo svetlo da bismo mogli da crtamo strele */
    glDisable(GL_LIGHTING);

   /* Crtamo strelu */
    glPushMatrix();
    glTranslatef(player_x, arrow_y, 0);
     glBegin(GL_LINES);
       glColor3f(0, 1, 1);
       glVertex3f(arrow_x, arrow_y - 0.3, 0);
       glVertex3f(arrow_x, arrow_y, 0);
     glEnd();
    glPopMatrix();
   
    /* Crtamo animirani sareni kvadrat koji ce imitirati pozadinu. */
    glPushMatrix();
    glTranslatef(cos(ball_x), sin(ball_y), 0);
     glBegin(GL_QUADS);	
      glColor3f(1.0, 0.0, 0.2);
      glVertex2f(-3.0, -3.0);  
      glVertex2f(3.0, -3.0);
      glColor3f(0.1, 0.4, 1.0);
      glVertex2f(3.0, 3.0);
      glVertex2f(-3.0, 3.0);
     glEnd();
    glPopMatrix();    

    /* Postavlja se nova slika u prozor. */
    glutSwapBuffers(); 
}

static void keyboard(unsigned char key, int x, int y){
 
    switch(key){
    case 'd':
             if(player_x < clip_area_x_right - 0.05){
             player_x += 0.02;
	     } 
	     break;
    case 'a':
	     if(player_x > clip_area_x_left + 0.05){
	     player_x -= 0.02;
	     } 
	     break;
    case 'f': 
	     arrow_animation = 0; 
	    
             if(animation_active){
          	 glutTimerFunc(refresh, timer, 2);
		 arrow_animation = 1;
	     }          	
             break;
    case 27:
	     exit(1);
    }
}
 
static void timer(int value)
{
 
    switch (value) {
    case 1:
             glutPostRedisplay();
	  
             if((ball_x - player_x) * (ball_x - player_x) + 
               (ball_y - player_y) * (ball_y - player_y) <= 
               (ball_radius) * (ball_radius)){
	           flag = 1;
	        // printf("Pogodjen igrac!\n");
              }

             if((ball2_x - player_x) * (ball2_x - player_x) + 
               (ball2_y - player_y) * (ball2_y - player_y) <= 
               (smaller_ball_radius) * (smaller_ball_radius)){
	           flag = 2;
	        // printf("Pogodjen igrac manjom lopticom!\n");
	      }
	  
             /* Open se postavlja tajmer. */
	     glutTimerFunc(refresh, timer, 1);
	     break;
    case 2:
	     /* Povecava se y koordinata strele. */
	     arrow_y += arrow_speed;
		
             /* Ispaljivanje strela. */
	     shoot();
	

             /* Proverava se da li je pogodjena velika lopta. */
	     if((ball_x - player_x) * (ball_x - player_x) + 
               (ball_y - arrow_y) * (ball_y - arrow_y) <= 
               (ball_radius) * (ball_radius)) {
	           flag = 3;
	        // printf("Pogodjena velika loptica!\n");	     	
             }

	     /* Proverava se da li je pogodjena manja loptica. */

             if((ball2_x - player_x) * (ball2_x - player_x) + 
               (ball2_y - arrow_y) * (ball2_y - arrow_y) <= 
               (smaller_ball_radius) * (smaller_ball_radius)){
	           flag = 4;
	     // printf("Pogodjena manja loptica!\n");
	     }

       	     /* Postavlja se ponovo tajmer. */
	     if (arrow_animation){
	       glutTimerFunc(refresh, timer, 2);
	     }
	     break;
    }
}

static void bounce(){

    /* Kretanje loptice */
    ball_x += x_speed;
    ball_y += y_speed;
    
    /* Granice za kretanje lopte */
    if (ball_x > ball_x_max) {
       ball_x = ball_x_max;
       x_speed = -x_speed;
    } else if (ball_x < ball_x_min) {
       ball_x = ball_x_min;
       x_speed = -x_speed;
    }
    if (ball_y > ball_y_max) {
       ball_y = ball_y_max;
       y_speed = -y_speed;
    } else if (ball_y < ball_x_min) {
       ball_y = ball_x_min;
       y_speed = -y_speed;
    }
}

static void bounce2(void){

    /* Kretanje loptice */
    ball2_x += x_speed;
    ball2_y += y_speed;
   
    /* Granice za kretanje lopte */

    if (ball2_x > ball_x_max) {
       ball2_x = ball_x_max;
       x_speed = - x_speed;
    } else if (ball2_x < ball_x_min) {
       ball2_x = ball_x_min;
       x_speed = -x_speed;
    }
    if (ball2_y > ball_y_max) {
       ball2_y = ball_y_max;
       y_speed = -y_speed;
    } else if (ball2_y < ball_x_min) {
       ball2_y = ball_x_min;
       y_speed = -y_speed;
    }

}
static void shoot(void){
	
    /* Ako strela predje y granicu, vracamo je dole i postaje neaktivna. */	

    if(arrow_y > ball_y_max){ 
       arrow_y = ball_x_min;
       arrow_animation = 0;
    }
}


void reshape(int width, int height) {

    if (height == 0) height = 1;    

    GLfloat aspect = (float)width / (float)height;
 
    glViewport(0, 0, width, height);
 
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();           
   
    if (width >= height) {
        clip_area_x_left   = -1.0 * aspect;
        clip_area_x_right  = 1.0 * aspect;
        clip_area_y_bottom = -1.0;
        clip_area_y_top    = 1.0;
    } else {
        clip_area_x_left   = -1.0;
        clip_area_x_right  = 1.0;
        clip_area_y_bottom = -1.0 / aspect;
        clip_area_y_top    = 1.0 / aspect;
    }
    gluOrtho2D(clip_area_x_left, clip_area_x_right, clip_area_y_bottom, clip_area_y_top);
    ball_x_min = clip_area_x_left + ball_radius;
    ball_x_max = clip_area_x_right - ball_radius;
    ball_x_min = clip_area_y_bottom + ball_radius;
    ball_y_max = clip_area_y_top/1.5 - ball_radius;
}

int main(int argc, char** argv) {

    /* Inicijalizija GLUT-a */
    glutInit(&argc, argv);          
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   
    /* Kreira se prozor. */ 
    glutInitWindowSize(window_width, window_height);  

    /* Prozor se pozicionira na sredinu ekrana. */
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - window_width)/2,
			   (glutGet(GLUT_SCREEN_HEIGHT) - window_height)/2); 
 
    /* Naziv programa. */
    glutCreateWindow(title);       

    /* OpenGL inicijalizacija. */
    glClearColor (0.0, 0.0, 0.0, 0.0); 
    glEnable(GL_DEPTH_TEST);
 
    /* Registrovanje funkcija za obradu dogadjaja */
    glutDisplayFunc(display);    
    glutReshapeFunc(reshape);   
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 1);  
    animation_active = 1;

    /* Ulazi se u glavnu petlju */
    glutMainLoop();              
    
    return 0;
}
