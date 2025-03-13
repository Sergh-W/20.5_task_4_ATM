#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

int comp (const void * a, const void * b)
{
    return ( *(int*)b - *(int*)a );
}

int main()
{
    std::srand(std::time(nullptr));

    int banknotes [] = {5000, 2000, 1000, 500, 200, 100};
    int count_5th = 0, count_2th = 0, count_1th = 0, count_5h = 0, count_2h = 0, count_1h = 0, sum = 0;
    int in_stock [1000] {};
    char operation = '+';

    while (operation == '+' || operation == '-')
    {
        std::ifstream inData_ATM("..\\bank.bin", std::ios::binary);

        for (int i = 0; i < 1000; ++i)
        {
            inData_ATM >> in_stock[i];

            if (in_stock[i] == 5000) ++count_5th;
            if (in_stock[i] == 2000) ++count_2th;
            if (in_stock[i] == 1000) ++count_1th;
            if (in_stock[i] == 500) ++count_5h;
            if (in_stock[i] == 200) ++count_2h;
            if (in_stock[i] == 100) ++count_1h;
        }

        inData_ATM >> sum;
        inData_ATM.close();

        if (sum == 0)
        {
            std::cerr << "There is no money in the ATM!!!" << std::endl;
            std::cerr << "You can only top up by pressing '+', otherwise the ATM will shut down!!!" << std::endl;
        }
        else
        {
            std::cout << std::endl;
            std::cout << "Hello. Available at ATM now:" << std::endl;
            std::cout << "Total amount = " << sum << std::endl;
            std::cout << "Banknotes: 5000 - " << count_5th << ", 2000 - " << count_2th << ", 1000 - " << count_1th <<
                      ", 500 - " << count_5h << ", 200 - " << count_2h << ", 100 - " << count_1h << std::endl;
        }
        std::cout << "*******************************************************************************" << std::endl;
        std::cout << "Please select an operation to continue:" << std::endl;
        std::cout << "To withdraw cash, press '-'" << std::endl;
        std::cout << "To deposit money into an ATM, press '+'" << std::endl;
        std::cout << "To exit and finish, press 'q' or another symbol" << std::endl;
        std::cout << "*******************************************************************************" << std::endl;

        std::cin >> operation;

        if ((sum == 0 && operation != '+') || (operation != '+' && operation != '-')) {
            break;
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

            sum = 0;
            
            for (int i = 0; i < 1000; ++i) {
                sum += in_stock[i];
                data_ATM << in_stock[i] << std::endl;
            }

            data_ATM << sum << std::endl;

            data_ATM.close();
        }

        if (operation == '-')
        {
            int cash = 0;
            std::cout << "What amount do you want to withdraw?" << std::endl;
            std::cin >> cash;

            if (cash % 100 != 0 || cash <= 0) {
                std::cerr << "Incorrect amount for withdrawal!" << std::endl;
                break;
            }

            else if ((sum - cash) < 0) {
                std::cerr << "Insufficient funds!" << std::endl;
                break;
            }

            else {
                int count = 0;
                while (cash > 0) {
                    if (in_stock[count] == 5000 && cash >= 5000) {
                        in_stock[count] = 0;
                        cash -= 5000;
                        ++count;
                    }
                    if (in_stock[count] == 2000 && cash >= 2000) {
                        in_stock[count] = 0;
                        cash -= 2000;
                        ++count;
                    }
                    if (in_stock[count] == 1000 && cash >= 1000) {
                        in_stock[count] = 0;
                        cash -= 1000;
                        ++count;
                    }
                    if (in_stock[count] == 500 && cash >= 500) {
                        in_stock[count] = 0;
                        cash -= 500;
                        ++count;
                    }
                    if (in_stock[count] == 200 && cash >= 200) {
                        in_stock[count] = 0;
                        cash -= 200;
                        ++count;
                    }
                    if (in_stock[count] == 100 && cash >= 100) {
                        in_stock[count] = 0;
                        cash -= 100;
                        ++count;
                    }
                }

                std::cout << "Money has been issued." << std::endl;

                std::ofstream data_ATM("..\\bank.bin", std::ios::binary);

                qsort(in_stock, 1000, sizeof(int), comp);

                sum = 0;

                for (int i = 0; i < 1000; ++i) {
                    sum += in_stock[i];
                    data_ATM << in_stock[i] << std::endl;
                }

                data_ATM << sum << std::endl;

                data_ATM.close();
            }
        }
    }

    return 0;
}