#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
using namespace std;

const int ALL_ENTRIES_NUMBER = 150;
const int DATASETS_NUMBER = 10;
const int NUMBER_OF_CLASSIFICATIONS = DATASETS_NUMBER;
const int TEST_DATASET_SIZE = ALL_ENTRIES_NUMBER / DATASETS_NUMBER;
const int TRAINING_DATASET_SIZE = ALL_ENTRIES_NUMBER - TEST_DATASET_SIZE;
const int ATTRIBUTES_NUMBER = 4; // not including the class attribute
const int NUMBER_OF_CLASSES = 3; 
const int DEFFAULT_VALUE_OF_K = sqrt(ALL_ENTRIES_NUMBER); 
const int INF = 1000000;

struct Entry {
    double features[ATTRIBUTES_NUMBER];
    char class_type;

    double calculate_distance_with(const Entry& other) const {
        double s_sum = 0;
        for (int i = 0; i < ATTRIBUTES_NUMBER; i++) {
            double s_diff = (features[i] - other.features[i]) * (features[i] - other.features[i]);
            s_sum += s_diff;
        }
        double distance = sqrt(s_sum);
        return distance;
    }

    bool same_with(const Entry& other) const {
        for (int i = 0; i < ATTRIBUTES_NUMBER; i++) {
            if (features[i] != other.features[i]) {
                return false;
            }
        }
        return true;
    }

    void print_without_class() const {
        for (int i = 0; i < ATTRIBUTES_NUMBER; i++) {
            printf("%.1f", features[i]);
            cout << " ";
        }
    }

    friend ostream& operator<<(ostream& os, const Entry& entry) {
        for (int i = 0; i < ATTRIBUTES_NUMBER; i++) {
            printf("%.1f", entry.features[i]);
            cout << " ";
        }
        os << entry.class_type << endl;

        return os;
    }
};

Entry test_entry;

bool compare_entries(const Entry& entry1, const Entry& entry2) {
    double distance1 = entry1.calculate_distance_with(test_entry);
    double distance2 = entry2.calculate_distance_with(test_entry);
    return distance1 <= distance2;
}

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

void print_classification(const Dataset& test_dataset, char classes_of_entries[]) {
    for (int i = 0; i < test_dataset.size; i++) {
        if (i <= 9) {
            cout << "ENTRY " << i << ":  ";
        } 
        else {
            cout << "ENTRY " << i << ": ";
        }

        test_dataset.entries[i].print_without_class();

        char class_t[30];

        if (classes_of_entries[i] == 'S') {
            strcpy(class_t, "Iris_Setosa");
        }

        if (classes_of_entries[i] == 'C') {
            strcpy(class_t, "Iris_Versicolor");
        }

        if (classes_of_entries[i] == 'V') {
            strcpy(class_t, "Iris_Virginica");
        }

        cout << "CLASSIFIED AS: " << class_t << endl;
    }
    cout << endl;
}

int count_class_in_a_dataset_with_size_K(char class_type, const Dataset& dataset, int K) {
    int count = 0;
    for (int i = 0; i < K; i++) {
        if (dataset.entries[i].class_type == class_type) {
            count++;
        }
    }
    return count;
}

char classify_single_entry(const Entry& entry, const Dataset& training_dataset, int K) {
    test_entry = entry; // very important row, actually defines the result from sort of the training set
    int training_dataset_size = training_dataset.size;
    sort(training_dataset.entries, training_dataset.entries + training_dataset_size, compare_entries);

    int class_S_number = count_class_in_a_dataset_with_size_K('S', training_dataset, K);
    int class_C_number = count_class_in_a_dataset_with_size_K('C', training_dataset, K);
    int class_V_number = count_class_in_a_dataset_with_size_K('V', training_dataset, K);

    if (class_S_number == class_C_number && class_S_number > class_V_number) {
        double class_S_best_distance = 0;
        double class_C_best_distance = 0;
        for (int i = 0; i < K; i++) {
            if (training_dataset.entries[i].class_type == 'S') {
                class_S_best_distance = training_dataset.entries[i].calculate_distance_with(test_entry);
                break;
            }
        }
        for (int i = 0; i < K; i++) {
            if (training_dataset.entries[i].class_type == 'C') {
                class_C_best_distance = training_dataset.entries[i].calculate_distance_with(test_entry);
                break;
            }
        }
        if (class_S_best_distance < class_C_best_distance) {
            return 'S';
        } 
        else {
            return 'C';
        }
    }

    if (class_C_number == class_V_number && class_C_number > class_S_number) {
        double class_C_best_distance = 0;
        double class_V_best_distance = 0;
        for (int i = 0; i < K; i++) {
            if (training_dataset.entries[i].class_type == 'C') {
                class_C_best_distance = training_dataset.entries[i].calculate_distance_with(test_entry);
                break;
            }
        }
        for (int i = 0; i < K; i++) {
            if (training_dataset.entries[i].class_type == 'V') {
                class_V_best_distance = training_dataset.entries[i].calculate_distance_with(test_entry);
                break;
            }
        }
        if (class_C_best_distance < class_V_best_distance) {
            return 'C';
        } 
        else {
            return 'V';
        }
    }

    if (class_S_number == class_V_number && class_S_number > class_C_number) {
        double class_S_best_distance = 0;
        double class_V_best_distance = 0;
        for (int i = 0; i < K; i++) {
            if (training_dataset.entries[i].class_type == 'S') {
                class_S_best_distance = training_dataset.entries[i].calculate_distance_with(test_entry);
                break;
            }
        }
        for (int i = 0; i < K; i++) {
            if (training_dataset.entries[i].class_type == 'V') {
                class_V_best_distance = training_dataset.entries[i].calculate_distance_with(test_entry);
                break;
            }
        }
        if (class_S_best_distance < class_V_best_distance) {
            return 'S';
        } 
        else {
            return 'V';
        }
    }

    int max_number = -INF;
    char best_class_type;

    if (class_S_number > max_number) {
        max_number = class_S_number;
        best_class_type = 'S';
    }

    if (class_C_number > max_number) {
        max_number = class_C_number;
        best_class_type = 'C';
    }

    if (class_V_number > max_number) {
        max_number = class_V_number;
        best_class_type = 'V';
    }

    return best_class_type;
    
    return 'u';
}

void classify_test_dataset(const Dataset& test_dataset, const Dataset& training_dataset, int K, char classes_of_entries[]) {
    for (int i = 0; i < test_dataset.size; i++) {
        char entry_class = classify_single_entry(test_dataset.entries[i], training_dataset, K);
        classes_of_entries[i] = entry_class;
    }
}

double calculate_single_test_accuracy(char classes_of_entries[], const Dataset& test_dataset) {
    double properly_classified = 0;

    for (int i = 0; i < test_dataset.size; i++) {
        if (test_dataset.entries[i].class_type == classes_of_entries[i]) {
            properly_classified++;
        }
    }

    return properly_classified / test_dataset.size;
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

    test_entry = test_dataset.entries[0];
    int dataset_size = training_dataset.size;

    // sort(training_dataset.entries, training_dataset.entries + dataset_size, compare_entries);

    cout << "NOW PRINTING SORTED TRAINING DATASET: " << endl << endl;

    training_dataset.print();

    // cout << "CLASSIFICTAION RESULT: " << classify_single_entry(test_dataset.entries[0], training_dataset, 12);
}

void KNN() {
    int index_of_the_testDtset = 0;
    int number_of_classifications_counter = 0;
    int K = DEFFAULT_VALUE_OF_K;
    int accuracy_index = number_of_classifications_counter;
    double accuracies[DATASETS_NUMBER];
    double average_accuracy = 0;
    char response = '1';

    Entry all_entries[ALL_ENTRIES_NUMBER];
    Dataset set_of_datasets[DATASETS_NUMBER];

    choice_of_K(K);

    read_data(all_entries);
    arrange_into_datasets(all_entries, set_of_datasets);

    while (number_of_classifications_counter < NUMBER_OF_CLASSIFICATIONS && response != '0') {
        choice_of_test_set(index_of_the_testDtset);

        Dataset training_dataset = unite_all_datasets_except(index_of_the_testDtset, set_of_datasets);
        Dataset test_dataset = set_of_datasets[index_of_the_testDtset];
        
        char classes_of_entries[TEST_DATASET_SIZE + 1] = { '\0' };
        classify_test_dataset(test_dataset, training_dataset, K, classes_of_entries);

        cout << "CURRENT TEST WITH DATASET " << index_of_the_testDtset << " AND K = " << K << endl;
        print_classification(test_dataset, classes_of_entries);

        accuracies[accuracy_index] = calculate_single_test_accuracy(classes_of_entries, test_dataset);
        cout << "ACCURACY FOR TEST WITH DATASET " << index_of_the_testDtset << ": " << accuracies[accuracy_index] * 100 << '%' << endl << endl;
        accuracy_index++;

        number_of_classifications_counter++;

        cout << endl << endl;

        if (number_of_classifications_counter != NUMBER_OF_CLASSIFICATIONS) {
            cout << "Enter 1 to start a new classification or 0 to stop: " << endl;
            cin >> response;
        }
    }

    for (int i = 0; i < DATASETS_NUMBER; i++) {
        average_accuracy += accuracies[i];
    }
    average_accuracy /= number_of_classifications_counter;

    cout << "IN GENERAL: " << number_of_classifications_counter << " CLASSIFICATIONS MADE." << " K = " << K << endl;
    cout << "THE AVERAGE ACCURACY IS: " << average_accuracy * 100 << '%' << endl << endl;
}