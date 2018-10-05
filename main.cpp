#include <iostream>
#include <stdio.h>   // cout, endl
#include <string.h>  // strcmp, strcpy
#include <fstream>   // open, is_open
#include <stdlib.h>
#include <string.h>

using namespace std;

string   fileName ;
char gProblem[100] ;             //做事的陣列
char gReg[100][100];
char gDo[100][100];                // 放做完頁置換的頁框資料
bool gFaultBool[100];               //看是否有頁錯誤
int gFaultInt = 0;               // gFaultBool 是 true 的
int gFrames = 0;                 // 空間大小
int gSC[100];                   // SC的計數器
int gLFUPR[100];                // LFU的計數器

void FIFO();
void token();
void LRU();
void ARB();
void SCP();
void LFU();
void MFU();
bool readProblem();
void writeAnswer();             // 寫檔
int gTest = 1;
fstream outFile;
string fname ;

int main()
{
    readProblem();

    fname = fileName + "-output.txt";
    outFile.open( fname.c_str(),fstream::out );

    FIFO();
    writeAnswer();
    gFaultInt = 0;               // gFaultBool 是 true 的

    LRU();
    gTest = 2;
    writeAnswer();
    gFaultInt = 0;               // gFaultBool 是 true 的

    ARB();
    gTest = 3;
    writeAnswer();
    gFaultInt = 0;               // gFaultBool 是 true 的

    SCP();
    gTest = 4;
    writeAnswer();
    gFaultInt = 0;               // gFaultBool 是 true 的

    LFU();
    gTest = 5;
    writeAnswer();
    gFaultInt = 0;               // gFaultBool 是 true 的

    MFU();
    gTest = 6;
    writeAnswer();

    outFile.close();
}


void MFU()
{
    int i = 0;
    int j = 0;
    while( i < gFrames )
    {
        gLFUPR[i] = 1;
        i = i + 1;
    }
    i = 0;

    while( gProblem[i] != '\n' && gProblem[i] != '\0' && gProblem[i] != '\t' )
    {
        if( i == 0 )
        {
            gFaultBool[i] = true;
            gFaultInt = 1;
            gDo[i][0] = gProblem[0];
        }

        if( i != 0 )
        {
            j = 0;
            while( j < gFrames && gDo[i-1][j] != gProblem[i] )
            {
                j = j + 1;
            }

            if( j < gFrames && i > gFrames )
            {
                gFaultBool[i] = false;

                gLFUPR[j] = gLFUPR[j] + 1;

                j = 0;
                while( j < gFrames )
                {
                    gDo[i][j] = gDo[i-1][j];
                    j = j + 1;
                }
            }
            else
            {
                gFaultBool[i] = true;
                gFaultInt = gFaultInt+1;
                j = gFrames - 2;

                int big = gFrames-1 ;

                while( j >= 0 && i > 2 )
                {
                    if( gLFUPR[big] < gLFUPR[j] )
                        big = j;

                    if( gLFUPR[big] == gLFUPR[j] )
                    {
                        if( gDo[i-1][big] == gProblem[i-1] )
                            big = j;
                        else
                        {
                            if( gDo[i-1][j] != gProblem[i-1] && gDo[i-1][big] == gProblem[i-2] )
                                big = j;
                            else
                            {
                                if( gDo[i-1][j] != gProblem[i-2] && gDo[i-1][j] != gProblem[i-1] && gDo[i-1][big] == gProblem[i-3] )
                                    big = j;
                            }
                        }
                    }

                    j = j - 1;
                }

                j = gFrames-1;

                int y = gFrames-1;

                if( i <= 2 )
                {
                    j = 1;
                    while( j < gFrames )
                    {
                        gDo[i][j] = gDo[i-1][j-1];
                        j = j + 1;
                    }
                }
                else
                {
                    while( j > 0 )
                    {
                        if( j == big )
                            y = y - 1;

                        gDo[i][j] = gDo[i-1][y];
                        gLFUPR[j] = gLFUPR[y];

                        j = j - 1;
                        y = y - 1;
                    }
                }
                gDo[i][0] = gProblem[i];
                gLFUPR[0] = 1;
            }
        }
        i = i + 1;
    }
}

void LFU()
{
    int i = 0;
    int j = 0;
    while( i < gFrames )
    {
        gLFUPR[i] = 1;
        i = i + 1;
    }
    i = 0;

    while( gProblem[i] != '\n' && gProblem[i] != '\0' && gProblem[i] != '\t' )
    {
        //cout << gLFUPR[2] << "  " << gLFUPR[1] << " " << gLFUPR[0] << "\n";
        if( i == 0 )
        {
            gFaultBool[i] = true;
            gFaultInt = 1;
            gDo[i][0] = gProblem[0];
        }

        if( i != 0 )
        {
            j = 0;
            while( j < gFrames && gDo[i-1][j] != gProblem[i] )
            {
                j = j + 1;
            }

            if( j < gFrames && i > gFrames )
            {
                gFaultBool[i] = false;

                gLFUPR[j] = gLFUPR[j] + 1;

                j = 0;
                while( j < gFrames )
                {
                    gDo[i][j] = gDo[i-1][j];
                    j = j + 1;
                }
            }
            else
            {
                gFaultBool[i] = true;
                gFaultInt = gFaultInt+1;
                j = gFrames - 2;

                int small = gFrames-1 ;

                while( j >= 0 && i > 2 )
                {
                    if( gLFUPR[small] > gLFUPR[j] )
                        small = j;

                    if( gLFUPR[small] == gLFUPR[j] )
                    {
                        if( gDo[i-1][small] == gProblem[i-1] )
                            small = j;
                        else
                        {
                            if( gDo[i-1][j] != gProblem[i-1] && gDo[i-1][small] == gProblem[i-2] )
                                small = j;
                            else
                            {
                                if( gDo[i-1][j] != gProblem[i-2] && gDo[i-1][j] != gProblem[i-1] && gDo[i-1][small] == gProblem[i-3] )
                                    small = j;
                            }
                        }
                    }

                    j = j - 1;
                }

                j = gFrames-1;

                int y = gFrames-1;

                if( i <= 2 )
                {
                    j = 1;
                    while( j < gFrames )
                    {
                        gDo[i][j] = gDo[i-1][j-1];
                        j = j + 1;
                    }
                }
                else
                {
                    while( j > 0 )
                    {
                        if( j == small )
                            y = y - 1;

                        gDo[i][j] = gDo[i-1][y];
                        gLFUPR[j] = gLFUPR[y];

                        j = j - 1;
                        y = y - 1;
                    }
                }
                gDo[i][0] = gProblem[i];
                gLFUPR[0] = 1;
            }
        }
        i = i + 1;
    }
}

void SCP()
{
    int i = 0;
    int j = 0;

    while( i < gFrames )
    {
        gSC[i] = 0;
        i = i + 1;
    }
    i = 0;

    while( gProblem[i] != '\n' && gProblem[i] != '\0' && gProblem[i] != '\t' )
    {

        if( i == 0 )
        {
            gFaultBool[i] = true;
            gFaultInt = 1;
            gDo[i][0] = gProblem[0];
        }

        if( i != 0 )
        {
            j = 0;
            while( j < gFrames && gDo[i-1][j] != gProblem[i] )
            {
                j = j + 1;
            }

            if( j < gFrames && i > gFrames )
            {
                gFaultBool[i] = false;

                j = 0;
                while( j < gFrames )
                {
                    gDo[i][j] = gDo[i-1][j];

                    if( gDo[i-1][j] == gProblem[i] )
                        gSC[j] = 1;

                    j = j + 1;
                }
            }
            else
            {
                gFaultBool[i] = true;
                gFaultInt = gFaultInt+1;
                gDo[i][0] = gProblem[i];
                int regSC[100];
                j = 0;

                while( j < gFrames )
                {
                    regSC[j] = gSC[j];
                    j = j + 1;
                }

                j = gFrames-1;

                while( j != 666 )
                {
                    if( gSC[j] == 0 )
                    {
                        int k = 1;
                        int y = j + 1;
                        if( y == gFrames )
                            y = 0;

                        while( k < gFrames )
                        {
                            if( y == j )
                                y = y + 1;
                            if( y == gFrames )
                                y = 0;

                            gDo[i][k] = gDo[i-1][y];
                            gSC[k] = regSC[y];
                            k = k + 1;
                            y = y + 1;

                            if( y == gFrames )
                                y = 0;
                        }
                        j = 667;
                    }
                    else
                    {
                        gSC[j] = 0;
                        regSC[j] = 0;
                    }

                    if( j == 0 )
                        j = gFrames-1;
                    else
                        j = j - 1;
                }

                if( i > 2 )
                    gSC[0] = 1;

                j = 0;
                while( j < gFrames )
                {
                    regSC[j] = gSC[j];
                    j = j + 1;
                }
            }
        }
        i = i + 1;
    }
}

void ARB()
{
    int i = 0;
    int j = 0;

    while( gProblem[i] != '\n' && gProblem[i] != '\0' && gProblem[i] != '\t' )
    {

        if( i == 0 )
        {
            gFaultBool[i] = true;
            gFaultInt = 1;
            gDo[i][0] = gProblem[0];
        }

        if( i != 0 )
        {
            j = 0;
            while( j < gFrames && gDo[i-1][j] != gProblem[i] )
            {
                j = j + 1;
            }

            if( j < gFrames )
            {
                gFaultBool[i] = false;
                j = 0;

                while( j < gFrames )
                {
                    gDo[i][j] = gDo[i-1][j];
                    j = j + 1;
                }
            }
            else
            {
                gFaultBool[i] = true;
                gFaultInt = gFaultInt+1;
                gDo[i][0] = gProblem[i];
                int k = gFrames-1;

                while( k > 0 )
                {
                    j = 1;
                    while( j < gFrames && gDo[i-1][k] != gProblem[i-j] )
                    {
                        j = j + 1;
                    }

                    if( j < gFrames && i > gFrames )
                        gDo[i][k] = gDo[i-1][k];
                    else
                    {
                        while( k > 0 )
                        {
                            gDo[i][k] = gDo[i-1][k-1];
                            k = k - 1;
                        }
                    }
                    k = k - 1;
                }
            }
        }
        i = i + 1;
    }
}

void LRU()
{
    int i = 0;
    int j = 0;

    while( gProblem[i] != '\n' && gProblem[i] != '\0' && gProblem[i] != '\t' )
    {
        if( i == 0 )
        {
            gFaultBool[i] = true;
            gFaultInt = gFaultInt+1;
            gDo[i][0] = gProblem[0];
        }

        if( i != 0 )
        {
            j = 0;
            while( j < gFrames && gDo[i-1][j] != gProblem[i] )
            {
                j = j + 1;
            }

            if( j < gFrames && i > gFrames )
            {
                gFaultBool[i] = false;
                gDo[i][0] = gDo[i-1][j];
                int k = 1;
                int y = 0;

                while( k < gFrames )
                {
                    if( y == j )
                        y = y + 1;

                    gDo[i][k] = gDo[i-1][y];
                    k = k + 1;
                    y = y + 1;
                }
            }
            else
            {
                gFaultBool[i] = true;
                gFaultInt = gFaultInt+1;
                j = 1;

                while( j < gFrames )
                {
                    gDo[i][j] = gDo[i-1][j-1];
                    j = j + 1;
                }
                gDo[i][0] = gProblem[i];
            }
        }
        i = i + 1;
    }
}

void FIFO()
{
    int i = 0;
    int j = 0;
    while( gProblem[i] != '\n' && gProblem[i] != '\0' && gProblem[i] != '\t' )
    {
        if( i == 0 )
        {
            gFaultBool[i] = true;
            gFaultInt = 1;
            gDo[i][0] = gProblem[0];
        }

        if( i != 0 )
        {
            j = 0;
            while( j < gFrames && gDo[i-1][j] != gProblem[i] )
            {
                j = j + 1;
            }

            if( j < gFrames && i > gFrames )
            {
                gFaultBool[i] = false;
                j = 0;
                while( j < gFrames )
                {
                    gDo[i][j] = gDo[i-1][j];
                    j = j + 1;
                }
            }
            else
            {
                gFaultBool[i] = true;
                gFaultInt = gFaultInt+1;
                j = 1;

                while( j < gFrames )
                {
                    gDo[i][j] = gDo[i-1][j-1];
                    j = j + 1;
                }
                gDo[i][0] = gProblem[i];
            }
        }
        i = i + 1;
    }
}

bool readProblem()
{                         // definition: read records from a file
    fstream     inFile;                                         // input file handle
    char        rBuf[255];
    int i = 0 ;

    cout << "請輸入檔名：" ;
    cin >> fileName ;                                           // input file name is fixed
    fileName = fileName + ".txt";

    inFile.open(fileName.c_str(), fstream::in);                 // open file to read

    if (!inFile.is_open())
    {                                  // unable to open file
        cout << endl << "### " << fileName << " does not exist! ###" << endl;
        return false;
    } // if
    else
    {
        inFile.getline(rBuf, 255, '\n');
        gFrames = rBuf[0]-'0' ;
        inFile.getline(gProblem, 100, '\n');

       // strcpy( gProblem, ( rBuf ) );

    }

    inFile.close();                                             // close file


    return true;
}   // end readProblem


void writeAnswer()
{
    int i = 0;


    if( !outFile.is_open() ){
        cout << endl << "## Can't create " << fname << " ! ##"<< endl;
        return;
    }

    if( gTest == 1  )
        outFile << "--------------FIFO-----------------------\n";

    if( gTest == 2 )
        outFile << "--------------LRU--------------------------\n";

    if( gTest == 3  )
        outFile << "--------------Additional Reference Bits-------------------\n";

    if( gTest == 4  )
        outFile << "--------------Second chance Page-------------------------\n";

    if( gTest == 5 )
        outFile << "--------------Least Frequently Used Page Replacement --------------------\n";

    if( gTest == 6 )
        outFile << "--------------Most Frequently Used Page Replacement ----------------------\n";

    while( gProblem[i] != '\n' && gProblem[i] != '\0' && gProblem[i] != '\t' )
    {
        outFile << gProblem[i] << "        " << gDo[i][0] << gDo[i][1] << gDo[i][2]  ;
        if( gFaultBool[i] == true )
            outFile <<  "       F";
        outFile <<  "\n";


        i = i + 1;
    }

    outFile << "Page Fault = " << gFaultInt << "   Page Replaces = " << gFaultInt-gFrames << "     Page Frames = " << gFrames << "\n\n";


}
