#pragma once

#include "pch.h"
#include "MathTypes.h"

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
    unsigned int GetMax(unsigned int x, unsigned int y);

}