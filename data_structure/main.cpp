#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <chrono>
#include <string>
#include <algorithm>

using namespace std;

// Song class definition
class Song {
public:
    string genre;
    string title;
    string artist;
    int year;

    Song(string g, string t, string a, int y) : genre(g), title(t), artist(a), year(y) {}

    bool operator<(const Song& other) const {
        return title < other.title;  // For BST ordering
    }

    bool operator>(const Song& other) const {
        return year > other.year;  // For priority queue ordering
    }
};

// Function to split a string by a delimiter
vector<string> split(const string &str, char delim) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to read songs from a file
vector<Song> readSongs(const string &filename) {
    vector<Song> songs;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        vector<string> parts = split(line, ',');
        if (parts.size() == 4) {
            songs.push_back(Song(parts[0], parts[1], parts[2], stoi(parts[3])));
        }
    }
    return songs;
}

// Function to visit all elements in a combined BST and priority queue structure
void visitBSTPriorityQueue(set<Song> &bst, priority_queue<Song, vector<Song>, greater<Song>> &pq) {
    for (const auto &song : bst) {
        // Do nothing, just visiting
    }
    while (!pq.empty()) {
        pq.pop();
    }
}

// Function to visit all elements in a 3D array
void visit3DArray(const vector<vector<vector<Song>>> &array3D) {
    for (const auto &array2D : array3D) {
        for (const auto &array1D : array2D) {
            for (const auto &song : array1D) {
                // Do nothing, just visiting
            }
        }
    }
}

// Function to visit all elements in a linked list
void visitLinkedList(const list<Song> &linkedList) {
    for (const auto &song : linkedList) {
        // Do nothing, just visiting
    }
}

// Function to visit all elements in a 2D array with linked list
void visit2DArrayWithLinkedList(const vector<list<Song>> &array2DList) {
    for (const auto &linkedList : array2DList) {
        for (const auto &song : linkedList) {
            // Do nothing, just visiting
        }
    }
}

// Function to find and print songs by artist in BST + Priority Queue
void findSongsByArtistInBSTPriorityQueue(const set<Song> &bst, const priority_queue<Song, vector<Song>, greater<Song>> &pq, const string &artist) {
    for (const auto &song : bst) {
        if (song.artist == artist) {
            cout << song.genre << ", " << song.title << ", " << song.artist << ", " << song.year << endl;
        }
    }
}
// Function to find and print songs by artist in 3D array
void findSongsByArtistIn3DArray(const vector<vector<vector<Song>>> &array3D, const string &artist) {
    for (const auto &array2D : array3D) {
        for (const auto &array1D : array2D) {
            for (const auto &song : array1D) {
                if (song.artist == artist) {
                    cout << song.genre << ", " << song.title << ", " << song.artist << ", " << song.year << endl;
                }
            }
        }
    }
}

// Function to find and print songs by artist in linked list
void findSongsByArtistInLinkedList(const list<Song> &linkedList, const string &artist) {
    for (const auto &song : linkedList) {
        if (song.artist == artist) {
            cout << song.genre << ", " << song.title << ", " << song.artist << ", " << song.year << endl;
        }
    }
}

// Function to find and print songs by artist in 2D array with linked list
void findSongsByArtistIn2DArrayWithLinkedList(const vector<list<Song>> &array2DList, const string &artist) {
    for (const auto &linkedList : array2DList) {
        for (const auto &song : linkedList) {
            if (song.artist == artist) {
                cout << song.genre << ", " << song.title << ", " << song.artist << ", " << song.year << endl;
            }
        }
    }
}


int main() {
    string filename = "songs.txt";
    vector<Song> songs = readSongs(filename);

    // Check if songs vector is empty
    if (songs.empty()) {
        cout << "No songs found in the file." << endl;
        return 1;
    }

    // 1. BST + Priority Queue
    set<Song> bst(songs.begin(), songs.end());
    priority_queue<Song, vector<Song>, greater<Song>> pq;
    for (const auto &song : songs) {
        pq.push(song);
    }

    // 2. 3D Array
    vector<vector<vector<Song>>> array3D(1, vector<vector<Song>>(1, songs));

    // 3. Linked List
    list<Song> linkedList(songs.begin(), songs.end());

    // 4. 2D Array with Linked List
    vector<list<Song>> array2DList(1, list<Song>(songs.begin(), songs.end()));

    // Measure time for BST + Priority Queue
    auto start = chrono::high_resolution_clock::now();
    visitBSTPriorityQueue(bst, pq);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> bstPqTime = end - start;

    // Measure time for 3D Array
    start = chrono::high_resolution_clock::now();
    visit3DArray(array3D);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> array3DTime = end - start;

    // Measure time for Linked List
    start = chrono::high_resolution_clock::now();
    visitLinkedList(linkedList);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> linkedListTime = end - start;

    // Measure time for 2D Array with Linked List
    start = chrono::high_resolution_clock::now();
    visit2DArrayWithLinkedList(array2DList);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> array2DListTime = end - start;

    // Output times
    cout << "BST + Priority Queue time: " << bstPqTime.count() << " seconds" << endl;
    cout << "3D Array time: " << array3DTime.count() << " seconds" << endl;
    cout << "Linked List time: " << linkedListTime.count() << " seconds" << endl;
    cout << "2D Array with Linked List time: " << array2DListTime.count() << " seconds" << endl;

    // Ask user for artist name and measure time to find their songs in each data structure
    string artist;
    cout << "Enter artist name: ";
    cin.ignore();  // Ignore leftover newline from previous input
    getline(cin, artist);

    // Measure time for BST + Priority Queue
    cout << "Songs by " << artist << " in BST + Priority Queue:" << endl;
    start = chrono::high_resolution_clock::now();
    findSongsByArtistInBSTPriorityQueue(bst, pq, artist);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> bstPqArtistTime = end - start;
    cout << "BST + Priority Queue time: " << bstPqArtistTime.count() << " seconds" << endl;

    // Measure time for 3D Array
    cout << "Songs by " << artist << " in 3D Array:" << endl;
    start = chrono::high_resolution_clock::now();
    findSongsByArtistIn3DArray(array3D, artist);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> array3DArtistTime = end - start;
    cout << "3D Array time: " << array3DArtistTime.count() << " seconds" << endl;

    // Measure time for Linked List
    cout << "Songs by " << artist << " in Linked List:" << endl;
    start = chrono::high_resolution_clock::now();
    findSongsByArtistInLinkedList(linkedList, artist);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> linkedListArtistTime = end - start;
    cout << "Linked List time: " << linkedListArtistTime.count() << " seconds" << endl;

    // Measure time for 2D Array with Linked List
    cout << "Songs by " << artist << " in 2D Array with Linked List:" << endl;
    start = chrono::high_resolution_clock::now();
    findSongsByArtistIn2DArrayWithLinkedList(array2DList, artist);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> array2DListArtistTime = end - start;
    cout << "2D Array with Linked List time: " << array2DListArtistTime.count() << " seconds" << endl;

    return 0;
}