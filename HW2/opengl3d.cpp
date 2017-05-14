#define _CRT_SECURE_NO_WARNINGS

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> //inverse, affineInverse, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewProjectionMatrix, ViewMatrix, ProjectionMatrix;

glm::mat4 ModelMatrix_CAR_BODY, ModelMatrix_CAR_WHEEL, ModelMatrix_CAR_NUT, ModelMatrix_CAR_DRIVER;
glm::mat4 ModelMatrix_CAR_BODY_to_DRIVER; // computed only once in initialize_camera()
glm::mat4 ModelMatrix_COW_BODY_to_COW_EYE; // computed only once in initialize_camera()

glm::mat4 ModelMatrix_OBJECT;

#define CAM_ROT_SENSITIVITY 0.15f
int leftbutton_pressed = 0;
int modify_prp = 0;

typedef struct {
	glm::vec3 prp, vrp, vup; // in this example code, make vup always equal to the v direction.
	float fov_y, aspect_ratio, near_clip, far_clip, zoom_factor;
} CAMERA;

#define NUMBER_OF_CAMERAS 3

CAMERA camera[NUMBER_OF_CAMERAS];
glm::mat4 viewProjectionMatrix[NUMBER_OF_CAMERAS], viewMatrix[NUMBER_OF_CAMERAS], projectionMatrix[NUMBER_OF_CAMERAS];
glm::mat4 modelViewProjectionMatrix; // This one is sent to vertex shader when it is ready.

#include "Camera.h"
#include "Geometry.h"
#include "Car.h"

int timer_car, timer_optimus, timer_boxes, timer_airplane, timer_robot;

float rotation_angle_car = 0.0f;
int stop_car = 0;

float rotation_angle_airplane = 0.0f;
int stop_airplane = 0;

int time_optimus = 0;
float rotation_angle_optimus = 0.0f;
int stop_optimus = 0;
int stop_optimus_rotate = 0;

float rotation_angle_boxes = 0.0f;
int stop_boxes = 0;

int rotation_angle_robot_arm = 0;
int stop_robot_arm = 0;

int rotation_angle_robot_leg = 0;
int stop_robot_leg = 0;

int rotation_angle_robot_body = 0.0f;
int stop_robot_body = 0;

int move_up = 0;

int rotate_uaxis = 0, rotate_vaxis = 0, rotate_naxis = 0;

void draw_objects_in_world(void) {
	//glm::mat4 ModelMatrix_OBJECT;
	int i;
	float angle, size, self_rotation_angle;
	glm::mat4 Matrix_TIGER_tmp;

	Matrix_TIGER_tmp = glm::rotate(glm::mat4(1.0f), -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	Matrix_TIGER_tmp = glm::scale(Matrix_TIGER_tmp, glm::vec3(0.01f, 0.01f, 0.01f));

	ModelMatrix_OBJECT = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, -35.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -rotation_angle_car, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(5.0f, 5.0f, 5.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	glUniform3f(loc_primitive_color, 0.980f, 0.980f, 0.824f); // color name: LightGoldenrodYellow  
	draw_geom_obj(GEOM_OBJ_ID_COW); // draw cow

	ModelMatrix_OBJECT = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 4.0f, -25.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(5.0f, 5.0f, 5.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	glUniform3f(loc_primitive_color, 0.741f, 0.718f, 0.420f); // color name: DarkKhaki
	draw_geom_obj(GEOM_OBJ_ID_COW); // draw cow

	ModelMatrix_OBJECT = glm::translate(glm::mat4(1.0f), glm::vec3(-42.0f, 2.0f, 26.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(2.0f, 2.0f, 2.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	glUniform3f(loc_primitive_color, 0.000f, 1.000f, 0.498f); // color name: SpringGreen
	draw_geom_obj(GEOM_OBJ_ID_TEAPOT); // draw teapot

	ModelMatrix_OBJECT = glm::translate(glm::mat4(1.0f), glm::vec3(22.0f, 4.0f, -15.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, 5.0f*rotation_angle_car, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(1.5f, 1.5f, 1.5f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	glUniform3f(loc_primitive_color, 0.529f, 0.808f, 0.980f); // color name: LightSkyBlue
	draw_geom_obj(GEOM_OBJ_ID_TEAPOT); // draw teapot

	static float jump_height = 0.0f;
	if(!stop_optimus) {
		if(time_optimus >= 0 && time_optimus <= 180)
			jump_height += 0.05f;
		else 
			jump_height -= 0.05f;
	}

	ModelMatrix_OBJECT = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, jump_height, -15.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, 5.0f*rotation_angle_optimus, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(0.3f, 0.3f, 0.3f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	ModelViewProjectionMatrix *= Matrix_TIGER_tmp;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_OPTIMUS, 0xcc / 255.0f, 0x00 / 255.0f, 0x00 / 255.0f); // Red

	for (i = 0; i < 18; i++) {
		angle = TO_RADIAN*20.0f*i;
		size = 1.0f + 2.0f*fabs(cosf(angle));
		if (i % 2 == 0) {
			self_rotation_angle = 0.1f*i*rotation_angle_boxes;
			glUniform3f(loc_primitive_color, 1.000f, 0.843f, 0.000f); // color name: Gold
		}
		else {
			self_rotation_angle = -0.5f*i*rotation_angle_boxes;
			glUniform3f(loc_primitive_color, 0.804f, 0.361f, 0.361f); // color name: IndianRed
		}
		//ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(0.0f, -0.3f, 0.0f));
		ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(60.0f, 2.0f, 0.0f));
		ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, self_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(size, size, size));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		draw_geom_obj(GEOM_OBJ_ID_BOX); // draw box

		ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(0.0f, 2.2f, 0.0f));
		//ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, 10.0f*rotation_angle_car, glm::vec3(0.0f, 1.0f, 0.0f));
		//ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		if (i % 6 == 1 || i % 6 == 4 || i % 6 == 5) {  // arange cow's movement and size
			ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(0.25f, -0.3f, 0.0f));
			ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, 10.0f*rotation_angle_boxes, glm::vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(3.0f, 3.0f, 3.0f));
		}
		if (i % 6 == 0) {  // arange teapot's size
			ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(0.0f, -0.5f, 0.0f));
			ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, 180.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(0.5f, 0.5f, 0.5f));
		}
		if (i % 6 == 3) {  // arange teapot's size
			ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(0.0f, -0.5f, 0.0f));
			ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, 180.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(0.5f, 0.5f, 0.5f));
		}
		if (i % 6 == 2) {
			ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(0.0f, -0.2f, 0.0f));
		}
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		switch (i % 6) {
		case 0:
			glUniform3f(loc_primitive_color, 0.941f, 1.000f, 0.941f); // color name: HoneyDue
			draw_geom_obj(GEOM_OBJ_ID_TEAPOT); // draw teapot
			break;
		case 1:
			glUniform3f(loc_primitive_color, 0.804f, 0.361f, 0.361f); // color name: IndianRed
			draw_geom_obj(GEOM_OBJ_ID_COW); // draw cow
			break;
		case 2:
			glUniform3f(loc_primitive_color, 0.184f, 0.310f, 0.310f); // color name: DarkSlateGray
			draw_geom_obj(GEOM_OBJ_ID_BOX); // draw box
			break;
		case 3:
			glUniform3f(loc_primitive_color, 0.184f, 0.310f, 0.310f); // color name: DarkSlateGray
			draw_geom_obj(GEOM_OBJ_ID_TEAPOT); // draw teapot
			break;
		case 4:
			glUniform3f(loc_primitive_color, 1.000f, 0.843f, 0.000f); // color name: gold
			draw_geom_obj(GEOM_OBJ_ID_COW); // draw cow
			break;
		case 5:
			glUniform3f(loc_primitive_color, 0.941f, 1.000f, 0.941f); // color name: HoneyDue
			draw_geom_obj(GEOM_OBJ_ID_COW); // draw cow
			break;
		}
		//draw_geom_obj(GEOM_OBJ_ID_TEAPOT); // draw teapot
	}

	//ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(0.0f, 0.0f, 4.25f));
	//ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 180.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
	ModelViewProjectionMatrix *= Matrix_TIGER_tmp;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_OPTIMUS, 0 / 255.0f, 255 / 255.0f, 255 / 255.0f); // Cyan
	
	//ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(-2.0f, 1.0f, -3.5f));
	ModelMatrix_CAR_BODY = glm::rotate(glm::mat4(1.0f), -rotation_angle_airplane, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_CAR_BODY = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(20.0f, 4.89f, 0.0f));
	ModelMatrix_CAR_BODY = glm::rotate(ModelMatrix_CAR_BODY, 90.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	ModelMatrix_OBJECT = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(0.0f, 20.0f, -50.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(10.0f, 10.0f, 10.0f));

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 30.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
	//ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 30.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix *= Matrix_TIGER_tmp;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_AIRPLANE, 255 / 255.0f, 255 / 255.0f, 255 / 255.0f); // White

	//ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[camera_index], glm::vec3(0.0f, 0.0f, -3.5f));
	ModelMatrix_OBJECT = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 0.0f, 15.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(10.0f, 10.0f, 10.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	ModelViewProjectionMatrix *= Matrix_TIGER_tmp;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_DRAGON, 0 / 255.0f, 0 / 255.0f, 255 / 255.0f); // Blue

	ModelMatrix_OBJECT = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(10.0f, 10.0f, 10.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_OBJECT;
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, -135.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix *= Matrix_TIGER_tmp;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_TIGER, 255 / 255.0f, 0 / 255.0f, 0 / 255.0f); // Red

	// draw a robot
	float size_x = 0.6f, size_y = 1.5, size_z = 0.6;
	glm::mat4 ModelMatrix_ROBOT_BODY;
	glm::mat4 ModelMatrix_ROBOT_ARM;
	glm::mat4 ModelMatrix_ROBOT_LEG;
	glm::mat4 ModelMatrix_ROBOT_HEAD;

	// body
	ModelMatrix_ROBOT_BODY = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 35.0f));
	ModelMatrix_ROBOT_BODY = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_ROBOT_BODY = glm::rotate(ModelMatrix_ROBOT_BODY, (rotation_angle_robot_body-90)*-2.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_ROBOT_BODY = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(10.0f, 0.0f, 0.0f));
	ModelMatrix_ROBOT_BODY = glm::rotate(ModelMatrix_ROBOT_BODY, (rotation_angle_robot_body-90)*2.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	//ModelMatrix_ROBOT_BODY = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(0.0f, 1.0f, 0.0f));
	//ModelMatrix_ROBOT_BODY = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(0.0f, 4.0f, 35.0f));
	//ModelMatrix_ROBOT_BODY = glm::rotate(ModelMatrix_ROBOT_BODY, rotation_angle_car, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_ROBOT_BODY = glm::rotate(ModelMatrix_ROBOT_BODY, 90.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_ROBOT_BODY = glm::scale(ModelMatrix_ROBOT_BODY, glm::vec3(size_x, size_y, size_z));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_ROBOT_BODY;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.529f, 0.808f, 0.980f); // color name: LightSkyBlue
	draw_geom_obj(GEOM_OBJ_ID_BOX); // draw robot body
	
	// arms
	size_x = size_z = 0.3f;
	size_y = 1.0f;
	static float arm_rotate_angle = 0.0f;
	if(!stop_robot_arm) {
		if(rotation_angle_robot_arm < 20) {
			arm_rotate_angle += 2.0f;
		}
		else {
			arm_rotate_angle -= 2.0f;
		}
	}
	ModelMatrix_ROBOT_ARM = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(1.5f, -0.2f, 0.0f));
	ModelMatrix_ROBOT_ARM = glm::translate(ModelMatrix_ROBOT_ARM, glm::vec3(0.0f, 0.5f, 0.0f));
	ModelMatrix_ROBOT_ARM = glm::rotate(ModelMatrix_ROBOT_ARM, arm_rotate_angle*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_ROBOT_ARM = glm::translate(ModelMatrix_ROBOT_ARM, glm::vec3(0.0f, -0.5f, 0.0f));
	ModelMatrix_ROBOT_ARM = glm::scale(ModelMatrix_ROBOT_ARM, glm::vec3(size_x, size_y, size_z));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_ROBOT_ARM;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.529f, 0.808f, 0.980f); // color name: LightSkyBlue
	draw_geom_obj(GEOM_OBJ_ID_BOX); // draw robot arm

	ModelMatrix_ROBOT_ARM = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(-1.5f, -0.2f, 0.0f));
	ModelMatrix_ROBOT_ARM = glm::translate(ModelMatrix_ROBOT_ARM, glm::vec3(0.0f, 0.5f, 0.0f));
	ModelMatrix_ROBOT_ARM = glm::rotate(ModelMatrix_ROBOT_ARM, -arm_rotate_angle*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_ROBOT_ARM = glm::translate(ModelMatrix_ROBOT_ARM, glm::vec3(0.0f, -0.5f, 0.0f));
	ModelMatrix_ROBOT_ARM = glm::scale(ModelMatrix_ROBOT_ARM, glm::vec3(size_x, size_y, size_z));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_ROBOT_ARM;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.529f, 0.808f, 0.980f); // color name: LightSkyBlue
	draw_geom_obj(GEOM_OBJ_ID_BOX); // draw robot arm

	// legs
	size_x = size_z = 0.3f;
	size_y = 1.0f;
	static float leg_rotate_angle = 0.0f;
	if(!stop_robot_leg) {
		//printf("r_angle: %d, leg_angle: %f\n", rotation_angle_robot_leg, leg_rotate_angle);
		if(rotation_angle_robot_leg < 20) {
			leg_rotate_angle += 2.0f;
		}
		else {
			leg_rotate_angle -= 2.0f;
		}
	}
	ModelMatrix_ROBOT_LEG = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(0.8f, -2.0f, 0.0f));
	ModelMatrix_ROBOT_LEG = glm::translate(ModelMatrix_ROBOT_LEG, glm::vec3(0.0f, 0.5f, 0.0f));
	ModelMatrix_ROBOT_LEG = glm::rotate(ModelMatrix_ROBOT_LEG, -leg_rotate_angle*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_ROBOT_LEG = glm::translate(ModelMatrix_ROBOT_LEG, glm::vec3(0.0f, -0.5f, 0.0f));
	ModelMatrix_ROBOT_LEG = glm::scale(ModelMatrix_ROBOT_LEG, glm::vec3(size_x, size_y, size_z));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_ROBOT_LEG;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.529f, 0.808f, 0.980f); // color name: LightSkyBlue
	draw_geom_obj(GEOM_OBJ_ID_BOX); // draw robot leg

	ModelMatrix_ROBOT_LEG = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(-0.8f, -2.0f, 0.0f));
	ModelMatrix_ROBOT_LEG = glm::translate(ModelMatrix_ROBOT_LEG, glm::vec3(0.0f, 0.5f, 0.0f));
	ModelMatrix_ROBOT_LEG = glm::rotate(ModelMatrix_ROBOT_LEG, leg_rotate_angle*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_ROBOT_LEG = glm::translate(ModelMatrix_ROBOT_LEG, glm::vec3(0.0f, -0.5f, 0.0f));
	ModelMatrix_ROBOT_LEG = glm::scale(ModelMatrix_ROBOT_LEG, glm::vec3(size_x, size_y, size_z));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_ROBOT_LEG;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.529f, 0.808f, 0.980f); // color name: LightSkyBlue
	draw_geom_obj(GEOM_OBJ_ID_BOX); // draw robot leg

	// head
	size_x = size_y = size_z = 0.5f;
	ModelMatrix_ROBOT_HEAD = glm::translate(ModelMatrix_ROBOT_BODY, glm::vec3(0.0f, 1.8f, 0.0f));
	//ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, self_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_ROBOT_HEAD = glm::rotate(ModelMatrix_ROBOT_HEAD, 70.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_ROBOT_HEAD = glm::scale(ModelMatrix_ROBOT_HEAD, glm::vec3(size_x, size_y, size_z));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_ROBOT_HEAD;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.529f, 0.808f, 0.980f); // color name: LightSkyBlue
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL); // draw robot head
}

#define rad 1.7f
#define ww 1.0f
void draw_wheel_and_nut() {
	// angle is used in Hierarchical_Car_Correct later
	int i;

	glUniform3f(loc_primitive_color, 0.000f, 0.808f, 0.820f); // color name: DarkTurquoise
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL); // draw wheel

	for (i = 0; i < 5; i++) {
		ModelMatrix_CAR_NUT = glm::rotate(ModelMatrix_CAR_WHEEL, TO_RADIAN*72.0f*i, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix_CAR_NUT = glm::translate(ModelMatrix_CAR_NUT, glm::vec3(rad - 0.5f, 0.0f, ww));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_NUT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		glUniform3f(loc_primitive_color, 0.690f, 0.769f, 0.871f); // color name: LightSteelBlue
		draw_geom_obj(GEOM_OBJ_ID_CAR_NUT); // draw i-th nut
	}
}

void draw_car_dummy(void) {
	glUniform3f(loc_primitive_color, 0.498f, 1.000f, 0.831f); // color name: Aquamarine
	draw_geom_obj(GEOM_OBJ_ID_CAR_BODY); // draw body

	glLineWidth(2.0f);
	draw_axes(); // draw MC axes of body
	glLineWidth(1.0f);

	ModelMatrix_CAR_DRIVER = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.0f, 0.5f, 2.5f));
	ModelMatrix_CAR_DRIVER = glm::rotate(ModelMatrix_CAR_DRIVER, TO_RADIAN*90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_DRIVER;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(5.0f);
	draw_axes(); // draw camera frame at driver seat
	glLineWidth(1.0f);

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, 4.5f));
	//printf("*** Wheel0(roll) rotation angle: %f\n", car_status.front_wheel_roll_angle);
	//printf("*** Wheel0(turn) rotation angle: %f\n", car_status.front_wheel_turn_angle);
	if(!stop_car) {
		car_status.front_wheel_roll_angle += FRONT_WHEEL_ROLL_ANGLE_DELTA;
		update_car_front_wheel_rotate_matrix();
		ModelMatrix_CAR_WHEEL *= car_status.Matrix_CAR_WHEEL_ROTATE;	// wheel rotates as car moves
	}
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 0

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, 4.5f));
	//printf("*** Wheel1(roll) rotation angle: %f\n", car_status.front_wheel_roll_angle);
	//printf("*** Wheel1(turn) rotation angle: %f\n", car_status.front_wheel_turn_angle);
	if(!stop_car) {
		car_status.front_wheel_roll_angle += FRONT_WHEEL_ROLL_ANGLE_DELTA;
		update_car_front_wheel_rotate_matrix();
		ModelMatrix_CAR_WHEEL *= car_status.Matrix_CAR_WHEEL_ROTATE;	// wheel rotates as car moves
	}
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 1

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, -4.5f));
	//printf("*** Wheel2(roll) rotation angle: %f\n", car_status.front_wheel_roll_angle);
	//printf("*** Wheel2(turn) rotation angle: %f\n", car_status.front_wheel_turn_angle);
	if(!stop_car) {
		car_status.front_wheel_roll_angle += FRONT_WHEEL_ROLL_ANGLE_DELTA;
		update_car_front_wheel_rotate_matrix();
		ModelMatrix_CAR_WHEEL *= car_status.Matrix_CAR_WHEEL_ROTATE;	// wheel rotates as car moves
		//ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	}
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 2

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, -4.5f));
	//printf("*** Wheel3(roll) rotation angle: %f\n", car_status.front_wheel_roll_angle);
	//printf("*** Wheel3(turn) rotation angle: %f\n", car_status.front_wheel_turn_angle);
	if(!stop_car) {
		car_status.front_wheel_roll_angle += FRONT_WHEEL_ROLL_ANGLE_DELTA;
		update_car_front_wheel_rotate_matrix();
		ModelMatrix_CAR_WHEEL *= car_status.Matrix_CAR_WHEEL_ROTATE;	// wheel rotates as car moves
		//ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	}
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 3
}

/*********************************  START: callbacks *********************************/
int flag_draw_world_objects = 1;

void display(void) {
	glm::mat4 ModelMatrix_big_cow, ModelMatrix_small_cow;
	glm::mat4 ModelMatrix_big_box, ModelMatrix_small_box;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ModelMatrix_CAR_BODY = glm::rotate(glm::mat4(1.0f), -rotation_angle_car, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_CAR_BODY = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(20.0f, 4.89f, 0.0f));
	ModelMatrix_CAR_BODY = glm::rotate(ModelMatrix_CAR_BODY, 90.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	if (camera_type == CAMERA_DRIVER) set_ViewMatrix_for_driver();

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_BODY;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car_dummy();

	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes();
	glLineWidth(1.0f);

	ModelViewProjectionMatrix = ViewProjectionMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_path();

	if (flag_draw_world_objects)
		draw_objects_in_world();

	glutSwapBuffers();
}

int prevx, prevy;
int viewDragon = 0;
float prp = -20.0f;

void motion(int x, int y) {
	if (!camera_wv.move || (camera_type != CAMERA_WORLD_VIEWER)) 
		return;

	// Move camera 
	// through camera's z or y direction
	if(!move_up)
		renew_cam_position_z(prevy - y);
	else
		renew_cam_position_y(prevy - y);
	// through camera's x direction
	renew_cam_position_x(prevx - x);

	// Rotate camera
	//if(rotate_vaxis)
	//renew_cam_orientation_rotation_around_v_axis(prevx - x);

	prevx = x; prevy = y;

	set_ViewMatrix_for_world_viewer();
	if(camera_type == CAMERA_WORLD_VIEWER)
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	//glutPostRedisplay();
}

void motion_1(int x, int y) {
	glm::mat4 mat4_tmp;
	glm::vec3 vec3_tmp;
	float delx, dely;

	if (leftbutton_pressed) {
		if(modify_prp) {
			prp += (prevy - y);	// moving the mouse up or down changes PRP of the camera
			camera[0].prp = glm::vec3(0.0f, 0.0f, prp);
		}
		delx = (float)(x - prevx), dely = -(float)(y - prevy);
		prevx = x, prevy = y;

		mat4_tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
		mat4_tmp = glm::rotate(mat4_tmp, CAM_ROT_SENSITIVITY*delx*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
		mat4_tmp = glm::translate(mat4_tmp, -camera[0].vrp);

 		camera[0].prp = glm::vec3(mat4_tmp*glm::vec4(camera[0].prp, 1.0f));
 		camera[0].vup = glm::vec3(mat4_tmp*glm::vec4(camera[0].vup, 0.0f));

		vec3_tmp = glm::cross(camera[0].vup, camera[0].vrp - camera[0].prp);
		mat4_tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
		mat4_tmp = glm::rotate(mat4_tmp, CAM_ROT_SENSITIVITY*dely*TO_RADIAN, vec3_tmp);
		mat4_tmp = glm::translate(mat4_tmp, -camera[0].vrp);

 		camera[0].prp = glm::vec3(mat4_tmp*glm::vec4(camera[0].prp, 1.0f));
	 	camera[0].vup = glm::vec3(mat4_tmp*glm::vec4(camera[0].vup, 0.0f));

		ViewMatrix = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		//glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'f':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 'l':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'd':
		camera_type = CAMERA_DRIVER;
		glutPostRedisplay();
		break;
	case 'w':
		if(viewDragon) {
			viewDragon = 0;
			set_ViewMatrix_for_world_viewer();
			glutMotionFunc(motion);
		}
		camera_type = CAMERA_WORLD_VIEWER;
		set_ViewMatrix_for_world_viewer();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 'o':
		flag_draw_world_objects = 1 - flag_draw_world_objects;
		glutPostRedisplay();
		break;
	case 'y':
		if (!move_up)
			move_up = 1;
		else
			move_up = 0;
		break;
	case 'u':
		if(camera_type == CAMERA_WORLD_VIEWER) {
			renew_cam_orientation_rotation_around_u_axis(1);
			set_ViewMatrix_for_world_viewer();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}
		else if(camera_type == CAMERA_DRIVER) {
			renew_cam_orientation_rotation_around_cam_driver_u_axis(1);
			//set_ViewMatrix_for_world_viewer();
			set_ViewMatrix_for_driver();
			ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 'i':
		if(camera_type == CAMERA_WORLD_VIEWER) {
			renew_cam_orientation_rotation_around_u_axis(-1);
			set_ViewMatrix_for_world_viewer();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}
		else if(camera_type == CAMERA_DRIVER) {
			renew_cam_orientation_rotation_around_cam_driver_u_axis(-1);
			//set_ViewMatrix_for_world_viewer();
			set_ViewMatrix_for_driver();
			ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 'v':
		if(camera_type == CAMERA_WORLD_VIEWER) {
			renew_cam_orientation_rotation_around_v_axis(1);
			set_ViewMatrix_for_world_viewer();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}
		else if(camera_type == CAMERA_DRIVER) {
			renew_cam_orientation_rotation_around_cam_driver_v_axis(1);
			//set_ViewMatrix_for_world_viewer();
			set_ViewMatrix_for_driver();
			ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 'b':
		if(camera_type == CAMERA_WORLD_VIEWER) {
			renew_cam_orientation_rotation_around_v_axis(-1);
			set_ViewMatrix_for_world_viewer();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}
		else if(camera_type == CAMERA_DRIVER) {
			renew_cam_orientation_rotation_around_cam_driver_v_axis(-1);
			//set_ViewMatrix_for_world_viewer();
			set_ViewMatrix_for_driver();
			ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 'n':
		if(camera_type == CAMERA_WORLD_VIEWER) {
			renew_cam_orientation_rotation_around_n_axis(1);
			set_ViewMatrix_for_world_viewer();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}
		else if(camera_type == CAMERA_DRIVER) {
			renew_cam_orientation_rotation_around_cam_driver_n_axis(1);
			//set_ViewMatrix_for_world_viewer();
			set_ViewMatrix_for_driver();
			ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 'm':
		if(camera_type == CAMERA_WORLD_VIEWER) {
			renew_cam_orientation_rotation_around_n_axis(-1);
			set_ViewMatrix_for_world_viewer();
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}
		else if(camera_type == CAMERA_DRIVER) {
			renew_cam_orientation_rotation_around_cam_driver_n_axis(-1);
			//set_ViewMatrix_for_world_viewer();
			set_ViewMatrix_for_driver();
			ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 't':
		if(!viewDragon) {
			puts("***** viewDragon mode is set! ****");
			viewDragon = 1;
			glutMotionFunc(motion_1);
			camera[0].vrp = glm::vec3(-30.0f, 0.0f, 15.0f); // look at the blue dragon
			ViewMatrix = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			// in this example code, make vup always equal to the v direction.
			camera[0].vup = glm::vec3(ViewMatrix[0].y, ViewMatrix[1].y, ViewMatrix[2].y);
			if(camera_type == CAMERA_WORLD_VIEWER)
				ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			else if(camera_type == CAMERA_DRIVER)
				ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		else {
			viewDragon = 0;
			//prp = -20.0f;
			modify_prp = 0;
			set_ViewMatrix_for_world_viewer();
			/*camera[0].prp = glm::vec3(0.0f, 0.0f, prp);
			ViewMatrix = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();*/
			glutMotionFunc(motion);
		}
		break;
	case 'p':
		if(viewDragon) {
			if(!modify_prp)	modify_prp = 1;
			else  modify_prp = 0;
		}
		break;
	case 'z':
		// zoom out (the size of objects filmed in the camera becomes smaller)
		if(camera_type == CAMERA_DRIVER) {
			camera[1].zoom_factor *= 1.05f;
			if(camera[1].zoom_factor > 6.0f)
				camera[1].zoom_factor = 6.0f;
			printf("[driver] camera[1].zoom_factor: %f\n", camera[1].zoom_factor);
			projectionMatrix[1] = glm::perspective(camera[1].zoom_factor*TO_RADIAN*camera[1].fov_y, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
			ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		else if(camera_type == CAMERA_WORLD_VIEWER) {
			camera_wv.zoom_factor *= 1.05f;
			if(camera_wv.zoom_factor >= 1.3f)
				camera_wv.zoom_factor = 1.3f;
			printf("[world] camera_wv.zoom_factor: %f\n", camera_wv.zoom_factor);
			ProjectionMatrix = glm::perspective(camera_wv.zoom_factor*TO_RADIAN*camera_wv.fovy, camera_wv.aspect_ratio, camera_wv.near_c, camera_wv.far_c);
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}
		//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 'x':
		// zoom in (the size of objects filmed in the camera becomes larger)
		if(camera_type == CAMERA_DRIVER) {
			camera[1].zoom_factor *= 0.95f;
			if(camera[1].zoom_factor < 0.1f)
				camera[1].zoom_factor = 0.1f;
			printf("[driver] camera[1].zoom_factor: %f\n", camera[1].zoom_factor);
			projectionMatrix[1] = glm::perspective(camera[1].zoom_factor*TO_RADIAN*camera[1].fov_y, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
			ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
		}
		else if(camera_type == CAMERA_WORLD_VIEWER) {
			camera_wv.zoom_factor *= 0.95f;
			if(camera_wv.zoom_factor < 0.1f)
				camera_wv.zoom_factor = 0.1f;
			printf("[world] camera_wv.zoom_factor: %f\n", camera_wv.zoom_factor);
			ProjectionMatrix = glm::perspective(camera_wv.zoom_factor*TO_RADIAN*camera_wv.fovy, camera_wv.aspect_ratio, camera_wv.near_c, camera_wv.far_c);
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}
		//ProjectionMatrix = glm::perspective(camera[1].zoom_factor*TO_RADIAN*camera[1].fov_y, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
		//ProjectionMatrix = glm::perspective(camera[1].zoom_factor*TO_RADIAN, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
		//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case '1':
		if(!stop_car)
			stop_car = 1;
		else
			stop_car = 0;
		break;
	case '2':
		if(!stop_optimus)
			stop_optimus = stop_optimus_rotate = 1;
		else
			stop_optimus = stop_optimus_rotate = 0;
		break;
	case '3':
		if(!stop_boxes)
			stop_boxes = 1;
		else
			stop_boxes = 0;
		break;
	case '4':
		if(!stop_airplane)
			stop_airplane = 1;
		else
			stop_airplane = 0;
		break;
	case '5':
		stop_robot_arm = -stop_robot_arm + 1;
		stop_robot_leg = -stop_robot_leg + 1;
		stop_robot_body = -stop_robot_body + 1;
		break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
	//update_car_front_wheel_rotate_matrix();
	//glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON)) {
		leftbutton_pressed = 1;
		if (state == GLUT_DOWN) {
			camera_wv.move = 1; 
			prevx = x; prevy = y;
		}
		else if (state == GLUT_UP) camera_wv.move = 0;
	}
	else {
		leftbutton_pressed = 0;
	}
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	
	if(camera_type == CAMERA_WORLD_VIEWER) {
		camera_wv.aspect_ratio = (float)width / height;
		ProjectionMatrix = glm::perspective(camera_wv.zoom_factor*TO_RADIAN*camera_wv.fovy, camera_wv.aspect_ratio, camera_wv.near_c, camera_wv.far_c);
		//ProjectionMatrix = glm::perspective(camera[1].zoom_factor*TO_RADIAN*camera[1].fov_y, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}
	else if(camera_type == CAMERA_DRIVER) {
		camera[1].aspect_ratio = (float)width / height;
		//ProjectionMatrix = glm::perspective(camera_wv.zoom_factor*TO_RADIAN*camera_wv.fovy, camera_wv.aspect_ratio, camera_wv.near_c, camera_wv.far_c);
		printf("<reshape> [driver] camera[1].zoom_factor: %f\n", camera[1].zoom_factor);
		projectionMatrix[1] = glm::perspective(camera[1].zoom_factor*TO_RADIAN*camera[1].fov_y, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
		ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
	}
	//ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	glutPostRedisplay();
}

void timer_scene(int timestamp_scene) {
	if(!stop_car) {
		rotation_angle_car = (timer_car % 360)*TO_RADIAN;
		timer_car = (timer_car + 1) % INT_MAX;
	}
	if(!stop_airplane) {
		rotation_angle_airplane = (timer_airplane % 360)*TO_RADIAN;
		timer_airplane = (timer_airplane + 1) % INT_MAX;
	}
	if(!stop_optimus) {
		time_optimus = (timer_optimus % 360);
		timer_optimus = (timer_optimus + 1) % INT_MAX;
	}
	if(!stop_optimus_rotate) {
		rotation_angle_optimus = (timer_optimus % 360)*TO_RADIAN;
		timer_optimus = (timer_optimus + 1) % INT_MAX;
	}
	if(!stop_boxes) {
		rotation_angle_boxes = (timer_boxes % 360)*TO_RADIAN;
		timer_boxes = (timer_boxes + 1) % INT_MAX;
	}
	if(!stop_robot_arm) {
		rotation_angle_robot_arm = (timer_robot % 40);
		timer_robot = (timer_robot + 1) % INT_MAX;
	}
	if(!stop_robot_leg) {
		rotation_angle_robot_leg = (timer_robot % 40);
		timer_robot = (timer_robot + 1) % INT_MAX;
	}
	if(!stop_robot_body) {
		rotation_angle_robot_body = (timer_robot % 360);
		timer_robot = (timer_robot + 1) % INT_MAX;
	}
	glutPostRedisplay();
	glutTimerFunc(100, timer_scene, (timestamp_scene + 1) % INT_MAX);
}

void cleanup(void) {
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);
}
/*********************************  END: callbacks *********************************/

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ViewMatrix = glm::mat4(1.0f);
	ProjectionMatrix = glm::mat4(1.0f);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix; 

	ModelMatrix_CAR_BODY = glm::mat4(1.0f);
	ModelMatrix_CAR_WHEEL = glm::mat4(1.0f);
	ModelMatrix_CAR_NUT = glm::mat4(1.0f);

	// 0th camera
	// initialize the 0th camera.
	camera[0].prp = glm::vec3(0.0f, 0.0f, prp);
	camera[0].vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	camera[0].vup = glm::vec3(0.0f, 1.0f, 0.0f);
	viewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	camera[0].vup = glm::vec3(viewMatrix[0][0].y, viewMatrix[0][1].y, viewMatrix[0][2].y); // in this example code, make vup always equal to the v direction.

	camera[0].fov_y = 15.0f;
	camera[0].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[0].near_clip = 0.1f;
	camera[0].far_clip = 250.0f;
	camera[0].zoom_factor = 1.0f; // will be used for zoomming in and out.

	// 1st camera (driver's view) - zoom factor and aspect ratio are needed
	camera[1].fov_y = 10.0f;
	camera[1].aspect_ratio = 1.0f;
	camera[1].near_clip = 0.1f;
	camera[1].far_clip = 1000.0f;
	camera[1].zoom_factor = 1.0f; // will be used for zoomming in and out.
	//projectionMatrix[1] = glm::mat4(1.0f);
	projectionMatrix[1] = glm::perspective(camera[1].zoom_factor*TO_RADIAN*camera[1].fov_y, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
	ViewProjectionMatrix = projectionMatrix[1] * ViewMatrix;
}

void prepare_scene(void) {
	prepare_axes(); 
	prepare_path();
	prepare_geom_obj(GEOM_OBJ_ID_CAR_BODY, "Data/car_body_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_CAR_WHEEL, "Data/car_wheel_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_CAR_NUT, "Data/car_nut_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_COW, "Data/cow_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_TEAPOT, "Data/teapot_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_BOX, "Data/box_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_optimus();
	prepare_tiger();
	prepare_airplane();
	prepare_dragon();
	//prepare_square();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
	initialize_camera();
	initialize_car();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void print_message(const char * m) {
	fprintf(stdout, "%s\n\n", m);
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 2
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 4.7.1.Hier_Car_Dummy_Driver_GLSL";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: 'f', l', 'd', 'w', 'o', 'ESC'",
											"    - Mouse used: L-Click and move" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutMainLoop();
}
