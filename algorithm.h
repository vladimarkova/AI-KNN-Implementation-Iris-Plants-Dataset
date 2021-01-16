#include <iostream>
#include <fstream>
#include <cstdio>
#include "Vector.h"
using namespace std;

const int ALL_ENTRIES_NUMBER = 150;
const int TEST_DATASET_SIZE = 20;
const int TRAIN_DATASET_SIZE = ALL_ENTRIES_NUMBER - TEST_DATASET_SIZE;
const int ATTRIBUTES_NUMBER = 4; // not including the class attribute
const int NUMBER_OF_CLASSES = 3; 

enum DatasetType {
    Iris_Setosa, Iris_Versisolor, Iris_Virginica
};

struct Entry {
    Vector<double> features;
    char class_type;

    friend ostream& operator<<(ostream& os, const Entry& entry) {
        // entry.features.print();
        for (int i = 0; i < entry.features.size(); i++) {
            printf("%.1f", entry.features[i]);
            cout << " ";
        }
        os << entry.class_type << endl << endl;

        return os;
    }
};

void read_data(Vector<Entry>& all_entries) {

    ifstream infile("iris.txt");

    if (infile.is_open())
    {
        string line;
        while (getline(infile, line))
        {
            Entry entry;
            Vector<double> numbers;
            for (int i = 0; i < ATTRIBUTES_NUMBER; i++) {
                numbers.push_back(0);
            }
            
            int power = 1;
            for (int i = 0; i < 3; i++)
            {
                if (i != 1)
                {
                    numbers[0] += (line[i] - '0') * pow(10, power);
                    power--;
                }
            }

            power = 1;
            for (int i = 4; i < 7; i++)
            {
                if (i != 5)
                {
                    numbers[1] += (line[i] - '0') * pow(10, power);
                    power--;
                }
            }

            power = 1;
            for (int i = 8; i < 11; i++)
            {
                if (i != 9)
                {
                    numbers[2] += (line[i] - '0') * pow(10, power);
                    power--;
                }
            }

            power = 1;
            for (int i = 12; i < 15; i++)
            {
                if (i != 13)
                {
                    numbers[3] += (line[i] - '0') * pow(10, power);
                    power--;
                }
            }

            for (int i = 0; i < ATTRIBUTES_NUMBER; i++) {
                entry.features.push_back(numbers[i] / double(10));
            }
            entry.class_type = line[16];
            cout << entry;
            all_entries.push_back(entry);
        }
    }
    infile.close();
}

char classify_single_entry() {
    return 'S';
}

void print_result_of_single_entry_classification() {
    
}

void isolated_tests() {
    Vector<Entry> all_entries;
    read_data(all_entries); 
    // all_entries.print();
}

void KNN() {

}

