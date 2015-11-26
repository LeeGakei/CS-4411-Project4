#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{ 
	XPOS, YPOS, ZPOS, HEIGHT, ROTATE,

	BODY_X,	//the x length of body
	BODY_Y,
	BODY_Z,
	BODY_ROTATION,	//
	BODY_PX,	//the x length of body
	BODY_PY,
	BODY_PZ,

	UPPER_WING_PX,
	UPPER_WING_PY,
	UPPER_WING_PZ,

	UPPER_WING_ROTATION,
	MIDDLE_WING_ROTATION,
	LOWER_WING_ROTATION,

	UPPER_TAIL_ROTATION1,
	UPPER_TAIL_ROTATION2,
	LOWER_TAIL_ROTATION1,
	LOWER_TAIL_ROTATION2,

	LIGHT0_X,
	LIGHT0_Y,
	LIGHT0_Z,
	LIGHT0_INTENSITY,

	LIGHT1_X,
	LIGHT1_Y,
	LIGHT1_Z,
	LIGHT1_INTENSITY,

	NECK_RADIUS,
	NECK_HEIGHT,

	NECK_ROTATION_PX,
	NECK_ROTATION_PY,

	OARS_ROTATION,
	OARS_SPEED,

	NUMCONTROLS,
};

//texture


// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))
#define SETVAL(c, v)(ModelerApplication::Instance()->SetControlValue(c, v))


#endif