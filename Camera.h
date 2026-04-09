//
// Created by r6awe on 3/31/2026.
//

#ifndef TERRAINGENERATIONEDITOR_CAMERA_H
#define TERRAINGENERATIONEDITOR_CAMERA_H

#include "glm/detail/type_quat.hpp"
#include <glm/glm.hpp>

namespace Engine
{
	class Camera
	{
	public:
		enum Type { Perspective, Orthographic, Other };

		Camera(Type type,
			   glm::vec3 pos,
			   glm::quat rot,
			   float aspect,
			   float near,
			   float far,
			   float fov = 90);

		[[nodiscard]] glm::mat4 GetViewMatrix() const;
		[[nodiscard]] glm::mat4 GetProjectionMatrix() const { return m_Projection; }

	public:
		glm::vec3 &GetPosition() { return m_Position; }
		glm::quat &GetRotation() { return m_Rotation; }

	protected:
		Type m_ProjectionType;

		glm::mat4 m_Projection;
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		float m_Near, m_Far;
	};
}	 // namespace Engine


#endif	  // TERRAINGENERATIONEDITOR_CAMERA_H
