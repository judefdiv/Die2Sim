#include "die2sim/TestPattern.hpp"
#include <string>

using namespace TestPattern;

VecFile TestPattern::parseVecFile(std::string vecFilePath){
    // Read entire vecfile into memory
    ifstream t(vecFilePath);
    stringstream buffer;
    buffer << t.rdbuf();

    VecFile f;

    // parse input order
    string line;
    getline(buffer, line);
    string token;
    for (const auto c : line){
        if (c == ','){
            f.inputOrder.push_back(token); 
            token.clear();
        } else {
            token.push_back(c);
        }
    }
    f.inputOrder.push_back(token); 

    // parse test patterns
    while (!buffer.eof()){  
        getline(buffer, line);
        if (line == "END") break;
        vector<bool> pattern;
        pattern.reserve(line.size());
        for (const auto c : line)
            pattern.push_back(c == '1');
        f.patterns.push_back(pattern);
    }

    return f;
}

std::string TestPattern::josimPwlLine(
        std::string inputName, 
        VecFile vecfile, 
        TestPatternParams params)
{
    // Find the index of the pattern associated with inputName
    int patternIdx = 0;
    for (const auto name : vecfile.inputOrder){
        if (name == inputName) break;
        else patternIdx++;
    }

    std::string pwlLine = "pwl(0 0 ";
    int time = (int) (params.clkDelay + params.ac_delay);
    int clkPeriod = 1000.0/params.clkFreq;
    for (const auto pattern : vecfile.patterns){
        pwlLine += 
            to_string(time-5) 
            + "p 0 " + to_string(time) 
            + "p " + ( pattern[patternIdx] ? to_string(params.inputPatPeak) : "0" )
            + "u " + to_string(time+5) 
            + "p 0 ";
        //increment time
        time += (int) (params.bp_delay * clkPeriod);
    }
    // End pwlLine
    pwlLine += ")";

    return pwlLine;
}