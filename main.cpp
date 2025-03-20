#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

int comp (const void * a, const void * b)
{
    return ( *(int*)b - *(int*)a );
}

bool isDigit (const std::string &str)
{
    for (int i: str) {
        if (std::isdigit(i)) continue;
        else {
            std::cerr << "Incorrect input!" << std::endl;
            std::cerr << "Ending an ATM session.\nUntil next time." << std::endl;
            return false;
        }
    }
    return true;
}

bool checkAmount(const int &cash, const int &sum)
{
    if (cash % 100 != 0 || cash == 0) {
        std::cerr << "Incorrect amount for withdrawal!" << std::endl;
        std::cerr << "Ending an ATM session.\nUntil next time." << std::endl;
        return false;
    }

    if (cash > sum) {
        std::cerr << "Insufficient funds!" << std::endl;
        std::cerr << "Ending an ATM session.\nUntil next time." << std::endl;
        return false;
    }
    return true;
}

int main()
{
    std::srand(std::time(nullptr));

    int banknotes [] = {5000, 2000, 1000, 500, 200, 100};
    int in_stock [1000] {};
    char operation = '+';

    while (operation == '+' || operation == '-')
    {
        int count_5th = 0, count_2th = 0, count_1th = 0, count_5h = 0, count_2h = 0, count_1h = 0, sum = 0;

        std::ifstream inData_ATM("..\\bank.bin", std::ios::binary);

        if (!inData_ATM.is_open())
        {
            std::cerr << "Error!!! Unable to read data from ATM." << std::endl;
        }
        else
        {
            inData_ATM.read((char *) in_stock, sizeof(in_stock));
            for (int i = 0; i < 1000; ++i)
            {
                if (in_stock[i] == 5000) ++count_5th;
                if (in_stock[i] == 2000) ++count_2th;
                if (in_stock[i] == 1000) ++count_1th;
                if (in_stock[i] == 500) ++count_5h;
                if (in_stock[i] == 200) ++count_2h;
                if (in_stock[i] == 100) ++count_1h;

                sum += in_stock[i];
            }
            inData_ATM.close();
        }
        if (sum == 0)
        {
            std::cerr << "*******************************************************************************" << std::endl;
            std::cerr << "There is no money in the ATM!!!" << std::endl;
            std::cerr << "To deposit money into an ATM, press '+'" << std::endl;
            std::cerr << "To exit and finish, press 'q' or another symbol" << std::endl;
            std::cerr << "*******************************************************************************" << std::endl;
            std::cin >> operation;
            if (operation != '+') {
                std::cout << "Ending an ATM session.\nUntil next time." << std::endl;
                break;
            }
        }

        if (sum > 0)
        {
            std::cout << std::endl;
            std::cout << "Hello. Available at ATM now:" << std::endl;
            std::cout << "Total amount = " << sum << std::endl;
            std::cout << "Banknotes: 5000 - " << count_5th << ", 2000 - " << count_2th << ", 1000 - " << count_1th <<
                      ", 500 - " << count_5h << ", 200 - " << count_2h << ", 100 - " << count_1h << std::endl;
            std::cout << "*******************************************************************************" << std::endl;
            std::cout << "Please select an operation to continue:" << std::endl;
            std::cout << "To withdraw cash, press '-'" << std::endl;
            std::cout << "To deposit money into an ATM, press '+'" << std::endl;
            std::cout << "To exit and finish, press 'q' or another symbol" << std::endl;
            std::cout << "*******************************************************************************" << std::endl;
            std::cin >> operation;
            if (operation != '+' && operation != '-') {
                std::cout << "Ending an ATM session.\nUntil next time." << std::endl;
                break;
            }
        }

        if (operation == '+')
        {
            std::ofstream data_ATM("..\\bank.bin", std::ios::binary);

            for (int i = 0; i < 1000; ++i) {
                if (in_stock[i] == 0) {
                    in_stock[i] = banknotes[std::rand() % 6];
                }
            }

            qsort(in_stock, 1000, sizeof(int), comp);
            data_ATM.write((char*) in_stock, sizeof(in_stock));
            data_ATM.close();
        }

        if (operation == '-') {
            int cash = 0;
            std::string enteredStr = "";
            std::cout << "What amount do you want to withdraw?" << std::endl;
            std::cin >> enteredStr;

            if (!isDigit(enteredStr)) break;

            cash = stoi(enteredStr);

            if (!checkAmount(cash, sum)) break;

            for (int count = 0; cash != 0; ++count) {
                if (in_stock[count] == 5000 && cash >= 5000) {
                    in_stock[count] = 0;
                    cash -= 5000;
                }
                if (in_stock[count] == 2000 && cash >= 2000) {
                    in_stock[count] = 0;
                    cash -= 2000;
                }
                if (in_stock[count] == 1000 && cash >= 1000) {
                    in_stock[count] = 0;
                    cash -= 1000;
                }
                if (in_stock[count] == 500 && cash >= 500) {
                    in_stock[count] = 0;
                    cash -= 500;
                }
                if (in_stock[count] == 200 && cash >= 200) {
                    in_stock[count] = 0;
                    cash -= 200;
                }
                if (in_stock[count] == 100 && cash >= 100) {
                    in_stock[count] = 0;
                    cash -= 100;
                }
            }

            std::cout << "Money has been issued." << std::endl;

            std::ofstream data_ATM("..\\bank.bin", std::ios::binary);
            qsort(in_stock, 1000, sizeof(int), comp);
            data_ATM.write((char *) in_stock, sizeof(in_stock));
            data_ATM.close();
        }
    }
    return 0;
}
