#ifndef FIALKACIPHER_H
#define FIALKACIPHER_H

#include <stdint.h> // uint64_t
#include <math.h>

typedef unsigned char byte;
typedef uint64_t pint;

#define ROT 10 // number of rotors
#define N 8    // N-bit cipher
#define MOD 256 // pow(2, n)

#define XOR 1 // comment to use modular operation

const byte REF_E[MOD] = { /* Reflector S-box */ };
const byte REF_D[MOD] = { /* inverse ref. S-box */ };
const byte SBOX[MOD] = { /* rotor S-box */ };
const byte SBOX_INV[MOD] = { /* inverse rotor S-box */ };

pint pins[ROT][4]; // 1 binary vector - 4 * 64 bits
byte lambda[ROT];  // 1 lambda - 8 bits

void setKey(pint k1[][4], byte k2[]) {
    for (int i = 0; i < ROT; i++) {
        for (int j = 0; j < 4; j++) {
            pins[i][j] = k1[i][j];
        }
        lambda[i] = k2[i];
    }
}

/∗ function & method prototypes ∗/
void wheelStepping(void);
byte rotors(byteinput) ;
byte rotorsInv(byteinput) ;

void encrypt(byte PT[] ,byte CT[], const long size );
void decrypt(byte PT[] ,byte CT[], const long size );

/∗ impl . ∗/
inline void wheelStepping(void) {
    for (register short i = 1; i < 10; i += 2) {
        pins[i][0] = ((pins[i][0] >> 1) | (pins[i][1] << 63));
        pins[i][1] = ((pins[i][1] >> 1) | (pins[i][2] << 63));
        pins[i][2] = ((pins[i][2] >> 1) | (pins[i][3] << 63));
        pins[i][3] = ((pins[i][3] >> 1) | (pins[i][0] << 63));
        lambda[i]++;
        lambda[i] %= MOD;
        if ((pins[i][0] & 0x0000000000000001) == 0) break;
    }
    for (register short i = 8; i >= 0; i -= 2) {
        pins[i][0] = ((pins[i][0] << 1) | (pins[i][3] >> 63));
        pins[i][1] = ((pins[i][1] << 1) | (pins[i][0] >> 63));
        pins[i][2] = ((pins[i][2] << 1) | (pins[i][1] >> 63));
        pins[i][3] = ((pins[i][3] << 1) | (pins[i][2] >> 63));
        lambda[i] += MOD - 1;
        lambda[i] %= MOD;
        if ((pins[i][0] & 0x0000000000000001) == 0) break;
    }
}

inline byte rotors(const byte input) {
    register byte x = input;
#ifdef XOR /* XOR */
    for (register short i = 0; i < 10; i++) {
        x = x ^ lambda[i];
        x = SBOX[x];
        x = x ^ lambda[i];
    }
#else /* +, - MOD 2^n */
    for (register short i = 0; i < 10; i++) {
        x += lambda[i], x %= MOD;
        x = SBOX[x];
        x += MOD - lambda[i], x %= MOD;
    }
#endif
    return x;
}

inline byte rotorsInv(byte input) {
    register byte x = input;
#ifdef XOR /* XOR */
    for (register short i = 9; i >= 0; i--) {
        x = x ^ lambda[i];
        x = SBOX_INV[x];
        x = x ^ lambda[i];
    }
#else /* +, - MOD 2^n */
    for (register short i = 9; i >= 0; i--) {
        x += lambda[i], x %= MOD;
        x = SBOX_INV[x];
        x += MOD - lambda[i], x %= MOD;
    }
#endif
    return x;
}

inline void encrypt(byte *PT, byte *CT, const long size) {
    for (long i = 0; i < size; i++) {
        byte x = PT[i];
        x = rotors(x);
        x = REF_E[x];
        x = rotorsInv(x);
        wheelStepping();
        CT[i] = x;
    }
}

inline void decrypt(byte *PT, byte *CT, const long size) {
    for (long i = 0; i < size; i++) {
        byte x = CT[i];
        x = rotors(x);
        x = REF_D[x];
        x = rotorsInv(x);
        wheelStepping();
        PT[i] = x;
    }
}

#endif
