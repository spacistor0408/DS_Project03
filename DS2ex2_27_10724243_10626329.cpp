// 電機四乙 10626329 袁倫御
// 商設三 10724243 石顥澐

#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <time.h>
#include <math.h>
#include <stack>

using namespace std;

#define COLUMNS 6
#define MAX_LEN 10
#define BIG_INT 255

typedef struct sT {

    char sid[MAX_LEN] ;
    char sname[MAX_LEN] ;
    unsigned char score[COLUMNS] ;
    float mean ;

} studentType ;

typedef struct HT {

    BinD info ;
    int hashvalue = -1 ;

} HashTable ;

class DM { // Data manage and In/Output

    vector< studentType > orinList;
    string fname ;

    void ReadFileName() {
        fname.clear() ;
        cin >> fname ;
    } // ReadFileName()

public:

    string GetFileName() {
        return fname ;
    } // GetFileName()

    bool Text2Binary() {
        fstream inFile, outFile ;
        int stNo = 0 ;

        /*---------- deal with File ----------*/


        inFile.open( ("input" + fname + ".txt").c_str(), fstream::in ) ; // open txt file

            // txt to bin


		/*---------- deal with data ----------*/
        if ( inFile.is_open() ) {
            fname = "input" + fname + ".bin" ;
            outFile.open( fname.c_str(), fstream::out | fstream::binary ) ;
            if ( outFile.is_open() ) {

                char rBuf[BIG_INT] ;

                while ( inFile.getline(rBuf,BIG_INT,'\n') ) { // retrieve one entire record from file

                    string temp ;
                    studentType oneSt ;
                    int cNo = 0, pre = 0, pos = 0 ;

                    stNo++ ;
                    temp.assign(rBuf) ;
                    pos = temp.find_first_of('\t',pre) ;

                    while( pos!= string::npos ) {
                        switch( ++cNo ) {
                            case 1:
                                strcpy(oneSt.sid, temp.substr(pre, pos-pre).c_str()) ;
                                break ;
                            case 2:
                                strcpy(oneSt.sname, temp.substr(pre, pos-pre).c_str()) ;
                                break ;
                            default:
                                oneSt.score[cNo-3] = atoi(temp.substr(pre, pos-pre).c_str()) ;
                                break ;

                        } // switch
                        pre = ++pos ;
                        pos = temp.find_first_of('\t', pre) ;
                    } // while
                    pos = temp.find_last_of('\t') ;
                    oneSt.mean = atof(temp.substr(pos+1).c_str()) ;
                    outFile.write( (char*)&oneSt, sizeof(oneSt)) ;
                } // while
                outFile.close() ;

            } // if
            inFile.close() ;
            return true ;
        } // if

        return false ;

    } // Text2Binary()

    bool ReadBin() {

        fstream binFile ;
        studentType oneSt ;
        int stNo = 0 ;

        binFile.open( ("input" + fname + ".bin" ).c_str(), fstream::in | fstream::binary ) ;

        if ( binFile.is_open() ) {

            binFile.seekg(0,binFile.end) ; // Goto the end of file
            stNo = binFile.tellg() / sizeof(oneSt) ;
            binFile.seekg(0,binFile.beg) ;

            for ( int i = 0 ;i<stNo ; i++ ) {
               binFile.read( (char*)&oneSt, sizeof(oneSt) ) ;
               orinList.push_back(oneSt) ;
               cout << "[" << i+1 << "]" << oneSt.sname << endl ;
            } // for

        } // if
        else {
            return false ;
        }
        binFile.close() ;
        return true ;

    } // ReadBin()

    bool ReadFile() {

        orinList.clear() ;

        /*---------- deal with File ----------*/
        do {
       		cout << "Input a file number (301, 302, ...[0]Quit): " ;
     		ReadFileName() ;

        	if ( fname == "0" ) {
        		return false ;
			} // Quit to open file

			cout << endl ;

            if ( ReadBin() ) {
                break ;
            }
            else if( Text2Binary() ) {
                ReadBin() ;
                break ;
            }
        	else {
            	cout << "Error:Open file fail" << endl << endl ;
			} // if Error

		} while ( true ) ;

        return true ;

    } // ReadFile()

}; // Class FileIO

class LinearHash {

    int size ;

    void SetSize() { //hash table size

        //cout << temp << endl;

        while (true) {
            int remainder ;
            bool not_Prime_num = false ;

            for (int i = 2 ; i < size ; i++) {
                remainder = size % i;
                if (remainder == 0) {
                    not_Prime_num = true ;
                    break ;
                } // if
            } // for

            if (not_Prime_num)
                size++;
            else
                break ;

        } // while

    } // searchPrime()

public:

    Hash( int hSize ){ size = (hSize*1.2) ; SetSize() ; HashTable[size] ; }

    void MissionOne() {



    } // MissionOne()

    int HashKey(int sid, int dsize) {

        int total = 0;
        float counter = 0;

        while(sid!=0) {
            counter ++;
            int i = sid % 10 + '0';
            sid = sid / 10;

            if ( counter == 1) {
                total = i;
            }
            else {
                total = total * i;

                while(total > dsize) {
                    total = total % dsize;
                }
            }
        } // while
        return total%size ;

    } // HashKey()

}; // DoubleHash()

void DisplayMission() {
    cout << endl << "********    HASH TABLE   ********" ;
    cout << endl << "                                 " ;
    cout << endl << "* 0. QUIT                       *" ;
    cout << endl << "* 1. Convert file to binary     *" ;
    cout << endl << "* 2. Linear probing             *" ;
    cout << endl << "* 3. Double hash                *" ;
    cout << endl << "                                 " ;
    cout << endl << "*********************************" ;
} // End displayMission



int main() {
    DM dataList ;
    int command ;

    do {
        DisplayMission() ;
        cout << endl << "Input a choice(0,1,2,3): " ;
        cin >> command ;
        cout << endl ;

        switch ( command ) {

            case 0:
                break ;
            case 1:
                dataList.Text2Binary() ;
                break ;

            case 2:
                if ( dataList.ReadFile() ) {

                }
                break ;

            case 3:
                if ( dataList.ReadFile() ) {

                }
                break ;

            default :
                cout << "Command dose not exist!" << endl ;
        } // switch()

        cout << endl ;

    } while ( command != 0 ) ;

    return 0;

} // main()





