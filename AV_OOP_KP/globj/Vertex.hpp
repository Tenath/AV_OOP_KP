#pragma once
#include <GL/gl3w.h>
#include "../math/av_vector_t.hpp"

namespace av
{
	// Структура типа "вертекс". Используется в VertexArray.
	struct Vertex
	{
		Vector3f position;
		Vector4f color;

		Vertex() {}
		Vertex(Vector3f pos, Vector4f col) : position(pos), color(col) {}

		static void SetupVertexArray(GLuint handle)
		{
			glBindVertexArray(handle);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindVertexArray(0);
		}

		// Здесь описываем компоновку аттрибутов в VBO
		static void SetupLayout(GLuint vbo)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

			//glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	};
}