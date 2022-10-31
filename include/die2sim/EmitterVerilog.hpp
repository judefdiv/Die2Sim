#pragma once
#include<string>
#include<vector>
#include<unordered_set>
#include "die2sim/ParserDef.hpp"

namespace verilogemit{

    using namespace std;

    enum PortType{
        IN,
        OUT,
        CLK,
        UNKNOWN
    };

     struct Port{
        string name;
        PortType portType;
        size_t ptlId;
    };

    struct Ptl{
        size_t id;
        double delay;
    };

    struct Cell{
        string moduleName;
        string cellName;
        vector<Port> ports;
    };

    class VerilogFile{

        private:
            unordered_set<string> inclist;
            string timescale = "";
            string topModuleName = "";
            vector<Port> ports;
            vector<Ptl> ptls;
            vector<Cell> cells; 

        public:
            void emit(string filePath);
            VerilogFile(def_file df){};
            VerilogFile(string timescale, string topModuleName){
                this->timescale = timescale; 
                this->topModuleName = topModuleName;
            };
            VerilogFile(){};
            void addPort(Port port) { ports.push_back(port); };
            void addPtl(Ptl ptl) { ptls.push_back(ptl); };
            void addCell(Cell cell) { cells.push_back(cell); };
            void addInclude(string include) { 
                if (inclist.find(include) == inclist.end()){
                    inclist.insert(include); 
                }
            };
    };
}