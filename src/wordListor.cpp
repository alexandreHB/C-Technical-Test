#include "wordListor.hpp"
#include <set>
#include <algorithm>
#include <cctype> //for the isalnum


//#include <execution>
/*
bonus : need to install extrnal lib 
sudo aptèget install libtbb-dev

The instructions prohibit the use of external libraries, 
It's just an optional bonus, it doesn't change at all how the code is implemented. 
It's just a library that makes execution faster.
*/

using namespace std;
namespace fs = std::filesystem;


//construct a WorldListor given a txt filename and a map wich associates char and their values
WordListor::WordListor(string txt_filename, const map<wchar_t, int>& table_char_values)
{
    total_points = 0;
    this->txt_filename = txt_filename;

    wifstream txt_file(txt_filename);                                        //open the txt file given in args
    if(fs::is_empty(txt_filename))
        throw std::invalid_argument("this txt file is empty !");            //when txt file empty
            
    if (!txt_file.is_open())
        throw std::invalid_argument("this csv file does not exist !");      //when csv file is invalid


    //building a set of pair of strings and their associated score (set used for unicity)
    set< pair<wstring, int>> temp_set;                                       
    wstring word;
    while (txt_file >> word)
    {
        remove_punctuation(word);
        temp_set.emplace(word, counting_word_score(word, table_char_values));
    }
    txt_file.close();


    //move content of temp_set to the vector to avoid copying data
    table_word_score.assign(
        std::make_move_iterator(temp_set.begin()),
        std::make_move_iterator(temp_set.end())
    );
    txt_file.close();


    //sorting the vector usig our lambdas func
    auto compare = [](const pair<wstring, int>& p1, const pair<wstring, int>& p2){return p1.second < p2.second;};
    std::sort(table_word_score.begin(), table_word_score.end(), compare);
    // bonus : if <execution> is included and tbb flag added to the compilation command in the cmakelist.txt : 
    //std::sort(std::execution::par_unseq, table_word_score.begin(), table_word_score.end(), compare);
}


//counts the score of a word and add it to the total score of the wordListor
int WordListor::counting_word_score(wstring s, const map<wchar_t, int>& table_char_values)
{
    int points = 0;
    for (wchar_t c : s)
    {
        auto it = table_char_values.find(c);
        if (it != table_char_values.end())
            points += it->second;
    }
    total_points += points;
    return points;
}

//generates a txt file with the desired output
void WordListor::create_output()
{
    //create the name of the output file
    string output_filename = txt_filename;
    output_filename = output_filename.insert(output_filename.size()-4, ".count");

    //write one the output file
    wofstream output_file(output_filename);
    for(const auto& p : table_word_score)
        output_file << p.first << ", " << p.second << endl;

    output_file.close();
}


//allow to display some important informations a wordListor
std::wostream& operator<<(std::wostream &os, const WordListor &wl)
{
    os << "*** SUMMARY ***" << endl;
    os << "source txt filename : " << wstring(wl.txt_filename.begin(), wl.txt_filename.end()) << endl;
    for (const auto& p : wl.table_word_score)
        os << p.first << " = " << p.second << "; ";
    os << endl;
    os << "number of unique word : " << wl.table_word_score.size() << "; ";
    os << "total points : " << wl.total_points << endl;
    os << "*** SUMMARY ***" << endl;

    return os;
}

void remove_end_ponctuation(wstring& word)
{
    wstring temp = word;
    while (!word.empty() && (word.back()== ',' || word.back()== '.'))
        word.pop_back();

    //when the 'word' is only composed by punctuation
    //we let it as it was before
    if(word.empty()) 
        word = temp; 

}
void remove_beg_ponctuation(wstring& word)
{
    wstring temp = word;
    auto it = word.begin();

    while (it != word.end() && (word.back()== ',' || word.back()== '.'))
        ++it;
    word.erase(word.begin(), it);

    //when the 'word' is only composed by punctuation
    //we let it as it was before
    if(it != word.end()) 
        word = temp; 
}
void remove_punctuation(wstring& word)
{
    remove_end_ponctuation(word);
    remove_beg_ponctuation(word);
}
