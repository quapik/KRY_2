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
    const uint16_t primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
     unsigned int h[8];
    
    for(i = 0; i < 8; i++)
    {
        double sqrt_prime = std::sqrt(primes[i]);
        // Získáme zlomkovou část
        double  fractional_part = sqrt_prime - std::floor(sqrt_prime);
        // Přenásobíme částí číslo 2^32 a přetypujeme na unsigned int
        h[i] = static_cast<uint32_t>((fractional_part) *  0xFFFFFFFF);
     
        cout << std::bitset<32>(h[i]) << endl;
    }


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