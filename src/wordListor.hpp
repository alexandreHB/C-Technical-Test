#include "csv_file_value.hpp"

#include <vector>
#include <utility>


class WordListor
{
    public:
        //attributes
        std::string txt_filename;
        int total_points;
        std::vector< std::pair<std::wstring, int> > table_word_score;

        WordListor(std::string txt_filename, const std::map<wchar_t, int>& table_char_values);

        //counts the score of a word and add it to the total score of the wordListor
        int counting_word_score(std::wstring s, const std::map<wchar_t, int>& table_char_values);

        //generates a txt file with the desired output
        void create_output();
};

//allow to display some important informations a wordListor
std::wostream& operator<<(std::wostream &output, const WordListor &wl);


//remove puntuation at the beginin and at the end of a word , .
void remove_end_ponctuation(std::wstring& word);
void remove_beg_ponctuation(std::wstring& word);
void remove_punctuation(std::wstring& word);

