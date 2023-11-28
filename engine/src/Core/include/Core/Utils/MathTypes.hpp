/***************************************************************/
/*  Utils/MathTypes.hpp                                        */
/* *************************************************************/
/*                 This file is a part of:                     */
/*                -- RADIANT GAME ENGINE --                    */
/*         https://github.com/Reaganak40/radiant               */
/***************************************************************/
/*            Copyright(c) 2023 Reagan Kelley                  */
/*                                                             */
/*  Permission  is  hereby  granted, free  of charge, to  any  */
/*  person obtaining a copy of this  software and  associated  */
/*  documentation  files(the  "Software"), to  deal  in   the  */
/*  Software without restriction,including without limitation  */
/*  the   rights   to  use,  copy,  modify,  merge,  publish,  */
/*  distribute,  sublicense,  and  /or  sell  copies  of  the  */
/*  Software,  and to permit persons to whom the  Software is  */
/*  furnished to do so, subject to the following conditions:   */
/*                                                             */
/*  The  above  copyright  notice  and this permission notice  */
/*  shall  be  included in all copies or substantial portions  */
/*  of the Software.                                           */
/*                                                             */
/*  THE  SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF  */
/*  ANY KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT LIMITED  */
/*  TO THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  */
/*  PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT  SHALL  */
/*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  */
/*  DAMAGES OR OTHER  LIABILITY,  WHETHER  IN  AN  ACTION  OF  */
/*  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT  OF  OR IN  */
/*  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  */
/*  IN THE SOFTWARE.                                           */
/***************************************************************/

#pragma once

/***************************************************************
* Headers
***************************************************************/
#include <Radiant/Utils/Export.hpp>

namespace rdt {
	struct Vec2f;

	struct RDT_UTILS_API Vec2d {
		double x, y;

		Vec2d(double nX = 0, double nY = 0)
			: x(nX), y(nY) {}

		Vec2d operator+=(const Vec2d& vector) {
			x += vector.x;
			y += vector.y;
			return (*this);
		}

		Vec2d operator-=(const Vec2d& vector) {
			x -= vector.x;
			y -= vector.y;
			return (*this);
		}

		Vec2d operator*=(const double multiplier) {
			x *= multiplier;
			y *= multiplier;
			return (*this);
		}

		operator Vec2f() const;

		/*
			Returns the magnitude of this vector.
		*/
		double Magnitude() const;

		/*
			Returns the normal (perpendicular) of this vector.
		*/
		Vec2d Normal() const;

		/*
			Returns the unit vector.
		*/
		Vec2d Normalize() const;

		/*
			Returns a zero vector.
		*/
		static Vec2d Zero();

		static double Dot(const Vec2d& A, const Vec2d& B);
	};

	bool  RDT_UTILS_API operator==(const Vec2d& vector1, const Vec2d& vector2);
	Vec2d RDT_UTILS_API operator* (float multiplier, const Vec2d& vector);
	Vec2d RDT_UTILS_API operator* (const Vec2d& vector, float multiplier);
	Vec2d RDT_UTILS_API operator* (double multiplier, const Vec2d& vector);
	Vec2d RDT_UTILS_API operator* (const Vec2d& vector1, const Vec2d& vector2);
	Vec2d RDT_UTILS_API operator* (const Vec2d& vector, double multiplier);
	Vec2d RDT_UTILS_API operator+ (const Vec2d& vector1, const Vec2d& vector2);
	Vec2d RDT_UTILS_API operator- (const Vec2d& vector1, const Vec2d& vector2);
	Vec2d RDT_UTILS_API operator/ (const Vec2d& vector, const double divider);
	Vec2d RDT_UTILS_API operator/ (const Vec2d& vector1, const Vec2d& vector2);

	

	struct RDT_UTILS_API Vec3f {
		float x, y, z;

		Vec3f(float nX = 0, float nY = 0, float nZ = 0)
			: x(nX), y(nY), z(nZ) {}

		static Vec3f Zero();
	};

	struct RDT_UTILS_API Vec4f {
		float x1, x2, x3, x4;

		Vec4f(float nA = 0.0f, float nB = 0.0f, float nC = 0.0f, float nD = 0.0f)
			: x1(nA), x2(nB), x3(nC), x4(nD) {}
	};

	struct RDT_UTILS_API Vec2i {
		int x, y;
		
		Vec2i(int nX=0, int nY=0) 
			: x(nX), y(nY) {}

		Vec2i operator+=(const Vec2i& vector) {
			x += vector.x;
			y += vector.y;
			return (*this);
		}

		Vec2i operator-=(const Vec2i& vector) {
			x -= vector.x;
			y -= vector.y;
			return (*this);
		}

		Vec2i operator*=(int factor) {
			x *= factor;
			y *= factor;
			return (*this);
		}

		/*
			Returns the magnitude of this vector.
		*/
		double Magnitude() const;

		/*
			Returns a zero vector.
		*/
		static Vec2i Zero();
	};
	bool operator==(const Vec2i& vector1, const Vec2i& vector2);
	Vec2i operator+ (const Vec2i& vector1, const Vec2i& vector2);
	Vec2i operator- (const Vec2i& vector1, const Vec2i& vector2);


	struct RDT_UTILS_API Vec2f {
		float x, y;

		Vec2f(float nX = 0, float nY = 0)
			: x(nX), y(nY) {}
		
		/*
				Returns a zero vector.
			*/
		static Vec2f Zero();

		Vec2f operator=(const Vec2d& other) {
			x = (float)other.x;
			y = (float)other.y;
			return (*this);
		}

		operator Vec2d() const;

	};
	


	/*
		Returns an absolute vector with all positive values.
	*/
	Vec2d Vabs(Vec2d vector);

	/*
		Returns an absolute vector with all positive values.
	*/
	Vec2i Vabs(Vec2i vector);

	/*
		Wrapper for angle computation
	*/
	struct Angle {
		float radians = 0;

		Angle operator=(const float other) {
			radians = other;
			return (*this);
		}

		operator float() const { return radians; }
	};
}