#include "pch.h"
#include "Utils.h"

namespace Radiant {


    namespace Utils {
        double ApplyEpsilon(double d)
        {
            if (abs(d) - abs(int(d)) < FLOAT_EPSILON) {
                return (int)d;
            }
            else if ((abs(ceil(d)) - abs(d)) < FLOAT_EPSILON) {
                return ceil(d);
            }

            return d;
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
                printf("Warning: Could not open file: [%s]\n", filepath.c_str());
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

        unsigned int Max(unsigned int x, unsigned int y)
        {
            return (x > y ? x : y);
        }
        double Max(double x, double y)
        {
            return (x > y ? x : y);
        }

        double Min(double x, double y)
        {
            return (x > y ? y : x);
        }

        void Swap(double& A, double& B)
        {
            double temp = A;
            A = B;
            B = temp;
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
    }
}