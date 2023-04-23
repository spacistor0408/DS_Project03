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

    studentType info ;
    int hashvalue ;

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

		/*---------- deal with data ----------*/
        if ( inFile.is_open() ) {
            outFile.open( ("input" + fname + ".bin").c_str(), fstream::out | fstream::binary ) ;
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
               // cout << "[" << i+1 << "]" << oneSt.sname << endl ;
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

	studentType GetData( int pos ) {
		return orinList.at(pos) ;
	} // GetData()

	int GetSize() {
		return orinList.size() ;
	} // GetSize()

	void Write( HashTable hashTable[], int tableSize, string fileName ) {

		ofstream fOut( (fileName + fname + ".txt").c_str() ) ;

        if ( fileName == "linear" )
            fOut << " --- Hash Table X --- (linear probing)" << endl ;
        else if ( fileName == "double" )
            fOut << " --- Hash Table Y --- (double hashing)" << endl ;
        else if ( fileName == "quadratic" )
            fOut << " --- Hash Table Z --- (quadratic probing)" << endl ;

        for(int i = 0 ; i < tableSize ; i ++ ) {

        	if ( hashTable[i].hashvalue != -1 ) {

        		fOut << "[" << right << setw(3) << i << "]"
                     << setw(11) << hashTable[i].hashvalue << ","
                     << setw(11) << hashTable[i].info.sid << ","
                     << setw(11) << hashTable[i].info.sname << ","
                     << setw(11) << hashTable[i].info.mean << endl ;

			} else {
				fOut << "[" << right << setw(3) << i << "] " << endl ;
			}

    	} // for

        fOut.close() ;

	} // Write()

}; // Class FileIO

class LinearHash {

protected:

    int size ;
	HashTable *hashTable ; // Create hash table

	int FindPrime( int num ) {

        while (true) {
            int remainder ;
            bool not_Prime_num = false ;

            for (int i = 2 ; i < num ; i++) {
                remainder = num % i;
                if (remainder == 0) {
                    not_Prime_num = true ;
                    break ;
                } // if
            } // for

            if (not_Prime_num)
                num++;
            else
                break ;

        } // while

	    return num ;
	}

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

    LinearHash( int hSize ){

        size = ceil( ((float)hSize*1.2) ) ; SetSize() ; hashTable = new HashTable[size] ; for( int i = 0 ; i < size ; i++ ) hashTable[i].hashvalue = -1 ;

    } // LinearHash

    void Insert2HT( studentType data, int &totalbump ) {

		// Create hash table
		int key = HashKey( atoi( data.sid ), size ) ;
		int pos = key ;

        while ( hashTable[pos].hashvalue != -1 ) {
        	pos++ ;
        	totalbump++ ;
        	if ( pos == size )
        		pos = 0 ;

		} // while

		hashTable[pos].hashvalue = key ;
		hashTable[pos].info = data ;
		totalbump++ ;
		// cout << "[" << key << "]" << '\t' << hashTable[pos].hashvalue << '\t' << hashTable[pos].info.sname << endl ;

    } // Insert2HT()

    int HashKey(int sid, int dsize ) {

        int total = 0 ;
        float counter = 0 ;

        while (sid != 0 ) {
            counter ++ ;
            /*---------- Get the first num ----------*/
            int i = sid % 10 + '0';
            sid = sid / 10 ;

            if ( counter == 1) {
                total = i;
            }
            else {
                total = total * i;

                while(total > dsize)
                    total = total % dsize ;

            } // else
        } // while

        return total%dsize ;

    } // HashKey()

	float unsuccessfulSearch( ) { //count of comparisons on average for unsuccessful search

        int times = 0 ;

        for ( int i = 0 ; i < size ; i++ ) {

            int count = i ;

            while ( true ) {

                if ( count == size ) count = 0 ;
                if ( hashTable[count].hashvalue == -1 ) break ;

                count ++ ;
                times ++ ;

            } // while

        } // for

        float avg = (float)times / (float)size ;

        cout << avg << endl ;

        return avg ;

    } // unsuccessfulSearch()

	HashTable* GetTable() {
		return hashTable ;
	} // GetTable

	int GetTableSize() {
		return size ;
	} // GetTableSize()



}; // class LinearHash

class DoubleHash:public LinearHash {

private:

    int FindHighestStep( int datasize ) {

        int temp = (datasize/3)+1 ;

        return FindPrime( temp ) ;

    } // FindHighestStep ()

public:

    DoubleHash( int hSize ):LinearHash( hSize ){
        size = ceil( ((float)hSize*1.2) ) ; SetSize() ; hashTable = new HashTable[size] ; for( int i = 0 ; i < size ; i++ ) hashTable[i].hashvalue = -1 ;
    }

    void Insert2HT( studentType data, int &totalbump, int dataSize ) {

		// Create hash table
		int key = HashKey( atoi( data.sid ), size ) ;
		int pos = key ;

        int highestStep = FindHighestStep( dataSize ) ;
        int step = highestStep - HashKey( atoi( data.sid ), highestStep ) ;

        while ( hashTable[pos].hashvalue != -1 ) {
        	pos += step ;
        	totalbump++ ;
        	if ( pos >= size )
        		pos -= size ;

		} // while

		hashTable[pos].hashvalue = key ;
		hashTable[pos].info = data ;
		totalbump++ ;
		cout << step << "  [" << pos << "]" << '\t' << hashTable[pos].hashvalue << '\t' << hashTable[pos].info.sname
             << '\t' << hashTable[pos].info.sid << '\t' << HashKey( atoi( data.sid ), highestStep ) << endl ;

    } // Insert2HT()



}; // class DoubleHash

class Quadratic:public LinearHash {

public:

    Quadratic( int hSize ):LinearHash( hSize ){
        size = ceil( ((float)hSize*1.2) ) ; SetSize() ; hashTable = new HashTable[size] ; for( int i = 0 ; i < size ; i++ ) hashTable[i].hashvalue = -1 ;
    }

    void Insert2HT( studentType data, int &totalbump, int dataSize ) {

		// Create hash table
		int key = HashKey( atoi( data.sid ), size ) ;
		int pos = key ;
		int orinpos = pos ;
		int step = 0 ;

        while ( hashTable[pos].hashvalue != -1 ) {

            step++ ;

        	pos = orinpos + ( step*step ) ;
        	totalbump ++ ;

        	if ( pos >= size )
        		pos = pos % size ;

		} // while

		hashTable[pos].hashvalue = key ;
		hashTable[pos].info = data ;
		totalbump++ ;
		// cout << "[" << i << "]" << '\t' << hashTable[pos].hashvalue << '\t' << hashTable[pos].info.sname << endl ;

    } // Insert2HT()


    float unsuccessfulSearch( ) { //count of comparisons on average for unsuccessful search

        int times = 0 ;
        int count = 0 ;

        for ( int i = 0 ; i < size ; i++ ) {

            count = i ;
            int pow = 1 ;

            while ( true )
            {

                if ( count >= size ) count = count % size ;
                if ( hashTable[count].hashvalue == -1 ) break ;

                count = i + pow*pow ;
                // count++ ;
                pow++ ;
                times ++ ;

            } // while

            // times += count ;

        } // for

        // times = count ;

        float avg = (float)times / (float)size ;
        return avg ;
	} // unsuccessfulSearch()

}; // class Quadratic

class Mission {

	DM dataList ;

public:

		void DisplayMission() {

  		  	cout << endl << "********    HASH TABLE   ********" ;
    		cout << endl << "                                 " ;
    		cout << endl << "* 0. QUIT                       *" ;
   			cout << endl << "* 1. Linear probing             *" ;
    		cout << endl << "* 2. Double hash                *" ;
    		cout << endl << "* 3. Double hash                *" ;
    		cout << endl << "                                 " ;
    		cout << endl << "*********************************" ;
		} // End displayMission

		void One( ) {

			if ( dataList.ReadFile() ) {

				int size = dataList.GetSize() ;
				LinearHash LHash(size) ;

				int totalbump = 0 ;

				for( int i = 0 ; i < size ; i++ ) {
					LHash.Insert2HT( dataList.GetData(i), totalbump ) ;
				} //for

                float ans2 = (( (float)totalbump/size )*10000) / 10000.0 ;
                float ans1 = (int)((LHash.unsuccessfulSearch()*10000)+0.5) / 10000.0 ;


                cout << "Hash Table X has been created." << endl ;                                   //印出題目要求
                cout << "unsuccessful search: " << ans1 << " " << "comparisons on average" << "\n";
                cout << "successful search: " << ans2 << " " << "comparisons on average"  << "\n";

				dataList.Write( LHash.GetTable(), LHash.GetTableSize(), "linear" ) ;

            } // if

		} // One()

		void Two( ) {

		    if ( dataList.ReadFile() ) {

                int size = dataList.GetSize() ;
                DoubleHash DHash(size) ;

				int totalbump = 0 ;

				for( int i = 0 ; i < size ; i++ ) {
					DHash.Insert2HT( dataList.GetData(i), totalbump, size ) ;
				} //for

                float ans2 = (( (float)totalbump/size )*10000) / 10000.0 ;
                float ans1 = (int)((DHash.unsuccessfulSearch()*10000)+0.5) / 10000.0 ;


                cout << "Hash Table X has been created." << endl ;                                   //印出題目要求
                cout << "unsuccessful search: " << ans1 << " " << "comparisons on average" << "\n";
                cout << "successful search: " << ans2 << " " << "comparisons on average"  << "\n";

				dataList.Write( DHash.GetTable(), DHash.GetTableSize(), "double" ) ;

		    } // if

		} // Two()

		void Three( ) {

            if ( dataList.ReadFile() ) {

                int size = dataList.GetSize() ;
                Quadratic QHash(size) ;

				int totalbump = 0 ;

				for( int i = 0 ; i < size ; i++ ) {
					QHash.Insert2HT( dataList.GetData(i), totalbump, size ) ;
				} //for

                float ans2 = (( (float)totalbump/size )*10000) / 10000.0 ;
                float ans1 = (int)((QHash.unsuccessfulSearch()*10000)+0.5) / 10000.0 ;


                cout << "Hash Table X has been created." << endl ;                                   //印出題目要求
                cout << "unsuccessful search: " << ans1 << " " << "comparisons on average" << "\n";
                cout << "successful search: " << ans2 << " " << "comparisons on average"  << "\n";

				dataList.Write( QHash.GetTable(), QHash.GetTableSize(), "quadratic" ) ;

		    } // if

		}

}; // class Mission


int main() {

    Mission mission ;
    int command ;

    do {
    	mission.DisplayMission() ;
        cout << endl << "Input a choice(0,1,2,3): " ;
        cin >> command ;
        cout << endl ;

        switch ( command ) {

            case 0:
                break ;
            case 1:
                mission.One() ;
                break ;

            case 2:
                mission.Two() ;
                break ;

            case 3:
                mission.Three() ;
                break ;

            default :
                cout << "Command dose not exist!" << endl ;
        } // switch()

        cout << endl ;

    } while ( command != 0 ) ;

    return 0;

} // main()





