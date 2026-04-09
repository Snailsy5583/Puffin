#pragma once

namespace Engine
{

	class GameObject;

	class Component
	{
	protected:
		Component(GameObject *owner);

	public:
		virtual ~Component() = default;
		virtual void UpdateComponent(float deltaTime) {}

	protected:
		GameObject *m_Owner;
	};

}	 // namespace Engine
