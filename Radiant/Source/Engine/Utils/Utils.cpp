#include "pch.h"
#include "Utils.h"
#include "Engine/Logging/Log.h"

#include <filesystem>

#ifdef RDT_PLATFORM_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace rdt {


    namespace Utils {
        
        Vec2i GetRatio(unsigned int x, unsigned int y)
        {
            int gcd = GCD(x, y);
            return Vec2i(x / gcd, y / gcd);

        }

        int GCD(unsigned int x, unsigned int y)
        {
            if (x == 0) {
                return y;
            }
            else if (y == 0) {
                return x;
            }

            if (y > x) { std::swap(x, y); }

            return GCD(y, x % y);
        }
        void SetRandomSeed()
        {
            srand((unsigned int)time(NULL));
        }

        float RandomFloat(float min, float max, unsigned int precision)
        {
            float val = min + ((max - min) * (std::rand() / (float)RAND_MAX));
            return val;
        }

        int RandInt(const int min, const int max)
        {
            return (rand() % (max - min + 1)) + min;
        }

        void ReadTextFile(const std::string& filepath, std::string& outContents)
        {
            outContents.clear();

            // Read from the text file
            std::ifstream infile(filepath);

            if (!infile.is_open()) {
                RDT_CORE_WARN("Warning: Could not open file: [{}]", filepath);
                return;
            }

            // Use a while loop together with the getline() function to read the file line by line
            std::string line;
            while (std::getline(infile, line)) {
                // Output the text from the file
                outContents += line + "\n";
            }

            // Close the file
            infile.close();
        }

        void ReplaceAll(std::string& src, const std::string& target, const std::string& nStr)
        {
            if (target == nStr) {
                return;
            }

            std::string::size_type n = 0;
            while ((n = src.find(target, n)) != std::string::npos)
            {
                src.replace(n, target.size(), nStr);
                n += nStr.size();
            }
        }

        unsigned int Max(unsigned int x, unsigned int y)
        {
            return (x > y ? x : y);
        }
        double Max(double x, double y)
        {
            return (x > y ? x : y);
        }

        float Max(float x, float y)
        {
            return (x > y ? x : y);
        }

        Vec2d GetClosestVertex(const std::vector<Vec2d>& vertices, const Vec2d& point)
        {
            constexpr double minDistance = std::numeric_limits<double>::max();
            unsigned int minIndex = 0;

            for (unsigned int i = 0; i < vertices.size(); i++) {
                double distance = GetDistance(vertices[i], point);

                if (distance < minDistance) {
                    distance = minDistance;
                    minIndex = i;
                }
            }

            return vertices[minIndex];
        }
        double GetDistance(const Vec2d& pointA, const Vec2d& pointB)
        {
            return sqrt(pow(pointB.x - pointA.x, 2) + pow(pointB.y - pointA.y, 2));
        }

        float RADIANT_API GetDistance(const Vec2f& pointA, const Vec2f& pointB)
        {
            return sqrtf(pow(pointB.x - pointA.x, 2) + pow(pointB.y - pointA.y, 2));
        }

        Vec2d GetManhattanDistance(const Vec2d& pointA, const Vec2d& pointB)
        {
            return pointB - pointA;
        }

        bool PathExists(const std::string& filepath)
        {
            return std::filesystem::exists(filepath);
        }
        double GetRotation(const Vec2d& origin, const Vec2d& point)
        {
            return std::atan2(point.y - origin.y, point.x - origin.x);
        }

        float RADIANT_API GetRotation(const Vec2f& origin, const Vec2f& point)
        {
            return std::atan2f(point.y - origin.y, point.x - origin.x);
        }

        void RotatePoint(const Vec2d& origin, Vec2d& point, const double dr)
        {
            double magnitude = GetDistance(origin, point);
            double theta = GetRotation(origin, point) + dr;
            point.x = origin.x + (magnitude * std::cos(theta));
            point.y = origin.y + (magnitude * std::sin(theta));
        }

        void RotatePoint(const Vec2f& origin, Vec2f& point, const float dr)
        {
            float magnitude = GetDistance(origin, point);
            float theta = GetRotation(origin, point) + dr;
            point.x = origin.x + (magnitude * std::cos(theta));
            point.y = origin.y + (magnitude * std::sin(theta));
        }

        float RADIANT_API DegreesToRadians(float degrees)
        {
            return degrees * ((float)M_PI) / 180.0f;
        }

        float RADIANT_API RadiansToDegrees(float radians)
        {
            return radians * 180 / ((float)M_PI);
        }

        std::string GetCWD()
        {
            char buff[FILENAME_MAX]; //create string buffer to hold path

#pragma warning( push )
#pragma warning( disable : 6031)
            GetCurrentDir(buff, FILENAME_MAX);
#pragma warning( pop ) 

            std::string current_working_dir(buff);
            return current_working_dir;
        }
        void Tokenize(const std::string& str, const std::string& delimeter, std::vector<std::string>& out)
        {
            std::string delimProgress = "";
            std::string curr = "";
            for (char c : str) {

                if (delimProgress.size() < delimeter.size() && delimeter[delimProgress.size()] == c) {
                    delimProgress += c;
                }
                else {
                    curr += delimProgress + c;
                    delimProgress.clear();
                }

                if (delimProgress.size() == delimeter.size()) {
                    out.push_back(curr);
                    curr.clear();
                    delimProgress.clear();
                }
            }

            if (curr.size() > 0) {
                out.push_back(curr);
            }
        }
        std::string ParentPath(const std::string& path)
        {
            size_t pos = path.find_last_of("\\/");
            return (std::string::npos == pos) ? "" : path.substr(0, pos);
        }
        void CopyFileTo(const std::string& src, const std::string& dest)
        {
            CopyFile(std::wstring(src.begin(), src.end()).c_str(), std::wstring(dest.begin(), dest.end()).c_str(), true);
        }
        void WriteFile(const std::string& filepath, const std::string& content)
        {
            std::ofstream ofile;
            ofile.open(filepath, std::ofstream::binary);
            ofile << content;
            ofile.close();
        }

        Vec2f Scale(Vec2f start, Vec2f end, Vec2f scale)
        {
            float dx = end.x - start.x;
            float dy = end.y - start.y;

            return Vec2f(scale.x * dx, scale.y * dy);
        }
        void Translate(Vec2d& point, const Vec2d& translation)
        {
            point.x += translation.x;
            point.y += translation.y;
        }
        void Translate(Vec2f& point, const Vec2f& translation)
        {
            point.x += translation.x;
            point.y += translation.y;
        }
  
    }
}