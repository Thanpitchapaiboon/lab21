#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <cmath> 

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please input numbers to find average." << endl;
        return 1;
    }

    vector<double> numbers;
    for (int i = 1; i < argc; ++i) {
        numbers.push_back(atof(argv[i]));
    }

    double sum = 0;
    for (double num : numbers) {
        sum += num;
    }

    double average = sum / numbers.size();
    
    cout << "---------------------------------" << endl;
    cout << "Average of " << numbers.size() << " numbers = ";
    
    if (floor(average) == average) {
        cout << static_cast<int>(average) << endl; 
    } else {
        cout << fixed << setprecision(3) << average << endl; 
    }

    cout << "---------------------------------" << endl;

    return 0;
}
