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
#define cypherSize 4096
// XOR arrays
void XOR(int *arr1, int *arr2, int *arr3)
{
    for (int i = 0; i < cypherSize; i++)
    {
        if (arr1[i] == arr2[i])
        {
            arr3[i] = 0;
        }
        else
        {
            arr3[i] = 1;
        }
    }
}

int main()
{
    string Cipher_text = "";
    string strkey = "";
    ifstream myoutfile("C:/Users/Abdul Ahad/Desktop/New folder (2)/cipherText.txt"); // write your ciphertext file path here
    if (myoutfile.is_open())                                                         // reading from ciphertext file
    {
        string temp = "";
        while (myoutfile.good())
        {
            getline(myoutfile, temp);
            Cipher_text.append(temp);
        }
    }
    myoutfile.close();
    ifstream mykeyfile("C:/Users/Abdul Ahad/Desktop/New folder (2)/Key.txt"); // write your key file path here
    if (mykeyfile.is_open())                                                         // reading from key file
    {
        string temp = "";
        while (mykeyfile.good())
        {
            getline(mykeyfile, temp);
            strkey.append(temp);
        }
    }
    mykeyfile.close();
    int cpherBit_Stream[cypherSize];
    int Key[cypherSize];
    for (int i = 0; i < cypherSize; i++)
    {
        cpherBit_Stream[i] = int(Cipher_text[i]) - 48;
        Key[i] = int(strkey[i]) - 48;
    }
    int Plain_text[cypherSize];
    XOR(cpherBit_Stream, Key, Plain_text);
    ofstream myfile("C:/Users/Abdul Ahad/Desktop/New folder (2)/Plaintext1.txt"); // add your plaintext file path here
    if (myfile.is_open())//writing to plaintext file
    {
        string temp = "";
        for (int i = 0; i < cypherSize; i++)
        {
            // cout << Cipher_Text[i];
            temp.append(to_string(Plain_text[i]));
        }
        myfile << temp;
    }

    myfile.close();
    return 0;
}
