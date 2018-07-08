#include <bits/stdc++.h>
#include <deque>
#include <vector>
using namespace std;

vector<string> split_string(string);

/**
 *  Node inside the trie
 */
struct Node {
    Node(char _ch)
    :
    chValue(_ch)
    {}

    bool isDictionaryNode = false;
    char chValue;
    std::map<char, unique_ptr<Node>> children;
    Node* parent = nullptr;
    Node* blueLink = nullptr;
    Node* greenLink = nullptr;
    std::vector<unsigned> indexes;
};

Node rootNode(' ');

std::deque<Node*> frontTraverse;
void buildBlueLinks() {
    while(!frontTraverse.empty()) {
        Node* curNode  = frontTraverse.front();
        Node* searchNode = curNode->parent->blueLink;
        frontTraverse.pop_front();

        assert(nullptr == curNode->blueLink);

        while(searchNode) {
            auto iter = searchNode->children.find(curNode->chValue);
            if(iter != searchNode->children.end() && iter->second.get() != curNode) {
                curNode->blueLink = iter->second.get();
                break;
            }
            searchNode = searchNode->blueLink;
        }
        if(!curNode->blueLink) {
            curNode->blueLink = &rootNode;
        }

        for(auto& child : curNode->children) {
            if(child.second) {
                frontTraverse.push_back(child.second.get());
            }
        }
    }
}

void buildGreenLinks(Node* curNode) {
    assert(!curNode->greenLink);

    if(curNode != &rootNode) {
        Node* curBlueLink = curNode->blueLink;
        while(curBlueLink != &rootNode) {

            if(curBlueLink->isDictionaryNode) {
                curNode->greenLink = curBlueLink;
                if(curBlueLink->indexes.size() == 1 && curBlueLink->indexes[0] == 1) {
                    Node* iterNode = curBlueLink;
                    iterNode = curNode;
                }
                break;
            } else {
                curBlueLink = curBlueLink->blueLink;
            }
        }
        if(!curNode->greenLink) {
            curNode->greenLink = &rootNode;
        }
    }

    for(auto& pairChild : curNode->children) {
        buildGreenLinks(pairChild.second.get());
    }
}

void buildTrie(vector<string>& genes, vector<int>& health)
{
    assert(genes.size() == health.size());
    for(int gIndex = 0; gIndex < genes.size(); ++gIndex) {
        const std::string& curGene = genes[gIndex];

        Node* curNode = &rootNode;
        for(char ch : curGene) {
            std::unique_ptr<Node>& nextNode = curNode->children[ch];
            if(nullptr == nextNode.get()) {
                nextNode.reset(new Node(ch));
                nextNode->parent = curNode;
            }
            curNode = nextNode.get();
        }
        curNode->indexes.push_back(gIndex);
        curNode->isDictionaryNode = true;
    }

    // Set the blue links manually for the first level of nodes
    for(auto& pairChildRoot : rootNode.children) {
        pairChildRoot.second->blueLink = &rootNode;
        for(auto& pairChildLevel1 : pairChildRoot.second->children) {
            frontTraverse.push_back(pairChildLevel1.second.get());
        }
    }
    // Set recursively the rest of the blue links
    buildBlueLinks();
    // Once the blue links are ready - build the green links
    for(auto& pairChild : rootNode.children) {
        buildGreenLinks(pairChild.second.get());
    }
}

// Increment the value of the current word if indexes match
void incrementValueWithNode(int start,
                            int end,
                            std::vector<unsigned>& indexes,
                            std::vector<int>& health,
                            long unsigned& value)
{
    for(int geneIndex : indexes)
    {
        if(geneIndex >= start && geneIndex <= end)
        {
            value += health[geneIndex];
        }
    }
}

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

    buildTrie(genes, health);

    int s;
    cin >> s;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    long unsigned bestString = 0;
    long unsigned worstString = ULONG_MAX;

    for (int s_itr = 0; s_itr < s; s_itr++)
    {
        string firstLastd_temp;
        getline(cin, firstLastd_temp);

        vector<string> firstLastd = split_string(firstLastd_temp);

        int first = stoi(firstLastd[0]);

        int last = stoi(firstLastd[1]);

        string d = firstLastd[2];

        long unsigned dValue = 0;
        Node* curPos = &rootNode;
        int dIndex = 0;

        while(dIndex < d.size())
        {
            long unsigned dValueBefore = dValue;
            auto iter = curPos->children.find(d[dIndex]);

            if(iter != curPos->children.end())
            {
                curPos = iter->second.get();
                // Add the value of the current node as well as all reachable by green links
                Node* greenLink = curPos;
                while(greenLink != &rootNode)
                {
                    incrementValueWithNode(first, last, greenLink->indexes, health, dValue);
                    greenLink = greenLink->greenLink;
                }
                ++dIndex;
            }
            else
            {
                if(curPos == &rootNode)
                {
                    // We are the root node and no searched string starts with this letter.
                    // Move to next letter.
                    ++dIndex;
                }
                else
                {
                    // Continue backtracking up the trie.
                    curPos = curPos->blueLink;
                }
            }

            // Assert not overflowing
            assert(dValue >= dValueBefore);
        }

        if(dValue > bestString) {
            bestString = dValue;
        }
        if(dValue < worstString) {
            worstString = dValue;
        }
    }

    std::cout<<worstString<<' '<<bestString<<std::endl;

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
