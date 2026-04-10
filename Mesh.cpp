//
// Created by r6awe on 4/9/2026.
//

#include "Mesh.h"

#include "OBJ_Loader.h"
#include "glad/glad.h"

namespace Engine
{

	Mesh::Mesh(Shader *shader)
	{
		GenRendererObj(shader);
		UpdateMesh();
	}

	Mesh::Mesh(std::string path, Shader *shader)
	{
		objl::Loader loader;
		loader.LoadFile(path);
		for (auto vert : loader.LoadedVertices) {
			p_Vertices.push_back(
				{glm::vec3 {vert.Position.X, vert.Position.Y, vert.Position.Z},
				 glm::vec3 {vert.Normal.X, vert.Normal.Y, vert.Normal.Z},
				 glm::vec2 {vert.TextureCoordinate.X,
							vert.TextureCoordinate.Y}});
		}
		for (auto ind : loader.LoadedIndices) {
			p_Indices.emplace_back(ind);
		}
		GenRendererObj(shader);
		UpdateMesh();
	}

	void Mesh::GenRendererObj(Shader *shader)
	{
		RenderObject &obj = p_RenderObject;
		obj.shader = shader;

		glGenVertexArrays(1, &obj.vao);
		glBindVertexArray(obj.vao);

		constexpr int stride =
			sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2);

		glGenBuffers(1, &obj.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBufferData(
			GL_ARRAY_BUFFER, obj.bufferSize, GetAggrVertexData(), p_Usage);

		glVertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE, stride, (void *) nullptr);
		glVertexAttribPointer(1,
							  3,
							  GL_FLOAT,
							  GL_FALSE,
							  5 * sizeof(float),
							  (void *) (3 * sizeof(float)));
		glVertexAttribPointer(2,
							  2,
							  GL_FLOAT,
							  GL_FALSE,
							  5 * sizeof(float),
							  (void *) (3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &obj.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 p_Indices.size() * sizeof(int),
					 p_Indices.data(),
					 p_Usage);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Mesh::UpdateMesh()
	{
		m_AggrVertices.clear();

		constexpr int stride =
			(sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2));

		p_RenderObject.bufferSize = p_Vertices.size() * stride;

		for (int i = 0; i < p_Vertices.size(); i++) {
			m_AggrVertices.emplace_back(p_Vertices[i].position.x);
			m_AggrVertices.emplace_back(p_Vertices[i].position.y);
			m_AggrVertices.emplace_back(p_Vertices[i].position.z);
			m_AggrVertices.emplace_back(p_Vertices[i].normal.x);
			m_AggrVertices.emplace_back(p_Vertices[i].normal.y);
			m_AggrVertices.emplace_back(p_Vertices[i].normal.z);
			m_AggrVertices.emplace_back(p_Vertices[i].texCoord.x);
			m_AggrVertices.emplace_back(p_Vertices[i].texCoord.y);
		}

		Renderer::UpdateVertexBuffer(*this);
	}

}	 // namespace Engine