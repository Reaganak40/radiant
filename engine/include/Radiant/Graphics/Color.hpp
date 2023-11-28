/***************************************************************/
/*  Graphics/Color.hpp                                         */
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
#include <Radiant/Graphics/Export.hpp>
#include <Radiant/System/MathTypes.hpp>

namespace rdt {
	enum ColorType {
		BLUE,
		RED,
		GREEN,
		ORANGE,
		BLACK,
		WHITE,
		POWERSHELL_BLUE,
		YELLOW,
		PINK
	};

	class RDT_GRAPHICS_API Color {
	private:
		Vec4f m_color;

	public:
		Color();
		Color(float r, float g, float b, float a, bool normalized=true);
		Color(ColorType color);
		~Color() {}

		const Vec4f& SetColor(float r, float g, float b, float a, bool normalized = true);
		const Vec4f& SetColor(const ColorType& nColor);


		const Vec4f& GetColor() const { return m_color; }

		inline Color operator=(const ColorType& nColor)
		{
			SetColor(nColor);
			return (*this);
		}

		inline bool operator==(const ColorType& colorType)
		{
			Color color = colorType;
			auto& colorVals = color.GetColor();
			return (m_color.x1 == colorVals.x1 &&
					m_color.x2 == colorVals.x2 &&
					m_color.x3 == colorVals.x3 &&
					m_color.x4 == colorVals.x4);
		}

		const Vec4f& SetRed(float r, bool normalized=true);
		const Vec4f& SetGreen(float g, bool normalized = true);
		const Vec4f& SetBlue(float b, bool normalized = true);
		const Vec4f& SetAlpha(float a, bool normalized = true);
	};
}