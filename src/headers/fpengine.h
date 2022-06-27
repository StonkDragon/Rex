#ifndef fpengine_v
#define fpengine_v 1

#include "register.h"

double f0 = 0.0f;
double f1 = 0.0f;
double f2 = 0.0f;
double f3 = 0.0f;
double f4 = 0.0f;
double f5 = 0.0f;
double f6 = 0.0f;
double f7 = 0.0f;

double fe_get(int i) {
    switch (i) {
        case 0: return f0;
        case 1: return f1;
        case 2: return f2;
        case 3: return f3;
        case 4: return f4;
        case 5: return f5;
        case 6: return f6;
        case 7: return f7;
        default: return 0.0f;
    }
}

void fe_set(int i, double f) {
    switch (i) {
        case 0: f0 = f; break;
        case 1: f1 = f; break;
        case 2: f2 = f; break;
        case 3: f3 = f; break;
        case 4: f4 = f; break;
        case 5: f5 = f; break;
        case 6: f6 = f; break;
        case 7: f7 = f; break;
    }
}

#endif
