//
//  Node.cpp
//  Lab 3 The Resistor Network Program
// Author: Joseph Mi
// Date: Sept 30, 2024

#include <iostream>
#include "Node.h"
#include <string>

Node::Node() : numRes(0), voltage(0), voltageIsSet(false) {} // Initialize Node

bool Node::canAddResistor() {
    return numRes < MAX_RESISTORS_PER_NODE;
}

void Node::addResistor(int rIndex) {
    if (canAddResistor()) {
        resIDArray[numRes] = rIndex;
        numRes++;
    }
}

double Node::getVoltage() {
    return voltage;
}

void Node::setVoltage(double voltage_) {
    voltage = voltage_;
    voltageIsSet = true;
}

void Node::setVoltageInternally(double voltage_) {
    voltage = voltage_;
}

bool Node::isVoltageSet() {
    return voltageIsSet;
}

int Node::getNumRes() {
    return numRes;
}

int* Node::getResIDArray() {
    return resIDArray;
}
 