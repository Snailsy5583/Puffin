//
// Created by r6awe on 3/31/2026.
//

#include "Camera.h"

#include "glm/gtc/quaternion.hpp"

#include <iostream>
#include <ostream>

namespace Engine
{
	Camera::Camera(Type type,
				   glm::vec3 pos,
				   glm::quat rot,
				   float aspect,
				   float near,
				   float far,
				   float fov)
		: m_ProjectionType(type), m_Position(pos), m_Rotation(rot),
		  m_Near(near), m_Far(far)
	{
		switch (type) {
		case Type::Perspective:

			m_Projection =
				glm::perspective(glm::radians(fov), aspect, near, far);
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

	glm::mat4 Camera::GetViewMatrix() const
	{
		glm::mat4 rotation = glm::mat4_cast(glm::conjugate(m_Rotation));
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_Position);

		glm::mat4 view = rotation * translation;

		return view;
	}
}	 // namespace Engine