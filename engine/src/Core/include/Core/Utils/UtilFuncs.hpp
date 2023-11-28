/***************************************************************/
/*  Utils/UtilFuncs.hpp                                        */
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
#include <Core/Utils/Export.hpp>

/***************************************************************
* Forward Delcarations
***************************************************************/
namespace rdt {
    struct Vec2i;
    class String;
}

namespace rdt::utils {

    /*
        Rounds the floating point number to resolve inprecision in float math.
    */
    float RDT_UTILS_API ApplyEpsilon(float num);
    /*
        Rounds the floating point number to resolve inprecision in float math.
    */
    double RDT_UTILS_API ApplyEpsilon(double num);

    /*
        Returns the integer ratio of x and y.
    */
    RDT_UTILS_API Vec2i GetRatio(unsigned int x, unsigned int y);

    /*
        Returns the greatest common divisor between x and y.
    */
    RDT_UTILS_API int GCD(unsigned int x, unsigned int y);

    /*
        Sets the seed for random calls.
    */
    RDT_UTILS_API void SetRandomSeed();

    /*
        Returns a float [min, max] with the assigned precision (default 2 sig figs).
    */
    RDT_UTILS_API float RandomFloat(float min, float max, unsigned int precision = 2);

    /*
        Returns a random int in the range [min, max].
    */
    RDT_UTILS_API int RandInt(const int min, const int max);

    /*
        Reads an ASCII text file and returns the contents to the passed reference.
    */
    RDT_UTILS_API int ReadTextFile(const String& filepath, String& outContents);

    /*
        Replaces all occurances of the target string in src with the replace string.
    */
    RDT_UTILS_API void ReplaceAll(String& src, const String& target, const String& replace);

    /*
        Returns the max between two types
    */
    double        RDT_UTILS_API Max(double       x, double   y);
    float         RDT_UTILS_API Max(float        x, float    y);
    int           RDT_UTILS_API Max(int          x, int      y);
    unsigned int  RDT_UTILS_API Max(unsigned int x, unsigned y);

    /*
        Returns the min between two types
    */
    double        RDT_UTILS_API Min(double       x, double   y);
    float         RDT_UTILS_API Min(float        x, float    y);
    int           RDT_UTILS_API Min(int          x, int      y);
    unsigned int  RDT_UTILS_API Min(unsigned int x, unsigned y);

    /*
        Returns the vertex in the list that is closest to the provided point.
    */
    Vec2d GetClosestVertex(const std::vector<Vec2d>& vertices, const Vec2d& point);

    /*
        Returns the distance between two points.
    */
    double RDT_UTILS_API GetDistance(const Vec2d& pointA, const Vec2d& pointB);
    float RDT_UTILS_API GetDistance(const Vec2f& pointA, const Vec2f& pointB);

    /*
        Returns the manhattan distance between two points.
    */
    Vec2d RDT_UTILS_API GetManhattanDistance(const Vec2d& pointA, const Vec2d& pointB);

    /*
        Returns true if the filepath exists.
    */
    bool RDT_UTILS_API PathExists(const std::string& filepath);

    /*
        Returns in radians the theta between these two points.
    */
    double RDT_UTILS_API GetRotation(const Vec2d& origin, const Vec2d& point);
    float RDT_UTILS_API GetRotation(const Vec2f& origin, const Vec2f& point);

    /*
        Rotates 'point' by dr about the origin.
    */
    void RDT_UTILS_API RotatePoint(const Vec2d& origin, Vec2d& point, const double dr);
    void RDT_UTILS_API RotatePoint(const Vec2f& origin, Vec2f& point, const float dr);

    /*
        Returns an angle in degrees into radians
    */
    float RDT_UTILS_API DegreesToRadians(float degrees);
    float RDT_UTILS_API RadiansToDegrees(float radians);

    /*
        Returns the absolute path to the current working directory.
    */
    std::string GetCWD();

    /*
        Tokenizes a string by the provided delimeter and returns a vector of the tokenized strings
    */
    void RDT_UTILS_API Tokenize(const char* str, const char* delimeter, STRING_ARRAY& tokens);

    /*
        Returns the parent directory of the filepath
    */
    std::string ParentPath(const std::string& path);

    /*
        Copys a file to new destination
    */
    void CopyFileTo(const std::string& src, const std::string& dest);

    /*
        Writes the content to the file at filepath
    */
    void WriteFile(const std::string& filepath, const std::string& content);

    /*
        Scales a line (start, end) by the given scale. Returns the
        new end point.
    */
    Vec2f Scale(Vec2f start, Vec2f end, Vec2f scale);

    /*
        Translates a point by the give translation
    */
    void Translate(Vec2d& point, const Vec2d& translation);
    void Translate(Vec2f& point, const Vec2f& translation);
}