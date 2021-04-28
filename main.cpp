#include <iostream>
#include <curses.h>

using namespace std;

const int RODS_NUMBER = 3;
const int MAX_DISK_NUMBER = 8;

int sizeOfArray(int *arr);
void printDiscs(WINDOW * win, int *discsArr, int * winBorders, int rodLeftLower, int baseLevel);
void printTowers(WINDOW * win, int *winBorders, int ARodLeftLower, int BRodLeftLower, int CRodLeftLower);
void hanoiIter(int *discsA, int *discsB, int *discsC, int sourceDisc, WINDOW *win, int *winBorders, int smallestDisc);
void initializeArray(int *arr, int arrLength, int dataLength);
void printAll(WINDOW *win, int *winBorders, int ARodLeftLower, int BRodLeftLower, int CRodLeftLower, int *discsA, int *discsB, int *discsC, int baseLevel);


// initialize array increasing numbers, data ends with -1
void initializeArray(int *arr, int arrLength, int dataLength)
{
    int i = 0;
    while (dataLength>0) {
        arr[i] = i+1;
        i++;
        dataLength--;
    }
    while (i< arrLength) {
        arr[i] = -1;
        i++;
    }
}

// determines size of array whose last element is followed by -1
int sizeOfArray(int *arr)
{
    int elements = 0;
    while (arr[elements] != -1) {
        elements++;
    }
    return elements;
}

void printAll(WINDOW *win, int *winBorders, int ARodLeftLower, int BRodLeftLower, int CRodLeftLower, int *discsA, int *discsB, int *discsC, int baseLevel)
{
    box(win, 0, 0);
    printTowers(win, winBorders, ARodLeftLower, BRodLeftLower, CRodLeftLower);
    printDiscs(win, discsA, winBorders, ARodLeftLower, baseLevel);
    printDiscs(win, discsB, winBorders, BRodLeftLower, baseLevel);
    printDiscs(win, discsC, winBorders, CRodLeftLower, baseLevel);
    wrefresh(win);
    getch();
    werase(win);
}


// discsArr: one "discs array"
void printDiscs(WINDOW * win, int *discsArr, int * winBorders, int rodLeftLower, int baseLevel)
{
    int k, h, discWidth, discPosition;

    k = 0;
    while(discsArr[k] != -1) {
        discWidth = -2*discsArr[k] + 23;
        discPosition = (12 - (discWidth-1)/2) + rodLeftLower;

        char *charsDisc;
        charsDisc = new char [discWidth+1];
        for (h=0; h<discWidth-1; h++) {
            charsDisc[h] = '#';
        }
        charsDisc[discWidth-1] = '\0';
        mvwprintw(win, baseLevel - (k+1), discPosition, charsDisc);
        k++;
        delete[] charsDisc;
    }
}

void hanoiIter(int *discsA, int *discsB, int *discsC, int sourceDisc, WINDOW *win, int *winBorders, int smallestDisc)
{
    int *allDiscs[] = {discsA, discsB, discsC};
    int rodWithSmallestDisc;
    int rightDisc, leftDisc;
    int smallestA, smallestB, smallestC;

    int ARodLeftLower, BRodLeftLower, CRodLeftLower;
    int winWidth = winBorders[1] - winBorders[0] - 1;
    int baseLevel = winBorders[3] - 10; // 10 from the bottom
    ARodLeftLower = winBorders[0] + 5; // 5 from right
    BRodLeftLower = ARodLeftLower + int(0.3 * winWidth); // 30% right to the A rod
    CRodLeftLower = BRodLeftLower + int(0.3 * winWidth); // 30% right to the B rod

    printAll(win, winBorders, ARodLeftLower, BRodLeftLower, CRodLeftLower, discsA, discsB, discsC,baseLevel);

    while (true) {
        if ( sizeOfArray(allDiscs[0]) == 0 ) smallestA = 0;
        else smallestA = allDiscs[0][(sizeOfArray(allDiscs[0]) - 1)];

        if ( sizeOfArray(allDiscs[1]) == 0 ) smallestB = 0;
        else smallestB = allDiscs[1][(sizeOfArray(allDiscs[1]) - 1)];

        if ( sizeOfArray(allDiscs[2]) == 0 ) smallestC = 0;
        else smallestC = allDiscs[2][(sizeOfArray(allDiscs[2]) - 1)];

        if (smallestA > smallestB && smallestA > smallestC) rodWithSmallestDisc = 0;
        else if (smallestB > smallestA && smallestB > smallestC) rodWithSmallestDisc = 1;
        else if (smallestC > smallestA && smallestC > smallestB) rodWithSmallestDisc = 2;

        if (rodWithSmallestDisc == 0) {
            rightDisc = 1;
            leftDisc = 2;
        }
        else if (rodWithSmallestDisc == 1) {
            rightDisc = 2;
            leftDisc = 0;
        }
        else {
            rightDisc = 0;
            leftDisc = 1;
        }

            allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] ) - 1] = -1; //remove smallest element
            allDiscs[leftDisc][sizeOfArray( allDiscs[leftDisc] ) ] = smallestDisc; // add smallest element

            printAll(win, winBorders, ARodLeftLower, BRodLeftLower, CRodLeftLower, discsA, discsB, discsC,baseLevel);

            if ( sizeOfArray( allDiscs[rodWithSmallestDisc] ) == 0 && sizeOfArray( allDiscs[rightDisc] ) == 0) // if rodWithSmallestDisc and right rod are empty
                break;

            else if (sizeOfArray( allDiscs[rodWithSmallestDisc] ) == 0) { // if rod with smallest is empty
                allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] ) ] = allDiscs[rightDisc][sizeOfArray( allDiscs[rightDisc] ) -1 ];
                allDiscs[rightDisc][sizeOfArray( allDiscs[rightDisc] ) -1 ]  = -1;
            }

            else if (allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] ) -1 ] > allDiscs[rightDisc][sizeOfArray( allDiscs[rightDisc] ) -1 ] && sizeOfArray( allDiscs[rightDisc] ) != 0 )  {   // rodWithSmallestDisc has smaller disc
                allDiscs[rightDisc][sizeOfArray( allDiscs[rightDisc] )] = allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] ) -1 ];
                allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] ) -1 ] = -1;
            }

            else if ( sizeOfArray( allDiscs[rightDisc] ) == 0 ) { // rod on the right is empty
                allDiscs[rightDisc][sizeOfArray( allDiscs[rightDisc] )] = allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] ) -1 ];
                allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] ) -1 ] = -1;
            }
            else if (  allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] ) -1 ] < allDiscs[rightDisc][sizeOfArray( allDiscs[rightDisc] )-1] )  { // rodWithSmallestDisc has bigger disk
                allDiscs[rodWithSmallestDisc][sizeOfArray( allDiscs[rodWithSmallestDisc] )] = allDiscs[rightDisc][sizeOfArray( allDiscs[rightDisc] )-1];
                allDiscs[rightDisc][sizeOfArray( allDiscs[rightDisc] )-1] = -1;
            }

        printAll(win, winBorders, ARodLeftLower, BRodLeftLower, CRodLeftLower, discsA, discsB, discsC,baseLevel);


    }

    printAll(win, winBorders, ARodLeftLower, BRodLeftLower, CRodLeftLower, discsA, discsB, discsC,baseLevel);

}


// WINDOWS * win: pointer to a window object
// discsX: takes an array corresponding to the sequence of discs on rod X; let's assume discsX ends with "-1" when no longer discs
// winBorders: 4-element array, left-right-upper-lower
void printTowers(WINDOW * win, int *winBorders, int ARodLeftLower, int BRodLeftLower, int CRodLeftLower)
{
    int baseLevel = winBorders[3] - 10; // 10 from the bottom

    mvwprintw(win, baseLevel, ARodLeftLower, "-------------------------"); //25 -
    mvwprintw(win, baseLevel, BRodLeftLower, "-------------------------");
    mvwprintw(win, baseLevel, CRodLeftLower, "-------------------------");
}


int main(){

    int discsA[MAX_DISK_NUMBER];
    int discsB[MAX_DISK_NUMBER];
    int discsC[MAX_DISK_NUMBER];
    int *allDiscs[] = {discsA, discsB, discsC};

    int leftBorder, rightBorder, upperBorder, lowerBorder, numberOfDiscs, rodNumber;

    do {
    cout << "Please provide the total number of discs [3-" << MAX_DISK_NUMBER << "]: ";
    cin >> numberOfDiscs;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        }
    } while (numberOfDiscs < 3 || numberOfDiscs > 8 || cin.fail());

    do {
    cout << "Please provide the starting rod number [1-3]: ";
    cin >> rodNumber;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        }
    } while (rodNumber < 1 || rodNumber > 3 || cin.fail());

    initializeArray(allDiscs[rodNumber -1 ], MAX_DISK_NUMBER, numberOfDiscs);
    initializeArray(allDiscs[(rodNumber -1 + 1 + 3) % 3], MAX_DISK_NUMBER, 0);
    initializeArray(allDiscs[(rodNumber -1 -1 + 3) % 3], MAX_DISK_NUMBER, 0);

    initscr(); // initialize screen, reserve memory
    cbreak(); // disable character buffering (character-at-a-time)
    noecho(); // stop keys echoing

    leftBorder = 1;
    rightBorder = COLS-2; // can only use COLS and LINES after invoking initscr()
    upperBorder = 1;
    lowerBorder = LINES - 2;
    int winBorders[] = {leftBorder, rightBorder, upperBorder, lowerBorder}; // left-right-upper-lower

    WINDOW * win = newwin(lowerBorder, rightBorder, upperBorder, leftBorder); // create a pointer to a window object
    refresh(); // refresh the whole screen

    box(win, 0, 0); // create a box around a window

    hanoiIter(discsA, discsB, discsC, rodNumber-1, win, winBorders, numberOfDiscs);

    wrefresh(win);
    getch();

    endwin(); // restore terminal settings

    return EXIT_SUCCESS;
}
