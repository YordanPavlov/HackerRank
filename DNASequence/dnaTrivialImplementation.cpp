#include <bits/stdc++.h>
#include <deque>
#include <vector>
#include "boost/date_time/posix_time/posix_time.hpp"
using namespace std;

vector<string> split_string(string);

int main()
{
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string genes_temp_temp;
    getline(cin, genes_temp_temp);

    vector<string> genes_temp = split_string(genes_temp_temp);

    vector<string> genes(n);

    for (int i = 0; i < n; i++) {
        string genes_item = genes_temp[i];

        genes[i] = genes_item;
    }

    string health_temp_temp;
    getline(cin, health_temp_temp);

    vector<string> health_temp = split_string(health_temp_temp);

    vector<int> health(n);

    for (int i = 0; i < n; i++) {
        int health_item = stoi(health_temp[i]);

        health[i] = health_item;
    }

    int s;
    cin >> s;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    long unsigned bestString = 0;
    long unsigned worstString = ULONG_MAX;;
    long unsigned worstStringNum = 0;
    auto timeStart = boost::posix_time::second_clock::local_time();

    for (int s_itr = 0; s_itr < s; s_itr++) {
        string firstLastd_temp;
        getline(cin, firstLastd_temp);

        vector<string> firstLastd = split_string(firstLastd_temp);

        int first = stoi(firstLastd[0]);

        int last = stoi(firstLastd[1]);

        string d = firstLastd[2];
        long unsigned dValue = 0;

        for(int indexWord=0; indexWord <d.size(); ++indexWord) {
            for(int indexGene = first; indexGene <= last; ++indexGene) {
              std::string& curGene = genes[indexGene];
              if(0 == strncmp(d.c_str() + indexWord, curGene.c_str(), curGene.length())) {
                if(dValue + health[indexGene] < dValue) {
                  std::cout<<"Type overflow"<<std::endl;
                }
                dValue += health[indexGene];
              }
            }
        }

        if(dValue > bestString) {
          bestString = dValue;
        }
        if(dValue < worstString) {
          worstString = dValue;
          worstStringNum = s_itr;
        }
    }

    auto timeEnd  = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = timeEnd - timeStart;

    std::cout<<worstString<<' '<<bestString<<std::endl;

    std::cout<<"Done in "<<diff.total_milliseconds()<<std::endl;
    std::cout<<"Worst string was at sequence: "<<worstStringNum<<std::endl;

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
