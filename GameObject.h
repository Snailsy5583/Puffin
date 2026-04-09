//
// Created by r6awe on 6/6/2023.
//

#pragma once

#include <functional>
#include <memory>

#include "Component.h"
#include "Renderer.h"
#include "glm/detail/type_quat.hpp"

namespace Engine
{

	class GameObject
	{
	public:
		virtual ~GameObject() = default;
		GameObject(Mesh mesh, glm::vec3 pos, glm::quat rot);

		void AddComponent(std::unique_ptr<Component> component);

		virtual void Move(glm::vec3 deltaPos, glm::quat deltaRot);

		virtual void Update(float deltaTime);

		void Render() const;

		void
		SetUpdateCallback(std::function<void(float, GameObject *)> callback);

	public:
		[[nodiscard]] RenderObject &GetRenderObject() { return mesh.renderObj; }
		[[nodiscard]] const glm::vec3 &GetPosition() const { return m_Position; }
		[[nodiscard]] const glm::quat &GetRotation() const { return
		    m_Rotation; }
		[[nodiscard]] float GetScale() const { return m_Scale; }

		template<class T>
		T *TryGetComponent()
		{
			for (auto &comp : m_Components) {
				T *specifiedComp = dynamic_cast<T *>(comp.get());
				if (specifiedComp) return specifiedComp;
			}
			return nullptr;
		}

	protected:
		Mesh mesh;

		std::vector<std::unique_ptr<Component>> m_Components;

		std::function<void(float, GameObject *)> m_UpdateCallback;

		glm::vec3 m_Position;
		glm::quat m_Rotation;
		float m_Scale = 1;
		glm::mat4 m_TransformationMat;
	};

	class Quad : public GameObject
	{
	public:
		Quad(float sideLength, glm::vec3 pos, glm::quat rot, Shader *shader);

		float GetSideLength() const { return m_SideLength; }

	protected:
		float m_SideLength;
	};

	class Circle : public GameObject
	{
	public:
		Circle(float radius, glm::vec3 pos, glm::quat rot, Shader *shader);

		float GetRadius() const { return m_Radius; }

	private:
		float m_Radius;
	};

}	 // namespace Engine
