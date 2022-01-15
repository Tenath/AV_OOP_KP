#pragma once
#include <vector>
#include <initializer_list>
#include <GL/gl3w.h>

namespace av
{
	enum class PrimitiveType : unsigned char
	{
		Points = 0,
		Lines = 1,
		Triangles = 2,
		TriFan = 3,
		TriStrip = 4
	};

	template <typename IndexT> class PrimitiveSequence
	{
	public:
		std::vector<IndexT> indices;
		PrimitiveType type = PrimitiveType::Triangles;
		size_t offset = 0;

	public:
		constexpr inline GLenum GetIndexType()
		{
			return sizeof(IndexT) == 1 ? GL_UNSIGNED_BYTE : 
				sizeof(IndexT) == 2 ? GL_UNSIGNED_SHORT : 
				GL_UNSIGNED_INT;
		}

		inline size_t GetIndexCount() { return indices.size(); }

		static GLenum PrimitiveTypeGetSymbol(PrimitiveType type)
		{
			GLenum result = 0;

			switch (type)
			{
			case PrimitiveType::Points: result = GL_POINTS; break;
			case PrimitiveType::Lines: result = GL_LINES; break;
			case PrimitiveType::Triangles: result = GL_TRIANGLES; break;
			case PrimitiveType::TriFan: result = GL_TRIANGLE_FAN; break;
			case PrimitiveType::TriStrip: result = GL_TRIANGLE_STRIP; break;
			}

			return result;
		}

		PrimitiveSequence() {}
		PrimitiveSequence(PrimitiveType t, std::initializer_list<IndexT> ilst) : type(t), indices(ilst) {}
		PrimitiveSequence(PrimitiveType t, std::vector<IndexT> vec) : type(t), indices(vec) {}

		void Draw()
		{
			glDrawElements(PrimitiveTypeGetSymbol(type), (GLsizei)indices.size(), GetIndexType(), (void*)offset);
		}
	};
}