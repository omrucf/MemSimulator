#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int L, S, C; // inputs: L,S; C = LS
int tag, indexBits, displacement;
int cacheClock;
int hit, miss, totalAccesses;
const int memoryAccessClocks = 120; 
vector<pair<bool, string>> cache; // cache represented as a vector (bool for validity value)

void readAccessSequence(string);
void cacheAccess(string);
string hexToBin(string);
bool readFromFile(string);

int main()
{
    system("clear");
    bool CC = false; // bool to check if the cache clock is between 1 and 10
    cout << "Please enter the following inputs: " << endl; // take required input from the user
    cout << "L: ";
    cin >> L;
    cout << "S: ";
    cin >> S;
    cout << "Cache Clock: ";
    cin >> cacheClock;
    if (cacheClock >= 1 && cacheClock <= 10)
    {
        CC = true;
    }
    while (!CC) // keep asking the user to enter a valid CC
    {
        cout << "Please enter a valid cache clock (between 1 and 10): ";
        cin >> cacheClock;
        if (cacheClock >= 1 && cacheClock <= 10)
        {
            CC = true;
        }
    }

    C = (double)S / (double)L; // S = CL => C=S/L = Number of lines 

    cache.resize(C, make_pair(false, "")); // initialize the vector to the size of the lines and initialize the cache content

    string filename = "addresses.txt"; 

    readFromFile(filename); // retrieve the addresses from a txt file 
}

string hexToBin(string hex)
{
    unsigned long dec = std::stoul(hex, nullptr, 16);

    // Convert decimal to binary
    string bin;
    while (dec > 0)
    {
        bin = std::to_string(dec % 2) + bin;
        dec /= 2;
    }

    return bin;
}

bool readFromFile(string filename)
{
    ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        cout << "Error opening the file." << std::endl;
        return false;
    }

    // Vector to store bytes
    string line;
    while (getline(inputFile, line, ',')) // data in txt file must be in csv format
    {
        cout << "\n\n\n\n\n\n";
        cout << "=================================================================" << endl;
        cout << "=============================New Access==========================" << endl;
        cout << "=================================================================" << endl;
        cacheAccess(line); //every time we read an address we access the cache with this address
    }

    inputFile.close();

    return true;
}

// reasd byteVector one by one
void cacheAccess(string address)
{
    string binAddress = hexToBin(address); // first transform the address from hexadecimal to binary 

    while (binAddress.length() < 24) // the size of the line of the address must be 24 bits so concatenate 0s if less
    {
        binAddress = "0" + binAddress;
    }

    if (binAddress.length() > 24) // if the size of the address line is greater than 24 take the least significant 24 bits
    {
        binAddress = binAddress.substr(binAddress.length() - 24, binAddress.length()); // to get the last 24 bits
    }

    cout << "Address in Hexadecimal: " << address << endl;

    displacement = log2(L); // equations from the slides 
    indexBits = log2(C);

    cout << "Address in Binary: " << binAddress << endl;

    tag = 24 - indexBits - displacement; // getting the size of the tags

    string indexTemp, tagTemp, dTemp; 
    tagTemp = binAddress.substr(0, tag); // getting the binary values of tag, index, and displacement
    indexTemp = binAddress.substr(tag, indexBits);
    dTemp = binAddress.substr(tag + indexBits, displacement);

    cout << "Displacement binary: " << dTemp << endl;
    cout << "Index binary: " << indexTemp << endl;

    int index = stoi(indexTemp, nullptr, 2); // getting the decimal value of the index


    // Check if tag is in cache
    if (!cache[index].first || cache[index].second != tagTemp) // this is to check for a miss 
    {
        ++miss; // increment miss counter and then load the address in the cache 
        cache[index].first = true;
        cache[index].second = tagTemp;
    }
    else
    {
        ++hit; // if there is a hit no need to do anything, just increment the hit counter
    }
    ++totalAccesses; // this where one access is done and the counter os accesses in incremented

    cout << "------------------------------Cache------------------------------" << endl; // keep track of the cache content and the results collected
    for (int i = 0; i < C; i++)
    {
        cout << i << " : valid = " << cache[i].first << " tag = " << cache[i].second << endl;
        cout << "-----------------------------------------------------------------" << endl;
    }
    cout << "Total Accesses: " << totalAccesses << endl;
    cout << "Hits: " << hit << endl;
    cout << "Misses: " << miss << endl;
    cout << "Hit Ratio: " << (double)hit / (double)totalAccesses << endl; // equations from the slides
    cout << "Miss Ratio: " << (double)miss / (double)totalAccesses << endl;

    cout << "Average Access Time: " << (double)(cacheClock) + (memoryAccessClocks * (double)miss / (double)totalAccesses) << endl;
}
