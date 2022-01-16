#pragma once
#include <vector>
#include <initializer_list>
#include <GL/gl3w.h>
#include "VertexGroup.hpp"

namespace av
{
	template <typename VertexT, typename IndexT> class DrawSequence
	{
	public:
		VertexGroup<VertexT>* vertices = nullptr;
		IndexGroup<IndexT>* indices = nullptr;
		size_t vertex_offset = 0;
		size_t index_offset = 0;

	public:
		constexpr inline GLenum GetIndexType()
		{
			return sizeof(IndexT) == 1 ? GL_UNSIGNED_BYTE : 
				sizeof(IndexT) == 2 ? GL_UNSIGNED_SHORT : 
				GL_UNSIGNED_INT;
		}

		inline size_t GetIndexCount() { return indices->GetIndices().size(); }

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

		DrawSequence() {}
		DrawSequence(VertexGroup<VertexT>* vgrp, IndexGroup<IndexT>* igrp) : vertices(vgrp), indices(igrp) {}

		void Draw()
		{
			glDrawElements(PrimitiveTypeGetSymbol(indices->type), (GLsizei)indices->GetIndices().size(), GetIndexType(), (void*)index_offset);
		}
	};
}