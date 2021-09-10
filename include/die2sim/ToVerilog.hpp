#pragma once
#include "die2sim/ParserDef.hpp"
#include<string>
#include<vector>
#include<unordered_map>
#include "die2sim/EmitterVerilog.hpp"

namespace verilog{

    using namespace std;

    int executeDef2Verilog(string ConfigFileName, string DefFileName, string vFilename);
    string vFileModuleName(string path);

    class def2verilog{
        private:
            unordered_map<string, string> USC2LSmitllMap;
            unordered_map<string, string> NetListLoc;
            unordered_map<string, vector<string>> NetListPins;
            unordered_map<string, int> NetListPinNo;
            string timeScale;
            string topModuleName;

            string para_dangling_net  = "????";
            bool para_PTL_length_bool = false;
            float para_PTL_length     = -1;
            bool para_mergeIntoSubcir = true;
            vector<string> input_keys;
            vector<string> output_keys;
            vector<string> clock_keys;

            vector<def_component> defComps;
            vector<def_net> defNets;

            unordered_map<string, string> moduleNames;

            verilogemit::VerilogFile vf;

        public:
            int fetchData(string ConfigFileName, string DefFileName);
            void genV();
            void dummyGenV();
    };
}