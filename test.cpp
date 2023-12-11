#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <errno.h>

using namespace std;

int dataL, dataS, dataC;
int instructionsL, instructionsS, instructionsC;
int dataTag, dataIndexBits, dataDisplacement;
int instructionsTag, instructionsIndexBits, instructionsDisplacement;
int instructionsCacheClock, dataCacheClock;
int dataHits, dataMisses, dataTotalAccesses;
int instructionsHits, instructionsMisses, instructionsTotalAccesses;
int memoryAccessClocks = 100;
vector<pair<bool, string>> dataCache;
vector<pair<bool, string>> instructionsCache;

void readAccessSequence(string);
void cacheAccessData(string);
void cacheAccessInstructions(string);
string hex_to_binary(string);

int main()
{
    string accessfile;

    // taking all necessary input from user
    // data cache
    cout << "Enter your total data cache size: ";
    cin >> dataS;

    cout << "Enter your data cache line size: ";
    cin >> dataL;

    // instructions cache
    cout << "Enter your total instructions cache size: ";
    cin >> instructionsS;

    cout << "Enter your instructions cache line size: ";
    cin >> instructionsL;

    // validate clock cycles to be between 1 and 10
    do
    {
        cout << "Enter the number of clock cycles needed to access the instructions cache: ";
        cin >> instructionsCacheClock;
        if (instructionsCacheClock < 1 || instructionsCacheClock > 10)
            cout << "Please enter a number between 1 and 10" << endl;
    } while (instructionsCacheClock < 1 || instructionsCacheClock > 10);

    do
    {
        cout << "Enter the number of clock cycles needed to access the data cache: ";
        cin >> dataCacheClock;
        if (dataCacheClock < 1 || dataCacheClock > 10)
            cout << "Please enter a number between 1 and 10" << endl;
    } while (dataCacheClock < 1 || dataCacheClock > 10);

    cout << "Enter the complete path for the .txt file with the access sequences for your program (data addreses should start with d & instruction addresses should start with i): ";
    cin >> accessfile;

    // calculating the number of bits for the tag, indexBits, and displacement for the data cache
    dataC = dataS / dataL;
    dataIndexBits = log2(dataC);
    dataDisplacement = log2(dataL);
    dataTag = 32 - dataIndexBits - dataDisplacement;

    // calculating the number of bits for the tag, indexBits, and displacement for the instructions cache
    instructionsC = instructionsS / instructionsL;
    instructionsIndexBits = log2(instructionsC);
    instructionsDisplacement = log2(instructionsL);
    instructionsTag = 32 - instructionsIndexBits - instructionsDisplacement;

    // initializing data cache size according to user input
    dataCache.resize(dataC);
    for (int i = 0; i < dataC; i++)
    {
        dataCache[i].first = false;
        dataCache[i].second = "";
    }

    // initializing instructions cache size according to user input
    instructionsCache.resize(instructionsC);
    for (int i = 0; i < instructionsC; i++)
    {
        instructionsCache[i].first = false;
        instructionsCache[i].second = "";
    }

    // reading in the access sequence from the file
    readAccessSequence(accessfile);
}

void readAccessSequence(string filename)
{
    ifstream infile;
    infile.open(filename);

    if (infile.fail())
    {
        cout << "Error opening your entered file path, terminating program..." << endl;
        exit(1);
    }

    string line;
    while (getline(infile, line))
    {
        cout << endl
             << "----------------------------------------------------------------------------------------------------" << endl;
        cout << "Memory Access: " << line << endl
             << endl;
        if (line.at(0) == 'd')
            cacheAccessData(line.substr(1));
        else if (line.at(0) == 'i')
            cacheAccessInstructions(line.substr(1));
    }
}

void cacheAccessData(string line)
{
    string bin = hex_to_binary(line);
    string tagbin = bin.substr(0, dataTag);
    string indexbin = bin.substr(dataTag, dataIndexBits);
    string displacementbin = bin.substr(dataTag + dataIndexBits, dataDisplacement);
    if (!dataCache[stoi(indexbin, nullptr, 2)].first || dataCache[stoi(indexbin, nullptr, 2)].second != tagbin)
    {
        dataMisses++;
        dataCache[stoi(indexbin, nullptr, 2)].first = true;
        dataCache[stoi(indexbin, nullptr, 2)].second = tagbin;
    }
    else
    {
        dataHits++;
    }
    dataTotalAccesses++;

    cout << "Data Cache:" << endl;
    for (int i = 0; i < dataC; i++)
    {
        cout << i << " : valid = " << dataCache[i].first << " tag = " << dataCache[i].second << endl;
    }
    cout << "Data Total Accesses: " << dataTotalAccesses << endl;
    cout << "Data Hits: " << dataHits << endl;
    cout << "Data Misses: " << dataMisses << endl;
    cout << "Data Hit Ratio: " << (double)dataHits / (double)dataTotalAccesses << endl;
    cout << "Data Miss Ratio: " << (double)dataMisses / (double)dataTotalAccesses << endl;

    cout << "Data Average Access Time: " << (double)(dataCacheClock) + (memoryAccessClocks * (double)dataMisses / (double)dataTotalAccesses) << endl;
}

void cacheAccessInstructions(string line)
{
    string bin = hex_to_binary(line);
    string tagbin = bin.substr(0, instructionsTag);
    string indexbin = bin.substr(instructionsTag, instructionsIndexBits);
    string displacementbin = bin.substr(instructionsTag + instructionsIndexBits, instructionsDisplacement);
    if (!instructionsCache[stoi(indexbin, nullptr, 2)].first || instructionsCache[stoi(indexbin, nullptr, 2)].second != tagbin)
    {
        instructionsMisses++;
        instructionsCache[stoi(indexbin, nullptr, 2)].first = true;
        instructionsCache[stoi(indexbin, nullptr, 2)].second = tagbin;
    }
    else
    {
        instructionsHits++;
    }
    instructionsTotalAccesses++;

    cout << "Instructions Cache:" << endl;
    for (int i = 0; i < instructionsC; i++)
    {
        cout << i << " : valid = " << instructionsCache[i].first << " tag = " << instructionsCache[i].second << endl;
    }
    cout << "Instructions Total Accesses: " << instructionsTotalAccesses << endl;
    cout << "Instructions Hits: " << instructionsHits << endl;
    cout << "Instructions Misses: " << instructionsMisses << endl;
    cout << "Instructions Hit Ratio: " << (double)instructionsHits / (double)instructionsTotalAccesses << endl;
    cout << "Instructions Miss Ratio: " << (double)instructionsMisses / (double)instructionsTotalAccesses << endl;

    cout << "Instructions Average Access Time: " << (double)(instructionsCacheClock) + (memoryAccessClocks * (double)instructionsMisses / (double)instructionsTotalAccesses) << endl;
}

string hex_to_binary(string hex)
{
    transform(hex.begin(), hex.end(), hex.begin(), ::toupper);
    string binary;
    for (int i = 0; i < hex.length(); i++)
    {
        switch (hex[i])
        {
        case '0':
            binary += "0000";
            break;
        case '1':
            binary += "0001";
            break;
        case '2':
            binary += "0010";
            break;
        case '3':
            binary += "0011";
            break;
        case '4':
            binary += "0100";
            break;
        case '5':
            binary += "0101";
            break;
        case '6':
            binary += "0110";
            break;
        case '7':
            binary += "0111";
            break;
        case '8':
            binary += "1000";
            break;
        case '9':
            binary += "1001";
            break;
        case 'A':
            binary += "1010";
            break;
        case 'B':
            binary += "1011";
            break;
        case 'C':
            binary += "1100";
            break;
        case 'D':
            binary += "1101";
            break;
        case 'E':
            binary += "1110";
            break;
        case 'F':
            binary += "1111";
            break;
        }
    }
    return binary;
}