#include <iostream> // cin, cout
#include <fstream>  // Failų įvedimui/išvedimui
#include <string>   // String tipo kintamiesiems
#include <map>      // Asociatyvus konteineris
#include <regex>    // Žodžių valymui
#include <vector>   // Vector konteineriui
#include <sstream>  // String stream eilutėms skaityti

using namespace std;

// 1 UŽDUOTIS
// Naikina skyrybos ženklus šalia žodžių (veikia su UTF-8)
string CleanWord(const string& z) {
    regex pattern(R"([A-Za-zĄČĘĖĮŠŲŪŽąčęėįšųūž-]+)");       // Regex pattern (žodžiams su UTF-8)
    //regex pattern(R"([A-Za-z0-9ĄČĘĖĮŠŲŪŽąčęėįšųūž-]+)");  // jei norim, kad rodytų ir skaičius

    smatch m; // Regex objektas
    if (regex_search(z, m, pattern)) return m.str();
    return "";
}

// Žodžių skaičiavimas
map<string,int> ZodziuDazniai(const string& failas) {
    map<string,int> m;
    ifstream fin(failas);
    string zodis;
    while (fin >> zodis) {
        string val = CleanWord(zodis);
        if (!val.empty()) m[val]++;
    }
    return m;
}

//2 UŽDUOTIS
// Cross-reference: kuriose eilutėse yra minimi pasikartojantys žodžiai
map<string,vector<int>> CrossRef(const string& failas) {
    map<string,vector<int>> m;          // žodis -> eilučių numeriai
    ifstream fin(failas);
    string line;
    int nr = 1;                         // eilutės numeris (pradedam nuo 1)
    while (getline(fin, line)) {
        istringstream iss(line);
        string zodis;
        while (iss >> zodis) {
            string val = CleanWord(zodis);
            if (!val.empty()) {
                // Įtraukiame tik unikalius eilutės numerius
                if (m[val].empty() || m[val].back() != nr)
                    m[val].push_back(nr);
            }
        }
        nr++;
    }
    return m;
}

// Išvedimas į failą
void IsvestiCrossRef(const map<string,vector<int>>& cr, const map<string,int>& dazniai) {
    ofstream out("cross-reference.txt");
    for(auto &p : cr) 
        if(dazniai.find(p.first) != dazniai.end() && dazniai.at(p.first) > 1) {
            out << p.first << " : ";
            for(size_t i=0; i<p.second.size(); i++) {
                out << p.second[i];
                if(i+1 < p.second.size()) out << ",";
            }
            out << "\n";
        }
}

int main() {
    string failas;
    cout << "Iveskite faila: ";
    cin >> failas;

    auto dazniai = ZodziuDazniai(failas);

    ofstream out("zodziu_daznis.txt");
    for(auto &p : dazniai)
        if(p.second > 1)
            out << p.first << " " << p.second << "\n";
    
    auto crossReference = CrossRef(failas);
    IsvestiCrossRef(crossReference, dazniai);
    cout << "Analize baigta\n";
}