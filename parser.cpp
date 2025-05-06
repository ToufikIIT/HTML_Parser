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
                    while (i < html.size() && isspace(html[i]))
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
        Token& token = tokens[i];
        if (token.type == TokenType::OPEN_TAG)
        {
            Node *element = new Node{NodeType::ELEMENT, token.value};
            stack.push_back(element);
            i++;
        }
        else if (token.type == TokenType::ATTRIBUTE)
        {
            if (!stack.empty())
            {
                string att = token.value;
                int equal_pos = att.find('=');
                if (equal_pos != string::npos)
                {
                    string key = att.substr(0, equal_pos);
                    string value = att.substr(equal_pos + 1);
                    if (value.front() == '\"')
                    {
                        value = value.substr(1, value.size() - 2);
                    }
                    stack.back()->attributes.push_back({key, value});
                }
            }
            i++;
        }
        else if (token.type == TokenType::TEXT)
        {
            Node *textNode = new Node{NodeType::TEXT};
            textNode->textContent = token.value;
            if (!stack.empty())
            {
                stack.back()->children.push_back(textNode);
            }
            i++;
        }else if (token.type == TokenType::CLOSE_TAG) {
            Node* node = stack.back();
            stack.pop_back();
            if (!stack.empty()) {
                stack.back()->children.push_back(node);
            } else {
                root = node;
            }
            i++;
        }
    }
    return root;
}
void printNode(Node* node, int depth = 0) {
    string indent(depth * 2, ' ');
    if (node->type == NodeType::ELEMENT) {
        cout << indent << "Element: " << node->tagName << "\n";
        for (auto attr : node->attributes) {
            cout << indent << "  Attribute: " << attr.first << " = " << attr.second << "\n";
        }
        for (auto child : node->children) {
            printNode(child, depth + 1);
        }
    } else if (node->type == NodeType::TEXT) {
        cout << indent << "Text: " << node->textContent << "\n";
    }
}
int main() {
    string html = "<div><p class=\"title\">Hello</p><span>World</span></div>";
    auto tokens = tokenize(html);
    Node* root = parse(tokens);
    printNode(root);
    return 0;
}