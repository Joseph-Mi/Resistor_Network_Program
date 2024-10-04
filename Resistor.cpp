//
//  Resistor.cpp
//  Lab 3 The Resistor Network Program
// Author: Joseph Mi
// Date: Sept 30 2024

#include "Resistor.h"

#include <iomanip>
#include <iostream>
#include "string.h"


// functions //

// initialize resistor
Resistor::Resistor(string name_, double resistance_, int endpoints_[2])
    : name(name_), resistance(resistance_) {
    endpointNodeIDs[0] = endpoints_[0];
    endpointNodeIDs[1] = endpoints_[1];
}

string Resistor::getName() {
  return name;
}

double Resistor::getResistance() {
  return resistance; 
}

void Resistor::setResistance(double resistance_) {
  resistance = resistance_;
}

void Resistor::print() {
  cout << std::left << std::setw(20) << name << std::right << std::setw(8)
       << std::fixed << std::setprecision(2) << resistance << " Ohms "
       << endpointNodeIDs[0] << " -> " << endpointNodeIDs[1] << endl;
}

int Resistor::getOtherEndpoint(int nodeIndex) {
  if (nodeIndex == endpointNodeIDs[0]) {
    return endpointNodeIDs[1];
  } 
  
  return endpointNodeIDs[0];  // Return the other node ID
  
  //add a last error check if nodeIndex is not in endpointNodeIDs at all if needed
}
