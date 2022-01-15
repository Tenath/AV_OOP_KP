#pragma once
#include <vector>
#include "BindableObject.hpp"
#include "PrimitiveSequence.hpp"

namespace av
{
	// Типизация вертекса реализована через статический полиморфизм, 
	// а не наследование, поскольку вертексы на видеокарту удобнее 
	// и быстрее передавать в плотном формате, а использование 
	// наследования с виртуальными методами приведёт к появлению в структуре
	// вертекса указателя на vtbl
	template <typename VertexT, typename IndexT> 
	class VertexArray : public BindableGLObject
	{
	private:
		// Vertex Array Object (VAO) = спецфикация структуры вертексов + VBO + IBO
		GLuint va_handle = 0;

		// Vertex Buffer Object (VBO) - блок памяти с массивом вертексов
		GLuint vb_handle = 0;

		// Index Buffer Object (IBO) aka Element Buffer Object (EBO) - блок памяти с массивом индексов к вертексам
		GLuint ib_handle = 0;

		std::vector<Vertex> vertices; // содержимое будет заливаться в VBO
		std::vector<IndexT> indices; // содержимое будет заливаться в IBO

		std::vector<PrimitiveSequence<IndexT>> primseq; // наборы примитивов для отрисовки

		void SetupGLObjects();

		void CleanupGLObjects();

		size_t TotalIndexCount();

		void RegenerateIndexArray();

		void UpdateVertexBuffer();

		void UpdateIndexBuffer();
	public:
		VertexArray();

		VertexArray(
			std::vector<VertexT>& p_vertices, 
			std::vector<PrimitiveSequence<IndexT>>& p_sequences
		);

		~VertexArray();

		void Bind();

		void Unbind();

		void Draw(size_t ps_index);
	};

	template <typename VertexT, typename IndexT> 
	void VertexArray<VertexT, IndexT>::SetupGLObjects()
	{
		// Создаём VAO
		glGenVertexArrays(1, &va_handle);
		// Создаём VBO (пустой)
		glGenBuffers(1, &vb_handle);
		// Создаём IBO (пустой)
		glGenBuffers(1, &ib_handle);
		VertexT::SetupVertexArray(va_handle);
	}

	template <typename VertexT, typename IndexT>
	size_t VertexArray<VertexT, IndexT>::TotalIndexCount()
	{
		size_t indexcount = 0;

		for (PrimitiveSequence<IndexT>& seq : primseq)
		{
			indexcount += seq.GetIndexCount();
		}

		return indexcount;
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::CleanupGLObjects()
	{
		if (va_handle > 0) glDeleteVertexArrays(1, &va_handle);
		if (vb_handle > 0) glDeleteBuffers(1, &vb_handle);
		if (ib_handle > 0) glDeleteBuffers(1, &ib_handle);
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::RegenerateIndexArray()
	{
		indices.clear();
		indices.reserve(TotalIndexCount());
		size_t offset = 0;

		for (PrimitiveSequence<IndexT>& seq : primseq)
		{
			// добавляем индексы последовательности в массив под IBO
			indices.insert(indices.end(), seq.indices.begin(), seq.indices.end());
			seq.offset = offset;
			offset += seq.indices.size() * sizeof(IndexT);
		}
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::UpdateVertexBuffer()
	{
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vb_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexT) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

		VertexT::SetupLayout(vb_handle);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::UpdateIndexBuffer()
	{
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexT) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <typename VertexT, typename IndexT>
	VertexArray<VertexT, IndexT>::VertexArray()
	{
		try { SetupGLObjects(); }
		catch (std::exception& e) { CleanupGLObjects(); throw e; }
	}

	template <typename VertexT, typename IndexT>
	VertexArray<VertexT, IndexT>::VertexArray(
		std::vector<VertexT>& p_vertices, 
		std::vector<PrimitiveSequence<IndexT>>& p_sequences
	) :	vertices(p_vertices), primseq(p_sequences)
	{
		try
		{
			SetupGLObjects();
			RegenerateIndexArray();
			UpdateVertexBuffer();
			UpdateIndexBuffer();
		}
		catch (std::exception& e)
		{
			CleanupGLObjects();
			throw e;
		}
	}

	template <typename VertexT, typename IndexT>
	VertexArray<VertexT, IndexT>::~VertexArray()
	{
		CleanupGLObjects();
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::Bind()
	{
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vb_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_handle);
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::Unbind()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::Draw(size_t ps_index)
	{
		if (ps_index > primseq.size())
			throw std::out_of_range("VertexArray.Draw(): PrimitiveSequence index out of bounds");
		primseq[ps_index].Draw();
	}
}