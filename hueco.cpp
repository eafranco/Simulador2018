#include "hueco.h"
#include <stdio.h>
Hole::Hole(int iniMem, int finMem) {
    this->iniMem = iniMem;
    this->finMem = finMem;
}

int Hole::size() {
    return finMem - iniMem;
}

void Hole::print() {
    printf("Hueco(%d, %d)\n", iniMem, finMem);
}
