#include "glad/glad.h"
#include <iostream>

#include "Mesh.h"
#include "Renderer.h"

#include "../../vendor/OBJLoader/OBJ_Loader.h"

namespace Engine
{

	const float Renderer::m_QuadVerts[] = {
		//      x,		y,	   z,	  u,	  v,
		-0.5f, -0.5f, 1.0f, 0.f, 0.f, 0.5f, -0.5f, 1.0f, 1.f, 0.f,
		-0.5f, 0.5f,  1.0f, 0.f, 1.f, 0.5f, 0.5f,  1.0f, 1.f, 1.f,
	};
	const unsigned int Renderer::m_QuadIndices[] = {0, 1, 2, 3, 2, 1};


	Mesh Renderer::GenQuad(glm::vec3 pos, const float sideLen, Shader *shader)
	{ return GenQuad(pos, glm::vec2(sideLen, sideLen), shader); }

	Mesh
	Renderer::GenQuad(const glm::vec3 pos, const glm::vec2 size, Shader *shader)
	{
		Mesh mesh(shader);
		int len = sizeof(m_QuadVerts) / (sizeof(float) * 5);

		for (int row = 0; row < len; row++) {
			glm::vec3 vert;
			vert.x = m_QuadVerts[row * 5 + 0] * size.x + pos.x;
			vert.y = (m_QuadVerts[row * 5 + 1] * size.y) + pos.y;
			vert.z = m_QuadVerts[row * 5 + 2] + pos.z;

			glm::vec2 texCoord {m_QuadVerts[row * 5 + 3],
								m_QuadVerts[row * 5 + 4]};

			mesh.p_Vertices.push_back({vert, glm::vec3 {0, 0, 0}, texCoord});
		}
		for (unsigned int ind : m_QuadIndices)
			mesh.p_Indices.push_back(ind);

		mesh.UpdateMesh();
		return mesh;
	}

	void Renderer::UpdateVertexBuffer(const Mesh &mesh)
	{
		const auto obj = mesh.p_RenderObject;
		GLenum usage;
		switch (mesh.p_Usage) {
		case Mesh::Dynamic: usage = GL_DYNAMIC_DRAW; break;
		case Mesh::Stream: usage = GL_STREAM_DRAW; break;
		default: usage = GL_STATIC_DRAW; break;
		}

		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBufferData(
			GL_ARRAY_BUFFER, obj.bufferSize, mesh.GetAggrVertexData(), usage);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 mesh.p_Indices.size() * sizeof(int),
					 mesh.p_Indices.data(),
					 usage);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Renderer::MoveQuad(RenderObject &obj, glm::vec3 newPos, float sideLen)
	{ MoveQuad(obj, newPos, glm::vec2(sideLen, sideLen)); }

	void Renderer::MoveQuad(RenderObject &obj, glm::vec3 newPos, glm::vec2 size)
	{
		float verts[sizeof(m_QuadVerts) / sizeof(float)];

		for (int row = 0; row < 6; row++) {
			// x
			verts[row * 5 + 0] =
				(m_QuadVerts[(row * 5) + 0] * size.x) + newPos.x;
			// y
			verts[row * 5 + 1] =
				(m_QuadVerts[(row * 5) + 1] * size.y) + newPos.y;
			// z
			verts[row * 5 + 2] = m_QuadVerts[(row * 5) + 0] + newPos.z;

			for (int col = 3; col < 5; col++)
				verts[row * 5 + col] = m_QuadVerts[row * 5 + col];
		}

		// update vbo
		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBufferData(GL_ARRAY_BUFFER, obj.bufferSize, verts, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
	}

	void Renderer::DeleteQuad(const RenderObject &object)
	{
		glDeleteBuffers(1, &object.vbo);
		glDeleteVertexArrays(1, &object.vao);
	}

	void Renderer::SubmitObject(const Mesh &mesh)
	{
		auto obj = mesh.p_RenderObject;
		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
		obj.shader->Bind();

		glDrawElements(GL_TRIANGLES,
					   mesh.p_Indices.size() * sizeof(unsigned),
					   GL_UNSIGNED_INT,
					   nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		obj.shader->Unbind();
	}

	void Renderer::SubmitObject(const Camera &camera, const Mesh &mesh)
	{
		mesh.p_RenderObject.shader->Bind();
		mesh.p_RenderObject.shader->SetUniformMat4("view",
												   camera.GetViewMatrix());
		mesh.p_RenderObject.shader->SetUniformMat4(
			"projection", camera.GetProjectionMatrix());

		SubmitObject(mesh);

		mesh.p_RenderObject.shader->Unbind();
	}

	std::vector<float> Renderer::GetVertices(const RenderObject &obj)
	{
		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);

		std::vector<float> data(obj.bufferSize);
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, obj.bufferSize, data.data());

		return data;
	}
}	 // namespace Engine