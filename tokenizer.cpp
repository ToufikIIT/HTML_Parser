#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

enum class TokenType{OPEN_TAG, CLOSE_TAG, TEXT, ATTRIBUTE};

struct Token{
    TokenType type;
    string value;
};

vector<Token> tokenize(string& html){
    vector<Token> tokens;
    int i=0;
    while (i<html.size()){
        if(html[i] == '<'){
            i++;
            if(html[i] == '/'){
                i++;
                string tagName;
                while(i < html.size() && isalnum(html[i])){
                    tagName += html[i++];
                }
                while(i < html.size() && html[i] != '>') i++;
                i++;
                tokens.push_back({TokenType::CLOSE_TAG, tagName});
            }
            else {
                string tagName;
                while(i < html.size() && isalnum(html[i])){
                    tagName+= html[i++];
                }
                tokens.push_back({TokenType::OPEN_TAG, tagName});
                while(i < html.size() && html[i] != '>'){
                    while(i < html.size() && html[i] != ' ') i++;
                    string att;
                    while(i < html.size() && html[i] != '>' && html[i] != '='){
                        att+=html[i++];
                    }
                    if (i < html.size() && html[i]== '='){
                        att+=html[i++];
                        if (html[i] == '\"') {
                            char quote = html[i++];
                            att += quote;
                            while (i < html.size() && html[i] != quote) {
                                att += html[i++];
                            }
                            if (i < html.size()) {
                                att += html[i++];
                            }
                        }
                    }
                    if (!att.empty()) {
                        tokens.push_back({TokenType::ATTRIBUTE, att});
                    }
                }
                if (i < html.size() && html[i] == '>') i++;
            }
        }
        else {
            string text;
            while (i < html.size() && html[i] != '<') {
                text += html[i++];
            }
            if (!text.empty()) {
                tokens.push_back({TokenType::TEXT, text});
            }
        }
    }
    return tokens;

    
}

int main() {
    string html = "<p class=\"title\">Hello</p>";
    auto tokens = tokenize(html);
    for (auto token : tokens) {
        string typeStr;
        switch (token.type) {
            case TokenType::OPEN_TAG: typeStr = "OPEN_TAG"; break;
            case TokenType::CLOSE_TAG: typeStr = "CLOSE_TAG"; break;
            case TokenType::TEXT: typeStr = "TEXT"; break;
            case TokenType::ATTRIBUTE: typeStr = "ATTRIBUTE"; break;
        }
        cout << typeStr << ": " << token.value << "\n";
    }
    return 0;
}
