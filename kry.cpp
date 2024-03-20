// Your First C++ Program

#include <iostream>
#include <getopt.h>
#include <string>
#include <bitset>
#include <cmath>


uint32_t rightrotate(uint32_t k_rotaci,  unsigned int velikost_rotace) { return (k_rotaci >> velikost_rotace) | (k_rotaci << (32 - velikost_rotace)); }
using namespace std;
void checksum(string msg){
    unsigned int i;
    unsigned int len = msg.length();
    const int size = 64;
    unsigned char messageblock[size];
    
    //zakladní vytvoření bloku
    std::fill_n(messageblock, size, 0);
    for(unsigned int i = 0; i < len; i++)
    {
         messageblock[i] = msg[i];
    }
    messageblock[len] = 0b10000000;
    messageblock[63] = len*8;

    //64-entry message schedule array
    uint32_t w[64] = {0};
    //copy 1st chunk into 1st 16 words
    for(i = 0; i < 16; i++)
    {
       w[i] |= messageblock[(i*4)]; w[i] <<= 8;
       w[i] |= messageblock[(i*4)+1]; w[i] <<= 8;
       w[i] |= messageblock[(i*4)+2]; w[i] <<= 8;  
       w[i] |= messageblock[(i*4)+3];
    }

    //Dopočítání ostatních w
    for(i = 16; i < 64; i++)
    {
    uint32_t a0 = rightrotate(w[i-15],7) ^ rightrotate(w[i-15],18) ^ (w[i-15] >> 3);
    uint32_t a1 = rightrotate(w[i-2],17) ^ rightrotate(w[i-2],19) ^ (w[i-2] >> 10);
    w[i] = w[i-16] + a0 + w[i-7] + a1;
    }
    //Initialize hash value h0 to h7: first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19).
    uint32_t hash[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    uint32_t k[64] =   {0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
                        0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
                        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
                        0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
                        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
                        0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
                        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
                        0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
                        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
                        0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
                        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
                        0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
                        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
                        0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
                        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
                        0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2	};
    
    //Initialize working variables to initial hash value
    uint32_t a = hash[0]; uint32_t b = hash[1]; uint32_t c = hash[2]; uint32_t d = hash[3]; uint32_t e = hash[4]; uint32_t f = hash[5]; uint32_t g = hash[6]; uint32_t h = hash[7];

    for(i = 0; i < 64; i++)
    {
        uint32_t sum1 =  rightrotate(e, 6) ^ rightrotate(e, 11) ^ rightrotate(e, 25);
        uint32_t choice = (e & f)  ^  ((~e)&g); //(e and f) xor ((not e) and g)
        uint32_t sum0 =  rightrotate(a, 2) ^ rightrotate(a, 13) ^ rightrotate(a, 22);
        uint32_t majority = (a & b) ^  (a & c) ^ (b & c);
        uint32_t temp1 = h + sum1 + choice + k[i] + w[i];
        uint32_t temp2 = sum0 + majority;
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    //Add the working variables to the current hash value
    hash[0] = hash[0] + a;
    hash[1] = hash[1] + b;
    hash[2]= hash[2] + c;
    hash[3] = hash[3] + d;
    hash[4] = hash[4] + e;
    hash[5] = hash[5] + f;
    hash[6] = hash[6] + g;
    hash[7]= hash[7] + h;

    // Append hash values to get final digest and print
    for(i = 0; i < 8; i++) cout << hex << hash[i];
    cout << endl;
}


int main(int argc, char** argv) {
    //Program bez parametrů - dokumentace na STD out
     if (argc == 1) {
        cout << "Dokumentace!" << endl;
        return 1;
    }
    //Nacteni vstupní zprávy
    string input = "";
    getline(cin, input);
    int argument;
    while ((argument  = getopt (argc, argv, "c")) != -1)
    {
        switch (argument) 
        {
            case 'c':
                checksum(input);
                break;
         }

    }

    return 0;
}