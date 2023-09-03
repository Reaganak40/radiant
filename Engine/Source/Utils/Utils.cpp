#include "pch.h"
#include "Utils.h"

namespace Utils {
    
    void SetRandomSeed()
    {
        srand(time(NULL));
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

    unsigned int GetMax(unsigned int x, unsigned int y)
    {
        return (x > y ? x : y);
    }
    
}
