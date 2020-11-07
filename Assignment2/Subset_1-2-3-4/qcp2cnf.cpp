#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int order;
map<string, int> pairing_map;

// Reads the QCP file
// Constructs a pairing value map
// Creates the unit_clauses
vector<int> readQCP(string file_name)
{

    ifstream input_file;
    input_file.open("qcp-files/" + file_name); // Opens the file

    string line;
    getline(input_file, line); // gets first line of QCP file

    order = stoi(line.erase(0, 6)); // gets the order

    int pairing_value = 1; // sets the initial pairing value to 1

    for (int i = 1; i <= order; i++)
    {
        for (int j = 1; j <= order; j++)
        {
            for (int k = 1; k <= order; k++)
            {
                string key = to_string(i) + " " + to_string(j) + " " + to_string(k); // creates the key for pairing value
                pairing_map.insert(make_pair(key, pairing_value));                   // sets key value pair for each paired value
                pairing_value++;
            }
        }
    }

    vector<int> unit_clauses; // array of unit clauses
    for (int i = 1; i <= order; i++)
    {
        getline(input_file, line);
        string k;
        stringstream ss(line);

        for (int j = 1; j <= order; j++)
        {
            ss >> k;
            if (k != "." && k != " ")
            {
                string key = to_string(i) + " " + to_string(j) + " " + k;
                int value = pairing_map.find(key)->second;
                unit_clauses.push_back(value);
            }
        }
    }
    input_file.close();

    return unit_clauses;
}

// (a) Each cell of the square contains a number in [n];
vector<vector<int>> property_a()
{
    vector<vector<int>> property_a_clauses;

    int pairing_value = 1;

    for (int i = 1; i <= order; i++)
    {
        for (int j = 1; j <= order; j++)
        {
            vector<int> clauses;
            for (int k = 1; k <= order; k++)
            {
                clauses.push_back(pairing_value);
                pairing_value++;
            }
            property_a_clauses.push_back(clauses);
        }
    }
    return property_a_clauses;
}

// (b) No cell contains two distinct numbers;
vector<vector<int>> property_b()
{
    vector<vector<int>> property_b_clauses;

    for (int i = 1; i <= order; i++)
    {

        for (int j = 1; j <= order; j++)
        {

            for (int k = 1; k <= order; k++)
            {

                string key = to_string(i) + " " + to_string(j) + " " + to_string(k);
                // cout << "Key1: " << key << endl;
                int value1 = pairing_map.find(key)->second;

                for (int k2 = k + 1; k2 <= order; k2++)

                {
                    vector<int> clauses;
                    string key = to_string(i) + " " + to_string(j) + " " + to_string(k2);
                    // cout << "Key2: " << key << endl;
                    int value2 = pairing_map.find(key)->second;
                    clauses.push_back(-(value1));
                    clauses.push_back(-(value2));
                    property_b_clauses.push_back(clauses);
                }
            }
        }
    }
    return property_b_clauses;
}

// (c) No row has two cells containing the same number;
vector<vector<int>> property_c()
{

    vector<vector<int>> property_c_clauses;

    for (int i = 1; i <= order; i++)
    {

        for (int k = 1; k <= order; k++)
        {

            for (int j = 1; j <= order; j++)
            {

                string key = to_string(i) + " " + to_string(j) + " " + to_string(k);
                // cout << "Key1: " << key << endl;
                int value1 = pairing_map.find(key)->second;

                for (int j2 = j + 1; j2 <= order; j2++)

                {
                    vector<int> clauses;
                    string key = to_string(i) + " " + to_string(j2) + " " + to_string(k);
                    // cout << "Key2: " << key << endl;
                    int value2 = pairing_map.find(key)->second;
                    clauses.push_back(-(value1));
                    clauses.push_back(-(value2));
                    property_c_clauses.push_back(clauses);
                }
            }
        }
    }
    return property_c_clauses;
}

// (d) No column has two cells containing the same number;
vector<vector<int>> property_d()
{

    vector<vector<int>> property_d_clauses;

    for (int j = 1; j <= order; j++)
    {

        for (int k = 1; k <= order; k++)
        {

            for (int i = 1; i <= order; i++)
            {

                string key = to_string(i) + " " + to_string(j) + " " + to_string(k);
                // cout << "Key1: " << key << endl;
                int value1 = pairing_map.find(key)->second;

                for (int i2 = i + 1; i2 <= order; i2++)

                {
                    vector<int> clauses;
                    string key = to_string(i2) + " " + to_string(j) + " " + to_string(k);
                    // cout << "Key2: " << key << endl;
                    int value2 = pairing_map.find(key)->second;
                    clauses.push_back(-(value1));
                    clauses.push_back(-(value2));
                    property_d_clauses.push_back(clauses);
                }
            }
        }
    }
    return property_d_clauses;
}

int main(int argc, char *argv[])
{
    order = 0;
    pairing_map.clear();
    // unit clauses
    vector<int> unit_clauses = readQCP(argv[1]);
    // property a clauses
    vector<vector<int>> property_a_clauses = property_a();
    // property a clauses
    vector<vector<int>> property_b_clauses = property_b();
    // property c clauses
    vector<vector<int>> property_c_clauses = property_c();
    // property d clauses
    vector<vector<int>> property_d_clauses = property_d();

    ofstream output_file;
    string output_name = argv[2];
    output_file.open("cnf-files/" + output_name); // opens output file

    // prints comments
    output_file << "c" << endl;
    output_file << "c" << endl;
    output_file << "c" << endl;

    int max_value = order * order * order; // max value in clauses
    // lines of clauses
    int lines = unit_clauses.size() + property_a_clauses.size() + property_b_clauses.size() + +property_c_clauses.size() + property_d_clauses.size();

    // output initial info line
    output_file << "p cnf " << to_string(max_value) << " " << to_string(lines) << endl;

    // output unit clauses
    for (int n : unit_clauses)
    {
        output_file << n << " 0\n";
    }

    // property a clauses
    for (int i = 0; i < property_a_clauses.size(); i++)
    {
        for (int j = 0; j < property_a_clauses[i].size(); j++)
        {
            output_file << property_a_clauses[i][j] << " ";
        }
        output_file << "0\n";
    }

    // property b clauses
    for (int i = 0; i < property_b_clauses.size(); i++)
    {
        for (int j = 0; j < property_b_clauses[i].size(); j++)
        {
            output_file << property_b_clauses[i][j] << " ";
        }
        output_file << "0\n";
    }

    // property c clauses
    for (int i = 0; i < property_c_clauses.size(); i++)
    {
        for (int j = 0; j < property_c_clauses[i].size(); j++)
        {
            output_file << property_c_clauses[i][j] << " ";
        }
        output_file << "0\n";
    }

    // property d clauses
    for (int i = 0; i < property_d_clauses.size(); i++)
    {
        for (int j = 0; j < property_d_clauses[i].size(); j++)
        {
            output_file << property_d_clauses[i][j] << " ";
        }
        output_file << "0\n";
    }

    output_file.close();

    return 0;
}