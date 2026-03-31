//
// Created by r6awe on 3/31/2026.
//

#include "Camera.h"

Camera::Camera(Type type,
			   glm::vec3 pos,
			   glm::quat rot,
			   float aspect,
			   float near,
			   float far,
			   float fov)
	: m_ProjectionType(type), m_Position(pos), m_Rotation(rot), m_Near(near),
	  m_Far(far)
{
	switch (type) {
	case Type::Perspective:

		m_Projection = glm::perspective(glm::radians(fov), aspect, near, far);
		break;
	case Type::Orthographic:
		// TODO: orthographic projection
		break;
	default: break;
	}
}

Camera::Camera(const glm::mat4 &projection, glm::vec3 pos, glm::quat rot)
	: m_ProjectionType(Other), m_Position(pos), m_Rotation(rot),
	  m_Projection(projection)
{
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::quat r = m_Rotation;
	glm::vec3 t = -m_Position;
	glm::mat4 view {1 - 2 * r.y * r.y - 2 * r.z * r.z,
					2 * r.x * r.y - 2 * r.z * r.w,
					2 * r.x * r.z + 2 * r.y * r.w,
					t.x,

					2 * r.x * r.y + 2 * r.z * r.w,
					1 - 2 * r.x * r.x - 2 * r.z * r.z,
					2 * r.y * r.z - 2 * r.x * r.w,
					t.y,

					2 * r.x * r.z - 2 * r.y * r.w,
					2 * r.y * r.z + 2 * r.x * r.w,
					1 - 2 * r.x * r.x - 2 * r.y * r.y,
					t.z,

					0,
					0,
					0,
					1};

	return view;
}