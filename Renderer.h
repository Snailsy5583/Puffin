#pragma once

#include "Shader.h"
#include <glm/glm.hpp>

namespace Engine
{
	struct RendererObject {
		unsigned int vao {};
		unsigned int vbo {};
		unsigned int ebo {};
		unsigned int bufferSize {};
		Shader *shader;	   // doesn't take ownership of shader
	};

	struct Mesh {
		enum Usage { none = 0, Static, Dynamic, Stream };
		Usage usage;

		// vbo
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;

		std::vector<int> indices;

		RendererObject renderer_object;
	};

	class Renderer
	{
	public:
		static RendererObject GenRendererObj(Mesh &mesh, Shader *shader);

		static Mesh GenQuad(glm::vec3 pos, float sideLen, Shader *shader);
		static Mesh GenQuad(glm::vec3 pos, glm::vec2 size, Shader *shader);

		static void
		MoveQuad(RendererObject &obj, glm::vec3 newPos, float sideLen);
		static void
		MoveQuad(RendererObject &obj, glm::vec3 newPos, glm::vec2 size);

		static void DeleteQuad(RendererObject &object);

		static void SubmitObject(const RendererObject &obj);

		static float *GetVertices(RendererObject &obj);

		// utility function
		static inline float *hsv2rgb(float in[4])
		{
			in[0] *= 360;

			float hh, p, q, t, ff;
			long i;
			static float out[4];

			if (in[1] <= 0.0) {	   // < is bogus, just shuts up warnings
				out[0] = in[2];
				out[1] = in[2];
				out[2] = in[2];
				out[3] = in[3];
				return out;
			}
			hh = in[0];
			if (hh >= 360.0) hh = 0.0f;
			hh /= 60.0f;
			i = (long) hh;
			ff = hh - i;
			p = in[2] * (1.0f - in[1]);
			q = in[2] * (1.0f - (in[1] * ff));
			t = in[2] * (1.0f - (in[1] * (1.0f - ff)));

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
		const static int m_QuadIndices[];
	};


}	 // namespace Engine