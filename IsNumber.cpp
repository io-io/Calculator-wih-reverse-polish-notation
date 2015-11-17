//
// Created by user on 17.11.2015.
//

#include "IsNumber.h"
#include <iostream>

namespace IsNumber {
    bool IsNumber(char *Symbol) {
        for (int i = 0; Symbol[i] != '\0'; i++) {
            if (((Symbol[i] < '0') || (Symbol[i] > '9')) && (Symbol[i] != '.')) {
                return false;
            }
        }
        return true;
    }
}