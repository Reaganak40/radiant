#pragma once

#include "pch.h"
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
            Rounds the double to resolve inprecision in float math.
        */
        double ApplyEpsilon(double d);

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
        float RandomFloat(float min, float max, unsigned int precision = 2);

        /*
            Returns a random int in the range [min, max].
        */
        int RandInt(const int min, const int max);

        /*
            Reads an ASCII text file and returns the contents to the passed reference.
        */
        void ReadTextFile(const std::string& filepath, std::string& outContents);

        /*
            Returns the max between two unsigned integers
        */
        unsigned int Max(unsigned int x, unsigned int y);

        /*
            Returns the max between two doubles
        */
        double Max(double x, double y);

        /*
            Returns the minimum between two doubles
        */
        double Min(double x, double y);

        /*
            Swaps the contents of the two values.
        */
        void Swap(double& A, double& B);
        void Swap(unsigned int& A, unsigned int& B);

        /*
            Returns the vertex in the list that is closest to the provided point.
        */
        Vec2d GetClosestVertex(const std::vector<Vec2d>& vertices, const Vec2d& point);

        /*
            Returns the distance between two points.
        */
        double GetDistance(const Vec2d& pointA, const Vec2d& pointB);

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
        double GetRotation(const Vec2d& origin, const Vec2d& point);

        /*
            Rotates 'point' by dr about the origin.
        */
        void RotatePoint(const Vec2d& origin, Vec2d& point, const double dr);
    }
}