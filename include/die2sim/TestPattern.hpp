#pragma once

#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct TestPatternParams{
    float timeStep;
    float timeDura;
    float clkFreq;   // [GHz]
    float clkDelay;
    int inputPatPeak;	// [uA]
    std::string vec_file_path;
    int bp_delay;
    float ac_delay;
};

struct VecFile{
    std::vector<std::string> inputOrder;
    std::vector<std::vector<bool>> patterns;
};

namespace TestPattern{

    using namespace std;

    VecFile parseVecFile(std::string vecFilePath);
    std::string josimPwlLine(
        std::string inputName, 
        VecFile vecfile, 
        TestPatternParams params
    );
}
