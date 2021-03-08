#include <iostream>     // std::cout
#include <string>       // std::string
#include <fstream>      // std::ifstream
#include <sstream>      // std::stringstream
#include <algorithm>    // std::random_shuffle 
#include <vector>       // std::vector
#include <algorithm>    // std::stoi

#include "aux.h"
#include "classes.h"

int main(int argc, char* argv[]){

    //Payout table.
    constexpr int MAX_PAYOUT_TAB_SIZ{15};                           //
    double payoutTable[MAX_PAYOUT_TAB_SIZ][MAX_PAYOUT_TAB_SIZ]{0};  //        
    std::string PAYOUT_TAB_FILE_PATH{"payout.dat"};                 //

    // TODO: getPayoutTableData(PAYOUT_TAB_FILE_PATH,&payoutTable,MAX_PAYOUT_TAB_SIZ);
    
    std::ifstream payoutFile{ PAYOUT_TAB_FILE_PATH, std::ios::in }; // 
    std::stringstream sss;                                          //
    std::string tempString;                                         //
    double tempDouble{0};                                           //
    int i_counter{0};                                               //
    int j_counter{0};                                               //

    while(not payoutFile.eof()){
        
        std::getline(payoutFile, tempString);
        sss<<tempString;
        while(j_counter<MAX_PAYOUT_TAB_SIZ){
            sss>>tempDouble;
            //TODO: CHECK FOR ERROR WITH TRY CATCH
            payoutTable[i_counter][j_counter] = tempDouble;
            j_counter++;
        }
        j_counter = 0;
        i_counter++;
    }
    payoutFile.close();
    
    //--------------  

    std::string filePath;                           // The file path informed by the user in argv.
    if (argc > 1){
        filePath = argv[1];
    }
    else{
        std::cout<<"Missing argument file file path.\n";
        return 1;
    }

    std::cout<<"Preparing to read bet file ["<<filePath<<"], please wait...\n";
    std::cout<<"--------------------------------------------------\n";
    Bet theBet = Bet();                              // Bet object.
    //TODO: More classes.
    
    // Moved to a class:
        //double initialCredit{0};                         // var1 = Initial Credit.
        //int numberOfRounds{0};                           // var2 = Number of Rounds.
        //std::vector<int> setOfNumbers(QUANT_NUMS);       // var 3 = Set of Numbers.
    
    std::cout<<"Class variable: "<<theBet.setOfNumbers[0]<<'\n';
    std::ifstream theFile{ filePath, std::ios::in }; // The File With the 3 Variables (var1, var2, var3).

    constexpr int QUANT_NUMS{15};                    // Max Quantity of the Set of Numbers.
    constexpr int MIN_NUM{1};                        // Minimum Range to Set of Numbers.
    constexpr int MAX_NUM{80};                       // Maximum Range to Set of Numbers.
    
    std::string tempStringToSetOfNumbers{};          // Temporary STRING to Help Parse the Numbers
    int errorStatus{0};                              // Error Status From the Function that Gets the Variables. 0 = Ok. 1 = Error reading the file.

    // Getting the variables from the file.
    errorStatus = auxiliar::getVariablesFromFile(&theFile, &theBet.initialCredit, &theBet.numberOfRounds, &tempStringToSetOfNumbers);
    // Closing the file.
    theFile.close();                                 
    if (errorStatus == 1){
        std::cout<<"Can't read the variables from file.\n";
        return 1;
    }
    
    // Getting and validating the set of numbers.
    std::stringstream ss;                            // Temporary STREAM to Help Parse the Numbers.                 
    int quantNumberChosen{0};                        // Auxiliar Counter for While Loop.
    int tempNum{0};                                  // Temporary Int to Hold the Number for Validation.
    ss<<tempStringToSetOfNumbers;
    
    // Initializing the vector to prevent segmentation fault.
    theBet.setOfNumbers.push_back(0);
    while(ss){
        ss>>tempNum;
        //TODO: CHECK FOR ERROR WITH TRY CATCH.
        // Validating the number.
        if (auxiliar::validNumber(tempNum,theBet.setOfNumbers,QUANT_NUMS,MIN_NUM,MAX_NUM)){
            theBet.setOfNumbers.push_back(tempNum);
            //setOfNumbers[quantNumberChosen] = tempNum;
            quantNumberChosen++;
        }
        if (quantNumberChosen>QUANT_NUMS-1){
            break;
        }
    }

    if (theBet.setOfNumbers.size()>0){
        std::cout<<"Bet successfully read!\n";
    }
    else {
        std::cout<<"Invalid bet!\n";
        return 1;
    }

    std::cout << "You are going to wage a total of $" << theBet.initialCredit << " dollars.\n";
    std::cout << "Going for a total of " << theBet.numberOfRounds << " rounds, waging $"<<theBet.initialCredit/theBet.numberOfRounds<<" per round.\n\n";
    std::cout << "Your bet has "<<3<<" numbers. They are: [ ";
    for ( int n{0}; n<QUANT_NUMS ; n++ ){
        if (theBet.setOfNumbers[n] != 0){
            std::cout <<theBet.setOfNumbers[n] << ' ';
        }
    }
    std::cout << "]\n";

    std::cout << "------+---------\n";
    std::cout << "Hits\t| Payout\n";
    std::cout << "------+---------\n";
    for ( int o{0}; o<=quantNumberChosen; o++ ){
        std::cout <<o<<"\t| "<<payoutTable[quantNumberChosen-1][o]<<'\n';
    }
    std::cout<<"--------------------------------------------------\n";  

    std::vector<int> numberPicker(80);
    for (int z{0};z<80;z++){
        numberPicker[z] = z+1;
    }
    std::vector<int> winnerNumbers(20);
    int counterRightNumbers{0};
    double gainForTheRound{0};
    double actualAmount{theBet.initialCredit};
    double betAmount{theBet.initialCredit/theBet.numberOfRounds};

// ROUNDS
    for (int round(1);round<=theBet.numberOfRounds;round++){
        
        std::cout << "This is round #"<<round<<" of "<<theBet.numberOfRounds<<", and your wage is $"<<betAmount<<". Good luck!\n";
        // Shuffle the numberPicker.
        std::random_shuffle(numberPicker.begin(), numberPicker.end());
        //TODO: Find out why the random numbers are always the same.

        // Copy first 20 shuffled numbers to winnerNumbers.
        for(int k{0}; k < 20; k++) {
            winnerNumbers[k] = numberPicker[k];
        }
        std::cout << "The hits are: [ ";
        for (int z{0};z<QUANT_NUMS;z++){
            std::cout << winnerNumbers[z] << ' ';
        }
        std::cout << "]\n\n";

        // Check how many numbers in setOfNUmbers are in winnerNumbers and save the count to rightNumbers.
        std::cout << "You hit the following number(s) [ ";
        for(int l{0}; l < QUANT_NUMS; l++) {
            if (std::count(winnerNumbers.begin(), winnerNumbers.end(), theBet.setOfNumbers[l])){
                std::cout << theBet.setOfNumbers[l]<<' ';
                counterRightNumbers++;
            }
        }
        std::cout << "], a total of "<<counterRightNumbers<<" hits out of "<<quantNumberChosen<<'\n';
        
        // Calculate the payout.
        gainForTheRound = betAmount * payoutTable[quantNumberChosen-1][counterRightNumbers];
        actualAmount = actualAmount + gainForTheRound;
        //std::cout << "\nYou gained this much in this round: " << gainForTheRound << '\n';
        //std::cout << "You have this in total now: " << actualAmount << '\n';      
        //std::cout << "\n--------------\n";

        for (int m{0};m<20;m++){
            winnerNumbers[m] = 0;
        }

        std::cout << "Payout rate is "<<payoutTable[quantNumberChosen-1][counterRightNumbers]<<", thus you came out with: $"<<gainForTheRound<<'\n';
        std::cout << "Your net balance so far is: $"<<actualAmount<<" dollars.\n";
        std::cout<<"--------------------------------------------------\n";
        counterRightNumbers = 0;
        gainForTheRound = 0;

    }

// ---- Test Printing - Payout Table----
    //std::cout << "Set of numbers as string:\t\"" << tempStringToSetOfNumbers << "\" \n";

    // std::cout << "\n\n\t\t\t\t\t\tPayout Table\n";
    // for (int x{0};x<MAX_PAYOUT_TAB_SIZ;x++){
    //     std::cout<<"#"<<x+1<<":\t";
    //     for (int y{0};y<MAX_PAYOUT_TAB_SIZ;y++){
    //         std::cout<<payoutTable[x][y]<<'\t';
    //     }
    //     std::cout<<'\n';
    // }
    // std::cout<<payoutTable[2][3];

    return 0;
}