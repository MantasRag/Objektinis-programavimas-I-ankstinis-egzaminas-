#include <iostream> // cin, cout
#include <fstream>  // Failų įvedimui/išvedimui
#include <string>   // String tipo kintamiesiems
#include <map>      // Asociatyvus konteineris
#include <regex>    // Žodžių valymui
#include <vector>   // Vector konteineriui
#include <sstream>  // String stream eilutėms skaityti
#include <set>      // Set'ui

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

void IsvestiZodziuDaznius(const map<string,int>& dazniai) {
    ofstream out("zodziu_daznis.txt");
    out << "Žodis, pasikartojimų skaičius\n";
    out << "---------------\n";
    for(auto &p : dazniai) {
        if(p.second > 1) {
            out << p.first << " " << p.second << "\n";
        }
    }
}

//2 UŽDUOTIS
// Cross-reference: kuriose eilutėse yra minimi pasikartojantys žodžiai
map<string,vector<int>> CrossRef(const string& failas) {
    map<string,vector<int>> m;
    ifstream fin(failas);
    string line;
    int nr = 1;
    while (getline(fin, line)) {
        istringstream iss(line);
        string zodis;
        while (iss >> zodis) {
            string val = CleanWord(zodis);
            if (!val.empty()) {
                if (m[val].empty() || m[val].back() != nr)
                    m[val].push_back(nr);
            }
        }
        nr++;
    }
    return m;
}

void IsvestiCrossRef(const map<string,vector<int>>& cr, const map<string,int>& dazniai) {
    ofstream out("cross-reference.txt");
    out << "Žodis, eilučių, kuriose jis paminėtas numeriai\n";
    out << "---------------\n";
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

//3 UŽDUOTIS
// URL paieška
set<string> RastiURL(const string& failas) {
    set<string> urls;
    ifstream fin(failas);
    string line;
    
    // Vienas regex visiems URL formatams
    regex urlPattern(R"((https?://|www\.)?[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(/[^\s]*)?)");
    
    while (getline(fin, line)) {
        auto begin = sregex_iterator(line.begin(), line.end(), urlPattern);
        auto end = sregex_iterator();
        
        for (auto it = begin; it != end; ++it) {
            string url = it->str();
            if (!url.empty() && url.back() == '.') {
                url.pop_back();
            }
            urls.insert(url);
        }
    }
    
    return urls;
}

void IsvestiURL(const set<string>& urls) {
    ofstream out("url_sarasas.txt");
    int nr = 1;
    out << "URL sąrašas\n";
    out << "---------------\n";
    for(const auto& url : urls) {
        out << nr << ". " << url << "\n";
        nr++;
    }
}

string GautiFaila() {
    string failas;
    while (true) {
        cout << "Iveskite faila: ";
        cin >> failas;
        ifstream aregzistuoja(failas);
        if (aregzistuoja.good()) {
            aregzistuoja.close();
            return failas;
        }
        
        cout << "Failas nerastas. Ivesti failo pavadinima is naujo.\n";
    }
}

int main() {
    string failas=GautiFaila();

    auto dazniai = ZodziuDazniai(failas);
    IsvestiZodziuDaznius(dazniai);
    
    auto crossReference = CrossRef(failas);
    IsvestiCrossRef(crossReference, dazniai);

    auto urls = RastiURL(failas);
    IsvestiURL(urls);
    cout << "Analize baigta\n";
}