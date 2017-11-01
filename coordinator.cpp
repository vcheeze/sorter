//
// Created by Peeta on 10/30/17.
//

#include "coordinator.h"

Coordinator::Coordinator() {

}

Coordinator::~Coordinator() {

}

ifstream Coordinator::getInputFile() {
    return inputFile;
}

void Coordinator::setInputFile(ifstream f) {
    inputFile = f;
}