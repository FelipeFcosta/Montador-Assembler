#include "Instruction.h"

void Instruction::split(string instruction) {
    int i = 0;

    // get label
    label = "";
    bool labelFound = false;
    for ( ; i < instruction.size(); i++) {
        if (instruction[i] == ':') {
            labelFound = true;
            break;
        }
        label += instruction[i];
    }
    if (!labelFound) {
        label = "";
        i = 0;
    }
    if (labelFound && i < instruction.size()-1) {
        instruction = instruction.substr(instruction[i+1] == ' ' ? i+2 : i+1, instruction.size());
    }
    else if (labelFound) return;

    i = 0;
    // get operation
    for ( ; i < instruction.size(); i++) {
        if (instruction[i] == ' ') {
            break;
        }
        operation += instruction[i];

        if (i == instruction.size()-1) {    // e.g STOP
            i++;
        }
    }
    if (i < instruction.size())
        instruction = instruction.substr(i+1, instruction.size());
    else return;

    // get op1
    i = 0;
    for ( ; i < instruction.size(); i++) {
        if (i != 0 && instruction[i] == ',') {
            break;
        }
        if (operation != "STOP" && operation != "CONST" && operation != "SPACE") {
            if (instruction[i] == '+' || instruction[i] == '-') {
                do {
                    if (i == instruction.size()-1) {
                        arit1 += instruction[i++];
                        i++;
                        break;
                    }
                    arit1 += instruction[i++];
                } while (instruction[i] != ' ' && instruction[i] != ',');
                break;
            }
        }
        op1 += instruction[i];

        if (i == instruction.size()-1) {    // e.g STORE TMP
            i++;
        }
    }
    if (i < instruction.size())
        instruction = instruction.substr(i+1, instruction.size());
    else return;

    // get op2
    i = 0;
    for ( ; i < instruction.size(); i++) {
        if (i != 0 && instruction[i] == ',') {
            break;
        }
        if (operation != "STOP" && operation != "CONST" && operation != "SPACE") {
            if (instruction[i] == '+' || instruction[i] == '-') {
                do {
                    if (i == instruction.size()-1) {
                        arit2 += instruction[i++];
                        i++;
                        break;
                    }
                    arit2 += instruction[i++];
                } while (instruction[i] != ' ' && instruction[i] != ',');
                break;
            }
        }
        op2 += instruction[i];
        if (i == instruction.size()-1) {
            i++;
        }
    }
    if (i < instruction.size())
        instruction = instruction.substr(i+1, instruction.size());
    else return;

    // get op3 (invalid)
    i = 0;
    for ( ; i < instruction.size(); i++) {
        op3 += instruction[i];
    }

}


Instruction::Instruction(string instruction) {
    split(instruction);
}
