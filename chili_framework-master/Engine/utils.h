#pragma once

#include <algorithm>
#include <vector>
#include <chrono>
#include <string>
#include <numeric>
#include <cmath>

#include "Vec2.h"
#include "Vec3.h"
#include "Mat2.h"
#include "Mat3.h"

#include "Random.h"

using namespace std::chrono;

//-----------------------------------------------------------------------------
template<typename T>
struct Tpoint
{
	Tpoint(const T & _x = 0, const T & _y = 0)
		: m_x(_x), m_y(_y)
	{}
	Tpoint(const Tpoint<T> & src)
		: m_x(src.m_x), m_y(src.m_y)
	{}
	Tpoint(const Tpoint<T> && src)
		: m_x(move(src.m_x)), m_y(move(src.m_y))
	{}
	Tpoint<T> & operator = (const Tpoint<T> & src)
	{
		if (this == &src)
			return *this;
		m_x = src.m_x;
		m_y = src.m_y;
		return *this;
	}
	Tpoint<T> & operator = (const Tpoint<T> && src)
	{
		if (this == &src)
			return *this;
		m_x = move(src.m_x);
		m_y = move(src.m_y);
		return *this;
	}
	inline bool operator == (const Tpoint<T> & rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	inline bool operator != (const Tpoint<T> & rhs) const
	{
		return !(*this == rhs);
	}
	T m_x;
	T m_y;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
struct Trect
{
	Trect(const Tpoint<T> & upleft = {}, const Tpoint<T> & downright = {})
		: m_upleft(upleft), m_downright(downright)
	{}
	Trect(const Trect<T> & src)
		: m_upleft(src.m_upleft), m_downright(src.m_downright)
	{}
	Trect(const Trect<T> && src)
		: m_upleft(move(src.m_upleft)), m_downright(move(src.m_downright))
	{}
	Trect<T> & operator = (const Trect<T> & src)
	{
		if (this == &src)
			return *this;
		m_upleft = src.m_upleft;
		m_downright = src.m_downright;
		return *this;
	}
	Trect<T> & operator = (const Trect<T> && src)
	{
		if (this == &src)
			return *this;
		m_upleft = move(src.m_upleft);
		m_downright = move(src.m_downright);
		return *this;
	}
	Tpoint<T> m_upleft;
	Tpoint<T> m_downright;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CTimer
{
public:
	CTimer(void)
	{
		start = std::chrono::system_clock::now();
	}
	inline double elapsed(void) const
	{
		return (double)(std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now() - start)).count();
	}
	inline void restart(void)
	{
		start = std::chrono::system_clock::now();
	}
private:
	std::chrono::time_point<std::chrono::system_clock> start;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T, typename U, typename V, typename W, typename X>
inline auto map_value(const T& value, const U& inmin, const V& inmax, const W& outmin, const X& outmax)
{
	return outmin + (value - inmin) * (outmax - outmin) / (inmax - inmin);
}

template <typename T>
inline auto constrain_value(const T& x, const  T& a, const T& b)
{
	return std::min(std::max(x, a), b);
}

/**
 *   \brief Rotates point
 *
 *   Rotates point p arount point (cx, cy) by angle (RAD)
 *
 *   \param cx center point X
 *   \param cy center point Y
 *   \param angle angle in radians
 *   \param p point to rotate
 *   \return rotated point 
 *
 **/
inline float2 rotate_point(float cx, float cy, float angle, float2 p)
{
	return float2(std::cos(angle) * (p.x - cx) - std::sin(angle) * (p.y - cy) + cx,
		std::sin(angle) * (p.x - cx) + std::cos(angle) * (p.y - cy) + cy);
}

template <typename T>
inline T sq_distance(const Tpoint<T>& a, const Tpoint<T>& b)
{
	return (T)((a.m_x - b.m_x) * (T)(a.m_x - b.m_x))
		+ (T)((a.m_y - b.m_y) * (T)(a.m_y - b.m_y));
}

inline void gen_random(std::string& str, const int len)
{
	Random rnd;
	str = "";
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i)
		str += alphanum[rnd.next() % (sizeof(alphanum) - 1)];
}

inline void gen_randascii(std::string& str, const int len)
{
	Random rnd;
	str = "";
	static const char arr[] = "!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	for (int i = 0; i < len; ++i)
		str += arr[rnd.next() % (sizeof(arr) - 1)];
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace custom_std
{
	template <class InputIterator, class OutputIterator, class UnaryPredicate>
	OutputIterator copy_if(InputIterator first, InputIterator last,
		OutputIterator result, UnaryPredicate pred)
	{
		while (first != last) {
			if (pred(*first)) {
				*result = *first;
			}
			++result;
			++first;
		}
		return result;
	}
};

namespace perlin {
	static constexpr uint_fast8_t perm[512] = {
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
		7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
		203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
		20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
		166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
		220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
		63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
		200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
		186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118,
		126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
		189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
		70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
		98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
		97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
		241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
		199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
		254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
		195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131,
		13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69,
		142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75,
		0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
		165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229,
		122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
		187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86,
		164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124,
		123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
		227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119,
		248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172,
		9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
		185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210,
		144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
		107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
		115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114,
		67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};

	template<class real>
	inline constexpr real fade(const real t)
	{
		static_assert(std::is_floating_point<real>::value, "real must be a floating point type");
		return pow(t, 3) * (t * (t * 6 - 15) + 10);
	}

	//template<class real>
	//inline constexpr real lerp(const real t, const real a, const real b)
	//{
	//	static_assert(std::is_floating_point<real>::value, "real must be a floating point type");
	//	return a + t * (b - a);
	//}

	template<class real>
	inline constexpr real grad(const int hash, const real x, const real y, const real z)
	{
		static_assert(std::is_floating_point<real>::value, "real must be a floating point type");
		const auto h = hash & 15;
		const auto u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	template<class real>
	inline constexpr real noise(const real x = 0.0, const real y = 0.0, const real z = 0.0)
	{
		static_assert(std::is_floating_point<real>::value, "real must be a floating point type");
		const auto unit_x = (int)floor(x) & 255,
			unit_y = (int)floor(y) & 255,
			unit_z = (int)floor(z) & 255;
		const auto sub_x = x - floor(x),
			sub_y = y - floor(y),
			sub_z = z - floor(z);
		const auto u = fade(sub_x),
			v = fade(sub_y),
			w = fade(sub_z);
		const auto a = perm[unit_x] + unit_y,
			aa = perm[a] + unit_z,
			ab = perm[a + 1] + unit_z,
			b = perm[unit_x + 1] + unit_y,
			ba = perm[b] + unit_z,
			bb = perm[b + 1] + unit_z;
		return (std::lerp(w,
			std::lerp(
				v,
				std::lerp(
					u,
					grad(
						perm[aa],
						sub_x,
						sub_y,
						sub_z
					),
					grad(
						perm[ba],
						sub_x - 1,
						sub_y,
						sub_z
					)
				),
				std::lerp(
					u,
					grad(
						perm[ab],
						sub_x,
						sub_y - 1,
						sub_z
					),
					grad(
						perm[bb],
						sub_x - 1,
						sub_y - 1,
						sub_z
					)
				)
			),
			std::lerp(
				v,
				std::lerp(
					u,
					grad(
						perm[aa + 1],
						sub_x,
						sub_y,
						sub_z - 1
					),
					grad(
						perm[ba + 1],
						sub_x - 1,
						sub_y,
						sub_z - 1
					)
				),
				std::lerp(
					u,
					grad(
						perm[ab + 1],
						sub_x,
						sub_y - 1,
						sub_z - 1
					),
					grad(
						perm[bb + 1],
						sub_x - 1,
						sub_y - 1,
						sub_z - 1
					)
				)
			)
		));
	}
};
//-----------------------------------------------------------------------------

template <typename T>
inline void println(T t)
{
	std::cout << t << "\n";
}

template <typename T, typename ...U>
inline void println(T t, U ...u)
{
	std::cout << t << " ";
	println(u...);
}

//-----------------------------------------------------------------------------
