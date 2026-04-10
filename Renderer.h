#pragma once

#include "Camera.h"
#include "Shader.h"
#include <glm/glm.hpp>

#include <iostream>

namespace Engine
{
	struct RenderObject {
		unsigned int vao {};
		unsigned int vbo {};
		unsigned int ebo {};
		unsigned int bufferSize {};
		Shader *shader;	   // doesn't take ownership of shader
	};

	class Mesh;

	class Renderer
	{
	public:
		static Mesh GenQuad(glm::vec3 pos, float sideLen, Shader *shader);
		static Mesh GenQuad(glm::vec3 pos, glm::vec2 size, Shader *shader);

		static void UpdateVertexBuffer(const Mesh &mesh);

		static void
		MoveQuad(RenderObject &obj, glm::vec3 newPos, float sideLen);
		static void
		MoveQuad(RenderObject &obj, glm::vec3 newPos, glm::vec2 size);

		static void DeleteQuad(const RenderObject &object);

		static void SubmitObject(const Mesh &mesh);
		static void SubmitObject(const Camera &camera, const Mesh &mesh);

		static std::vector<float> GetVertices(const RenderObject &obj);

		// utility function
		static float *hsv2rgb(float in[4])
		{
			in[0] *= 360;

			static float out[4];

			if (in[1] <= 0.0) {	   // < is bogus, just shuts up warnings
				out[0] = in[2];
				out[1] = in[2];
				out[2] = in[2];
				out[3] = in[3];
				return out;
			}
			float hh = in[0];
			if (hh >= 360.0)
				hh = 0.0f;
			hh /= 60.0f;
			long i = (long) hh;
			float ff = hh - i;
			float p = in[2] * (1.0f - in[1]);
			float q = in[2] * (1.0f - (in[1] * ff));
			float t = in[2] * (1.0f - (in[1] * (1.0f - ff)));

			switch (i) {
			case 0:
				out[0] = in[2];
				out[1] = t;
				out[2] = p;
				break;
			case 1:
				out[0] = q;
				out[1] = in[2];
				out[2] = p;
				break;
			case 2:
				out[0] = p;
				out[1] = in[2];
				out[2] = t;
				break;

			case 3:
				out[0] = p;
				out[1] = q;
				out[2] = in[2];
				break;
			case 4:
				out[0] = t;
				out[1] = p;
				out[2] = in[2];
				break;
			case 5:
			default:
				out[0] = in[2];
				out[1] = p;
				out[2] = q;
				break;
			}
			out[3] = in[3];
			return out;
		}

	private:
		const static float m_QuadVerts[];
		const static unsigned int m_QuadIndices[];
	};

}	 // namespace Engine