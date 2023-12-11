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
vector<pair<bool, string>> cache;

void readAccessSequence(string);
void cacheAccess(string);
string hexToBin(string);
bool readFromFile(string);
void initCache();

int main()
{
    bool CC = false;
    cout << "Please enter the following inputs: " << endl;
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
    while (!CC)
    {
        cout << "Please enter a valid cache clock (between 1 and 10): ";
        cin >> cacheClock;
        if (cacheClock >= 1 && cacheClock <= 10)
        {
            CC = true;
        }
    }

    C = (double)L / (double)S;

    cache.resize(C);

    initCache();

    string filename = "addresses.txt";

    readFromFile(filename);
}

int binaryStringToInt(string &binaryString)
{
    int result = 0;

    for (int i = binaryString.size() - 1; i >= 0; --i)
    {
        int bit = binaryString[i] - '0';

        result += bit * static_cast<int>(pow(2, binaryString.size() - 1 - i));
    }

    return result;
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
    while (getline(inputFile, line, ','))
    {
        cacheAccess(line);
    }

    inputFile.close();

    return true;
}

void initCache()
{
    for (int i = 0; i < S; i++)
    {
        cache.push_back(make_pair(false, ""));
    }
}

// reasd byteVector one by one
void cacheAccess(string address)
{
    string binAddress = hexToBin(address);
    string indexTemp;

    displacement = log2(L);
    indexBits = log2(C);

    tag = 24 - indexBits - displacement;

    string index, tag;

    // Check if tag is in cache
    if (!cache[indexBits].first | cache[indexBits].second != tag)
    {
        miss++;
        cache[indexBits].first = true;
        cache[indexBits].second = tag;
    }
    else
    {
        hit++;
    }
    ++totalAccesses;

    cout << "------------------------------Cache------------------------------" << endl;
    for (int i = 0; i < C; i++)
    {
        cout << i << " : valid = " << cache[i].first << " tag = " << cache[i].second << endl;
        cout << "-----------------------------------------------------------------" << endl;
    }
    cout << "Total Accesses: " << totalAccesses << endl;
    cout << "Hits: " << hit << endl;
    cout << "Misses: " << miss << endl;
    cout << "Hit Ratio: " << (double)hit / (double)totalAccesses << endl;
    cout << "Miss Ratio: " << (double)miss / (double)totalAccesses << endl;

    cout << "Average Access Time: " << (double)(cacheClock) + (memoryAccessClocks * (double)miss / (double)totalAccesses) << endl;
}