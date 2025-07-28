

// Updated C++ code for parsing JSON test cases and decoding values with various bases
// Goal: Compute the constant term `c` of a polynomial using Lagrange interpolation
// GMP is used to support 256-bit numbers and beyond

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <gmpxx.h>
#include <nlohmann/json.hpp>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

typedef pair<mpz_class, mpz_class> Share;

// Function to decode a string of digits in a given base to mpz_class
mpz_class decodeBase(const string& value, int base) {
    mpz_class result;
    mpz_set_str(result.get_mpz_t(), value.c_str(), base);
    return result;
}

// Lagrange Interpolation to find f(0)
mpz_class lagrangeInterpolationAtZero(const vector<Share>& shares) {
    mpz_class secret = 0;
    int k = shares.size();

    for (int i = 0; i < k; i++) {
        mpz_class xi = shares[i].first;
        mpz_class yi = shares[i].second;
        mpz_class num = 1, den = 1;

        for (int j = 0; j < k; j++) {
            if (i == j) continue;
            mpz_class xj = shares[j].first;
            num *= -xj;
            den *= (xi - xj);
        }

        mpz_class term = yi * num / den;
        secret += term;
    }

    return secret;
}

// Read and parse a JSON file into shares
vector<Share> parseSharesFromJson(const string& filename, int& k_out) {
    ifstream file(filename);
    json j;
    file >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];
    k_out = k;

    vector<Share> shares;
    for (auto& [key, value] : j.items()) {
        if (key == "keys") continue;

        mpz_class x(key);
        int base = stoi(value["base"].get<string>());
        string encoded = value["value"];
        mpz_class y = decodeBase(encoded, base);

        shares.emplace_back(x, y);
    }
    return shares;
}

int main() {
    vector<string> testFiles = {"testcase1.json", "testcase2.json"};

    for (const string& file : testFiles) {
        int k;
        vector<Share> shares = parseSharesFromJson(file, k);

        vector<vector<Share>> combs;
        vector<Share> current;

        function<void(int, int)> generateCombinations = [&](int start, int depth) {
            if (current.size() == k) {
                combs.push_back(current);
                return;
            }
            for (int i = start; i < shares.size(); i++) {
                current.push_back(shares[i]);
                generateCombinations(i + 1, depth + 1);
                current.pop_back();
            }
        };

        generateCombinations(0, 0);

        map<string, int> freq;
        map<string, vector<vector<Share>>> sourceMap;

        for (const auto& comb : combs) {
            try {
                mpz_class secret = lagrangeInterpolationAtZero(comb);
                string secretStr = secret.get_str();
                freq[secretStr]++;
                sourceMap[secretStr].push_back(comb);
            } catch (...) {
                continue;
            }
        }

        string bestSecret;
        int maxFreq = 0;
        for (auto& [s, count] : freq) {
            if (count > maxFreq) {
                maxFreq = count;
                bestSecret = s;
            }
        }
        
        cout << "Secret for file " << file << ": " << bestSecret << "\n\n";
        
    // Step 1: Collect valid shares from combinations that produced the correct secret
    set<mpz_class> validIndices;
    for (const auto& comb : sourceMap[bestSecret]) {
        for (const auto& [x, y] : comb) {
            validIndices.insert(x);
        }
    }

    // Step 2: Identify all indices in input
    set<mpz_class> allIndices;
    for (const auto& [x, y] : shares) {
        allIndices.insert(x);
    }

    // Step 3: Print invalid share indices
    cout << "Invalid shares for file " << file << ": ";
    bool foundInvalid = false;
    for (const auto& x : allIndices) {
        if (validIndices.find(x) == validIndices.end()) {
            cout << x.get_str() << " ";
            foundInvalid = true;
        }
    }
    if (!foundInvalid) {
        cout << "None";
    }
    cout << "\n\n";

    }

    return 0;
}
