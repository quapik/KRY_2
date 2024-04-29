#include <iostream>
#include <getopt.h>
#include <string>
#include <bitset>
#include <cmath>
#include <regex>



uint32_t rightrotate(uint32_t k_rotaci,  unsigned int velikost_rotace) { return (k_rotaci >> velikost_rotace) | (k_rotaci << (32 - velikost_rotace)); }
using namespace std;
string checksum(string msg, string chs){
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
    uint32_t hash_primes[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    uint32_t hash[8]; 
    for(int i = 0; i <8; i++)
    {
        if (chs=="") hash[i] = hash_primes[i];
        else 
        {
            hash[i] = std::stoul(chs.substr(i*8, 8), nullptr, 16);
            //cout << chs.substr(i*8, 8)<<endl;
        }

        //cout << hash[i] << endl;
                
    }
    
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

    stringstream ss;
    // Append hash values to get final digest and print
    for(i = 0; i < 8; i++) ss << hex << hash[i];
    return(ss.str());
}

int vypocetMAC(string msg, string key)
{
     if (!regex_match(key, regex("^[A-Za-z0-9]*$"))) 
     {
        cerr<< "Nesplneny regex  ^[A-Za-z0-9]*$ pro -k KEY" << endl;
        return -1;
     }
     else{
        cout << checksum(key+msg, "") << endl;
        return 0;
     }
}

int main(int argc, char** argv) {
    string out = "";
    //Program bez parametrů - dokumentace na STD out
     if (argc == 1) {
        cout << "Dokumentace!" << endl;
        return 1;
    }
    //Nacteni vstupní zprávy
    string input = "";
    string key = "";
    string chs = ""; 
    int num;
    string msg = "";
    bool s_flag = false;
    bool k_flag = false;
    bool c_flag = false;
    bool v_flag = false;
    bool m_flag = false;
    bool n_flag = false;
    bool a_flag = false;
    bool e_flag = false;
    getline(cin, input);
    int argument;
    while ((argument  = getopt (argc, argv, "csk:vm:n:a:e")) != -1)
    {
        switch (argument) 
        {
        case 'c':
            c_flag = true;
            break;
        case 's':
            s_flag = true;
            break;
        case 'v':
            v_flag = true;
            break;
        case 'k':
            k_flag = true;
            key = optarg;
            break;
        case 'm':
            m_flag = true;
            chs = optarg;
            break;
        case 'n':
            n_flag = true;
            num = atoi(optarg);
            break;
        case 'a':
            a_flag = true;
            msg = optarg;
            break;
        case 'e':
            e_flag = true;
            break;
        }
    }

    //-c – Vypočte a tiskne SHA-256 checksum vstupní zprávy na STDOUT (zakončeno právě jedním \n).
    if (c_flag) 
    {
        out = checksum(input,"");
        cout << out << endl; //TODO REMOVE
        return 0;
    }

    //-s – Vypočte MAC, použitím implementované SHA-256, pro vstupní zprávu a tisne výsledek na STDOUT (zakončeno právě jedním \n). Musí být použito v kombinaci s -k parametrem.
    if (s_flag)
    {
        if (k_flag) 
        {
            if (!regex_match(key, regex("^[A-Za-z0-9]*$"))) 
            {
                cerr<< "Nesplneny regex  ^[A-Za-z0-9]*$ pro -k KEY" << endl;
                return -1;
            }
            else{
                cout << checksum(key+input, "") << endl;
                return 0;
            }
        }
        else
        {
            cerr << "Spatne zadany argument -s bez použití -k KEY" << endl;
            return -1;
        }
    }
    // v – Ověří MAC pro daný klíč a vstupní zprávu. Vrací 0 v případě validního MAC, jinak 1. Musí být použito v kombinaci s -k a -m parametry.
    if (v_flag)
    {
        if(k_flag && m_flag)  
        {
            
           if (checksum(key+input, "") == chs)
           {
            cout << "OK!!" << endl; //TODO SMAZAT
            return 0;
           }
           else  {
            cout << "NOT OK!!" << endl;  //TODO SMAZAT
            return 1;
           }
        }
        else
        {
        cerr << "Spatne zadany argument -v bez použití -k KEY či -m CHS" << endl;
        return -1;
        }
    }
    /*
    -e – Provede length extension útok na MAC a vstupní zprávu. Přepočítaný MAC a
    prodloužená zpráva jsou tištěny na STDOUT v tomto pořadí (každá položka zakončena
    právě jedním \n). Padding přidaný ke vstupní zprávě bude ve formátu posloupnosti
    escapovaných znaků \xXX, kde XX bude ASCII hodnota znaku v hexadecimální
    soustavě. Musí být použito v kombinaci s -m, -n a -a parametry*/
    if(e_flag)
    {
        if(m_flag && n_flag && a_flag)
        {
            if (!regex_match(msg , regex("^[a-zA-Z0-9!#$%&’\"()*+,\\-.\\/:;<>=?@[\\]\\^_{}|~]*$"))) 
            {
                cerr<< "Nesplneny regex  ^[a-zA-Z0-9!#$%&’\"()*+,\\-.\\/:;<>=?@[\\]\\^_{}|~]*$ pro -m msg" << endl;
                return -1;
            }
            int input_len = input.length();
            if (chs.length()!=64)
            {
                cerr<< "Spatna delka MAC zpravyy (not 64)" << endl;
                return -1;
            }

            //string test = checksum("aa", chs);

            int padding_size = 64 - 1 - 1 - msg.length() - num;

            string qq = "";
            stringstream ss;

            ss << input << hex << 128;
             for (int i = 0; i < 64 - (1+1+input_len+num); i++)
            {
                ss << hex << 0;
            }
            ss << hex <<8*(input_len + 5);

            ss << msg << hex << 128;
            for (int i = 0; i < padding_size; i++)
            {
            ss << hex << 0;
            }
            ss << hex << (msg.length() + 0) * 8;
            cout << ss.str() << endl;

            cout << checksum(ss.str(), chs);


            // cout << input << "\\x" << hex << 128;
            // for (int i = 0; i < 64 - (1+1+input_len+num); i++)
            // {
            //     cout << "\\x0" << hex << 0;
            // }


            // int bit_length = 8*(input_len + 5);
            // if (bit_length > 15) cout << "\\x" << hex << bit_length << msg << endl;
            // else cout << "\\x0" << hex << bit_length << msg << endl;


            
            


        }
        else 
        {
        cerr << "Spatne zadany argument -a bez použití -m CHS, -n NUM ci -a MSG" << endl;
        return -1;
        }

    }



    //TODO Return pokud nic nepatterne
    return 0;
}