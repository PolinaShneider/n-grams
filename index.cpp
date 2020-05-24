#include <iostream>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <sys/time.h>
#include <time.h>

using namespace std;

void removeChar(char *s, char charToRemove)
{
    char *copy = s;
    char *temp = s;

    while (*copy)
    {
        if (*copy != charToRemove)
        {
            *temp = *copy; // overwrite the character pointed by temp (the wrong one) with the one pointed by copy
            temp++;
        }
        copy++;
    }
    *temp = 0; // eliminates the last redundant element
}

char *readTextFromFile()
{

    FILE *pFile;
    long size;
    char *buffer;
    size_t result;

    pFile = fopen("./text.txt", "r"); // open the file .txt with the command to read and save on pFile the pointer at that file
    if (pFile == NULL)
    {
        fputs("File not found", stderr);
        exit(1); // fputs is used for insert a string message in a file.
                 // In this case is used "stderr" for default error messages
    }

    //file size:
    fseek(pFile, 0, SEEK_END); // Sets the position indicator associated with the stream to a new position.
                               // pFile is the indicator, 0 is the offset and SEEK_END indicates the end of file.

    size = ftell(pFile); // Returns the dimension in bytes of the file .txt
    rewind(pFile);       // Sets the position indicator associated with stream to the beginning of the file.

    //allocate memory:
    buffer = (char *)malloc(sizeof(char) * size); // sizeof(char) depends on environment -- 4 on 32bit & 8 on 64bit
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    //copy the file into the buffer:
    result = fread(buffer, 1, size, pFile); // Buffer --> Pointer to a block of memory ; 1 --> size, in bytes, of each element to be read.
                                            // size --> Number of elements, each one with a size of size bytes ; pFile --> Pointer to a FILE object
    if (result != size)
    {
        fputs("Reading error", stderr);
        exit(3);
    }

    // converts the characters in UPPER CASE to LOWER CASE
    for (int i = 0; i < size; ++i)
    {
        char c = tolower(buffer[i]);
        buffer[i] = c;
    }

    char blackList[] = "~`!@#$%^&*()_-+=|][{}-':;?/>.<,òàèì°ç§é "; // build the vector with characters to remove

    // Cicle that calls the function to find and remove each characters of the blacklist vector
    for (int i = 0; i < strlen(blackList); i++)
    {
        removeChar(buffer, blackList[i]);
    }

    fclose(pFile); // close ther file opened
    return buffer;
}

unordered_map<string, int> computeNgrams(int n, char *fileString)
{
    struct timeval startRead, endRead;

    unordered_map<string, int> map;
    string fileStr = fileString;

    fileStr.erase(remove(fileStr.begin(), fileStr.end(), '\n'), fileStr.end());

    gettimeofday(&startRead, NULL);
    for (int i = 0; i < fileStr.length() - n + 1; i++)
    {
        string key = "";
        // used for create an n-grams
        for (int j = 0; j < n; j++)
        {
            key = key + fileStr[i + j];
        }

        // control if there's other n-grams "key" in the map.
        if (map.find(key) == map.end()) // FALSE -> insert the new n-grams in the map with value=1
        {
            pair<string, int> pair(key, 1);
            map.insert(pair);
        }
        else // TRUE -> increments the value associated with "key"
        {
            map[key] += 1;
        }
    }
    return map;
}

int main(int argc, char const *argv[])
{

    setlocale(LC_ALL, "Russian");
    char *text = readTextFromFile();

    struct timeval start, end;  // The struct timeval structure represents an elapsed time. It is declared in sys/time.h
                                // Has "time_t tv_sec" that represents elapsed time in seconds and "long int tv_usec" represent elapsed time in microseconds
    gettimeofday(&start, NULL); // the function uses variables of the struct timeval and get current the time of when it was called
    unordered_map<string, int> map = computeNgrams(3, text);
    gettimeofday(&end, NULL);

    double elapsed_time = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6; // calcolate the elapsed time using the information memorized in the struct timeval thanks to the function gettimeofday()
    cout << elapsed_time << endl;

    // Print al the n-grams with the corresponding value
    for (auto &x : map)
    {
        cout << x.first << ": " << x.second << endl;
    }

    return 0;
}
