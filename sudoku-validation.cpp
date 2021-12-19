//jazz sajan
#include <memory>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

// validationRow function
void validationRow(int lower, int upper, unique_ptr<int []> &ptr1, unique_ptr<int> &invalid, string puzzleNum){
    unique_ptr<int []> tempArr(new int[9]());
    int row = upper / 9 + 1; // get row number
    
    for(int i = lower; i <= upper; i++) {
        int index = ptr1.get()[i];  // get element from ptr1 at index i
        if (index != 0)     // not a space/0
            tempArr.get()[index-1]+=1;  // increment element from tempArr at index-1
    }
    for(int k = 0; k <= 8; k++) {
        if(tempArr.get()[k] > 1) {
            if(*invalid.get() != 1) {
                *invalid = 1;
            }
            cout << puzzleNum << "\t" << "invalid" << "\t" << "row " << row << " has multiple " << k+1 << "s" << endl;
        }
    }
    tempArr = nullptr; //reset tempArr to all zeros
}

// validationCol function
void validationCol(unique_ptr<int []> &ptr2, unique_ptr<int> &invalid, string puzzleNum) {
    unique_ptr<int []> tempArr(new int[9]());
    for(int i = 0; i < 9; i++) {
        for(int j = i; j < 81; j+=9) {
            int index = ptr2.get()[j];  // get element from ptr2 at index j
            if (index != 0)
                tempArr.get()[index-1] += 1;    // increment element from tempArr at index-1
        }
        
        for(int k = 0; k <= 8; k++){
            if(tempArr.get()[k] > 1) {
                if(*invalid.get() != 1) {
                    *invalid = 1;
                }
                cout << puzzleNum << "\t" << "invalid" << "\t" << "column " << i+1 << " has multiple " << k+1 << "s" << endl;
            }
            
           tempArr.get()[k] = 0;
        }
    }
    tempArr = nullptr;  // reset tempArr to all zeros
}

// validationSec function
// startRow and startCol is upper corner
void validationSec(int startRow, int startCol, unique_ptr<int []> &ptr4, unique_ptr<int> &invalid, string section, string puzzleNum) {
    int row = startRow+1;
    int col = startCol;
    unique_ptr<int []> tempArr(new int[9]());
    // loop through all numbers in a section
    for(int n = 0; n < 9; n++) {
        if (row <= startRow+3) {    // read max 3 rows
            int index = ptr4.get()[row*9-9+col];
            if (index != 0)
                tempArr.get()[index-1] += 1;
            col++;

            if(col == startCol+3) { // read max 3 cols
              row += 1; // change row
              col = startCol; // reset col to zero
            }
        }
    }

    for(int k = 0; k <= 8; k++)
    {
        if(tempArr.get()[k] > 1) {
            if(*invalid.get() != 1) {
                *invalid = 1;
            }
            cout << puzzleNum << "\t" << "invalid" << "\t" << section << " has multiple " << k+1 << "s" << endl;
        }
    }
}

// validationSecHelper function
void validationSecHelper(unique_ptr<int []> &ptr3, unique_ptr<int> &invalid, string puzzleNum) {
    // call validationSec function with upper corners numbers of each sections
    validationSec(0, 0, ptr3, invalid, "upper left", puzzleNum);
    validationSec(0, 3, ptr3, invalid, "upper middle", puzzleNum);
    validationSec(0, 6, ptr3, invalid, "upper right", puzzleNum);
    validationSec(3, 0, ptr3, invalid, "left", puzzleNum);
    validationSec(3, 3, ptr3, invalid, "middle", puzzleNum);
    validationSec(3, 6, ptr3, invalid, "right", puzzleNum);
    validationSec(6, 0, ptr3, invalid, "lower left", puzzleNum);
    validationSec(6, 3, ptr3, invalid, "lower middle", puzzleNum);
    validationSec(6, 6, ptr3, invalid, "lower right", puzzleNum);
}

int main()
{
    unique_ptr<int []> ptr(new int[81]);
    
    string line;
    string puzzleNum;
    string filename;
    string blank = "";
    
    unique_ptr<int> invalid(new int(0)); // points to an int, 1 if invalid, 0 if valid
    bool space = false;
    ifstream file;
    cin >> filename; //prompt user for file name
    file.open(filename, ios::in | ios:: binary); // open file
    
    if(file.is_open()) { // check if file open
        while(!file.eof()) // until end of line
        {
            int k = 0; // index of ptr 0-80
            getline(file, puzzleNum); // get puzzle number

            for (int i=0; i<=8; i++) // rows
            {
                getline(file, line); // get line numbers
                for (int j=0; j<=8; j++) // columns
                {
                    // change spaces to 0
                    if (line[j] == ' ')
                    {
                        space = true;
                        line[j] = '0';
                    }
                    string num(1, line[j]);   // convert char to string
                    ptr.get()[k] = atoi(num.c_str());     // convert string to const char* to int
                    k++;
                }
                validationRow(k-9, k-1, ptr, invalid, puzzleNum); // call validationRow function
            }
            validationCol(ptr, invalid, puzzleNum); // call validationCol function
            validationSecHelper(ptr, invalid, puzzleNum); // call validationSecHelper function
            getline(file, blank); //get blank line between puzzles
            
            if (space && *invalid.get() != 1) // check if puzzle contains space and is not invalid
                cout << puzzleNum << "\t" << "valid" << endl;
            else if (*invalid.get() != 1) { // puzzle does not contain space
                cout << puzzleNum << "\t" << "solved" << endl;
            }
            *invalid = 0; // reset invalid to 0
            space = false; // reset bool space
        }
    }
    file.close(); // close file
}
