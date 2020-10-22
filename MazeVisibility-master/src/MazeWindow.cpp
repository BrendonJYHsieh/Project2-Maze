/************************************************************************
     File:        MazeWindow.cpp

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison

						Class header file for the MazeWindow class. The MazeWindow is
						the window in which the viewer's view of the maze is displayed.
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/

#include "MazeWindow.h"

glm::mat4x4 pmatrix;
glm::mat4x4 modelviewmatrix;
//*************************************************************************
//
// * Constructor
//=========================================================================
MazeWindow::
MazeWindow(int x, int y, int width, int height, const char *label,Maze *m)
	: Fl_Gl_Window(x, y, width, height, label)
//=========================================================================
{
	maze = m;

	// The mouse button isn't down and there is no key pressed.
	down = false;
	z_key = 0;
}


//*************************************************************************
//
// * Set the maze. Also causes a redraw.
//=========================================================================
void MazeWindow::
Set_Maze(Maze *m)
//=========================================================================
{
	// Change the maze
	maze = m;

	// Force a redraw
	redraw();
}
void Perspective(const float& angleOfView, const float& imageAspectRatio, const float& n, const float& f) {
	float r, t, b,l;
	float scale = tan(angleOfView * 0.5 * M_PI / 180) * n;
	r = imageAspectRatio * scale, l = -r;
	t = scale, b = -t;
	GLdouble matrix[16] = { 0 };
	matrix[0] = 2 * n / (r - l);
	matrix[5] = 2 * n / (t - b);
	matrix[8] = (r + l) / (r - l);
	matrix[9] = (t + b) / (t - b);
	matrix[10] = -(f + n) / (f - n);
	matrix[11] = -1;
	matrix[14] = -(2*f * n) / (f - n);
	for (int i = 0; i < 16; i++) {
		pmatrix[i / 4][i % 4] = matrix[i];
	}
	
	glLoadMatrixd(matrix);
}

void LookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
	GLdouble centerx, GLdouble centery, GLdouble centerz,
	GLdouble upx, GLdouble upy, GLdouble upz) {
	GLdouble matrix1[16] = { 0 };
	glm::mat4x4 matrix;
	glm::vec3 eyes,at,ups,x,y,z,temp;
	eyes[0] = eyex;eyes[1] = eyey;eyes[2] = eyez;
	at[0] = centerx;at[1] = centery;at[2] = centerz;
	ups[0] = upx;ups[1] = upy;ups[2] = upz;
	temp[0] = 0;temp[1] = 1;temp[2] = 0;

	z = glm::normalize(eyes- at);
	x = glm::normalize(glm::cross(ups, z));
	y = glm::cross(z, x);

	matrix[0][0] = x[0];
	matrix[1][0] = x[1];
	matrix[2][0] = x[2];
	matrix[0][1] = y[0];
	matrix[1][1] = y[1];
	matrix[2][1] = y[2];
	matrix[0][2] = z[0];
	matrix[1][2] = z[1];
	matrix[2][2] = z[2];
	matrix[3][0] = -1 * glm::dot(x, eyes);
	matrix[3][1] = -1 * glm::dot(y, eyes);
	matrix[3][2] = -1 * glm::dot(z, eyes);
	matrix[0][3] = 0;
	matrix[1][3] = 0;
	matrix[2][3] = 0;
	matrix[3][3] = 1;
	/*
	matrix[0] = x[0];
	matrix[1] = y[0];
	matrix[2] = z[0];
	matrix[3] = 0;
	matrix[4] = x[1];
	matrix[5] = y[1];
	matrix[6] = z[1];
	matrix[7] = 0;
	matrix[8] = x[2];
	matrix[9] = y[2];
	matrix[10] = z[2];
	matrix[11] = 0;
	matrix[12] =-1* glm::dot(x,eyes);
	matrix[13] =-1* glm::dot(y,eyes);
	matrix[14] =-1* glm::dot(z, eyes);
	matrix[15] = 1;*/

	modelviewmatrix=matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix1[i * 4 + j] = modelviewmatrix[i][j];
		}
		cout << endl;
		
	}
	glLoadMatrixd(matrix1);
}
//*************************************************************************
//
// * draw() method invoked whenever the view changes or the window
//   otherwise needs to be redrawn.
//=========================================================================
void MazeWindow::
draw(void)
//=========================================================================
{
	float   focal_length;

	if ( ! valid() ) {
		// The OpenGL context may have been changed
		// Set up the viewport to fill the window.
		glViewport(0, 0, w(), h());

		// We are using orthogonal viewing for 2D. This puts 0,0 in the
		// middle of the screen, and makes the image size in view space
		// the same size as the window.
		gluOrtho2D(-w() * 0.5, w() * 0.5, -h() * 0.5, h() * 0.5);

		// Sets the clear color to black.
		glClearColor(0.0, 0.0, 0.0, 1.0);
	}

	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_QUADS);
		// Draw the "floor". It is an infinite plane perpendicular to
		// vertical, so we know it projects to cover the entire bottom
		// half of the screen. Walls of the maze will be drawn over the top
		// of it.
		glColor3f(0.2f, 0.2f, 0.2f);
		glVertex2f(-w() * 0.5f, -h() * 0.5f);
		glVertex2f( w() * 0.5f, -h() * 0.5f);
		glVertex2f( w() * 0.5f, 0.0       );
		glVertex2f(-w() * 0.5f, 0.0       );

		// Draw the ceiling. It will project to the entire top half
		// of the window.
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex2f( w() * 0.5f,  h() * 0.5f);
		glVertex2f(-w() * 0.5f,  h() * 0.5f);
		glVertex2f(-w() * 0.5f, 0.0       );
		glVertex2f( w() * 0.5f, 0.0       );

		

	glEnd();


	if ( maze ) {
		/*
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glm::mat4x4 Mproj;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Mproj[i][j]=0;
			}
		}
		setProjectionMatrix(maze->viewer_fov, 0.001, 200, Mproj);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				cout << Mproj[i][j]<<" ";
			}
			cout << endl;
		}*/

		focal_length = w()/ (float)(2.0*tan(Maze::To_Radians(maze->viewer_fov)*0.5));
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float aspect = (float)w() / h();
		Perspective(maze->viewer_fov,aspect,0.01, 200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		float viewer_pos[3] = { maze->viewer_posn[Maze::Y],0.0f,maze->viewer_posn[Maze::X] };
		LookAt(viewer_pos[Maze::X], viewer_pos[Maze::Y], viewer_pos[Maze::Z],
			viewer_pos[Maze::X] + sin(Maze::To_Radians(maze->viewer_dir)),
			viewer_pos[Maze::Y],
			viewer_pos[Maze::Z] + cos(Maze::To_Radians(maze->viewer_dir)),
			0.0, 1.0, 0.0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		maze->Draw_View(focal_length);
	}
}


//*************************************************************************
//
// *
//=========================================================================
bool MazeWindow::
Drag(float dt)
//=========================================================================
{
	float   x_move, y_move, z_move;

	if ( down ) {
		int	dx = x_down - x_last;
		int   dy = y_down - y_last;
		float dist;

		// Set the viewing direction based on horizontal mouse motion.
		maze->Set_View_Dir(d_down + 360.0f * dx / (float)w());

		// Set the viewer's linear motion based on a speed (derived from
		// vertical mouse motion), the elapsed time and the viewing direction.
		dist = 10.0f * dt * dy / (float)h();
		x_move = dist * (float)cos(Maze::To_Radians(maze->viewer_dir));
		y_move = dist * (float)sin(Maze::To_Radians(maze->viewer_dir));
	}
	else {
		x_move = 0.0;
		y_move = 0.0;
	}

	// Update the z posn
	z_move = z_key * 0.1f;
	z_key = 0;

	// Tell the maze how much the view has moved. It may restrict the motion
	// if it tries to go through walls.
	maze->Move_View_Posn(x_move, y_move, z_move);

	return true;
}


//*************************************************************************
//
// *
//=========================================================================
bool MazeWindow::
Update(float dt)
//=========================================================================
{
	// Update the view

	if ( down || z_key ) // Only do anything if the mouse button is down.
		return Drag(dt);

	// Nothing changed, so no need for a redraw.
	return false;
}


//*************************************************************************
//
// *
//=========================================================================
int MazeWindow::
handle(int event)
//=========================================================================
{
	if (!maze)
		return Fl_Gl_Window::handle(event);

	// Event handling routine.
	switch ( event ) {
		case FL_PUSH:
			down = true;
			x_last = x_down = Fl::event_x();
			y_last = y_down = Fl::event_y();
			d_down = maze->viewer_dir;
			return 1;
		case FL_DRAG:
			x_last = Fl::event_x();
			y_last = Fl::event_y();
			return 1;
			case FL_RELEASE:
			down = false;
			return 1;
		case FL_KEYBOARD:
			/*
			if ( Fl::event_key() == FL_Up )	{
				z_key = 1;
				return 1;
			}
			if ( Fl::event_key() == FL_Down ){
				z_key = -1;
				return 1;
			}
			*/
			return Fl_Gl_Window::handle(event);
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;
	}

	// Pass any other event types on the superclass.
	return Fl_Gl_Window::handle(event);
}


