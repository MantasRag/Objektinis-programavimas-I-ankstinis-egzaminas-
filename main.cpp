#include <iostream> // cin, cout
#include <fstream>  // Failų įvedimui/išvedimui
#include <string>   // String tipo kintamiesiems
#include <map>      // Asociatyvus konteineris
#include <regex>    // 
#include <vector>   //
#include <sstream>  //

using namespace std;

// 1 UŽDUOTIS
// Naikina skyrybos ženklus šalia žodžių (veikia su UTF-8)
string CleanWord(const string& z) {
    regex pattern(R"([A-Za-zĄČĘĖĮŠŲŪŽąčęėįšųūž-]+)");       // Regex pattern (žodžiams su UTF-8)
    //regex pattern(R"([A-Za-z0-9ĄČĘĖĮŠŲŪŽąčęėįšųūž-]+)");  // jei norim, kad rodytų ir skaičius

    smatch m; // Regex match objektas
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

int main() {
    string failas;
    cout << "Iveskite faila: ";
    cin >> failas;

    auto dazniai = ZodziuDazniai(failas);

    ofstream out("zodziu_daznis.txt");
    for(auto &p : dazniai)
        if(p.second > 1)
            out << p.first << " " << p.second << "\n";

    cout << "Analize baigta\n";
}
