#pragma once

#include "pch.h"
#include "MathTypes.h"

namespace Radiant {
    namespace Utils
    {
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

        /*
            Returns the vertex in the list that is closest to the provided point.
        */
        Vec2d GetClosestVertex(const std::vector<Vec2d>& vertices, const Vec2d& point);

        /*
            Returns the distance between two points.
        */
        double GetDistance(const Vec2d& pointA, const Vec2d& pointB);

    }
}