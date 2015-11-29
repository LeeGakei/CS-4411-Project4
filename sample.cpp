// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"
#include "particleSystem.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
	void animate();
	Mat4f getModelViewMatrix();
	void SpawnParticles(Mat4f cameraTransform);

private:
	int timer = 0;
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();

	this->matrix = getModelViewMatrix();

	//animation
	//if (ModelerApplication::Instance()->GetAnimFlag())
	//	this->animate();

	// draw the floor
	setAmbientColor(0.1f,0.1f,0.1f);
	setDiffuseColor(COLOR_RED);

	glPushMatrix();
	glTranslated(VAL(BODY_PX), VAL(BODY_PY), VAL(BODY_PZ));
	glRotated(VAL(BODY_ROTATION), 0.0, 0.0, 1.0);
	drawEllipsoid(VAL(BODY_X), VAL(BODY_Y), VAL(BODY_Z));

	//wing
	{
		glPushMatrix();
		glTranslated(VAL(BODY_X) / VAL(UPPER_WING_PX), VAL(BODY_Y) / VAL(UPPER_WING_PY), -VAL(BODY_Z) / VAL(UPPER_WING_PZ));
		glRotated(VAL(UPPER_WING_ROTATION), 0, 0, 1);
		drawBox(2.5, 0.01f, 2);

			//middle right wing
			glPushMatrix();
			glTranslated(2.5,0,0);
			glRotated(VAL(MIDDLE_WING_ROTATION), 0, 0, 1);
			drawBox(2.5, 0.01f, 2);
			//drawTriangularPrism(3, 2, 0.1f);

				//lower right wing
				glPushMatrix();
				glTranslated(2.5, 0, 1);
				glRotated(VAL(LOWER_WING_ROTATION), 0, 0, 1);
				drawTriangularPrism(3, 2, 0.1f);
				glPopMatrix();

			glPopMatrix();
			
		glPopMatrix();

		glPushMatrix();
		glTranslated(-VAL(BODY_X) / VAL(UPPER_WING_PX), VAL(BODY_Y) / VAL(UPPER_WING_PY), -VAL(BODY_Z) / VAL(UPPER_WING_PZ));
		glRotated(VAL(UPPER_WING_ROTATION), 0, 0, -1);
		drawBox(-2.5, 0.01f, 2);

			//middle left wing
			glPushMatrix();
			glTranslated(-2.5, 0, 0);
			glRotated(VAL(MIDDLE_WING_ROTATION), 0, 0, -1);
			drawBox(-2.5, 0.01f, 2);

				//lower left wing
				glPushMatrix();
				glTranslated(-2.5, 0, 1);
				glRotated(VAL(LOWER_WING_ROTATION), 0, 0, -1);
				drawTriangularPrism(-3, 2, 0.1f);
				glPopMatrix();

			glPopMatrix();
			
		glPopMatrix();
	}

	//tails
	{
		glPushMatrix();
		glTranslated(0, 0, - VAL(BODY_Z) * 0.9);
		glRotated(180,1,1,0);
		glRotated(VAL(UPPER_TAIL_ROTATION1), 1, 0, 0);
		glRotated(VAL(UPPER_TAIL_ROTATION2), 0, 1, 0);
		drawSphere(0.7);
		drawCylinder(2,0.7,0.5);

			//lower tails and torus
			glPushMatrix();
			glTranslated(0, 0, 2);
			glRotated(VAL(LOWER_TAIL_ROTATION1), 1, 0, 0);
			
				//torus
				glPushMatrix();
				glRotated(-90, 1, 0, 0);
				drawTorus(2.5, 0.2);
				glPopMatrix();


				glPushMatrix();
				glRotated(90, 0, 1, 0);
				glRotated(VAL(OARS_ROTATION), 0, 1, 0);
				drawCylinder(2.5, 0, 0.5);
				glPopMatrix();

				glPushMatrix();
				glRotated(90, 0, 1, 0);
				glRotated(120, 0, 1, 0);
				glRotated(VAL(OARS_ROTATION), 0, 1, 0);
				drawCylinder(2.5, 0, 0.5);
				glPopMatrix();

				glPushMatrix();
				glRotated(90, 0, 1, 0);
				glRotated(240, 0, 1, 0);
				glRotated(VAL(OARS_ROTATION), 0, 1, 0);
				drawCylinder(2.5, 0, 0.5);
				glPopMatrix();
			glPopMatrix();

		glPopMatrix();
	}

	//neck
	{
		glPushMatrix();
		glTranslated(0, 0, VAL(BODY_Z));
		glRotated(VAL(NECK_ROTATION_PX), 0, -1, 0);
		glRotated(VAL(NECK_ROTATION_PY), -1, 0, 0);
		drawCylinder(VAL(NECK_HEIGHT), VAL(NECK_RADIUS), VAL(NECK_RADIUS));

		//oars centre
		{
			glPushMatrix();

			glTranslated(0, 0, VAL(NECK_HEIGHT));
			drawSphere(0.5);
			SpawnParticles(this->matrix);

			//3 oars
			{
				glPushMatrix();
				glRotated(90, 1, 0, 0);
				glRotated(VAL(OARS_ROTATION), 0, 1, 0);
				drawCylinder(3, 0, 0.5);
				glPopMatrix();

				glPushMatrix();
				glRotated(90, 1, 0, 0);
				glRotated(120, 0, 1, 0);
				glRotated(VAL(OARS_ROTATION), 0, 1, 0);
				drawCylinder(3, 0, 0.5);
				glPopMatrix();

				glPushMatrix();
				glRotated(90, 1, 0, 0);
				glRotated(240, 0, 1, 0);
				glRotated(VAL(OARS_ROTATION), 0, 1, 0);
				drawCylinder(3, 0, 0.5);
				glPopMatrix();
			}

			glPopMatrix();
		}
		
		glPopMatrix();
	}

	glPopMatrix();
}

void SampleModel::animate(){
	timer++;
	//move body vertically
	if ((timer / 90) % 4 == 0)
		SETVAL(BODY_PX, VAL(BODY_PX) + 0.03);
	else if ((timer / 90) % 4 == 2)
		SETVAL(BODY_PX, VAL(BODY_PX) - 0.03);

	//move body horizontally
	if ((timer / 90) % 4 == 1)
		SETVAL(BODY_PY, VAL(BODY_PY) + 0.01);
	else if ((timer / 90) % 4 == 3)
		SETVAL(BODY_PY, VAL(BODY_PY) - 0.01);


	//move body rotationally
	if ((timer / 90) % 4 == 1)
		SETVAL(BODY_ROTATION, VAL(BODY_ROTATION) + 0.5);
	else if ((timer / 90) % 4 == 3)
		SETVAL(BODY_ROTATION, VAL(BODY_ROTATION) - 0.5);

	
	//move neck
	if ((timer/90)%2==0)
		SETVAL(NECK_HEIGHT, VAL(NECK_HEIGHT) + 0.01);
	else
		SETVAL(NECK_HEIGHT, VAL(NECK_HEIGHT) - 0.01);

	//move torus and oars
	if (VAL(OARS_ROTATION)>=360)
		SETVAL(OARS_ROTATION, 0);
	SETVAL(OARS_ROTATION, VAL(OARS_ROTATION)+VAL(OARS_SPEED));
}

Mat4f SampleModel::getModelViewMatrix(){
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);
	return matMV.transpose(); // because the matrix GL returns is column major
}

void SampleModel::SpawnParticles(Mat4f cameraTransform){
	Mat4f matrix = this->getModelViewMatrix();
	Mat4f inverse = cameraTransform.inverse();
	Mat4f transform = inverse * matrix;
	Vec4<float> result = transform * Vec4<float>(0,0,0,1);

	double x = result[0] / result[3];
	double y = result[1] / result[3];
	double z = result[2] / result[3];
	std::cout << x << " ";
	std::cout << y << " ";
	std::cout << z << " " << "\n";
	ModelerApplication::Instance()->GetParticleSystem()->addParticleStartingAt(x, y, z);
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);

	//body
	controls[BODY_X] = ModelerControl("BODY X",0,5,0.1f,1.1);
	controls[BODY_Y] = ModelerControl("BODY Y",0,5,0.1f,1.6);
	controls[BODY_Z] = ModelerControl("BODY Z",0,5,0.1f,3.9 );
	controls[BODY_ROTATION] = ModelerControl("BODY ROTATION",0,360,1,0);
	controls[BODY_PX] = ModelerControl("BODY PX", 0, 5, 0.1f, 0);
	controls[BODY_PY] = ModelerControl("BODY PY", 0, 5, 0.1f, 0);
	controls[BODY_PZ] = ModelerControl("BODY PZ", 0, 5, 0.1f, 0);

	//WING
	controls[UPPER_WING_PX] = ModelerControl("UPPER WING PX", 0.1, 3, 0.1f, 1.3);
	controls[UPPER_WING_PY] = ModelerControl("UPPER WING PY", 0.1, 3, 0.1f, 3);
	controls[UPPER_WING_PZ] = ModelerControl("UPPER WING PZ", 0.1, 3, 0.1f, 3);
	controls[UPPER_WING_ROTATION] = ModelerControl("UPPER WING ROTATION",  -180, 180, 1, 15);
	controls[MIDDLE_WING_ROTATION] = ModelerControl("MIDDLE WING ROTATION", -180, 180, 1, -34);
	controls[LOWER_WING_ROTATION] = ModelerControl("LOWER WING ROTATION",  -180, 180, 1, 76);

	//TAIL
	controls[UPPER_TAIL_ROTATION1] = ModelerControl("UPPER_TAIL_ROTATION1", -180, 180, 1, 0);
	controls[UPPER_TAIL_ROTATION2] = ModelerControl("UPPER_TAIL_ROTATION2", -180, 180, 1, 90);
	controls[LOWER_TAIL_ROTATION1] = ModelerControl("LOWER_TAIL_ROTATION1", -180, 180, 1, 90);
	controls[LOWER_TAIL_ROTATION2] = ModelerControl("LOWER_TAIL_ROTATION2", -180, 180, 1, 45);

	//LIGHT
	controls[LIGHT0_X] = ModelerControl("LIGHT0_X", -5, 5, 1, 4);
	controls[LIGHT0_Y] = ModelerControl("LIGHT0_Y", -5, 5, 1, 2);
	controls[LIGHT0_Z] = ModelerControl("LIGHT0_Z", -5, 5, 1, -4);
	controls[LIGHT0_INTENSITY] = ModelerControl("LIGHT0_INTENSITY", 0, 1, 0.1, 0.8);

	controls[LIGHT1_X] = ModelerControl("LIGHT1_X", -5, 5, 1, -2);
	controls[LIGHT1_Y] = ModelerControl("LIGHT1_Y", -5, 5, 1, 1);
	controls[LIGHT1_Z] = ModelerControl("LIGHT1_Z", -5, 5, 1, 5);
	controls[LIGHT1_INTENSITY] = ModelerControl("LIGHT1_INTENSITY", 0, 1, 0.1, 0.5);

	//NECK
	controls[NECK_RADIUS] = ModelerControl("NECK RADIUS", 0.1, 3, 0.1f, 0.3);
	controls[NECK_HEIGHT] = ModelerControl("NECK HEIGHT", 0.1, 3, 0.1f, 0.5);

	controls[NECK_ROTATION_PX] = ModelerControl("NECK ROTATION PX", -90, 90, 1, 0);
	controls[NECK_ROTATION_PY] = ModelerControl("NECK ROTATION PY", -90, 90, 1, 0);

	//OARS
	controls[OARS_ROTATION] = ModelerControl("OARS ROTATION", -180, 180, 1, 0);
	controls[OARS_SPEED] = ModelerControl("OARS SPEED", 0, 30, 1, 15);

	//initializeTexture();
    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);

	//hook up particle system
	ParticleSystem *ps = new ParticleSystem();
	ModelerApplication::Instance()->SetParticleSystem(ps);

    return ModelerApplication::Instance()->Run();
}
