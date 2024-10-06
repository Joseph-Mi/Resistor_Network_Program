#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"

using namespace std;

Resistor** resistors = nullptr;
Node* nodes = nullptr;
int maxNodeNumber = 0;
int maxResistors = 0;
int resistorsCount = 0;

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

int main() {
    string line, cmd;

    while (true) {
        cout << ">>> ";
        cout.flush();

        if (!getline(cin, line)) {
            break;
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
                cout << "Error: " << errorArray[0] << endl;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    delete[] nodes;
    for (int i = 0; i < resistorsCount; i++) {
        delete resistors[i];
    }
    delete[] resistors;

    return 0;
}

bool getInteger(stringstream& ss, int& x) {
    string temp;
    if (ss >> temp) {
        try {
            x = stoi(temp);
            return true;
        } catch (const invalid_argument&) {
            throw invalid_argument(errorArray[1]);  // invalid argument (type)
        } catch (const out_of_range&) {
            throw invalid_argument(errorArray[1]);  // invalid argument (out of range)
        }
    }
    throw invalid_argument(errorArray[6]);  // too few arguments
}

bool getString(stringstream& ss, string& s) {
    if (ss >> s) {
        return true;
    }
    throw invalid_argument(errorArray[6]);  // too few arguments
}

bool getDouble(stringstream& ss, double& s) {
    string temp;
    if (ss >> temp) {
        try {
            s = stod(temp);
            return true;
        } catch (const invalid_argument&) {
            throw invalid_argument(errorArray[1]);  // invalid argument type
        } catch (const out_of_range&) {
            throw invalid_argument(errorArray[1]);  // invalid argument (out of range)
        }
    }
    throw invalid_argument(errorArray[6]);  // too few arguments
}

void handleMaxVal(stringstream& ss) {
    int newMaxNodeNumber, newMaxResistors;
    
    getInteger(ss, newMaxNodeNumber);
    getInteger(ss, newMaxResistors);
    
    if (newMaxNodeNumber <= 0 || newMaxResistors <= 0) {
        throw invalid_argument(errorArray[1]);  // invalid argument
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

    getString(ss, name);
    getDouble(ss, resistance);
    getInteger(ss, nodeID1);
    getInteger(ss, nodeID2);

    if (name == "all") {
        throw invalid_argument(errorArray[4]);
    }

    if (resistance < 0) {
        throw invalid_argument(errorArray[2]);
    }

    if (nodeID1 == nodeID2) {
        throw invalid_argument(errorArray[5]);
    }
 
    if (nodeID1 > maxNodeNumber || nodeID2 > maxNodeNumber || nodeID1 <= 0 || nodeID2 <= 0) {
        throw invalid_argument(errorArray[3]);
    }

    for (int resistor = 0; resistor < resistorsCount; resistor++) {
        if (resistors[resistor]->getName() == name) {
            throw invalid_argument("resistor name already exists");
        }
    }

    if (resistorsCount >= maxResistors) {
        throw invalid_argument("maximum number of resistors reached");
    }

    if (!nodes[nodeID1 - 1].canAddResistor() || !nodes[nodeID2 - 1].canAddResistor()) {
        throw invalid_argument("node has reached maximum number of resistors");
    }

    int endpoints[2] = {nodeID1 - 1, nodeID2 - 1};
    Resistor* newResistor = new Resistor(name, resistance, endpoints);
    resistors[resistorsCount] = newResistor;

    nodes[nodeID1 - 1].addResistor(resistorsCount);
    nodes[nodeID2 - 1].addResistor(resistorsCount);
    resistorsCount++;

    cout << "Inserted: resistor " << name << " " << fixed << setprecision(2) 
         << resistance << " Ohms " << nodeID1 << " -> " << nodeID2 << endl;
}

void handleModifyR(stringstream& ss) {
    string name;
    double newResistance;

    getString(ss, name);
    getDouble(ss, newResistance);

    if (newResistance < 0) {
        throw invalid_argument(errorArray[2]);
    }

    for (int i = 0; i < resistorsCount; i++) {
        if (resistors[i]->getName() == name) {
            double oldResistance = resistors[i]->getResistance();
            resistors[i]->setResistance(newResistance);
            cout << "Modified: resistor " << name << " from " << fixed << setprecision(2) 
                 << oldResistance << " Ohms to " << newResistance << " Ohms" << endl;
            return;
        }
    }

    throw invalid_argument("resistor name not found");
}

void handlePrintR(stringstream& ss) {
    string name;

    getString(ss, name);

    for (int i = 0; i < resistorsCount; i++) {
        if (resistors[i]->getName() == name) {
            cout << "Print: ";
            resistors[i]->print();
            return;
        }
    }

    throw invalid_argument("resistor name not found");
}

void handleDeleteR(stringstream& ss) {
    string arg;

    getString(ss, arg);

    if (arg == "all") {
        for (int i = 0; i < resistorsCount; i++) {
            delete resistors[i];
        }
        resistorsCount = 0;
        
        for (int i = 0; i < maxNodeNumber; i++) {
            while (nodes[i].getNumRes() > 0) {
                nodes[i].addResistor(-1);
            }
        }

        cout << "Deleted: all resistors" << endl;
    } else {
        throw invalid_argument(errorArray[1]);
    }
}

void handleSetV(stringstream& ss) {
    int nodeId;
    double voltage;

    getInteger(ss, nodeId);
    getDouble(ss, voltage);

    if (nodeId < 1 || nodeId > maxNodeNumber) {
        throw invalid_argument(errorArray[3]);
    }

    nodes[nodeId - 1].setVoltage(voltage);
    cout << "Set: node " << nodeId << " to " << fixed << setprecision(2) << voltage << " Volts" << endl;
}