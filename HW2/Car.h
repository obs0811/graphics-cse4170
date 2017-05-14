#define WHEEL_NUT_RADIUS				(1.7f - 0.5f)
#define WHEEL_NUT_Z_OFFSET				1.0f

#define FRONT_WHEEL_ROLL_ANGLE_DELTA	5.0f
#define FRONT_WHEEL_TURN_ANGLE_DELTA	2.5f
#define FRONT_WHEEL_TURN_ANGLE_MAX		45.0f
#define BODY_YAW_SENSITIVITY			0.1f

struct _CAR_STATUS {
	float body_yaw_angle;
	float front_wheel_roll_angle;
	float front_wheel_turn_angle;
	glm::mat4 Matrix_CAR_WHEEL_ROTATE;
	int flag_body_yaw_mode;
} car_status;

void update_car_body_transformation(void) {
	/*ModelMatrix_CAR_BODY = glm::rotate(glm::mat4(1.0f),
		TO_RADIAN*car_status.body_yaw_angle, glm::vec3(0.0f, 1.0f, 0.0f));

	ModelMatrix_CAR_BODY_INVERSE = glm::rotate(glm::mat4(1.0f),
		-TO_RADIAN*car_status.body_yaw_angle, glm::vec3(0.0f, 1.0f, 0.0f));*/
}

void update_car_front_wheel_rotate_matrix(void) {
	car_status.Matrix_CAR_WHEEL_ROTATE = glm::rotate(glm::mat4(1.0f),
		TO_RADIAN*car_status.front_wheel_turn_angle, glm::vec3(0.0f, 1.0f, 0.0f));

	car_status.Matrix_CAR_WHEEL_ROTATE = glm::rotate(car_status.Matrix_CAR_WHEEL_ROTATE,
		TO_RADIAN*car_status.front_wheel_roll_angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void initialize_car(void) {
	car_status.body_yaw_angle = 0.0f;
	//update_car_body_transformation();
	/*if (camera_type == CAMERA_DRIVER) {

#ifdef USE_set_ViewMatrix_for_driver2
		set_ViewMatrix_for_driver2();
#else
		set_ViewMatrix_for_driver();
#endif
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}*/

	car_status.front_wheel_roll_angle = 0.0f;
	car_status.front_wheel_turn_angle = 0.0f;
	update_car_front_wheel_rotate_matrix();
}