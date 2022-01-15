#ifndef AV_VECTOR_HPP
#define AV_VECTOR_HPP

#include <cstddef>
#include <cmath>

#define AV_USE_EXCEPTIONS 1

#if AV_USE_EXCEPTIONS==1
#include <stdexcept>
#endif

namespace av {

template <class T, size_t D> class Vector
{
	protected:
	T vec[D];
	
	template<class C, size_t n> class RecSet
	{
		public:
		
		static void set(C value, C* ref)
		{
			*ref=value;
			RecSet<C,n-1>::set(value,++ref);
		}
	};

	template <class C> class RecSet<C, 0>
	{
		public:
		
		static void set(C value, C* ref) { (void)value; (void)ref; }
	};
	
	public:
	
	template<typename... Args> Vector(Args... args) : vec{args...}
	{
		static_assert(sizeof...(Args)==D, "Argument count must be equal to vector size");
	}
	
	Vector() : vec() {}
	static constexpr size_t size() { return D; }
	
	// Preferred method of access.
	template <size_t n> constexpr T GetElement() const { static_assert(n<=D, "Array index out of bounds."); return vec[n]; }
	template <size_t n> constexpr T Get() const { static_assert(n<=D, "Array index out of bounds."); return vec[n]; }
	template <size_t n> void SetElement(T value) { static_assert(n<=D, "Array index out of bounds."); vec[n]=value; }
	template <size_t n> void Set(T value) { static_assert(n<=D, "Array index out of bounds."); vec[n]=value; }
	
	template<typename... Args> void assign(Args... args)
	{
		static_assert(sizeof...(Args)==D, "Argument count must be equal to vector size");
		vec={args...};
	}
	
	// Range-based for, algorithms
	T* begin() { return vec; }
	T* end() { return &(vec[D]); }
	
	T* data() { return vec; }
	
	// std::array compatibility
	T& operator[](size_t pos) { return vec[pos]; }

	T& at(size_t pos) 
	{ 
		if(pos>=D) throw std::out_of_range("Vector range check");
		else return vec[pos];
	}

	const T& at(size_t pos) const
	{
		if (pos >= D) throw std::out_of_range("Vector range check");
		else return vec[pos];
	}
	
	void fill(const T& value)
	{
		for(size_t i=0; i!=D; ++i)
		{
			vec[i]=value;
		}
	}
	
	void RecFill(const T& value)
	{
		RecSet<T,D>::set(value,&(vec[0]));
	}
	
	void reset()
	{
		for(size_t i=0; i!=D; ++i)
		{
			vec[i]=T();
		}
	}
	
	// Scalar multiplication
	Vector<T,D>& operator*=(const T& multiplier)
	{
		for(size_t i=0; i!=D; ++i)
		{
			vec[i]*=multiplier;
		}
		
		return *this;
	}
	
	Vector<T,D> operator*(const T& multiplier)
	{
		Vector<T,D> result(*this);
		
		for(size_t i=0; i!=D; ++i)
		{
			result.vec[i]*=multiplier;
		}
		
		return result;
	}
	
	Vector<T,D>& operator+=(const Vector<T,D>& v)
	{
		for(size_t i=0; i!=D; ++i)
		{
			vec[i]+=v.vec[i];
		}
		
		return *this;
	}

	Vector<T, D>& operator-=(const Vector<T, D>& v)
	{
		for (size_t i = 0; i != D; ++i)
		{
			vec[i] -= v.vec[i];
		}

		return *this;
	}
	
	Vector<T,D> operator+(const Vector<T,D>& v)
	{
		Vector<T,D> result(*this);
		
		for(size_t i=0; i!=D; ++i)
		{
			result.vec[i]=vec[i]+v.vec[i];
		}
		
		return result;
	}

	Vector<T, D> operator-(const Vector<T, D>& v)
	{
		Vector<T, D> result(*this);

		for (size_t i = 0; i != D; ++i)
		{
			result.vec[i] = vec[i] - v.vec[i];
		}

		return result;
	}

	constexpr T& X() { static_assert(D >= 1, "Vector component index out of bounds."); return vec[0]; }
	constexpr T& Y() { static_assert(D >= 2, "Vector component index out of bounds."); return vec[1]; }
	constexpr T& Z() { static_assert(D >= 3, "Vector component index out of bounds."); return vec[2]; }
	constexpr T& W() { static_assert(D >= 4, "Vector component index out of bounds."); return vec[3]; }

	constexpr const T& X() const { static_assert(D >= 1, "Vector component index out of bounds."); return vec[0]; }
	constexpr const T& Y() const  { static_assert(D >= 2, "Vector component index out of bounds."); return vec[1]; }
	constexpr const T& Z() const { static_assert(D >= 3, "Vector component index out of bounds."); return vec[2]; }
	constexpr const T& W() const { static_assert(D >= 4, "Vector component index out of bounds."); return vec[3]; }
};

// Curiously recurring template pattern
// Template specializations
/*template<typename T> class Vector4 : public Vector<T,4>
{
	public:
	T& x = this->vec[0];
	T& y = this->vec[1];
	T& z = this->vec[2];
	T& w = this->vec[3];

	template<typename... Args> Vector4(Args... args) : Vector<T,4>(args...) {}
	Vector4() {}
};

template<typename T> class Vector3 : public Vector<T, 3>
{
	public:
	T& x = this->vec[0];
	T& y = this->vec[1];
	T& z = this->vec[2];

	template<typename... Args> Vector3(Args... args) : Vector<T, 3>(args...) {}
	Vector3() {}
};

template<typename T> class Vector2 : public Vector<T, 2>
{
	public:
	T& x = this->vec[0];
	T& y = this->vec[1];

	template<typename... Args> Vector2(Args... args) : Vector<T, 2>(args...) {}
	Vector2() {}
};

template<typename T> class Vector1 : public Vector<T, 1>
{
	public:
	T& x = this->vec[0];

	template<typename... Args> Vector1(Args... args) : Vector<T, 1>(args...) {}
	Vector1() {}
};

typedef Vector4<float> Vector4f;
typedef Vector3<float> Vector3f;
typedef Vector2<float> Vector2f;

typedef Vector4<int> Vector4i;
typedef Vector3<int> Vector3i;
typedef Vector2<int> Vector2i;*/

typedef Vector<float,4> Vector4f;
typedef Vector<float,3> Vector3f;
typedef Vector<float,2> Vector2f;

typedef Vector<int,4> Vector4i;
typedef Vector<int,3> Vector3i;
typedef Vector<int,3> Vector2i;

template<typename T, size_t D> T VectorDotProduct(const Vector<T,D>& v1, const Vector<T, D>& v2)
{
	T result = T();
	for (size_t i = 0; i < D; i++)
	{
		result += v1.at(i) * v2.at(i);
	}

	return result;
}

inline float VectorLength(const Vector3f& v)
{
	return std::sqrtf(v.X() * v.X() + v.X() * v.X() + v.Z() * v.Z());
}

inline Vector3f VectorNormalize(const Vector3f& v)
{
	float len = VectorLength(v);
	Vector3f result = v;
	result.X() /= len;
	result.Y() /= len;
	result.Z() /= len;
	return result;
}

template<typename T> Vector<T, 3> VectorCross(const Vector<T, 3>& v1, const Vector<T, 3>& v2)
{
	Vector<T, 3> vr
	{
		v1.Y() * v2.Z() - v1.Z() * v2.Y(),
		v1.Z() * v2.X() - v1.X() * v2.Z(),
		v1.X() * v2.Y() - v1.Y() * v2.X()
	};

	return vr;
}

} // namespace AV

#endif
