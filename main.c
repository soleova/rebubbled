#include <GL/glut.h>  
#include <math.h>     
 

char title[] = "Rebbubled";  
int windowWidth  = 500;   
int windowHeight = 500;    
int windowPosX   = 50;      
int windowPosY   = 50;      

GLfloat playerX = 0.0;
GLfloat ballRadius = 0.07f;   
GLfloat ballX = 0.0f;         
GLfloat ballY = 0.0f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin;
GLfloat xSpeed = 0.02f;     
GLfloat ySpeed = 0.07f;
int refreshMillis = 30;  
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
 
/* Deklaracije callback funkcija */
static void keyboard(unsigned char key, int x, int y);
static void reshape(int width, int height);
static void display(void);
static void on_timer(int value);
 
void display(void) {

   /* Postavlja se boja svih piksela na zadatu boju pozadine. */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
   glMatrixMode(GL_MODELVIEW);   
   glLoadIdentity();           
 
    /* Pozicija svetla. */
    GLfloat light_position[] = { 1, 1, 1, 0 };

    /* Nulti koeficijenti refleksije materijala. */
    GLfloat no_material[] = { 0, 0, 0, 1 };

    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat material_ambient[] = { 0.7, 0.7, 0.7, 1 };

    /* Koeficijenti ambijentalne refleksije materijala za slucaj
       nehomogene ambijentalne refleksije. */
    GLfloat material_ambient_heterogeneous[] = { 0.8, 0.8, 0.2, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat material_diffuse[] = { 0.1, 0.5, 0.8, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat material_specular[] = { 1, 1, 1, 1 };

    /* Koeficijent spekularne refleksije za slucaj kada nema spekularne refleksije. */
    GLfloat no_shininess[] = { 0 };

    /* Koeficijent spekularne refleksije za slucaj male vrednosti koeficijenta. */
    GLfloat low_shininess[] = { 5 };

    /* Koeficijent spekularne refleksije za slucaj velike vrednosti koeficijenta. */
    GLfloat high_shininess[] = { 100 };

    /* Emisiona boja materijala. */
    GLfloat material_emission[] = { 0.3, 0.2, 0.2, 0 };

    /* Pozicionira se svetlo. */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    /* Crtamo igraca. */
    glPushMatrix();
       glTranslatef(playerX, -0.95, 0);
       glutSolidCube(0.1);
    glPopMatrix();

    /* Crtamo loptu */
    glPushMatrix();
       glTranslatef(ballX, ballY, 0.0f);
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_material);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
       glutSolidSphere(ballRadius, 15, 15);
    glPopMatrix();
 
    /* Postavlja se nova slika u prozor. */
   glutSwapBuffers(); 
    
   /* Kretanje loptice */
   ballX += xSpeed;
   ballY += ySpeed;
   
   /* Granice za kretanje lopte */

   if (ballX > ballXMax) {
      ballX = ballXMax;
      xSpeed = -xSpeed;
   } else if (ballX < ballXMin) {
      ballX = ballXMin;
      xSpeed = -xSpeed;
   }
   if (ballY > ballYMax) {
      ballY = ballYMax;
      ySpeed = -ySpeed;
   } else if (ballY < ballYMin) {
      ballY = ballYMin;
      ySpeed = -ySpeed;
   }

}

static void keyboard(unsigned char key, int x, int y)
{
   /* Kretanje igraca u desno */
   if(key == 'd'){
	if(playerX < clipAreaXRight - 0.1){
	playerX+=0.02;
	}
   } /*Kretanje igraca u levo */
   else if(key == 'a'){
	if(playerX > clipAreaXLeft+0.1){
	playerX-=0.2;
	}
   } /*Izlazak iz programa */
   if(key == 27){
   exit(1);
 }
}

void reshape(int width, int height) {

   if (height == 0) height = 1;    

   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION);  
   glLoadIdentity();            
   if (width >= height) {
      clipAreaXLeft   = -1.0 * aspect;
      clipAreaXRight  = 1.0 * aspect;
      clipAreaYBottom = -1.0;
      clipAreaYTop    = 1.0;
   } else {
      clipAreaXLeft   = -1.0;
      clipAreaXRight  = 1.0;
      clipAreaYBottom = -1.0 / aspect;
      clipAreaYTop    = 1.0 / aspect;
   }
   gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);

   ballXMin = clipAreaXLeft + ballRadius;
   ballXMax = clipAreaXRight - ballRadius;
   ballYMin = clipAreaYBottom + ballRadius;
   ballYMax = clipAreaYTop/1.5 - ballRadius;
}
 
void timer(int value) {
   /* Ponovno iscrtavanje prozora */
   glutPostRedisplay();  
   glutTimerFunc(refreshMillis, timer, 0); 
}
 
int main(int argc, char** argv) {

   /* Inicijalizija GLUT-a */
   glutInit(&argc, argv);          
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   
   /* Kreira se prozor */ 
   glutInitWindowSize(windowWidth, windowHeight);  
   glutInitWindowPosition(windowPosX, windowPosY); 
 
   /* Naziv programa */
   glutCreateWindow(title);      
 
    /* Boje svetla */
   GLfloat light_ambient[] = { 0, 0, 0, 1 };
   GLfloat light_diffuse[] = { 1, 1, 0.9, 1 };
   GLfloat light_specular[] = { 1, 1, 1, 1 };
   GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };

   /* OpenGL inicijalizacija. */
   glClearColor (0.0, 0.0, 0.0, 1.0); 
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

   /* Registrovanje funkcija za obradu dogadjaja */
   glutDisplayFunc(display);    
   glutReshapeFunc(reshape);   
   glutKeyboardFunc(keyboard);
   glutTimerFunc(0, timer, 0);  
   
   /* Ulazi se u glavnu petlju */
   glutMainLoop();              
   return 0;
}
