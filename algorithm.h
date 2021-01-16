#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>
using namespace std;

const int ALL_ENTRIES_NUMBER = 150;
const int DATASETS_NUMBER = 10;
const int NUMBER_OF_CLASSIFICATIONS = DATASETS_NUMBER;
const int TEST_DATASET_SIZE = ALL_ENTRIES_NUMBER / DATASETS_NUMBER;
const int TRAINING_DATASET_SIZE = ALL_ENTRIES_NUMBER - TEST_DATASET_SIZE;
const int ATTRIBUTES_NUMBER = 4; // not including the class attribute
const int NUMBER_OF_CLASSES = 3; 
const int DEFFAULT_VALUE_OF_K = sqrt(ALL_ENTRIES_NUMBER); 

struct Entry {
    double features[ATTRIBUTES_NUMBER];
    char class_type;

    friend ostream& operator<<(ostream& os, const Entry& entry) {
        for (int i = 0; i < ATTRIBUTES_NUMBER; i++) {
            printf("%.1f", entry.features[i]);
            cout << " ";
        }
        os << entry.class_type << endl;

        return os;
    }
};

struct Dataset {
    Entry entries[ALL_ENTRIES_NUMBER];
    int size = 0;

    void add(const Entry& entry) {
        entries[size] = entry;
        size++;
    }

    void print() const {
        cout << "NUMBER OF ENTRIES: " << size << endl;
        for (int i = 0; i < size; i++) {
            cout << entries[i];
        }
        cout << endl << endl;
    } 
};

void read_data(Entry all_entries[]) {
    ifstream infile("iris.txt");
    int number_of_entries = 0;

    if (infile.is_open())
    {
        string line;
        while (getline(infile, line))
        {
            Entry entry;
            double numbers[ATTRIBUTES_NUMBER];
            for (int i = 0; i < ATTRIBUTES_NUMBER; i++) {
                numbers[i] = 0;
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
                entry.features[i] = (numbers[i] / double(10));
            }
            entry.class_type = line[16];
            all_entries[number_of_entries] = entry;
            number_of_entries++;
        }
    }
    infile.close();
}

void choice_of_test_set(int& index_of_the_testDtset) {
    cout << "Enter a number of the test set (a number from 0 to 9, incl.): " << endl
         << endl;
    cin >> index_of_the_testDtset;
    while (index_of_the_testDtset < 0 || index_of_the_testDtset > 9)
    {
        cout << "Enter a number from 0 to 9, incl." << endl
             << endl;
        cin >> index_of_the_testDtset;
    }
    cout << endl
         << endl;
}

void choice_of_K(int& K) {
    cout << "Enter number of nearest neighbours for the algorithm to work with(K): " << endl << endl;
    cin >> K;
    while (K <= 0) {
        cout << "Enter a number greater than zero: " << endl;
        cin >> K;
    }
    cout << endl;
}

void arrange_into_datasets(Entry all_entries[], Dataset set_of_datasets[]) {
    random_shuffle(all_entries, all_entries + ALL_ENTRIES_NUMBER);

    for (int i = 0; i < DATASETS_NUMBER; i++) {
        for (int j = 0; j < TEST_DATASET_SIZE; j++) {
            set_of_datasets[i].add(all_entries[i * TEST_DATASET_SIZE + j]);
        }
    }
}

Dataset unite_all_datasets_except(int idx_of_the_test_dataset, Dataset set_of_datasets[]) {
    Dataset training_dataset;

    for (int i = 0; i < DATASETS_NUMBER; i++) {
        if (i != idx_of_the_test_dataset) {
            for (int j = 0; j < set_of_datasets[i].size; j++) {
                training_dataset.add(set_of_datasets[i].entries[j]);
            }
        }
    }

    return training_dataset;
}

void print_set_of_datasets(const Dataset set_of_datasets[], int datasets_number) {
    for (int i = 0; i < datasets_number; i++) {
        cout << "DATASET " << i << ": " << endl << endl;
        set_of_datasets[i].print();
    }
}

void isolated_tests() {
    int K = DEFFAULT_VALUE_OF_K;

    choice_of_K(K);

    Entry all_entries[ALL_ENTRIES_NUMBER];
    read_data(all_entries);

    Dataset set_of_datasets[DATASETS_NUMBER];
    arrange_into_datasets(all_entries, set_of_datasets);
    // print_set_of_datasets(set_of_datasets, DATASETS_NUMBER);
    int index_of_the_testDtset = 0;
    choice_of_test_set(index_of_the_testDtset);
    Dataset training_dataset = unite_all_datasets_except(index_of_the_testDtset, set_of_datasets);
    Dataset test_dataset = set_of_datasets[index_of_the_testDtset];
    cout << "NOW PRINTING TRAINING DATASET: " << endl << endl;
    training_dataset.print();
    cout << "NOW PRINTING TEST DATASET: " << endl << endl;
    test_dataset.print();
}

void KNN() {
    int index_of_the_testDtset = 0;
    int number_of_learnings_counter = 0;
    int K = DEFFAULT_VALUE_OF_K;
    int accuracy_index = number_of_learnings_counter;
    double accuracies[DATASETS_NUMBER];
    double average_accuracy = 0;
    char response = '1';

    Entry all_entries[ALL_ENTRIES_NUMBER];
    Dataset set_of_datasets[DATASETS_NUMBER];

    choice_of_K(K);

    read_data(all_entries);
    arrange_into_datasets(all_entries, set_of_datasets);

    while (number_of_learnings_counter < NUMBER_OF_CLASSIFICATIONS && response != '0') {
        choice_of_test_set(index_of_the_testDtset);

        Dataset training_dataset = unite_all_datasets_except(index_of_the_testDtset, set_of_datasets);
        Dataset test_dataset = set_of_datasets[index_of_the_testDtset];
        
        char classes_of_entries[TEST_DATASET_SIZE + 1] = { '\0' };
        // classify_test_dataset(test_dataset, classes_of_entries);

        cout << "CURRENT TEST WITH DATASET " << index_of_the_testDtset << " AND K = " << K << endl;
        // print_classification(classes_of_entries);

        // accuracies[accuracy_index] = calculate_singleTest_accuracy(classes_of_entries, test_dataset);
        cout << "ACCURACY FOR TEST WITH DATASET " << index_of_the_testDtset << ": " << accuracies[accuracy_index] * 100 << '%' << endl << endl;
        accuracy_index++;

        number_of_learnings_counter++;

        cout << endl << endl;

        if (number_of_learnings_counter != NUMBER_OF_CLASSIFICATIONS) {
            cout << "Enter 1 to start a new classification or 0 to stop: " << endl;
            cin >> response;
        }
    }

    for (int i = 0; i < DATASETS_NUMBER; i++) {
        average_accuracy += accuracies[i];
    }
    average_accuracy /= number_of_learnings_counter;

    cout << "IN GENERAL: " << number_of_learnings_counter << " CLASSIFICATIONS MADE." << " K = " << K << endl;
    cout << "THE AVERAGE ACCURACY IS: " << average_accuracy * 100 << '%' << endl << endl;
}