//
// Created by r6awe on 4/9/2026.
//

#ifndef TERRAINGENERATIONEDITOR_MESH_H
#define TERRAINGENERATIONEDITOR_MESH_H

#include "glad/glad.h"
#include <string>

#include "Renderer.h"

namespace Engine
{
	struct Vertex {
		glm::vec3 position, normal;
		glm::vec2 texCoord;
	};

	class Mesh
	{
	public:
		enum Usage {
			none = 0,
			Static = GL_STATIC_DRAW,
			Dynamic = GL_DYNAMIC_DRAW,
			Stream = GL_STREAM_DRAW
		};

		Mesh() { }
		Mesh(Shader *shader);
		Mesh(std::string path, Shader *shader);

		void GenRendererObj(Shader *shader);

		void UpdateMesh();

		[[nodiscard]] const float *GetAggrVertexData() const
		{ return m_AggrVertices.data(); }

	public:
		Usage p_Usage {};

		std::vector<Vertex> p_Vertices {};
		std::vector<unsigned int> p_Indices {};
		RenderObject p_RenderObject {};

	protected:
		std::vector<float> m_AggrVertices {};
	};

}	 // namespace Engine


#endif	  // TERRAINGENERATIONEDITOR_MESH_H
