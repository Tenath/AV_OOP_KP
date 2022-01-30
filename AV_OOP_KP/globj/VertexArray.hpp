#pragma once
#include <vector>
#include <string>
#include <initializer_list>
#include <algorithm>
#include "BindableObject.hpp"
#include "DrawSequence.hpp"
#include "VertexGroup.hpp"
#include "../utils/Utils.hpp"

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

		std::vector<VertexT> vertices; // содержимое будет отправляться в VBO
		std::vector<IndexT> indices; // содержимое будет отправляться в IBO

		IndexT primitive_restart = 65535;

		std::vector<DrawSequence<VertexT,IndexT>*> drawseq; // наборы примитивов для отрисовки

		void SetupGLObjects();

		void CleanupGLObjects();

		size_t TotalIndexCount();
		size_t TotalVertexCount();

		void RegenerateVertexArray();
		void RegenerateIndexArray();

		void UpdateVertexBuffer();

		void UpdateIndexBuffer();

		void RegenerateAndUpdate()
		{
			RegenerateVertexArray();
			RegenerateIndexArray();

			UpdateVertexBuffer();
			UpdateIndexBuffer();
		}
	public:
		VertexArray();

		VertexArray(std::vector<DrawSequence<VertexT,IndexT>>& p_sequences);

		~VertexArray();

		GLuint GetHandle() override { return va_handle; }

		void Bind() override;

		void Unbind() override;

		void RestoreBufferBindings()
		{
			Bind();
			glBindBuffer(GL_ARRAY_BUFFER, vb_handle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_handle);
			Unbind();
		}

		void Draw(size_t ps_index);

		void AddDS(DrawSequence<VertexT,IndexT>* ds)
		{
			if (!VectorContains(drawseq, ds))
			{
				drawseq.push_back(ds);
				RegenerateAndUpdate();
			}
		}

		void RemoveDS(DrawSequence<VertexT, IndexT>* ds)
		{
			if (VectorContains(drawseq, ds))
			{
				drawseq.erase(std::find(drawseq.begin(), drawseq.end(), ds));
				//std::remove(drawseq.begin(), drawseq.end(), ds);
				RegenerateAndUpdate();
			}
		}

		IndexT GetPrimitiveRestart() { return primitive_restart; }
		void SetPrimitiveRestart(IndexT value) { primitive_restart = value; }
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

		glBindVertexArray(va_handle);
		VertexT::SetupVertexArray(va_handle);
		RestoreBufferBindings();
		glBindVertexArray(0);
	}

	template <typename VertexT, typename IndexT>
	size_t VertexArray<VertexT, IndexT>::TotalIndexCount()
	{
		size_t indexcount = 0;

		for (DrawSequence<VertexT,IndexT>* seq : drawseq)
		{
			indexcount += seq->GetIndexCount();
		}

		return indexcount;
	}

	template <typename VertexT, typename IndexT>
	size_t VertexArray<VertexT, IndexT>::TotalVertexCount()
	{
		size_t vertexcount = 0;

		for (DrawSequence<VertexT, IndexT>& seq : drawseq)
		{
			vertexcount += seq.GetVertexCount();
		}

		return vertexcount;
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
		size_t byte_offset = 0;
		IndexT vertex_offset = 0;

		for (DrawSequence<VertexT,IndexT>* seq : drawseq)
		{
			// добавляем индексы последовательности в массив под IBO
			//indices.insert(indices.end(), seq.indices.begin(), seq.indices.end());
			for (IndexT& n : seq->GetIndexGroup()->GetIndices())
			{
				//n += static_cast<IndexT>(seq->GetVertexOffset());
				if (n == primitive_restart) indices.push_back(n);
				else indices.push_back(n + vertex_offset);
			}
			seq->SetIndexOffset(byte_offset);
			byte_offset += seq->GetIndexGroup()->GetIndices().size() * sizeof(IndexT);
			vertex_offset += seq->GetVertexGroup()->GetVertices().size();
		}
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::RegenerateVertexArray()
	{
		vertices.clear();
		vertices.reserve(TotalIndexCount());
		size_t offset = 0;

		for (DrawSequence<VertexT, IndexT>* seq : drawseq)
		{
			// добавляем вертексы последовательности в массив под VBO
			vertices.insert(vertices.end(), 
				seq->GetVertexGroup()->GetVertices().begin(), 
				seq->GetVertexGroup()->GetVertices().end());
			seq->SetVertexOffset(offset);
			offset += seq->GetVertexGroup()->GetVertices().size()/* * sizeof(VertexT)*/;
		}
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::UpdateVertexBuffer()
	{
		glBindVertexArray(va_handle);
		//glBindBuffer(GL_ARRAY_BUFFER, vb_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexT) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

		VertexT::SetupLayout(vb_handle);

		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::UpdateIndexBuffer()
	{
		glBindVertexArray(va_handle);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_handle);
		size_t buf_size = sizeof(IndexT) * indices.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexT) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
		glPrimitiveRestartIndex(primitive_restart);
		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	template <typename VertexT, typename IndexT>
	VertexArray<VertexT, IndexT>::VertexArray()
	{
		try { SetupGLObjects(); }
		catch (std::exception& e) { CleanupGLObjects(); throw e; }
	}

	template <typename VertexT, typename IndexT>
	VertexArray<VertexT, IndexT>::VertexArray(std::vector<DrawSequence<VertexT,IndexT>>& p_sequences) :	drawseq(p_sequences)
	{
		try
		{
			SetupGLObjects();
			RegenerateVertexArray();
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
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::Unbind()
	{
		glBindVertexArray(0);
	}

	template <typename VertexT, typename IndexT>
	void VertexArray<VertexT, IndexT>::Draw(size_t ps_index)
	{
		if (ps_index >= drawseq.size())
			throw std::out_of_range("VertexArray.Draw(): DrawSequence index out of bounds");
		drawseq[ps_index]->Draw();
	}

	template <typename VertexT, typename IndexT>
	class VertexArrayPointer
	{
		VertexArray<VertexT, IndexT>* va = nullptr;
		DrawSequence<VertexT, IndexT>* seq = nullptr;

		void Attach(VertexArray<VertexT, IndexT>* p_va)
		{
			if(p_va != nullptr) va->AddDS(seq);
		}
	public:
		void Free()
		{
			if (va != nullptr && seq !=nullptr)
			{
				va->RemoveDS(seq);
				va = nullptr;
				delete seq;
				seq = nullptr;
			}
		}

		VertexArrayPointer(
			VertexArray<VertexT, IndexT>& varray,
			DrawSequence<VertexT, IndexT>* drawseq
		) : va(&varray), seq(&drawseq)
		{
			Attach(va);
		}

		~VertexArrayPointer()
		{
			Free();
		}

		void Setup(
			VertexArray<VertexT, IndexT>& p_va,
			VertexGroup<VertexT>* vg,
			IndexGroup<IndexT>* ig
		)
		{
			Free();
			va = p_va;
			seq = new DrawSequence<VertexT, IndexT>(vg,ig);
			va->AddDS(seq);
		}

		VertexArrayPointer() {}

		VertexArray<VertexT, IndexT>& GetVertexArray() { return va; }
		DrawSequence<VertexT, IndexT>& GetDrawSequence() { return seq; }
	};
}