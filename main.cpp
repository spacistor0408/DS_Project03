// 電機四乙 10626329 袁倫御
// 商設三 10724243 石顥澐

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>

#define INPUT_FILE "input"
#define LINEAR_FILE "linear"
#define DOUBLE_FILE "double"
#define QUADRATIC_FILE "quadratic"
#define TEXT_NAME ".txt"
#define BIN_NAME ".bin"

using namespace std;


typedef struct BinData  //student data
{
    char sid[10];
    char sname[10];
    unsigned char score[6];
    float avg;
} BinD;

typedef struct HT  //for hash table
{
    BinD info;
    int hashvalue = -1;
} HashTable;


bool fileExist = true;

vector<BinD> data;

class Student
{

public:

    vector<BinD> getData()
    {
        return data;
    } // getData()

    void TextToBinary(string file_num)
    {
        fstream inFile, outFile;
        string file_name;
        int stuNo = 0;

        inFile.open((INPUT_FILE + file_num + TEXT_NAME).c_str(), fstream::in);
        if (inFile.is_open())
        {
            file_name = INPUT_FILE + file_num + BIN_NAME;

            outFile.open(file_name.c_str(), fstream::out | fstream::binary);

            char rBuf[255];
            while(inFile.getline( rBuf, 255, '\n'))
            {
                string temp;
                BinD aStudent;
                int col = 0, pre = 0, pos = 0;

                stuNo++;
                temp.assign(rBuf);
                pos = temp.find_first_of('\t', pre);

                while( pos != string::npos )
                {
                    switch (++col)
                    {
                    case 1:
                        strcpy(aStudent.sid, temp.substr(pre, pos - pre).c_str());
                        break;
                    case 2:
                        strcpy(aStudent.sname, temp.substr(pre, pos - pre).c_str());
                        break;
                    default:
                        aStudent.score[col - 3] = atoi(temp.substr(pre, pos - pre).c_str());
                        break;
                    }

                    pre = ++pos;
                    pos = temp.find_first_of('\t', pre);
                }

                pos = temp.find_last_of('\t');
                aStudent.avg = atof( temp.substr(pos + 1).c_str());
                outFile.write((char *)&aStudent, sizeof( aStudent ));
            }
            outFile.close();
        }
        inFile.close();
    } // TextToBinary()

    bool readBinary(string filenum)     //讀取bin檔
    {
        bool open = false;
        fstream binFile;
        BinD aStudent;
        int stuNo = 0;
        string filename = INPUT_FILE + filenum + BIN_NAME;
        binFile.open(filename.c_str(), fstream::in | fstream::binary);
        if(binFile.is_open())
        {
            open = true ;
            binFile.seekg(0, binFile.end);
            stuNo = binFile.tellg() / sizeof(aStudent);
            binFile.seekg(0, binFile.beg);
            cout << endl;
            for(int i = 0; i < stuNo; i++)
            {
                binFile.read((char *)&aStudent, sizeof(aStudent));
                data.push_back(aStudent);
            }
        }

        binFile.close();
        return open ;
    } // readBinary()

    void Clear()
    {
        if(!data.empty())
            data.clear();
    } // Clear()
}; // class Student


class ONE
{

public:
    Student student ;

    void OneMain()
    {

        string file_num;
        string bin_file;
        string txt_file;
        float totalbump = 0;
        bool binexist = false;

        while(true)
        {
            cout << "Input 301, 302, ...[0]Quit):";
            cin >> file_num;

            bin_file = INPUT_FILE + file_num + BIN_NAME;
            txt_file = INPUT_FILE + file_num + TEXT_NAME;

            fstream inFile_bin(bin_file.c_str());
            fstream inFile_txt(txt_file.c_str());


            if(file_num == "0")
                break;
            else if(!inFile_bin.is_open() && !inFile_txt.is_open())
                cout << "### " <<  txt_file << " does not exist! ###\n" << endl;
            else
            {
                double start, end ;
                start = clock() ;

                binexist = student.readBinary(bin_file);
                if (!binexist)
                {
                    student.TextToBinary(file_num);
                    student.readBinary(file_num);
                }


                data = student.getData();
                int tablesize = findsize(data.size());
                int hashvalue[tablesize];
                HashTable tableX[tablesize];


                for (int i = 0; i < data.size(); i ++)     // create hash
                {
                    int counter = 1;
                    bool PutInHash = false;
                    hashvalue[i] = CountHashValue(atoi(data[i].sid), tablesize) % tablesize;
                    int temp = hashvalue[i];
                    while (PutInHash == false)
                    {
                        if (tableX[temp].hashvalue == -1)
                        {
                            tableX[temp].info = data[i];
                            tableX[temp].hashvalue = hashvalue[i];
                            PutInHash = true;
                            totalbump = totalbump + counter;
                        }
                        else if(temp == tablesize)
                        {
                            temp = 0;
                        }
                        else
                        {
                            temp ++;
                            counter ++;
                        }
                    }
                }

                end = clock() ;

                float tempans = totalbump / data.size() ;
                cout << "tempans: " << tempans << endl;
                float ans2 = (int)((tempans*10000)) / 10000.0;      //Round to the fourth decimal place (successful search)
                tempans = unsuccessfulSearch(tableX, tablesize) ;
                float ans1 = (int)((tempans*10000)+0.5) / 10000.0;  //Round to the fourth decimal place (unsuccessful search)


                cout << "Hash Table X has been created." << endl ;                                   //output comparisons on average
                cout << "unsuccessful search: " << ans1 << " " << "comparisons on average" << "\n";
                cout << "successful search: " << ans2 << " " << "comparisons on average"  << "\n";
                cout << "avgtime: " << ((end - start) ) / data.size() << endl ;

                // output file
                string outname = LINEAR_FILE + file_num + TEXT_NAME;
                ofstream fout(outname.c_str());
                fout << "--- Hash Table X --- (linear probing)" << endl;
                for(int r = 0; r < tablesize; r ++)
                {
                    if (tableX[r].hashvalue != -1)
                    {
                        fout << "[ " << r << "]  " << "\t" << tableX[r].hashvalue << "," << "\t" << tableX[r].info.sid << "," << "\t";
                        fout << tableX[r].info.sname << "," << "\t" << tableX[r].info.avg << endl;
                    }
                    else
                    {
                        fout << "[ " << r << "]  " << endl;
                    }

                }
                fout.close();

                student.Clear();
                break;
            }
        } // while
    }

    int CountHashValue(int sid, int dsize)     //count hash value
    {
        int total = 0;
        float counter = 0;
        while(sid!=0)
        {
            counter ++;
            int i = sid % 10 + '0';
            sid = sid / 10;
            if ( counter == 1)
            {
                total = i;
            }
            else
            {
                total = total * i;

                while(total > dsize)
                {
                    total = total % dsize;
                }
            }
        }
        return total;
    }

    int findsize(int datasize)       //hash table size
    {
        int temp = (datasize*1.2) + 1;
        //cout << temp << endl;

        while (true)
        {
            int c;
            bool not_Prime_num = false;
            for (int b = 2; b < temp; b++)
            {
                c = temp % b;
                if (c == 0)
                {
                    not_Prime_num = true;
                }
            }
            if (not_Prime_num)
            {
                temp++;
            }
            else
            {
                //cout << temp << endl;
                return temp;
            }
        }
    }


    float unsuccessfulSearch(HashTable table[], int tablesize)  //count of comparisons on average for unsuccessful search
    {
        int searchnum = 0;
       int totalcount = 0;
       float ans = 0;
       int pos = 0;
       while(totalcount < tablesize)
       {
           if(totalcount == 0)
           {
               while(table[pos].hashvalue != -1){
                    pos++;
                    searchnum++;
                    totalcount++;
               }
           }
           else{
                while(table[pos].hashvalue != -1 && pos < tablesize){
                    pos++;
                    searchnum++;
                    totalcount++;
               }
           }
           while(searchnum > 0)
           {
               ans = ans + searchnum;
               searchnum--;
           }
           pos++;
           totalcount++;
       }
       ans = ans/tablesize;
       return ans;
    }

};


class TWO {

public:
    Student student ;

    void TwoMain() {

        string file_num;
        string bin_file;
        string txt_file;
        float totalbump = 0;
        bool binexist = false;

        while(true) {
            cout << "Input 301, 302, ...[0]Quit):";
            cin >> file_num;

            bin_file = INPUT_FILE + file_num + BIN_NAME;
            txt_file = INPUT_FILE + file_num + TEXT_NAME;

            fstream inFile_bin(bin_file.c_str());
            fstream inFile_txt(txt_file.c_str());

            if(file_num == "0")
                break;
            else if(!inFile_bin.is_open() && !inFile_txt.is_open())
                cout << "### " <<  txt_file << " does not exist! ###\n" << endl;
            else {
                binexist = student.readBinary(bin_file);
                if (!binexist) {
                    student.TextToBinary(file_num);
                    student.readBinary(file_num);
                }


                data = student.getData();
                int tablesize = findsize(data.size());
                int hashvalue[tablesize];
                HashTable tableX[tablesize];

                int highestStep = FindHighestStep(data.size()) ;


                for (int i = 0; i < data.size(); i ++)     // create hash
                {
                    int counter = 1;
                    bool PutInHash = false;
                    hashvalue[i] = CountHashValue(atoi(data[i].sid), tablesize) % tablesize;
                    int step = highestStep - ( CountHashValue(atoi(data[i].sid), highestStep) ) ;

                    //cout << "[" << i << "]\t" << CountHashValue(atoi(data[i].sid), highestStep) << '\t' << highestStep << '\t' << hashvalue[i] << '\t' << step << endl ; // debug line

                    int temp = hashvalue[i];
                    while (PutInHash == false)
                    {

                        if (tableX[temp].hashvalue == -1)
                        {
                            tableX[temp].info = data[i];
                            tableX[temp].hashvalue = hashvalue[i];
                            PutInHash = true;
                            totalbump = totalbump + counter;
                        }
                        else if(temp == tablesize)
                        {
                            temp = 0;
                        }
                        else
                        {
                        	temp += step ;

                           	if ( temp > tablesize )
                                temp -= tablesize ;

                            counter ++;
                        }
                    }
                }

                float tempans = totalbump / data.size() ;                                   //四捨五入到小數點第四位
                float ans2 = (int)((tempans*10000)+0.5) / 10000.0;
                tempans = unsuccessfulSearch(tableX, tablesize) ;
                float ans1 = (int)((tempans*10000)+0.5) / 10000.0;


                cout << "Hash Table Y has been created." << endl ;                                   //印出題目要求
                cout << "unsuccessful search: " << ans1 << " " << "comparisons on average" << "\n";
                cout << "successful search: " << ans2 << " " << "comparisons on average"  << "\n";

                //---------------------------------　　　　　　　　　　　　　　//輸出要求檔案
                string outname = "double" + file_num + TEXT_NAME;
                ofstream fout(outname.c_str());
                fout << "--- Hash Table Y --- (double hashing)" << endl;
                for(int r = 0; r < tablesize; r ++)
                {
                    if (tableX[r].hashvalue != -1)
                    {
                        fout << "[ " << r << "]  " << "\t" << tableX[r].hashvalue << "," << "\t" << tableX[r].info.sid << "," << "\t";
                        fout << tableX[r].info.sname << "," << "\t" << tableX[r].info.avg << endl;
                    }
                    else
                    {
                        fout << "[ " << r << "]  " << endl;
                    }

                }
                fout.close();

                student.Clear();
                break;
            }
        } // while
    }

    int FindHighestStep( int datasize ) {
        int temp = ceil(((float)datasize/3)) ;
        while (true)
        {
            int c;
            bool not_Prime_num = false;
            for (int b = 2; b < temp; b++)
            {
                c = temp % b;
                if (c == 0)
                {
                    not_Prime_num = true;
                }
            }
            if (not_Prime_num)
            {
                temp++;
            }
            else
            {
                //cout << temp << endl;
                return temp;
            }
        }
    } // FindHighestStep ()

    int CountHashValue(int sid, int dsize)     //count hash value
    {
        int total = 0;
        float counter = 0;
        while(sid!=0)
        {
            counter ++;
            int i = sid % 10 + '0';
            sid = sid / 10;
            if ( counter == 1)
            {
                total = i;
            }
            else
            {
                total = total * i;

                while(total > dsize)
                {
                    total = total % dsize;
                }
            } //else
        }
        return total;
    } // CountHashValue()

    int findsize(int datasize)       //hash table size
    {
        int temp = (datasize*1.2) + 1;
        //cout << temp << endl;

        while (true)
        {
            int c;
            bool not_Prime_num = false;
            for (int b = 2; b < temp; b++)
            {
                c = temp % b;
                if (c == 0)
                {
                    not_Prime_num = true;
                }
            }
            if (not_Prime_num)
            {
                temp++;
            }
            else
            {
                //cout << temp << endl;
                return temp;
            }
        }
    }

    float unsuccessfulSearch(HashTable table[], int tablesize)  //count of comparisons on average for unsuccessful search
    {
        int totalcount = 0;
        int tempcount = 0;
        float ans = 0;
        int k = 0;
        int i = 0;
        while (totalcount < tablesize)
        {
            tempcount = 0;
            if (totalcount == 0)
            {
                k = tablesize - 1;
                while(table[k].hashvalue != -1)
                {
                    k--;
                    tempcount++;
                    totalcount++;
                }

                while(table[i].hashvalue != -1)
                {
                    i++;
                    tempcount++;
                    totalcount++;
                }
            }
            else
            {
                while(table[i].hashvalue != -1)
                {
                    i++ ;
                    tempcount++;
                    totalcount++;
                }
            }
            while (tempcount > 0)
            {
                ans = ans + tempcount;
                tempcount--;
            }
            i++ ;
            totalcount++;
        }

        ans = ans / tablesize;
        return ans;
    }



}; // class TWO


class THREE
{
public:
    Student student;

    void ThreeMain()
    {

        string file_num;
        string bin_file;
        string txt_file;
        float totalbump = 0;
        bool binexist = false;

        while(true)
        {
            cout << "Input 301, 302, ...[0]Quit):";
            cin >> file_num;

            bin_file = INPUT_FILE + file_num + BIN_NAME;
            txt_file = INPUT_FILE + file_num + TEXT_NAME;

            fstream inFile_bin(bin_file.c_str());
            fstream inFile_txt(txt_file.c_str());

            if(file_num == "0")
                break;
            else if(!inFile_bin.is_open() && !inFile_txt.is_open())
                cout << "### " <<  txt_file << " does not exist! ###\n" << endl;
            else
            {
                double start, end ;
                start = clock() ;

                binexist = student.readBinary(bin_file);
                if (!binexist)
                {
                    student.TextToBinary(file_num);
                    student.readBinary(file_num);
                }

                data = student.getData();
                int tablesize = findsize(data.size());
                int hashvalue[tablesize];
                HashTable tableZ[tablesize];

                for (int i = 0; i < data.size(); i ++)     // create hash
                {
                    int counter = 1;
                    bool PutInHash = false;
                    hashvalue[i] = CountHashValue(atoi(data[i].sid), tablesize) % tablesize;
                    int temp = hashvalue[i];
                    int oritemp = hashvalue[i];

                    while (PutInHash == false)
                    {
                        if (tableZ[temp].hashvalue == -1)
                        {
                            tableZ[temp].info = data[i];
                            tableZ[temp].hashvalue = hashvalue[i];
                            PutInHash = true;
                            totalbump = totalbump + counter;
                        }

                        else
                        {
                            temp = oritemp + counter*counter;
                            counter++;
                            if(temp >= tablesize)
                                temp = temp % tablesize;
                        }
                    }
                }

                end = clock() ;

                float tempans = totalbump / data.size();
                //cout << "tempans: " << tempans << endl;
                float ans2 = (int)((tempans*10000)) / 10000.0;      //Round to the fourth decimal place (successful search)
                tempans = unsuccessfulSearch(tableZ, tablesize) ;
                float ans1 = (int)((tempans*10000)+0.5) / 10000.0;  //Round to the fourth decimal place (unsuccessful search)


                cout << "Hash Table Z has been created." << endl ;                                   //output comparisons on average
                cout << "unsuccessful search: " << ans1 << " " << "comparisons on average" << "\n";
                cout << "successful search: " << ans2 << " " << "comparisons on average"  << "\n";

                //cout << "avgtime: " << ((end - start)) / data.size() << endl ;

                // output file
                string outname = QUADRATIC_FILE + file_num + TEXT_NAME;
                ofstream fout(outname.c_str());
                fout << "--- Hash Table Z --- (quadratic probing)" << endl;
                for(int r = 0; r < tablesize; r ++)
                {
                    if (tableZ[r].hashvalue != -1)
                    {
                        fout << "[ " << r << "] " << "\t" << tableZ[r].hashvalue << "," << "\t" << tableZ[r].info.sid << "," << "\t";
                        fout << tableZ[r].info.sname << "," << "\t" << tableZ[r].info.avg << endl;
                    }
                    else
                    {
                        fout << "[ " << r << "]  " << endl;
                    }

                }
                fout.close();

                student.Clear();
                break;
            }
        } // while
    }

    int findsize(int dsize)
    {
        int temp = (dsize * 1.2) + 1;
        while (true)
        {
            int c;
            bool not_Prime_num = false;
            for (int b = 2; b < temp; b++)
            {
                c = temp % b;
                if (c == 0)
                {
                    not_Prime_num = true;
                }
            }
            if (not_Prime_num)
            {
                temp++;
            }
            else
            {
                //cout << temp << endl;
                return temp;
            }
        }
    }

    int CountHashValue(int sid, int dsize)
    {
        int total = 0;
        float counter = 0;
        while(sid!=0)
        {
            counter ++;
            int i = sid % 10 + '0';
            sid = sid / 10;
            if ( counter == 1)
            {
                total = i;
            }
            else
            {
                total = total * i;

                while(total > dsize)
                {
                    total = total % dsize;
                }
            }
        }
        return total;
    }

    float unsuccessfulSearch(HashTable table[], int tablesize)  //count of comparisons on average for unsuccessful search
    {
       int searchnum = 0;
       int totalcount = 0;
       float ans = 0;
       int pos = 0;
       while(totalcount < tablesize)
       {
           if(totalcount == 0)
           {
               while(table[pos].hashvalue != -1){
                    pos++;
                    searchnum++;
                    totalcount++;
               }
           }
           else{
                while(table[pos].hashvalue != -1 && pos < tablesize){
                    pos++;
                    searchnum++;
                    totalcount++;
               }
           }
           while(searchnum > 0)
           {
               ans = ans + searchnum;
               searchnum--;
           }
           pos++;
           totalcount++;
       }
       ans = ans/tablesize;
       return ans;
    }

};


int main()
{
    int command;

    do
    {
        cout << endl << "***** Hash Table Utilities *****";
        cout << endl << "* 0. Quit                      *";
        cout << endl << "* 1. linear probing            *";
        cout << endl << "* 2. double hash               *";
        cout << endl << "* 3. quadratic probing         *";
        cout << endl << "********************************";
        cout << endl << "Input a choice(0, 1, 2, 3): ";
        cin >> command;

        ONE one;
        TWO two;
        THREE three;
        switch(command)
        {
        case 0:
            break;

        case 1:
            one.OneMain();
            break;

        case 2:
            two.TwoMain();
            break;

        case 3:
            three.ThreeMain();
            break;

        default:
            cout << endl << "Command does not exist!" << endl;
            break;
        }
    }
    while(command!=0);
    return 0;
}
