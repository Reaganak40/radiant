#pragma once
#include "utils_api.h"
#include "MathTypes.h"
#include "rdt_array.h"

namespace rdt::utils {
   
    /*
        Rounds the floating point number to resolve inprecision in float math.
    */
    float UTILS_API ApplyEpsilon(float num);
    /*
        Rounds the floating point number to resolve inprecision in float math.
    */
    double UTILS_API ApplyEpsilon(double num);

    /*
        Returns the integer ratio of x and y.
    */
    UTILS_API Vec2i GetRatio(unsigned int x, unsigned int y);

    /*
        Returns the greatest common divisor between x and y.
    */
    UTILS_API int GCD(unsigned int x, unsigned int y);

    /*
        Sets the seed for random calls.
    */
    UTILS_API void SetRandomSeed();

    /*
        Returns a float [min, max] with the assigned precision (default 2 sig figs).
    */
    UTILS_API float RandomFloat(float min, float max, unsigned int precision = 2);

    /*
        Returns a random int in the range [min, max].
    */
    UTILS_API int RandInt(const int min, const int max);

    /*
        Reads an ASCII text file and returns the contents to the passed reference.
    */
    UTILS_API int ReadTextFile(const std::string& filepath, std::string& outContents);

    /*
        Replaces all occurances of the target string in src with the replace string.
    */
    UTILS_API void ReplaceAll(std::string& src, const std::string& target, const std::string& replace);

    /*
        Returns the max between two types
    */
    double        UTILS_API Max(double       x, double   y);
    float         UTILS_API Max(float        x, float    y);
    int           UTILS_API Max(int          x, int      y);
    unsigned int  UTILS_API Max(unsigned int x, unsigned y);

    /*
        Returns the min between two types
    */
    double        UTILS_API Min(double       x, double   y);
    float         UTILS_API Min(float        x, float    y);
    int           UTILS_API Min(int          x, int      y);
    unsigned int  UTILS_API Min(unsigned int x, unsigned y);

    /*
        Returns the vertex in the list that is closest to the provided point.
    */
    Vec2d GetClosestVertex(const std::vector<Vec2d>& vertices, const Vec2d& point);

    /*
        Returns the distance between two points.
    */
    double UTILS_API GetDistance(const Vec2d& pointA, const Vec2d& pointB);
    float UTILS_API GetDistance(const Vec2f& pointA, const Vec2f& pointB);

    /*
        Returns the manhattan distance between two points.
    */
    Vec2d UTILS_API GetManhattanDistance(const Vec2d& pointA, const Vec2d& pointB);

    /*
        Returns true if the filepath exists.
    */
    bool UTILS_API PathExists(const std::string& filepath);

    /*
        Returns in radians the theta between these two points.
    */
    double UTILS_API GetRotation(const Vec2d& origin, const Vec2d& point);
    float UTILS_API GetRotation(const Vec2f& origin, const Vec2f& point);

    /*
        Rotates 'point' by dr about the origin.
    */
    void UTILS_API RotatePoint(const Vec2d& origin, Vec2d& point, const double dr);
    void UTILS_API RotatePoint(const Vec2f& origin, Vec2f& point, const float dr);

    /*
        Returns an angle in degrees into radians
    */
    float UTILS_API DegreesToRadians(float degrees);

    float UTILS_API RadiansToDegrees(float radians);

    /*
        Returns the absolute path to the current working directory.
    */
    std::string GetCWD();

    /*
        Tokenizes a string by the provided delimeter and returns a vector of the tokenized strings
    */
    void UTILS_API Tokenize(const char* str, const char* delimeter, STRING_ARRAY& tokens);

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