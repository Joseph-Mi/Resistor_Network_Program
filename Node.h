#ifndef NODE_H
#define NODE_H

#include "Resistor.h"
#define MAX_RESISTORS_PER_NODE 5

using namespace std;
class Node {
 private:
  int numRes;                              // number of resistors on Node
  int resIDArray[MAX_RESISTORS_PER_NODE];  // array to store the IDs of
                                           // resistors in the Node
  double voltage;                          // voltage of Node set to 0 initially
  bool voltageIsSet;  // if the voltage of the node is set by the user

 public:
  Node();  // sets numRes to 0, voltage to 0 and voltageIsSet to false
  bool canAddResistor();  // returns if the number of resistors is less than
                          // MAX_RESISTORS_PER_NODE
  void addResistor(int rIndex);  // adds a resistor to resIDArray array and
                                 // increments number of resistors

  double getVoltage();  // returns voltage
  void setVoltage(
      double voltage_);  // sets voltage and sets voltageIsSet to true
  void setVoltageInternally(
      double voltage_);  // sets voltage temporarily during iterations for solve
                         // command
  bool isVoltageSet();   // returns voltageIsSet

  int getNumRes();       // returns numRes
  int* getResIDArray();  // returns resIDArray
};

#endif /* NODE_H */
