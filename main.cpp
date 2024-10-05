//
//  main.cpp
//  Lab 3 The Resistor Network Program
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-30
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"

using namespace std;

Resistor** resistors =
    nullptr;  // Pointer that should point to an array of Resistor pointers
Node* nodes = nullptr;  // pointer that should hold address to an array of Nodes
int maxNodeNumber = 0;  // maximum number of nodes as set in the command line
int maxResistors = 0;  // maximum number of resistors as set in the command line
int resistorsCount = 0;  // count the number of resistors

string errorArray[10] = {
    "invalid command",                                  // 0
    "invalid argument",                                 // 1
    "negative resistance",                              // 2
    "node value is out of permitted range",             // 3
    "resistor name cannot be keyword \"all\"",          // 4
    "both terminals of resistor connect to same node",  // 5
    "too few arguments",                                // 6
};

// Function Prototypes
bool getInteger(stringstream& ss, int& x);
bool getString(stringstream& ss, string& s);
bool getDouble(stringstream& ss, double& s);

void handleMaxVal(stringstream& ss);
void handleInsertR(stringstream& ss);
void handleModifyR(stringstream& ss);
void handlePrintR(stringstream& ss);
void handleDeleteR(stringstream& ss);
void handleSetV(stringstream& ss);

bool getInteger(stringstream& ss, int& x) {
  ss >> x;
  return !ss.fail();
}

bool getString(stringstream& ss, string& s) {
  ss >> s;
  return !ss.fail();
}

bool getDouble(stringstream& ss, double& s) {
  ss >> s;
  return !ss.fail();
}

int main() {
  string line, cmd;
  // TODO: Implement the main function here

  while (true) {
    cout << ">>> ";
    cout.flush();

    if (!getline(cin, line)) {
      break; // end of line ezit
    }

    stringstream ss(line);
    if (!(ss >> cmd)) {
      continue;
    }

    try {
      if (cmd == "maxVal") {
          handleMaxVal(ss);
      } else if (cmd == "insertR") {
          handleInsertR(ss);
      } else if (cmd == "modifyR") {
          handleModifyR(ss);
      } else if (cmd == "printR") {
          handlePrintR(ss);
      } else if (cmd == "deleteR") {
          handleDeleteR(ss);
      } else if (cmd == "setV") {
          handleSetV(ss);
      } else {
          cout << "Error: " << errorArray[0] << endl;  // invalid command
      }
    } catch (const char* error) {
      cout << "Error: " << error << endl;
    }
  }

  delete[] nodes;
    for (int i = 0; i < resistorsCount; i++) {
        delete resistors[i];
    }
  delete[] resistors;

  return 0;
}

// TODO: Implement functions here

void handleMaxVal(stringstream& ss) {
  int newMaxNodeNumber, newMaxResistors;
    
  if (!getInteger(ss, newMaxNodeNumber) || !getInteger(ss, newMaxResistors)) {
      throw errorArray[6];  // too few arguments
  }
    
  if (newMaxNodeNumber <= 0 || newMaxResistors <= 0) {
      throw errorArray[1];  // invalid argument
  }

  delete[] nodes;
  for (int i = 0; i < resistorsCount; i++) {
      delete resistors[i];
  }
  delete[] resistors;

  maxNodeNumber = newMaxNodeNumber;
  maxResistors = newMaxResistors;
  nodes = new Node[maxNodeNumber];
  resistors = new Resistor*[maxResistors];
  resistorsCount = 0;

  cout << "New network: max node number is " << maxNodeNumber 
       << "; max resistors is " << maxResistors << endl;
}

void handleInsertR(stringstream& ss) {
  string name;
  double resistance;
  int nodeID1, nodeID2;

  if (!getString(ss, name) || !getDouble(ss, resistance) || !getInteger(ss, nodeID1) || !getInteger(ss, nodeID2)) {
    throw errorArray[6];  // too few arguments
  }

  if (name == "all") {
    throw errorArray[4];  // resistor name cannot be the keyword "all"
  }

  if (resistance < 0) {
    throw errorArray[2]; //negative R
  }

  if (nodeID1 == nodeID2) {
    throw errorArray[5]; // both resistor terminals to same node
  }

  if (nodeID1 > maxNodeNumber || nodeID2 > maxNodeNumber || nodeID1 <= 0|| nodeID2 <= 0) {
    throw errorArray[3];
  }

  for (int i = 0; i < resistorsCount; i++) {
    if (resistors[i]->getName() == name) {
        throw "resistor name already exists";
    }
  }

  // if (resistorsCount >= maxResistors) {
  //       throw "max number of resistors reached";
  // }

   // Check if nodes can accept more resistors
  if (!nodes[nodeID1 - 1].canAddResistor() || !nodes[nodeID2 - 1].canAddResistor()) {
      throw "node has reached maximum number of resistors";
  }

  // Create new resistor
  int endpoints[2] = {nodeID1 - 1, nodeID2 - 1};
  Resistor* newResistor = new Resistor(name, resistance, endpoints);
  resistors[resistorsCount] = newResistor;

    // Update nodes
  nodes[nodeID1 - 1].addResistor(resistorsCount);
  nodes[nodeID2 - 1].addResistor(resistorsCount);
  resistorsCount++;

  cout << "Inserted: resistor " << name << " " << fixed << setprecision(2) 
       << resistance << " Ohms " << nodeID1 << " -> " << nodeID2 << endl;
}

void handleModifyR(stringstream& ss) {
    string name;
    double newResistance;

    if (!getString(ss, name) || !getDouble(ss, newResistance)) {
        throw errorArray[6];  // too few arguments
    }

    if (newResistance < 0) {
        throw errorArray[2];  // negative resistance
    }

    // Find and modify the resistor
    for (int i = 0; i < resistorsCount; i++) {
        if (resistors[i]->getName() == name) {
            double oldResistance = resistors[i]->getResistance();
            resistors[i]->setResistance(newResistance);
            cout << "Modified: resistor " << name << " from " << fixed << setprecision(2) 
                 << oldResistance << " Ohms to " << newResistance << " Ohms" << endl;
            return;
        }
    }

    throw "resistor name not found";
}

void handlePrintR(stringstream& ss) {
    string name;

    if (!getString(ss, name)) {
        throw errorArray[6];  // too few arguments
    }

    // Find and print the resistor
    for (int i = 0; i < resistorsCount; i++) {
        if (resistors[i]->getName() == name) {
            cout << "Print: ";
            resistors[i]->print();
            return;
        }
    }

    throw "resistor name not found";
}

void handleDeleteR(stringstream& ss) {
    string arg;

    if (!getString(ss, arg)) {
        throw errorArray[6];  // too few arguments
    }

    if (arg == "all") {
        // Delete all resistors
        for (int i = 0; i < resistorsCount; i++) {
            delete resistors[i];
        }
        resistorsCount = 0;
        
        // Reset all nodes
        for (int i = 0; i < maxNodeNumber; i++) {
            while (nodes[i].getNumRes() > 0) {
                nodes[i].addResistor(-1);  // Remove a resistor by adding an invalid index
            }
        }

        cout << "Deleted: all resistors" << endl;
    } else {
        throw errorArray[1];  // invalid argument
    }
}

void handleSetV(stringstream& ss) {
    int nodeId;
    double voltage;

    if (!getInteger(ss, nodeId) || !getDouble(ss, voltage)) {
        throw errorArray[6];  // too few arguments
    }

    if (nodeId < 1 || nodeId > maxNodeNumber) {
        throw errorArray[3];  // node value is out of permitted range
    }

    nodes[nodeId - 1].setVoltage(voltage);
    cout << "Set: node " << nodeId << " to " << fixed << setprecision(2) << voltage << " Volts" << endl;
}

