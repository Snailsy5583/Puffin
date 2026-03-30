#include "glad/glad.h"
#include <iostream>

#include "Renderer.h"

namespace Engine
{


	const float Renderer::m_QuadVerts[] = {
		//      x,		y,	   z,	  u,	  v,
		-0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.5f, -0.5f, 0.0f, 1.f, 1.f,
		-0.5f, 0.5f,  0.0f, 0.f, 0.f, 0.5f, 0.5f,  0.0f, 1.f, 0.f,
	};
	const int Renderer::m_QuadIndices[] = {0, 1, 2, 3, 1, 2};

	RendererObject Renderer::GenRendererObj(Mesh &mesh, Shader *shader)
	{
		RendererObject obj = {0, 0, 0, 0, shader};

		glGenVertexArrays(1, &obj.vao);
		glBindVertexArray(obj.vao);

		int stride =
			(sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2));
		obj.bufferSize = mesh.vertices.size() * stride;


		float *buffer = new float[obj.bufferSize / sizeof(float)];

		for (int i = 0; i < mesh.vertices.size(); i++) {
			buffer[i] = mesh.vertices[i].x;
			buffer[i + 1] = mesh.vertices[i].y;
			buffer[i + 2] = mesh.vertices[i].z;
			buffer[i + 3] = mesh.normals[i].x;
			buffer[i + 4] = mesh.normals[i].y;
			buffer[i + 5] = mesh.normals[i].z;
			buffer[i + 6] = mesh.texCoords[i].x;
			buffer[i + 7] = mesh.texCoords[i].y;
		}

		GLenum usage;
		switch (mesh.usage) {
		case Mesh::Dynamic: usage = GL_DYNAMIC_DRAW; break;
		case Mesh::Stream: usage = GL_STREAM_DRAW; break;
		default: usage = GL_STATIC_DRAW; break;
		}

		glGenBuffers(1, &obj.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBufferData(GL_ARRAY_BUFFER, obj.bufferSize, buffer, usage);

		delete[] buffer;

		glVertexAttribPointer(0,
							  3,
							  GL_FLOAT,
							  GL_FALSE,
							  stride,
							  (void *) nullptr);
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
					 mesh.indices.size() * sizeof(int),
					 mesh.indices.data(),
					 usage);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		return obj;
	}

	Mesh Renderer::GenQuad(glm::vec3 pos,
						   const float sideLen,
						   Engine::Shader *shader)
	{ return GenQuad(pos, glm::vec2(sideLen, sideLen), shader); }

	Mesh
	Renderer::GenQuad(glm::vec3 pos, glm::vec2 size, Engine::Shader *shader)
	{
		Mesh mesh;
		int len = sizeof(m_QuadVerts) / sizeof(float);

		for (int row = 0; row < len; row++) {
			glm::vec3 vert;
			vert.x = (m_QuadVerts[(row * 5) + 0] * size.x) + pos.x;
			vert.y = (m_QuadVerts[(row * 5) + 1] * size.y) + pos.y;
			vert.z = m_QuadVerts[(row * 5) + 0] + pos.z;

			glm::vec2 texCoord {m_QuadVerts[(row * 5) + 3],
								m_QuadVerts[(row * 5) + 4]};

			mesh.vertices.push_back(vert);
			mesh.texCoords.push_back(texCoord);
			mesh.normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
		}
		for (int ind : m_QuadIndices) mesh.indices.push_back(ind);

		mesh.renderer_object = GenRendererObj(mesh, shader);
		return mesh;
	}

	void
	Renderer::MoveQuad(RendererObject &obj, glm::vec3 newPos, float sideLen)
	{ MoveQuad(obj, newPos, glm::vec2(sideLen, sideLen)); }

	void
	Renderer::MoveQuad(RendererObject &obj, glm::vec3 newPos, glm::vec2 size)
	{
		float verts[sizeof(m_QuadVerts) / sizeof(float)];

		for (int row = 0; row < 6; row++) {
			// x
			verts[(row * 5) + 0] =
				(m_QuadVerts[(row * 5) + 0] * size.x) + newPos.x;
			// y
			verts[(row * 5) + 1] =
				(m_QuadVerts[(row * 5) + 1] * size.y) + newPos.y;
			// z
			verts[(row * 5) + 2] = m_QuadVerts[(row * 5) + 0] + newPos.z;

			for (int col = 3; col < 5; col++)
				verts[(row * 5) + col] = m_QuadVerts[(row * 5) + col];
		}

		// update vbo
		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBufferData(GL_ARRAY_BUFFER, obj.bufferSize, verts, GL_STATIC_DRAW);

		glVertexAttribPointer(0,
							  3,
							  GL_FLOAT,
							  GL_FALSE,
							  5 * sizeof(float),
							  (void *) nullptr);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
	}

	void Renderer::DeleteQuad(RendererObject &obj)
	{
		glDeleteBuffers(1, &obj.vbo);
		glDeleteVertexArrays(1, &obj.vao);
	}

	void Renderer::SubmitObject(const RendererObject &obj)
	{
		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		obj.shader->Bind();

		glDrawArrays(GL_TRIANGLES, 0, obj.bufferSize);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		obj.shader->UnBind();
	}

	float *Renderer::GetVertices(RendererObject &obj)
	{
		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);

		float *data = nullptr;
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, obj.bufferSize, data);

		return data;
	}
}	 // namespace Engine