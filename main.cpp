#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

string ReadLine()
{
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber()
{
    int result = 0;
    cin >> result;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return result;
}

vector<string> SplitIntoWords(const string& text)
{
    istringstream in(text);
    vector<string> words(istream_iterator<string>(in), {});
    return words;
}

set<string> ParseStopWords(const string& text)
{
    auto words = SplitIntoWords(text);
    set<string> stop_words(words.begin(), words.end());
    return stop_words;
}

vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words)
{
    vector<string> words;
    auto const arr = SplitIntoWords(text);
    copy_if(arr.begin(), arr.end(), back_inserter(words),
            [&stop_words](auto const& str) { return stop_words.count(str) == 0U; });
    return words;
}

void AddDocument(vector<vector<string>>& documents, const set<string>& stop_words,
                 const string& document)
{
    const vector<string> words = SplitIntoWordsNoStop(document, stop_words);
    documents.push_back(words);
}

set<string> ParseQuery(const string& text, const set<string>& stop_words)
{
    auto query_words = SplitIntoWordsNoStop(text, stop_words);
    return { query_words.begin(), query_words.end() };
}

bool MatchDocument(const vector<string>& document_words, const set<string>& query_words)
{
    return any_of(document_words.begin(), document_words.end(),
                  [&query_words](auto const& str) { return query_words.count(str) != 0U; });
}

vector<int> FindDocuments(const vector<vector<string>>& documents, const set<string>& stop_words,
                          const string& query)
{
    vector<int> matched_documents;
    auto const parse_query = ParseQuery(query, stop_words);
    for (size_t i = 0; i < documents.size(); ++i) {
        if (MatchDocument(documents[i], parse_query)) {
            matched_documents.push_back(i);
        }
    }
    return matched_documents;
}

int main()
{
    const string stop_words_joined = ReadLine();
    const set<string> stop_words = ParseStopWords(stop_words_joined);

    // Read documents
    vector<vector<string>> documents;
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        AddDocument(documents, stop_words, ReadLine());
    }

    const string query = ReadLine();
    for (const int document_id : FindDocuments(documents, stop_words, query)) {
        cout << "{ document_id = "s << document_id << " }"s << endl;
    }
}
