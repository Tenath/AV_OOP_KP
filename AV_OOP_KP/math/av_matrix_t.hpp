#pragma once
#ifndef AV_MATRIX_T_HPP
#define AV_MATRIX_T_HPP

#include <cstddef>
#include <stdexcept>

// (c) 2013-2020 Andrei Veeremaa

namespace av {

	template <class T, size_t W, size_t H> class Matrix
	{
	public:
		// Data
		T data[W * H];

		// Methods
		constexpr unsigned area() const { return W * H; }

		// Slow, argument-checking, failsafe. Use for singular access.
		T Get(size_t r, size_t c) const { if (c >= W || r >= H) return T(); else return data[c * H + r]; }
		void Set(size_t r, size_t c, const T& value) { if (c >= W || r >= H) return; data[c * H + r] = value; }

		// Safe version
		T* GetPtr(size_t r, size_t c) { if (c >= W || r >= H) return end(); else return data + c * H + r; }
		const T* GetPtr(size_t r, size_t c) const { if (c >= W || r >= H) return end(); else return data + c * H + r; }

		// The "Either I'm dumb or I really know what I'm doing" version, no checks.
		T* ComputePtr(size_t r, size_t c)
		{
			return data + c * H + r;
		}

		const T* ComputePtr(size_t r, size_t c) const
		{
			return data + c * H + r;
		}

		const T* begin() const { return data; }
		const T* end() const { return data + area(); }

		T* begin() { return data; }
		T* end() { return data + area(); }

		inline T* colptr(size_t column) { if (column > H) return 0; return data + (H * column); }

		Matrix()
		{
			for (size_t i = 0; i < area(); i++)
			{
				data[i] = T();
			}
		}

		// Piecewise multiplication
		Matrix<T, W, H> pwmult(const Matrix<T, W, H>& m)
		{
			Matrix<T, W, H> result;

			T* thisptr = this->begin();
			const T* mptr = m.begin();
			T* rptr = result.begin();

			T* endptr = this->end();
			while (thisptr != endptr)
			{
				*(rptr++) = *(mptr++) * *(thisptr++);
			}

			return result;
		}

		T& operator()(size_t r, size_t c) { return (c >= W || r >= H) ? throw std::invalid_argument("Matrix row/column argument out of range") : data[c * H + r]; }
		const T& operator()(size_t r, size_t c) const { return (c >= W || r >= H) ? throw std::invalid_argument("Matrix row/column argument out of range") : data[c * H + r]; }
		//T& operator[](size_t r, size_t c) { return (c >= W || r >= H) ? T() : data[c * H + r]; }
		//const T& operator[](size_t r, size_t c) { return (c >= W || r >= H) ? T() : data[c * H + r]; }

		/* Matrix Addition */
		Matrix<T, W, H>& operator+=(const Matrix<T, W, H>& m)
		{
			T* thisptr = this->begin();
			const T* mptr = m.begin();

			T* endptr = this->end();
			while (thisptr != endptr)
			{
				*(thisptr++) *= *(mptr++);
			}

			return *this;
		}

		Matrix<T, W, H> operator+(const Matrix<T, W, H>& m)
		{
			Matrix<T, W, H> result;

			T* thisptr = this->begin();
			const T* mptr = m.begin();
			T* rptr = result.begin();

			T* endptr = this->end();
			while (thisptr != endptr)
			{
				*(rptr++) = *(mptr++) + *(thisptr++);
			}

			return result;
		}
		/* Matrix Addition - End */

		Matrix<T, W, H>& operator*=(const T& scalar)
		{
			for (T& r : *this)
			{
				r *= scalar;
			}

			return *this;
		}

		Matrix<T, W, H> operator*(const T& scalar)
		{
			Matrix<T, W, H> result;

			T* rptr = result.begin();
			for (T& r : *this)
			{
				*(rptr++) = r * scalar;
			}

			return result;
		}

		Matrix<T, W, H>& operator=(const Matrix<T, W, H>& other)
		{
			for (int i = 0; i < H; i++)
			{
				for (int j = 0; j < W; j++)
				{
					data[j * H + i] = other.data[j * H + i];
				}
			}

			return *this;
		}

		/*Matrix<T, W, H>& operator=(const Matrix<T, W-1, H-1>& other)
		{
			for (int i = 0; i < H-1; i++)
			{
				for (int j = 0; j < W-1; j++)
				{
					data[j * H + i] = other(i,j);
				}
			}

			return *this;
		}*/

		private:
		static constexpr size_t DetW() { return W > 1 ? W - 1 : 1; }
		static constexpr size_t DetH() { return H > 1 ? H - 1 : 1; }
		
		public:
		void explicit_minor_assign(const Matrix<T, W-1, H-1>& other)
		{
			for (int i = 0; i < H - 1; i++)
			{
				for (int j = 0; j < W - 1; j++)
				{
					data[j * H + i] = other(i, j);
				}
			}
		}

		public:

		/*template<> T det<T, 1, 1>()
		{
			return data[0];
		}*/

		Matrix<T, W - 1, H - 1> minor(size_t r, size_t c)
		{
			const size_t Wm = W - 1; const size_t Hm = H - 1;

			Matrix<T, W - 1, H - 1> result;

			// top-left corner TODO: FIX
			for (size_t x = 0; x < c; x++)
			{
				for (size_t y = 0; y < r; y++)
				{
					result(x, y) = (*this)(x, y);
				}
			}

			// top-right corner TODO: FIX
			for (size_t x = c + 1; x < W; x++)
			{
				for (size_t y = 0; y < r; y++)
				{
					result(y, x - 1) = (*this)(y, c);
				}
			}

			// bottom-left corner
			for (size_t x = 0; x < c; x++)
			{
				for (size_t y = r + 1; y < H; y++)
				{
					result(y - 1 - r, x) = (*this)(y,x);
				}
			}

			// bottom-right corner
			for (size_t x = c + 1; x < W; x++)
			{
				for (size_t y = r + 1; y < H; y++)
				{
					result(y - 1 - r, x - 1) = (*this)(y, x);
				}
			}

			return result;
		}

		T det()
		{
			return det_<T,W,H>();
		}

		template<class T, size_t W, size_t H> T det_()
		{
			T result = T();

			/* No. This here is the wrong formula, only valid for (3,3) matrix */
			// Direct, Positive diagonals [check]
			/*for (size_t c = 0; c < W; c++)
			{
				T colres = *GetPtr(0,c);
				for (size_t r = 1; r < H; r++)
				{
					colres *= *GetPtr(r, (c+r)%W);
				}

				result += colres;
			}

			// Inverse, Negative diagonals [check]
			for (size_t c = 0; c < W; c++)
			{
				size_t inv_c = W-1-c;
				T colres = *GetPtr(0, inv_c);
				for (size_t r = 1; r < H; r++)
				{
					colres *= *GetPtr(r, (inv_c - r) % W);
				}

				result -= colres;
			}*/
			if (W == 1 && H == 1) return data[0];

			for (size_t c = 0; c < W; c++)
			{
				// Figure out the sign
				//T sgn = (c % 2 == 0) ? T() : -T();
				//T colres = sgn * (*this)(0, c);

				//Matrix<T, Matrix<T, W, H>::DetW(), Matrix<T, W, H>::DetH()> min = minor(0, c);
				//Matrix<T, W-1, H-1> min = ;
				if ((*this)(0, c) == 0) continue;
				T colres = (*this)(0,c)*minor(0, c).det_<T,W-1,H-1>();
				if (c % 2 != 0) colres = -colres;

				result += colres;
			}

			return result;
		}

		template<> T det_<T, 1, 0>() { return data[0]; }
		template<> T det_<T, 0, 0>() { return 1; }
		template<> T det_<T, 1, 1>() { return data[0]; }
		template<> T det_<T, 0, 1>() { return 1; }

		Matrix<T, W, H> cofactor()
		{
			Matrix<T, W, H> result;
			
			for (int i = 0; i < H; i++)
			{
				for (int j = 0; j < W; j++)
				{
					bool sgn = ((i + 1) + (j + 1)) % 2 == 0;

					Matrix<T,W-1,H-1> m = minor(j, i);
					result(i,j) = sgn ? m.det() : -m.det();
				}
			}

			return result;
		}
	
		Matrix<T, H, W> inverse()
		{
			T d = det();

			Matrix<T, H, W> result = TransposeMatrix(cofactor());

			for (int i=0; i < H; i++)
			{
				for (int j = 0; j < W; j++)
				{
					result(i, j) = result(i, j) / d;
				}
			}

			return result;
		}
	};

	template <class T, size_t RowL, size_t ColLRowR, size_t ColR>
	Matrix<T, ColR, RowL> operator*(const Matrix<T, ColLRowR, RowL>& lhs, const Matrix<T, ColR, ColLRowR>& rhs)
	{
		// Generic, unaccelerated solution
		Matrix<T, ColR, RowL> result;

		T* rptr = result.begin();
		const T* lhptr = lhs.begin();
		const T* rhptr = rhs.begin();

		const T* rhcolstart = rhptr;

		float cell_value = 0.0f;

		for (unsigned c = 0; c != ColR; c++)
		{
			for (unsigned r = 0; r != RowL; r++)
			{
				rhptr = rhs.ComputePtr(0, c);
				lhptr = lhs.ComputePtr(r, 0);

				for (unsigned d = 0; d != ColLRowR; d++)
				{
					cell_value += (*lhptr) * (*rhptr);
					++rhptr;
					lhptr += ColLRowR;
				}
				*rptr = cell_value;
				cell_value = 0.0f;
				++rptr;
			}
		}

		return result;
	}

	template <class T, size_t W, size_t H> Matrix<T, H, W> TransposeMatrix(const Matrix<T, W, H>& src)
	{
		Matrix<T, H, W> dst;

		const T* endptr = src.end();
		T* dstptr = dst.begin();
		for (const T* ptr = src.begin(); ptr != endptr; ptr++)
		{
			*dstptr = *ptr;
			dstptr++;
		}

		return dst;
	}

	template <class T, size_t S> Matrix<T, S, S> DiagonalMatrix(T value)
	{
		Matrix<T, S, S> res;
		T* endptr = res.end(); size_t inc = S + 1;
		for (T* ptr = res.begin(); ptr < endptr; ptr += inc) *ptr = value;
		return res;
	}

	template <size_t S> Matrix<float, S, S> IdentityMatrixf()
	{
		Matrix<float, S, S> res;
		for (int i = 0; i < S; i++)
		{
			res(i, i) = 1.0f;
		}
		return res;
	}

	/*
	template<class T, size_t sz> float DeterminantMatrixf(const Matrix<T,sz,sz>&)
	{
		float result=0.0f;
		// ...
		return result;
	}
	*/

	/* Typedefs for float and double matrices */

	// Float uneven 2-4
	typedef Matrix<float, 2, 3> Matrix2x3f;
	typedef Matrix<float, 2, 4> Matrix2x4f;

	typedef Matrix<float, 3, 2> Matrix3x2f;
	typedef Matrix<float, 3, 4> Matrix3x4f;

	typedef Matrix<float, 4, 2> Matrix4x2f;
	typedef Matrix<float, 4, 3> Matrix4x3f;

	// Float square 2-4
	typedef Matrix<float, 2, 2> Matrix2f;
	typedef Matrix<float, 3, 3> Matrix3f;
	typedef Matrix<float, 4, 4> Matrix4f;

	// Double uneven 2-4
	typedef Matrix<double, 2, 3> Matrix2x3d;
	typedef Matrix<double, 2, 4> Matrix2x4d;

	typedef Matrix<double, 3, 2> Matrix3x2d;
	typedef Matrix<double, 3, 4> Matrix3x4d;

	typedef Matrix<double, 4, 2> Matrix4x2d;
	typedef Matrix<double, 4, 3> Matrix4x3d;

	// Double square 2-4
	typedef Matrix<double, 2, 2> Matrix2d;
	typedef Matrix<double, 3, 3> Matrix3d;
	typedef Matrix<double, 4, 4> Matrix4d;
	/* Typedefs for float and double matrices - END */

} // namespace msp

#endif