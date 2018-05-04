#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <utility>
#include <map>
#include <iomanip>

using namespace std;

struct Standing {
    int mp;
    int w;
    int d;
    int l;
    int gf;
    int ga;
    int gd;
    int pts;

    Standing() {
        this->mp = this->w = this->d = this->l = this->gf = this->ga = this->gd = this->pts = 0;
    }
    friend ostream& operator << (ostream &os, const Standing &stg) {
        os << stg.mp << "\t" << stg.w << "\t" << stg.d << "\t" << stg.l 
        << "\t" << stg.gf << "\t" << stg.ga << "\t" << stg.gd << "\t" << stg.pts;
        return os;
    }
};

struct CMP {
    bool operator() (pair<string, Standing> p, pair<string, Standing> p2) const {
        return (p.second.pts < p2.second.pts) || (p.second.pts == p2.second.pts && p.second.gd < p2.second.gd) 
            || (p.second.pts == p2.second.pts && p.second.gd == p2.second.gd && p.second.gf < p2.second.gf); 
    }
};

string clubs = "../clubs.txt";
string results = "../results.txt";
map<string, Standing> mp;
priority_queue< pair<string, Standing>, vector< pair<string, Standing> >, CMP> pq;

void spaces(int n) {
    while (n--) {
        cout << " ";
    }
}

int main() {
    freopen("../standings.txt", "w", stdout);

    ifstream f1;
    ifstream f2;
    string str, tmp, tmp2, club, club2;
    int dot_pos, goal, goal2, pos, cnt = 0;
    pair<string, Standing> pss;

    f1.open(clubs);
    if (f1.is_open()) {
        while(getline(f1, str)) {
            mp[str] = Standing();
        }
    }
    f1.close();

    f2.open(results);
    if (f2.is_open()) {
        while(getline(f2, str)) {
            if (str.substr(0, 8) == "Matchday" || str == "") {
                continue;
            }
            dot_pos = str.find(":");
            tmp = str.substr(0, dot_pos);
            tmp2 = str.substr(dot_pos + 1, str.length() - dot_pos - 1);
            goal = stoi(tmp.substr(tmp.rfind(" ")));
            goal2 = stoi(tmp2.substr(0, tmp2.find(" ")));
            club = tmp.substr(0, tmp.rfind(" "));
            club2 = tmp2.substr(tmp2.find(" ") + 1);
            pos = club.length() - 1;
            while (club[pos] == ' ') {
                --pos;
            }
            club = club.substr(0, pos + 1);
            pos = 0;
            while (club2[pos] == ' ') {
                ++pos;
            }
            club2 = club2.substr(pos);
            // cout << "[" << club << "]" << "[" << club2 <<"]" << endl;
            if (goal > goal2) {
                mp[club].pts += 3;
                mp[club].w += 1;
                mp[club2].l += 1;
            }
            else if (goal < goal2) {
                mp[club2].pts += 3;
                mp[club].l += 1;
                mp[club2].w += 1;
            }
            else {
                mp[club].pts += 1;
                mp[club2].pts += 1;
                mp[club].d += 1;
                mp[club2].d += 1;
            }
            mp[club].mp += 1;
            mp[club2].mp += 1;
            mp[club].gf += goal;
            mp[club2].gf += goal2;
            mp[club].ga += goal2;
            mp[club2].ga += goal;
            mp[club].gd = mp[club].gf - mp[club].ga;
            mp[club2].gd = mp[club2].gf - mp[club].ga;
        }
    }
    f2.close();
    cout << "RANK"<< "\t" << "CLUB";
    spaces(16);
    cout << "\t" << "MP" << "\t" << "W" << "\t" << "D" << "\t" << "L" << "\t" << "GF" << "\t" << 
        "GA" << "\t" << "GD" << "\t" << "PTS" << endl;
    for (map<string, Standing>::iterator it = mp.begin(); it != mp.end(); ++it) {
        pq.push(make_pair(it->first, it->second));
    }
    while (!pq.empty()) {
        pss = pq.top();
        pq.pop();
        cout << ++cnt << "\t\t" << pss.first;
        spaces(20 - (pss.first).length());
        cout << "\t" << pss.second << endl;
    }
    return 0;
}