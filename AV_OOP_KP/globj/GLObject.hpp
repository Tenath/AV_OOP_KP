#pragma once
#include <vector>
#include <initializer_list> // vec = {1.0f,2.0f,3.0f}
#include <GL\glew.h>
#include "av_vector_t.hpp"
using namespace av;

class BindableGLObject
{
public :
	virtual void Bind() = 0;
	virtual void Unbind() = 0;
};

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
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};

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
		return sizeof(IndexT) == 1 ? GL_UNSIGNED_BYTE : sizeof(IndexT) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
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

	PrimitiveSequence(PrimitiveType t, std::initializer_list<IndexT> ilst) : type(t), indices(ilst) {}

	void Draw()
	{
		glDrawElements(PrimitiveTypeGetSymbol(type), (GLsizei)indices.size(), GetIndexType(), (void*)offset);
	}
};

template <typename VertexT, typename IndexT> class VertexArray : public BindableGLObject
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

	void SetupGLObjects()
	{
		// Создаём VAO
		glGenVertexArrays(1, &va_handle);
		// Создаём VBO (пустой)
		glGenBuffers(1, &vb_handle);
		// Создаём IBO (пустой)
		glGenBuffers(1, &ib_handle);
		VertexT::SetupVertexArray(va_handle);
	}

	void CleanupGLObjects()
	{
		if (va_handle > 0) glDeleteVertexArrays(1, &va_handle);
		if (vb_handle > 0) glDeleteBuffers(1, &vb_handle);
		if (ib_handle > 0) glDeleteBuffers(1, &ib_handle);
	}

	size_t TotalIndexCount()
	{
		size_t indexcount = 0;

		for (PrimitiveSequence<IndexT>& seq : primseq)
		{
			indexcount += seq.GetIndexCount();
		}

		return indexcount;
	}

	void RegenerateIndexArray()
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

	void UpdateVertexBuffer()
	{
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vb_handle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexT) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
		
		VertexT::SetupLayout(vb_handle);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UpdateIndexBuffer()
	{
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexT) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
public:

	VertexArray()
	{
		try { SetupGLObjects(); }
		catch (std::exception& e) { CleanupGLObjects(); throw e; }
	}

	VertexArray(std::vector<VertexT>& p_vertices, std::vector<PrimitiveSequence<IndexT>>& p_sequences) :
		vertices(p_vertices), primseq(p_sequences)
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

	~VertexArray()
	{
		CleanupGLObjects();
	}

	void Bind()
	{
		glBindVertexArray(va_handle);
		glBindBuffer(GL_ARRAY_BUFFER, vb_handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_handle);
	}

	void Unbind()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Draw(size_t ps_index)
	{
		if (ps_index > primseq.size()) 
			throw std::out_of_range("VertexArray.Draw(): PrimitiveSequence index out of bounds");
		primseq[ps_index].Draw();
	}
};

enum class ShaderType : unsigned char
{
	Vertex=0,
	Geometry=1,
	Fragment=2,
	TessControl=3,
	TessEval=4,
	Undefined
};

struct ShaderFile
{
	std::string filename;
	ShaderType type;

	ShaderFile(const std::string& src, ShaderType t) : filename(src), type(t) {}

	// Загружает CSV-файл со списком шейдеров в программе и формирует список
	static std::vector<ShaderFile> LoadShaderList(const std::string& filename);
};

class Shader
{
private:
	GLuint handle = 0;
	ShaderType type = ShaderType::Undefined;
	bool compiled = false;
	std::string source;
	std::string error;

	void Setup();
public:
	Shader(const std::string& p_source, ShaderType p_type);
	Shader(Shader&& old) noexcept : handle(old.handle), type(old.type),
		compiled(old.compiled), source(old.source), error(old.error)
	{
		old.handle = 0;
	}

	void Compile();

	~Shader();

	GLuint GetHandle() { return handle; }
};

class Program : BindableGLObject
{
private:
	GLuint handle = 0;
	std::vector<Shader> shaders;
	std::string error;
	bool linked = false;

	void Build();
public:
	Program(const std::string& slist_file);
	~Program();

	void Bind();
	void Unbind();

	GLuint GetHandle() { return handle; }
	bool IsBuilt() { return linked; }
	std::string GetError() { return error; }
};