#ifndef TEXTGRAPH_HPP
#define TEXTGRAPH_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cctype>
#include <algorithm>

class TextGraph
{
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> adjacencyList;
    std::vector<std::string> words;

    std::string cleanWord(const std::string &word)
    {
        std::string cleaned;
        for (char c : word)
        {
            if (std::isalpha(c))
                cleaned += std::tolower(c);
        }
        return cleaned;
    }

public:
    void processText(const std::string &text)
    {
        adjacencyList.clear();
        words.clear();

        std::string currentWord;
        for (char c : text)
        {
            if (std::isalpha(c) || c == '\'')
                currentWord += c;
            else if (!currentWord.empty())
            {
                std::string cleaned = cleanWord(currentWord);
                if (!cleaned.empty() && cleaned.length() > 2)
                    words.push_back(cleaned);
                currentWord.clear();
            }
        }

        if (!currentWord.empty())
        {
            std::string cleaned = cleanWord(currentWord);
            if (!cleaned.empty() && cleaned.length() > 2)
                words.push_back(cleaned);
        }

        for (size_t i = 1; i < words.size(); ++i)
        {
            adjacencyList[words[i]].insert(words[i - 1]);
            adjacencyList[words[i - 1]].insert(words[i]);
        }
    }

    std::unordered_map<std::string, std::unordered_set<std::string>> &getGraph() { return adjacencyList; }
    std::vector<std::string> &getWords() { return words; }
};

#endif // TEXTGRAPH_HPP
