#pragma once

typedef unsigned GLuint;

namespace av
{
	class BindableGLObject
	{
	public:
		virtual GLuint GetHandle() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}