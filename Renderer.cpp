#include "glad/glad.h"
#include <iostream>

#include "Renderer.h"

#include "OBJ_Loader.h"

namespace Engine
{

	const float Renderer::m_QuadVerts[] = {
		//      x,		y,	   z,	  u,	  v,
		-0.5f, -0.5f, 1.0f, 0.f, 0.f, 0.5f, -0.5f, 1.0f, 1.f, 0.f,
		-0.5f, 0.5f,  1.0f, 0.f, 1.f, 0.5f, 0.5f,  1.0f, 1.f, 1.f,
	};
	const unsigned int Renderer::m_QuadIndices[] = {0, 1, 2, 3, 2, 1};

	RenderObject Renderer::GenRendererObj(const Mesh &mesh, Shader *shader)
	{
		RenderObject obj = {0, 0, 0, 0, shader};

		glGenVertexArrays(1, &obj.vao);
		glBindVertexArray(obj.vao);

		constexpr int stride =
			(sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2));
		GLenum usage;
		switch (mesh.usage) {
		case Mesh::Dynamic: usage = GL_DYNAMIC_DRAW; break;
		case Mesh::Stream: usage = GL_STREAM_DRAW; break;
		default: usage = GL_STATIC_DRAW; break;
		}

		glGenBuffers(1, &obj.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBufferData(GL_ARRAY_BUFFER,
					 obj.bufferSize,
					 mesh.GetAggrVertexData(),
					 usage);

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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return obj;
	}

	Mesh Renderer::GenQuad(glm::vec3 pos,
						   const float sideLen,
						   Engine::Shader *shader)
	{ return GenQuad(pos, glm::vec2(sideLen, sideLen), shader); }

	Mesh
	Renderer::GenQuad(const glm::vec3 pos, const glm::vec2 size, Shader *shader)
	{
		Mesh mesh;
		int len = sizeof(m_QuadVerts) / (sizeof(float) * 5);

		for (int row = 0; row < len; row++) {
			glm::vec3 vert;
			vert.x = (m_QuadVerts[(row * 5) + 0] * size.x) + pos.x;
			vert.y = (m_QuadVerts[(row * 5) + 1] * size.y) + pos.y;
			vert.z = m_QuadVerts[(row * 5) + 2] + pos.z;

			glm::vec2 texCoord {m_QuadVerts[(row * 5) + 3],
								m_QuadVerts[(row * 5) + 4]};

			mesh.vertices.push_back(vert);
			mesh.texCoords.push_back(texCoord);
			mesh.normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		}
		for (unsigned int ind : m_QuadIndices) mesh.indices.push_back(ind);

		mesh.renderObj = GenRendererObj(mesh, shader);
		mesh.UpdateMesh();
		return mesh;
	}

	void Renderer::UpdateVertexBuffer(const Mesh &mesh)
	{
		const auto obj = mesh.renderObj;
		GLenum usage;
		switch (mesh.usage) {
		case Mesh::Dynamic: usage = GL_DYNAMIC_DRAW; break;
		case Mesh::Stream: usage = GL_STREAM_DRAW; break;
		default: usage = GL_STATIC_DRAW; break;
		}

		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBufferData(GL_ARRAY_BUFFER,
					 obj.bufferSize,
					 mesh.GetAggrVertexData(),
					 usage);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 mesh.indices.size() * sizeof(int),
					 mesh.indices.data(),
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

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
	}

	void Renderer::DeleteQuad(const RenderObject &obj)
	{
		glDeleteBuffers(1, &obj.vbo);
		glDeleteVertexArrays(1, &obj.vao);
	}

	void Renderer::SubmitObject(const Mesh &mesh)
	{
		auto obj = mesh.renderObj;
		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
		obj.shader->Bind();

		glDrawElements(GL_TRIANGLES,
					   mesh.indices.size() * sizeof(unsigned),
					   GL_UNSIGNED_INT,
					   nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		obj.shader->UnBind();
	}

	void Renderer::SubmitObject(const Camera &camera, const Mesh &mesh)
	{
		mesh.renderObj.shader->Bind();
		mesh.renderObj.shader->SetUniformMat4("view", camera.GetViewMatrix());
		mesh.renderObj.shader->SetUniformMat4("projection",
											  camera.GetProjectionMatrix());

		SubmitObject(mesh);

		mesh.renderObj.shader->UnBind();
	}

	float *Renderer::GetVertices(const RenderObject &obj)
	{
		glBindVertexArray(obj.vao);
		glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);

		float *data = nullptr;
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, obj.bufferSize, data);

		return data;
	}

	Mesh Mesh::ImportFromOBJ(std::string path, Shader *shader)
	{
		Mesh mesh;
		objl::Loader loader;
		loader.LoadFile(path);
		for (auto vert : loader.LoadedVertices) {
			mesh.vertices.emplace_back(vert.Position.X,
									   vert.Position.Y,
									   vert.Position.Z);
			mesh.texCoords.emplace_back(vert.TextureCoordinate.X,
										vert.TextureCoordinate.Y);
			mesh.normals.emplace_back(vert.Normal.X,
									  vert.Normal.Y,
									  vert.Normal.Z);
		}
		for (auto ind : loader.LoadedIndices) {
			mesh.indices.emplace_back(ind);
		}
		mesh.renderObj = Renderer::GenRendererObj(mesh, shader);
		mesh.UpdateMesh();
		return mesh;
	}
}	 // namespace Engine