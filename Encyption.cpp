#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include <bitset>
#include <string>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;
#define cypherSize 4096

//structure to keep track of the sensor value and path
struct sensor
{
    /* data */
    string path = "";
    float output;
};

// concatenate 16 sensors data
string sensCat(sensor *sensorlist, int no_of_sensors)
{
    string fbits = "";
    for (int i = 0; i < no_of_sensors; i++)
    {
        fbits += bitset<32>(*(long unsigned int *)(&sensorlist[0].output)).to_string();//converting to bits
    }
    return fbits;
}
// get a single sensor data
void getSensorData(string path, float *output)
{
    if (path == "")
    {
        return;
    }
    // Create a text string, which is used to output the text file
    string myText;

    // Read from the text file
    ifstream MyReadFile(path);

    // getline() function to read the file line
    getline(MyReadFile, myText);
    *output = stof(myText);//converting string value to float
    MyReadFile.close();
}

// copy sensor value to another
void copySensor(sensor *sensorlist, int from, int to, string path, int multiplier = 1)
{
    ofstream MyWriteFile;
    getSensorData(sensorlist[from - 1].path, &sensorlist[from - 1].output);
    sensorlist[to - 1].path = path + "//Sensor_" + to_string(to) + ".txt";
    // Read from the text file
    MyWriteFile.open(sensorlist[to - 1].path);
    MyWriteFile << int(sensorlist[from - 1].output) * multiplier;
    MyWriteFile.close();
}

// utility function to convert string to binary string
string toBinary(string const &str)
{
    string binary = "";
    for (char const &c : str)
    {
        binary += bitset<8>(c).to_string();
    }
    return binary;
}
// concatenate two integer arrays
void conactenate(int *arr1, int *arr2, int arr1Size, int arr2Size, int *arrCon)
{
    // int *arrCon = new int(arr1Size + arr2Size);
    for (int i = 0; i < arr1Size; i++)
    {
        arrCon[i] = arr1[i];
    }
    for (int j = 0; j < arr2Size; j++)
    {
        arrCon[arr1Size + j] = arr2[j];
    }
}
// XOR arrays
void XOR(int *arr1, int *arr2, int *cyphertext)
{
    for (int i = 0; i < cypherSize; i++)
    {
        if (arr1[i] == arr2[i])
        {
            cyphertext[i] = 0;
        }
        else
        {
            cyphertext[i] = 1;
        }
    }
}

// // LFSR 12
// int LFSR(int seed)
// {
//     static unsigned int ShiftRegister = seed;
//     ShiftRegister = ((((ShiftRegister >> 3) ^ (ShiftRegister)) & 1) << 11) | (ShiftRegister >> 1);
//     return ShiftRegister & 1;
// }

int main()
{
    string Plain_text = "";
    ifstream myfile("C:/Users/Abdul Ahad/Desktop/New folder (2)/Plaintext.txt");//add your plaintext file path here
    //reading from plaintext file
    if (myfile.is_open())
    {
        string temp = "";
        while (myfile.good())
        {
            getline(myfile, temp);
            Plain_text.append(temp);
        }
    }
    myfile.close();
    // cout<<Plain_text;

    string path = "C:/Users/Abdul Ahad/Desktop/testfolder";//write your sensor files path here
    string sbit = toBinary(Plain_text);//converting plaintext to binary
    int Size = sbit.length();
    // int *Bit_Stream = new int(Size);
    //converting string bits to integer
    int Bit_Stream[Size];
    for (int i = 0; i < Size; i++)
    {
        Bit_Stream[i] = int(sbit[i]) - 48;
    }
    // for (int j = 0; j < Size; j++)
    // {
    //     cout << Bit_Stream[j];
    // }
    int Message_Padding_Size;
    if (Size < 4096)
    {
        Message_Padding_Size = 4096 - Size;
    }
    else
    {
        Message_Padding_Size = Size - 4096;
    }
    time_t t = time(NULL);
    struct tm *tmp = gmtime(&t);
    int Hour = tmp->tm_hour;
    int Min = tmp->tm_min;
    int Sec = tmp->tm_sec;
    // cout<< endl << Hour << " " << Min << " " << Sec;
    // Hour = Hour>>2;
    // cout<<endl<<Hour;
    // int seed1 = ((0000000000000000|(1100000000000000&Hour))|((0000000011000000&Hour)<<6));
    // int seed2 = ((seed1|((1100000000000000&Hour)>>4))|((0000000011000000&Hour)<<2));
    // int seed3 = ((seed1|((1100000000000000&Hour)>>8))|((0000000011000000&Hour)>>2));
    int seed = (((((0 | (15 & Hour)) << 4) | (15 & Min)) << 4)) | (15 & Sec);//creating seed value from current time
    // cout<<endl<<seed;
    // Hour = Hour&1100000011000000;
    // Min = Min&1100000011000000;
    // Min = Min>>2;
    // Sec = Sec&1100000011000000;
    // Sec = Sec>>4;


    //implementing LFSR
    int Message_Padding[Message_Padding_Size];
    for (int i = 0; i < Message_Padding_Size; i++)
    {
        seed = ((((seed >> 3) ^ (seed)) & 1) << 11) | (seed >> 1);
        Message_Padding[i] = seed & 1;
    }

    // for (int i = 0; i < Message_Padding_Size; i++)
    // {
    //     cout<<Message_Padding[i];
    // }

    int Bit_Stream1[Size + Message_Padding_Size];
    conactenate(Message_Padding, Bit_Stream, Message_Padding_Size, Size, Bit_Stream1);

    // for (int i = 0; i < Message_Padding_Size+Size; i++)
    // {
    //     cout<<Bit_Stream1[i];
    // }
    sensor sensorlist[16];//creating a sensorlist
    int no_of_sensors = 0;
    for (const auto &entry : fs::directory_iterator(path))//read number of files in the given directory
    {
        sensorlist[no_of_sensors].path = entry.path().string();
        no_of_sensors++;
    }
    // for (int i = 0; i < no_of_sensors; i++)
    // {
    //     /* code */
    //     cout << sensorlist[i].path << endl;
    // }
    // getSensorData(sensorlist[0].path, &sensorlist[0].output);
    // cout << sensorlist[0].output;
    while (no_of_sensors > 16)
    {
        /* code */
        no_of_sensors--;
    }
    if (no_of_sensors < 16)
    {
        switch (no_of_sensors)
        {
        case 15:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 16, path);
            }
            else
            {
                copySensor(sensorlist, 2, 16, path);
            }
            no_of_sensors = 16;
            break;
        case 14:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 15, path);
            }
            else
            {
                copySensor(sensorlist, 2, 15, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 3, 16, path);
            }
            else
            {
                copySensor(sensorlist, 4, 16, path);
            }
            no_of_sensors = 16;
            break;
        case 13:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 14, path);
            }
            else
            {
                copySensor(sensorlist, 2, 14, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 3, 15, path);
            }
            else
            {
                copySensor(sensorlist, 4, 15, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 5, 16, path);
            }
            else
            {
                copySensor(sensorlist, 6, 16, path);
            }
            no_of_sensors = 16;
            break;
        case 12:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 13, path);
            }
            else
            {
                copySensor(sensorlist, 2, 13, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 3, 14, path);
            }
            else
            {
                copySensor(sensorlist, 4, 14, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 5, 15, path);
            }
            else
            {
                copySensor(sensorlist, 6, 15, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 7, 16, path);
            }
            else
            {
                copySensor(sensorlist, 8, 16, path);
            }
            no_of_sensors = 16;
            break;
        case 11:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 12, path);
            }
            else
            {
                copySensor(sensorlist, 2, 12, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 3, 13, path);
            }
            else
            {
                copySensor(sensorlist, 4, 13, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 5, 14, path);
            }
            else
            {
                copySensor(sensorlist, 6, 14, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 7, 15, path);
            }
            else
            {
                copySensor(sensorlist, 8, 15, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 9, 16, path);
            }
            else
            {
                copySensor(sensorlist, 10, 16, path);
            }
            no_of_sensors = 16;
            break;
        case 10:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 11, path);
            }
            else
            {
                copySensor(sensorlist, 2, 11, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 3, 12, path);
            }
            else
            {
                copySensor(sensorlist, 4, 12, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 5, 13, path);
            }
            else
            {
                copySensor(sensorlist, 6, 13, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 7, 14, path);
            }
            else
            {
                copySensor(sensorlist, 8, 14, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 9, 15, path);
            }
            else
            {
                copySensor(sensorlist, 10, 15, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 16, path);
            }
            else
            {
                copySensor(sensorlist, 2, 16, path);
            }
            no_of_sensors = 16;
            break;
        case 9:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 10, path);
            }
            else
            {
                copySensor(sensorlist, 2, 10, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 3, 11, path);
            }
            else
            {
                copySensor(sensorlist, 4, 11, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 5, 12, path);
            }
            else
            {
                copySensor(sensorlist, 6, 12, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 7, 13, path);
            }
            else
            {
                copySensor(sensorlist, 8, 13, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 9, 14, path);
            }
            else
            {
                copySensor(sensorlist, 1, 14, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 2, 15, path);
            }
            else
            {
                copySensor(sensorlist, 3, 15, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 4, 16, path);
            }
            else
            {
                copySensor(sensorlist, 5, 16, path);
            }
            no_of_sensors = 16;
            break;
        case 8:
            copySensor(sensorlist, 1, 9, path, 2);
            copySensor(sensorlist, 2, 10, path, 2);
            copySensor(sensorlist, 3, 11, path, 2);
            copySensor(sensorlist, 4, 12, path, 2);
            copySensor(sensorlist, 5, 13, path, 2);
            copySensor(sensorlist, 6, 14, path, 2);
            copySensor(sensorlist, 7, 15, path, 2);
            copySensor(sensorlist, 8, 16, path, 2);
            no_of_sensors = 16;
            break;
        case 7:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 8, path);
            }
            else
            {
                copySensor(sensorlist, 2, 8, path);
            }
            copySensor(sensorlist, 1, 9, path, 2);
            copySensor(sensorlist, 2, 10, path, 2);
            copySensor(sensorlist, 3, 11, path, 2);
            copySensor(sensorlist, 4, 12, path, 2);
            copySensor(sensorlist, 5, 13, path, 2);
            copySensor(sensorlist, 6, 14, path, 2);
            copySensor(sensorlist, 7, 15, path, 2);
            copySensor(sensorlist, 8, 16, path, 2);
            no_of_sensors = 16;
            break;
        case 6:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 7, path);
            }
            else
            {
                copySensor(sensorlist, 2, 7, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 3, 8, path);
            }
            else
            {
                copySensor(sensorlist, 4, 8, path);
            }
            copySensor(sensorlist, 1, 9, path, 2);
            copySensor(sensorlist, 2, 10, path, 2);
            copySensor(sensorlist, 3, 11, path, 2);
            copySensor(sensorlist, 4, 12, path, 2);
            copySensor(sensorlist, 5, 13, path, 2);
            copySensor(sensorlist, 6, 14, path, 2);
            copySensor(sensorlist, 7, 15, path, 2);
            copySensor(sensorlist, 8, 16, path, 2);
            no_of_sensors = 16;
            break;
        case 5:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 6, path);
            }
            else
            {
                copySensor(sensorlist, 2, 6, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 3, 7, path);
            }
            else
            {
                copySensor(sensorlist, 4, 7, path);
            }
            if (rand() % 2)
            {
                copySensor(sensorlist, 5, 8, path);
            }
            else
            {
                copySensor(sensorlist, 6, 8, path);
            }
            copySensor(sensorlist, 1, 9, path, 2);
            copySensor(sensorlist, 2, 10, path, 2);
            copySensor(sensorlist, 3, 11, path, 2);
            copySensor(sensorlist, 4, 12, path, 2);
            copySensor(sensorlist, 5, 13, path, 2);
            copySensor(sensorlist, 6, 14, path, 2);
            copySensor(sensorlist, 7, 15, path, 2);
            copySensor(sensorlist, 8, 16, path, 2);
            no_of_sensors = 16;
            break;
        case 4:
            copySensor(sensorlist, 1, 5, path, 4);
            copySensor(sensorlist, 1, 6, path, 4);
            copySensor(sensorlist, 1, 7, path, 4);
            copySensor(sensorlist, 1, 8, path, 4);
            copySensor(sensorlist, 2, 9, path, 4);
            copySensor(sensorlist, 2, 10, path, 4);
            copySensor(sensorlist, 2, 11, path, 4);
            copySensor(sensorlist, 2, 12, path, 4);
            copySensor(sensorlist, 3, 13, path, 4);
            copySensor(sensorlist, 3, 14, path, 4);
            copySensor(sensorlist, 3, 15, path, 4);
            copySensor(sensorlist, 3, 16, path, 4);
            copySensor(sensorlist, 4, 1, path, 4);
            copySensor(sensorlist, 4, 2, path, 4);
            copySensor(sensorlist, 4, 3, path, 4);
            no_of_sensors = 16;
            break;
        case 3:
            if (rand() % 2)
            {
                copySensor(sensorlist, 1, 4, path);
            }
            else
            {
                copySensor(sensorlist, 2, 4, path);
            }
            copySensor(sensorlist, 1, 5, path, 4);
            copySensor(sensorlist, 1, 6, path, 4);
            copySensor(sensorlist, 1, 7, path, 4);
            copySensor(sensorlist, 1, 8, path, 4);
            copySensor(sensorlist, 2, 9, path, 4);
            copySensor(sensorlist, 2, 10, path, 4);
            copySensor(sensorlist, 2, 11, path, 4);
            copySensor(sensorlist, 2, 12, path, 4);
            copySensor(sensorlist, 3, 13, path, 4);
            copySensor(sensorlist, 3, 14, path, 4);
            copySensor(sensorlist, 3, 15, path, 4);
            copySensor(sensorlist, 3, 16, path, 4);
            copySensor(sensorlist, 4, 1, path, 4);
            copySensor(sensorlist, 4, 2, path, 4);
            copySensor(sensorlist, 4, 3, path, 4);
            no_of_sensors = 16;
            break;
        case 2:
            copySensor(sensorlist, 1, 11, path, 7);
            copySensor(sensorlist, 1, 12, path, 7);
            copySensor(sensorlist, 1, 13, path, 7);
            copySensor(sensorlist, 1, 14, path, 7);
            copySensor(sensorlist, 1, 15, path, 7);
            copySensor(sensorlist, 1, 16, path, 7);
            copySensor(sensorlist, 2, 3, path, 7);
            copySensor(sensorlist, 2, 4, path, 7);
            copySensor(sensorlist, 2, 5, path, 7);
            copySensor(sensorlist, 2, 6, path, 7);
            copySensor(sensorlist, 2, 7, path, 7);
            copySensor(sensorlist, 2, 8, path, 7);
            no_of_sensors = 16;
            break;
        case 1:
            copySensor(sensorlist, 1, 2, path, 15);
            copySensor(sensorlist, 1, 3, path, 15);
            copySensor(sensorlist, 1, 4, path, 15);
            copySensor(sensorlist, 1, 5, path, 15);
            copySensor(sensorlist, 1, 6, path, 15);
            copySensor(sensorlist, 1, 7, path, 15);
            copySensor(sensorlist, 1, 8, path, 15);
            copySensor(sensorlist, 1, 9, path, 15);
            copySensor(sensorlist, 1, 10, path, 15);
            copySensor(sensorlist, 1, 11, path, 15);
            copySensor(sensorlist, 1, 12, path, 15);
            copySensor(sensorlist, 1, 13, path, 15);
            copySensor(sensorlist, 1, 14, path, 15);
            copySensor(sensorlist, 1, 15, path, 15);
            copySensor(sensorlist, 1, 16, path, 15);
            no_of_sensors = 16;
            break;
        case 0:
            cout << "Algorithm not supported" << endl;
            break;

        default:
            cout << "Execution Error" << endl;
            break;
        }
    }
    if (no_of_sensors == 16)
    { // getting values of all sensors in float
        for (int i = 0; i < no_of_sensors; i++)
        {
            getSensorData(sensorlist[i].path, &sensorlist[i].output);
            // cout << "Path : " << sensorlist[i].path << "  Value : " << sensorlist[i].output << endl;
        }
    }
    string Seed_Value_01 = sensCat(sensorlist, no_of_sensors);
    // cout << Seed_Value_01.length();
    string Seed_01 = Seed_Value_01;
    string Seed_02 = Seed_Value_01;
    string Chromosome_01 = Seed_01 + Seed_02;

    // getting values of all sensors in float
    for (int i = 0; i < no_of_sensors; i++)
    {
        getSensorData(sensorlist[i].path, &sensorlist[i].output);
        // cout << "Path : " << sensorlist[i].path << "  Value : " << sensorlist[i].output << endl;
    }
    string Seed_Value_02 = sensCat(sensorlist, no_of_sensors);
    // cout << Seed_Value_02.length();
    string Seed_03 = Seed_Value_02;
    string Seed_04 = Seed_Value_02;
    string Chromosome_02 = Seed_03 + Seed_04;
    string Seed_05 = Seed_01;
    string Seed_06 = Seed_02;
    string Seed_07 = Seed_03;
    string Seed_08 = Seed_04;
    string Paternal_Chromosome_01 = Seed_01 + Seed_03;
    string Paternal_Chromosome_02 = Seed_02 + Seed_04;
    string Maternall_Chromosome_01 = Seed_05 + Seed_07;
    string Maternall_Chromosome_02 = Seed_06 + Seed_08;
    string Tetrad_01 = Paternal_Chromosome_01 + Maternall_Chromosome_01;
    string Tetrad_02 = Paternal_Chromosome_02 + Maternall_Chromosome_02;
    // getting values of all sensors in float
    for (int i = 0; i < no_of_sensors; i++)
    {
        getSensorData(sensorlist[i].path, &sensorlist[i].output);
        // cout << "Path : " << sensorlist[i].path << "  Value : " << sensorlist[i].output << endl;
    }
    string Seed_Value_03 = sensCat(sensorlist, no_of_sensors);
    // replacing first 512 bits of Tetrad_01
    for (int i = 0; i < 512; i++)
    {
        Tetrad_01[i] = Seed_Value_03[i];
    }

    // getting values of all sensors in float
    for (int i = 0; i < no_of_sensors; i++)
    {
        getSensorData(sensorlist[i].path, &sensorlist[i].output);
        // cout << "Path : " << sensorlist[i].path << "  Value : " << sensorlist[i].output << endl;
    }
    string Seed_Value_04 = sensCat(sensorlist, no_of_sensors);
    // replacing first 512 bits of Tetrad_02
    for (int i = 0; i < 512; i++)
    {
        Tetrad_02[i] = Seed_Value_04[i];
    }
    // swapping last 512 bits of Tetrad_01 and Tetrad_02
    char temp;
    for (int i = Tetrad_01.length() - 1; i >= 512; i--)
    {
        temp = Tetrad_01[i];
        Tetrad_01[i] = Tetrad_02[i];
        Tetrad_02[i] = temp;
    }
    int keySize = Tetrad_01.length() + Tetrad_02.length();
    int Key[keySize];
    for (int i = 0; i < Tetrad_01.length(); i++)
    {
        Key[i] = int(Tetrad_01[i]) - 48;
    }
    for (int i = Tetrad_01.length(); i < keySize; i++)
    {
        Key[i] = int(Tetrad_01[i - Tetrad_01.length()]) - 48;
    }
    // for (int i = 0; i < keySize; i++)
    // {
    //     cout<<Key[i];
    // }
    int Cipher_Text[cypherSize];
    XOR(Bit_Stream1, Key, Cipher_Text);
    ofstream myoutfile("C:/Users/Abdul Ahad/Desktop/New folder (2)/cipherText.txt");//write your ciphertext file path here
    if (myoutfile.is_open())//writing to ciphertext file
    {
        string temp = "";
        for (int i = 0; i < cypherSize; i++)
        {
            // cout << Cipher_Text[i];
            temp.append(to_string(Cipher_Text[i]));
        }
        myoutfile << temp;
    }
    myoutfile.close();
    ofstream mykeyfile("C:/Users/Abdul Ahad/Desktop/New folder (2)/Key.txt"); // write your key file path here
    if (mykeyfile.is_open())//writing to key file
    {
        string temp = "";
        for (int i = 0; i < cypherSize; i++)
        {
            // cout << Cipher_Text[i];
            temp.append(to_string(Key[i]));
        }
        mykeyfile << temp;
    }
    mykeyfile.close();
    return 0;
}