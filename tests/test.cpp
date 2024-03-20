#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../src/csv_file_value.hpp"
#include "../src/wordListor.hpp"

using namespace std;
namespace fs = std::filesystem;

// generic path
const auto empty_csv_file = PROJECT_PATH / "empty.csv"; 
const auto csv_filename = PROJECT_PATH / "test.csv";
const auto txt_filename = PROJECT_PATH /"texts"/ "test.txt";


TEST_CASE("Map creation from csv file", "[map_from_csv_file]")
{
    SECTION("Invalid csv filename ")
    {
        REQUIRE_THROWS(create_map_from_csv("fake.csv"));
    }

    SECTION("Existing csv filename but empty")
    {
        REQUIRE_THROWS(create_map_from_csv(empty_csv_file));
    }

    SECTION("Valid csv file")
    {
        //considering certain a csv file
        std::ofstream csv_file(csv_filename);
        csv_file << "A,32" << '\n' << "B,36" << '\n' << "z,26";
        csv_file.close();

        auto myMap = create_map_from_csv(csv_filename);
        REQUIRE(myMap.size() == 3);
        REQUIRE(myMap['A'] == 32);
        REQUIRE(myMap['B'] == 36);
        REQUIRE(myMap['z'] == 26);

        fs:remove(csv_filename);
    }
}


TEST_CASE("WordListor Constructor", "[WL_constructor]")
{
    SECTION("Invalid txt file (regardless of csv file)")
    {
        //considering certain a csv file
        std::ofstream csv_file(csv_filename);
        csv_file << "A,32" << '\n' << "B,36" << '\n' << "z,26";
        csv_file.close();
        auto myMap = create_map_from_csv(csv_filename);

        REQUIRE_THROWS(WordListor("fake.txt", myMap));

        fs::remove(csv_filename);
    }

    SECTION("Existing but empty txt file")
    {
        //considering a certain csv file
        std::ofstream csv_file(csv_filename);
        csv_file << "A,32" << '\n' << "B,36" << '\n' << "z,26";
        csv_file.close();
        auto myMap = create_map_from_csv(csv_filename);

        //considering an empty txt file
        std::ofstream txt_file(txt_filename); 
        txt_file.close();

        REQUIRE_THROWS(WordListor(txt_filename, myMap));

        fs::remove(csv_filename);
        fs::remove(txt_filename);
    }


    SECTION("Empty csv file")
    {
        //considering a valid txt file
        std::ofstream txt_file(txt_filename);
        txt_file << "hello Sym&Cure :)";
        txt_file.close();

        REQUIRE_THROWS(WordListor(txt_filename, create_map_from_csv(empty_csv_file)));
        fs::remove(txt_filename);
    }
    
    SECTION("Valid txt file and csv file")
    {
        //considering a certain map
        const auto TAB_CHAR_VALUE = create_map_from_csv(CSV_FILE);

        //considering a certain txt file
        std::ofstream txt_file(txt_filename);
        txt_file << "hello Sym&Cure :) Nice to meet you! My name is Alex";
        txt_file.close();

        WordListor wl = WordListor(txt_filename, TAB_CHAR_VALUE);

        //check attributes initialization
        REQUIRE(wl.txt_filename.compare(txt_filename) == 0);
        REQUIRE(wl.table_word_score.size() == 11);
        REQUIRE(wl.total_points == 616);


        // check if table_word_score is correctly sorted
        REQUIRE
        (
            std::all_of(wl.table_word_score.begin() + 1, wl.table_word_score.end(),
            [&](const auto& pair)
            {
                size_t index = &pair - &(*wl.table_word_score.begin());
                return pair.second >= wl.table_word_score[index - 1].second;
            })
        );

        txt_file.close();
        fs::remove(txt_filename);
    }
}


TEST_CASE("Counting score of a word", "[word_score]")
{
    //considering 2 maps, one filled, and one empty
    map<wchar_t, int> table_value2;
    map<wchar_t, int> table_value;
    table_value['A'] = 4;
    table_value['B'] = 7; 
    table_value['E'] = 5; 
    table_value['R'] = 1; 

    //considering a certain txt file
    std::ofstream txt_file(txt_filename, std::ios::trunc);
    txt_file << "Arbre";
    txt_file.close();

    WordListor wl = WordListor(txt_filename, table_value);
    REQUIRE(wl.counting_word_score(wl.table_word_score[0].first, table_value) == 4);

    txt_file.open(txt_filename, std::ios::trunc);
    txt_file << "ARBRE";
    txt_file.close();

    WordListor wl2 = WordListor(txt_filename, table_value);
    REQUIRE(wl2.counting_word_score(wl2.table_word_score[0].first, table_value) == 18);
    REQUIRE(wl2.counting_word_score(wl2.table_word_score[0].first, table_value2) == 0);

    fs::remove(txt_filename);
}

TEST_CASE("Ouput txt file generation", "[output_file]")
{
    const auto TAB_CHAR_VALUE = create_map_from_csv(CSV_FILE);

    //considering a certain txt file
    ofstream txt_file(txt_filename);
    txt_file << "apple banana orange";
    txt_file.close();

    WordListor wl = WordListor(txt_filename, TAB_CHAR_VALUE);
    wl.create_output();

    //check the name of the output file
    const string output_filename = txt_filename.string().insert(txt_filename.string().size()-4, ".count");
    REQUIRE(txt_file.good() == true);

    //compare each line of the file whith what is expected
    string line;
    vector<string> expected_lines = {"banana, 11", "orange, 38", "apple, 44"};

    std::ifstream output_file(output_filename);
    bool all_lines_correct = std::all_of(expected_lines.begin(), expected_lines.end(),
                                    [&](const string& expected_line)
                                    {
                                        if (std::getline(output_file, line))
                                            return line == expected_line;
                                        else                                        
                                            return false;
                                    });
    REQUIRE(all_lines_correct);

    //check there is no more lines after that
    REQUIRE_FALSE(std::getline(output_file, line));
    output_file.close();
    fs::remove(txt_filename);
    fs::remove(output_filename);
}




