#include <iostream>
#include "Solution.h"


int main(int argc, char** argv)
{
    std::setlocale(LC_ALL, "");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    //default file
    std::string filename{ "./input.txt" };

    if (argc == 2) {
        filename = argv[1];
    }


    KHAS::Solution solution{ KHAS::DataInput {
            .title = "Моделирование матричной игры 2x2"
            , .number_of_unknown_variables = 2
            , .number_of_parties = 100 }
    , filename };
    solution.start();



    system("pause");
}