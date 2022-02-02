#pragma once
#include <vector>
#include <initializer_list>
#include <GL/gl3w.h>
#include "VertexGroup.hpp"

namespace av
{
	template <typename VertexT, typename IndexT>
	class DrawSequence
	{
	private:
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

		inline size_t GetIndexCount() 
		{ 
			return (indices != nullptr) ? indices->GetIndices().size() : 0; 
		}

		inline size_t GetVertexCount()
		{
			return (vertices != nullptr) ? vertices->GetVertices().size() : 0;
		}

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

		void Draw();

		VertexGroup<VertexT>* GetVertexGroup() { return vertices; }
		IndexGroup<IndexT>* GetIndexGroup() { return indices; }

		size_t GetVertexOffset() { return vertex_offset; }
		size_t GetIndexOffset() { return index_offset; }

		void SetVertexOffset(size_t offset) { vertex_offset = offset; }
		void SetIndexOffset(size_t offset) { index_offset = offset; }
	};

	template <typename VertexT, typename IndexT>
	void DrawSequence<VertexT, IndexT>::Draw()
	{
		glDrawElements(PrimitiveTypeGetSymbol(indices->GetType()), (GLsizei)indices->GetIndices().size(), GetIndexType(), (void*)index_offset);
	}
}