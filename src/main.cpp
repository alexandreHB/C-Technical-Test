#include "wordListor.hpp"

using namespace std;
namespace fs = std::filesystem;


int main(int argc, char** argv)
{
    //handle cases where argument is invalid
    try
    {
        if (argc < 2 || argv[1] == nullptr)
            throw std::invalid_argument("File required !");
        if(fs::path(argv[1]).extension() != ".txt")
            throw std::invalid_argument("File given is not a .txt file !");
    }
    catch(const std::invalid_argument& e)
    {
        cerr << "Error : " << e.what() << endl;
        return 1;
    }

    //create the map associated to the csv file
    const std::map<wchar_t, int> TAB_CHAR_VALUE = create_map_from_csv(CSV_FILE);
    display_map_info(TAB_CHAR_VALUE);


    string txt_filename = argv[1];
    WordListor wl = WordListor(txt_filename, TAB_CHAR_VALUE);
    wl.create_output();
    std::wcout << wl << endl;
}