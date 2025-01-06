#include "utils.h"
#include <math.h>

float fsign(float x) { return (signbit(x) == 0) * 2 - 1; }