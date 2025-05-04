#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

enum class TokenType
{
    OPEN_TAG,
    CLOSE_TAG,
    TEXT,
    ATTRIBUTE
};
struct Token
{
    TokenType type;
    string value;
};

enum class NodeType
{
    ELEMENT,
    TEXT
};

struct Node
{
    NodeType type;
    string tagName;
    vector<pair<string, string>> attributes;
    string textContent;
    vector<Node *> children;
};

vector<Token> tokenize(string &html)
{
    vector<Token> tokens;
    int i = 0;
    while (i < html.size())
    {
        if (html[i] == '<')
        {
            i++;
            if (html[i] == '/')
            {
                i++;
                string tagName;
                while (i < html.size() && isalnum(html[i]))
                {
                    tagName += html[i++];
                }
                while (i < html.size() && html[i] != '>')
                    i++;
                i++;
                tokens.push_back({TokenType::CLOSE_TAG, tagName});
            }
            else
            {
                string tagName;
                while (i < html.size() && isalnum(html[i]))
                {
                    tagName += html[i++];
                }
                tokens.push_back({TokenType::OPEN_TAG, tagName});
                while (i < html.size() && html[i] != '>')
                {
                    while (i < html.size() && html[i] != ' ')
                        i++;
                    string att;
                    while (i < html.size() && html[i] != '>' && html[i] != '=')
                    {
                        att += html[i++];
                    }
                    if (i < html.size() && html[i] == '=')
                    {
                        att += html[i++];
                        if (html[i] == '\"')
                        {
                            char quote = html[i++];
                            att += quote;
                            while (i < html.size() && html[i] != quote)
                            {
                                att += html[i++];
                            }
                            if (i < html.size())
                            {
                                att += html[i++];
                            }
                        }
                    }
                    if (!att.empty())
                    {
                        tokens.push_back({TokenType::ATTRIBUTE, att});
                    }
                }
                if (i < html.size() && html[i] == '>')
                    i++;
            }
        }
        else
        {
            string text;
            while (i < html.size() && html[i] != '<')
            {
                text += html[i++];
            }
            if (!text.empty())
            {
                tokens.push_back({TokenType::TEXT, text});
            }
        }
    }
    return tokens;
}

Node *parse(vector<Token> &tokens)
{
    vector<Node *> stack;
    Node *root = nullptr;
    int i = 0;
    while (i < tokens.size())
    {
        if (token.type == TokenType::OPEN_TAG)
        {
            Node *element = new Node{NodeType::ELEMENT, token.value};
            stack.push_back(element);
            i++;
        }
        else if (token.type == TokenType::ATTRIBUTE)
        {
            if (!stack.empty())
            
        }
    }
}
