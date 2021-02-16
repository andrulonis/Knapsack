#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>

int max(int a, int b){
    return (a > b) ? a : b;
}

int storeKnapsack(std::vector<int> sackValues, std::vector<int> sackWeights, std::vector<bool> &isInSack, int capacity, int numOfItem){
    int maxSackValue, includeItem, excludeItem;
    std::vector<bool> sackInclude = isInSack, sackExclude = isInSack;

    if (capacity == 0 || numOfItem == sackWeights.size()) {
        return 0;
    }

    if (sackWeights.at(numOfItem) > capacity) {
        return storeKnapsack(sackValues, sackWeights, isInSack, capacity, numOfItem + 1);
    }
    else {
        sackInclude.at(numOfItem) = true;
        includeItem = sackValues.at(numOfItem) + storeKnapsack(sackValues, sackWeights, sackInclude, capacity - sackWeights.at(numOfItem), numOfItem + 1);
        excludeItem = storeKnapsack(sackValues, sackWeights, sackExclude, capacity, numOfItem + 1);
        
        maxSackValue = max(includeItem, excludeItem);

        if (includeItem > excludeItem) {
            isInSack = sackInclude;
        }
        else {
            isInSack = sackExclude;
        }
        return maxSackValue;
    }
}

void sumSack(int &maxWeight, int &maxValue, std::vector<bool> isInSack, std::vector<int> sackValues, std::vector<int> sackWeights) {
    for (int i = 0; i < isInSack.size(); i++) {
        if (isInSack.at(i) == true) {
            maxWeight += sackWeights.at(i);
            maxValue += sackValues.at(i);
        }
    }
}

void printContents(int capacity, int maxValue, int maxWeight, std::vector<int> sackWeights, std::vector<int> sackValues, std::vector<bool> isInSack){
    std::cout << "With a capacity of " << capacity << " kg, the total value is " << maxValue;
    std::cout << " Eur, leading to a total weight of " << maxWeight << " kg" << std::endl;
    std::cout << "The following items are included:";

    for (int i = 0; i < sackWeights.size(); i++) {
        if (isInSack.at(i) == true) {
            std::cout << " [" << sackWeights.at(i) << "," << sackValues.at(i) << "]";
        }
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        std::ifstream inFS;
        std::vector<int> sackWeights, sackValues;
        int capacity, maxValue = 0, maxWeight = 0, input1, input2;
        
        if (argc == 1) {
            throw std::runtime_error("No input filename given");
        }

        inFS.open(argv[1]);

        if (!inFS.is_open()) {
            throw std::runtime_error("Could not open file " + static_cast<std::string>(argv[1]));
        }

        inFS >> capacity;

        while(!inFS.eof()) {
            inFS >> input1;
            inFS >> input2;
            if (!inFS.fail()) {
                sackWeights.push_back(input1);
                sackValues.push_back(input2);
            }
        }

        std::vector<bool> isInSack(sackWeights.size());

        storeKnapsack(sackValues, sackWeights, isInSack, capacity, 0);

        sumSack(maxWeight, maxValue, isInSack, sackValues, sackWeights);

        printContents(capacity, maxValue, maxWeight, sackWeights, sackValues, isInSack);

        inFS.close();
    }
    catch (std::runtime_error& excpt) {
        std::cout << excpt.what() << std::endl;
    }
    return 0;
}