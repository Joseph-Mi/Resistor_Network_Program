#ifndef RESISTOR_H
#define RESISTOR_H

#include <iostream>
#include <string>
using namespace std;

class Resistor {
 private:
  string name;             // name of the resistor
  double resistance;       // resistance (in Ohms)
  int endpointNodeIDs[2];  // IDs of nodes it attaches to

 public:
  Resistor(string name_, double resistance_,
           int endpoints_[2]);  // sets name, resistance and endpoint node IDs

  string getName();                        // returns the name
  double getResistance();                  // returns the resistance
  void setResistance(double resistance_);  // sets the resistance

  void print();  // prints resistor info

  int getOtherEndpoint(
      int nodeIndex);  // returns the other node ID that the
                       // resistor is attached to (other than nodeIndex)
};

#endif /* RESISTOR_H */
