#ifndef _OPENGL_CAMERA_CAMERA_H_
#define _OPENGL_CAMERA_CAMERA_H_

#include <glm/glm.hpp>

namespace CppUtil {
	namespace OpenGL {
		class Camera
		{
		public:
			enum ENUM_Movement {
				MOVE_FORWARD,
				MOVE_BACKWARD,
				MOVE_LEFT,
				MOVE_RIGHT,
				MOVE_UP,
				MOVE_DOWN
			};

			enum ENUM_Projection {
				PROJECTION_PERSEPCTIVE,
				PROJECTION_ORTHO
			};

		public:
			Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 4.0f), float yaw = YAW, float pitch = PITCH, float ratioWH = RATIO_WH, float nearPlane = NEAR_PLANE, float farPlane = FAR_PLANE, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), ENUM_Projection projectionMode = PROJECTION_MODE);

		public:
			glm::mat4 GetViewMatrix();
			glm::mat4 GetProjectionMatrix();
			glm::vec3 & GetPos();
			glm::vec3 GetFront() const { return front; }

			float GetFOV() const { return fov; }
			float GetAspectRatio() const { return ratioWH; }
			float GetNearPlane() const { return nearPlane; }
			float GetFarPlane() const { return farPlane; }

		public:
			void ProcessKeyboard(ENUM_Movement direction, float deltaTime);
			void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
			void ProcessMouseScroll(float yoffset);

			void SetPerspective();
			void SetOrtho();

			void SetRatioWH(float w, float h) { ratioWH = w / h; }
			void SetRatioWH(float ratioWH) { this->ratioWH = ratioWH; }

			void SetPose(const glm::vec3 & pos, float yaw, float pitch);

		private:
			void updateCameraVectors();

			// Camera Attributes
			glm::vec3 position;
			glm::vec3 front;
			glm::vec3 up;
			glm::vec3 right;
			glm::vec3 worldUp;
			float fov;
			float ratioWH;
			float nearPlane;
			float farPlane;
			float yaw;
			float pitch;
			ENUM_Projection projectionMode;

			// Default camera values
			static const float RATIO_WH;
			static const float NEAR_PLANE;
			static const float FAR_PLANE;
			static const float YAW;
			static const float PITCH;
			static const float FOV;
			static const ENUM_Projection PROJECTION_MODE;
		};
	}
}
#endif//! _OPENGL_CAMERA_CAMERA_H_
