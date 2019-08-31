#include "utils.hpp"

// Linear interpolation
// Not provided by GDNative
float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

// Create a unique integer from x/y positions
int hash_position(int x, int y) {
	int xx = x >= 0 ? x * 2 : x * -2 - 1;
    int yy = y >= 0 ? y * 2 : y * -2 - 1;
	return (xx >= yy) ? (xx * xx + xx + yy) : (yy * yy + xx);
}