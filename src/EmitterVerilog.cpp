#include "die2sim/EmitterVerilog.hpp"
#include <iostream>
#include <fstream>

using namespace verilogemit;

void VerilogFile::emit(string filePath){
    ofstream outfile;
    outfile.open(filePath);
    for (string inc : inclist){
        outfile << "`include \"" << inc << "\"" << endl;
    }
    outfile << "`timescale " << timescale << endl;
    outfile << "module " << topModuleName << " (" << endl;
    vector<Port> inPorts;
    vector<Port> outPorts;
    for (int i = 0; i < ports.size(); i++){
        if (ports[i].portType == OUT){
            outPorts.push_back(ports[i]);
        }else{
            inPorts.push_back(ports[i]);
        }
        outfile << "\t" << ports[i].name;
        if (i+1 != ports.size()){
            outfile << ",";
        } 
        outfile << endl;
    }
    outfile << ");" << endl;
    outfile << "input" << endl << "\t";
    for (int i = 0; i < inPorts.size(); i++){
        outfile << inPorts[i].name;
        if (i+1 != inPorts.size()){
            outfile << ",";
        } else {
            outfile << ";" << endl;
        }
    }
    outfile << endl << "output" << endl << "\t";
    for (int i = 0; i < outPorts.size(); i++){
        outfile << outPorts[i].name;
        if (i+1 != outPorts.size()){
            outfile << ",";
        } else {
            outfile << ";" << endl;
        }
    }
    outfile << endl;
    int nPtls = ptls.size();
    outfile << "reg  [" << nPtls + 1 << ":0] net_r = 0;" << endl;
    outfile << "wire [" << nPtls + 1 << ":0] net_w;" << endl;
    for (Port inport : inPorts){
        outfile << "assign net_w[" << inport.ptlId;
        outfile << "] = " << inport.name << ";" << endl; 
    }
    for (Port outport : outPorts){
        outfile << "assign " << outport.name << " = ";
        outfile << "net_r[" << outport.ptlId << "];" << endl; 
    }
    for (Ptl ptl : ptls){
        outfile << "always @(posedge net_w[" << ptl.id << "] ";
        outfile << "or negedge net_w[" << ptl.id << "]) begin";
        outfile << endl << "\t";
        outfile << "#" << ptl.delay << " ";
        outfile << "net_r[" << ptl.id << "] <= net_w["; 
        outfile << ptl.id << "];" << endl;
        outfile << "end" << endl;
    }
    for (Cell cell : cells){
        outfile << cell.moduleName << " " << cell.cellName << " (";
        vector<Port> ports = cell.ports;
        for (int i = 0; i < ports.size(); i++){
            Port port = ports[i];
            outfile << "." << port.name << "(";
            if (port.ptlId == 0){ //not connected
                outfile << ")";
            }
            else if (port.portType == OUT){
                outfile << "net_w[" << port.ptlId << "])";
            } else {
                outfile << "net_r[" << port.ptlId << "])";
            }
            if (i+1 != ports.size()){
                outfile << ", ";
            }
        }
        outfile << ");" << endl;
    }
    outfile << "endmodule";
    outfile.close();
    
    return;
}
