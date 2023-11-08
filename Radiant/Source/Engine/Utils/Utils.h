#pragma once
#include "Core.h"
#include "MathTypes.h"

namespace rdt {

#define FLOAT_EPSILON 0.0001

    namespace Utils
    {
        static double Radians_Up = M_PI / 2;
        static double Radians_Left = M_PI;
        static double Radians_Down = M_PI * 3 / 2;
        static double Radians_Right = 0;

        /*
            Rounds the floating point number to resolve inprecision in float math.
        */
        template<typename T>
        T ApplyEpsilon(T d)
        {
            {
                if (d > 0) {
                    if (abs(d) - abs(floor(d)) < (T)FLOAT_EPSILON) {
                        return floor(d);
                    }
                    else if ((abs(ceil(d)) - abs(d)) < (T)FLOAT_EPSILON) {
                        return ceil(d);
                    }
                }
                else if (d < 0) {
                    if (abs(d) - abs(ceil(d)) < (T)FLOAT_EPSILON) {
                        return ceil(d);
                    }
                    else if ((abs(floor(d)) - abs(d)) < (T)FLOAT_EPSILON) {
                        return floor(d);
                    }
                }

                return d;
            }
        }

        /*
            Returns the integer ratio of x and y.
        */
        Vec2i GetRatio(unsigned int x, unsigned int y);

        /*
            Returns the greatest common divisor between x and y.
        */
        int GCD(unsigned int x, unsigned int y);

        /*
            Sets the seed for random calls.
        */
        void SetRandomSeed();

        /*
            Returns a float [min, max] with the assigned precision (default 2 sig figs).
        */
        float RADIANT_API RandomFloat(float min, float max, unsigned int precision = 2);

        /*
            Returns a random int in the range [min, max].
        */
        int RADIANT_API RandInt(const int min, const int max);

        /*
            Reads an ASCII text file and returns the contents to the passed reference.
        */
        void ReadTextFile(const std::string& filepath, std::string& outContents);

        /*
            Replaces all occurances of the target string in src with the replace string.
        */
        void ReplaceAll(std::string& src, const std::string& target, const std::string& replace);
        
        /*
            Returns the max between two unsigned integers
        */
        unsigned int RADIANT_API Max(unsigned int x, unsigned int y);

        /*
            Returns the max between two doubles
        */
        double RADIANT_API Max(double x, double y);

        /*
            Returns the max between two floats.
        */
        float RADIANT_API Max(float x, float y);

        /*
            Returns the minimum between two doubles
        */
        template<typename T>
        T Min(T x, T y)
        {
            return (x > y ? y : x);
        }

        /*
            Returns the vertex in the list that is closest to the provided point.
        */
        Vec2d GetClosestVertex(const std::vector<Vec2d>& vertices, const Vec2d& point);

        /*
            Returns the distance between two points.
        */
        double RADIANT_API GetDistance(const Vec2d& pointA, const Vec2d& pointB);
        float RADIANT_API GetDistance(const Vec2f& pointA, const Vec2f& pointB);

        /*
            Returns the manhattan distance between two points.
        */
        Vec2d GetManhattanDistance(const Vec2d& pointA, const Vec2d& pointB);

        /*
            Returns true if the filepath exists.
        */
        bool PathExists(const std::string& filepath);

        /*
            Returns in radians the theta between these two points.
        */
        double RADIANT_API GetRotation(const Vec2d& origin, const Vec2d& point);
        float RADIANT_API GetRotation(const Vec2f& origin, const Vec2f& point);

        /*
            Rotates 'point' by dr about the origin.
        */
        void RotatePoint(const Vec2d& origin, Vec2d& point, const double dr);
        void RotatePoint(const Vec2f& origin, Vec2f& point, const float dr);

        /*
            Returns an angle in degrees into radians
        */
        float RADIANT_API DegreesToRadians(float degrees);

        float RADIANT_API RadiansToDegrees(float radians);

        /*
            Returns the absolute path to the current working directory.
        */
        std::string GetCWD();

        /*
            Tokenizes a string by the provided delimeter and returns a vector of the tokenized strings
        */
        void Tokenize(const std::string& str, const std::string& delimeter, std::vector<std::string>& out);

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
        void Translate(Vec2f& point, const Vec2f& translation);
    }
}