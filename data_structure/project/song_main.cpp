#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <unordered_map>
#include <chrono>
#include <string>

using namespace std;

// Song class definition
class Song {
public:
    string genre;
    string title;
    string artist;
    int year;
    int count;

    Song(string g, string t, string a, int y) : genre(g), title(t), artist(a), year(y), count(0) {}

    // For BST ordering by year
    bool operator<(const Song& other) const {
        return year < other.year;
    }
    
    // For priority queue ordering by count
    bool operator>(const Song& other) const {
        return count > other.count;
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

// Function to recommend songs based on user input using Hashmap + BST
void recommendSongsHashmapBST(const vector<Song>& songs, const string& preferredGenre, int userAge, int& maxCount, string& recommendedSong) {
    int currentYear = 2023;
    int minYear = currentYear - userAge - 10;
    int maxYear = currentYear - userAge - 5;

    unordered_map<string, int> songCountMap;
    set<Song> bst;

    for (const auto& song : songs) {
        if (song.genre == preferredGenre && song.year >= minYear && song.year <= maxYear) {
            songCountMap[song.title]++;
            bst.insert(song);
        }
    }

    maxCount = 0;
    recommendedSong = "";
    for (const auto& entry : songCountMap) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            recommendedSong = entry.first;
        }
    }
}

// Function to recommend songs based on user input using 4D array
void recommendSongs4DArray(const vector<vector<vector<vector<Song>>>>& array4D, const string& preferredGenre, int userAge, int& maxCount, string& recommendedSong) {
    int currentYear = 2023;
    int minYear = currentYear - userAge - 10;
    int maxYear = currentYear - userAge - 5;

    unordered_map<string, int> songCountMap;

    for (const auto& array3D : array4D) {
        for (const auto& array2D : array3D) {
            for (const auto& array1D : array2D) {
                for (const auto& song : array1D) {
                    if (song.genre == preferredGenre && song.year >= minYear && song.year <= maxYear) {
                        songCountMap[song.title]++;
                    }
                }
            }
        }
    }

    maxCount = 0;
    recommendedSong = "";
    for (const auto& entry : songCountMap) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            recommendedSong = entry.first;
        }
    }
}

// Function to recommend songs based on user input using linked list
void recommendSongsLinkedList(const list<Song>& linkedList, const string& preferredGenre, int userAge, int& maxCount, string& recommendedSong) {
    int currentYear = 2023;
    int minYear = currentYear - userAge - 10;
    int maxYear = currentYear - userAge - 5;

    unordered_map<string, int> songCountMap;

    for (const auto& song : linkedList) {
        if (song.genre == preferredGenre && song.year >= minYear && song.year <= maxYear) {
            songCountMap[song.title]++;
        }
    }

    maxCount = 0;
    recommendedSong = "";
    for (const auto& entry : songCountMap) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            recommendedSong = entry.first;
        }
    }
}

// Function to recommend songs based on user input using BST
void recommendSongsBST(const set<Song>& bst, const string& preferredGenre, int userAge, int& maxCount, string& recommendedSong) {
    int currentYear = 2023;
    int minYear = currentYear - userAge - 10;
    int maxYear = currentYear - userAge - 5;

    unordered_map<string, int> songCountMap;

    for (const auto& song : bst) {
        if (song.genre == preferredGenre && song.year >= minYear && song.year <= maxYear) {
            songCountMap[song.title]++;
        }
    }

    maxCount = 0;
    recommendedSong = "";
    for (const auto& entry : songCountMap) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            recommendedSong = entry.first;
        }
    }
}

// Function to recommend songs based on user input using BST-based Priority Queue
void recommendSongsBSTPriorityQueue(const vector<Song>& songs, const string& preferredGenre, int userAge, int& maxCount, string& recommendedSong) {
    int currentYear = 2023;
    int minYear = currentYear - userAge - 10;
    int maxYear = currentYear - userAge - 5;

    unordered_map<string, int> songCountMap;
    set<Song> bst;

    for (const auto& song : songs) {
        if (song.genre == preferredGenre && song.year >= minYear && song.year <= maxYear) {
            songCountMap[song.title]++;
            bst.insert(song);
        }
    }

    maxCount = 0;
    recommendedSong = "";
    for (const auto& song : bst) {
        if (songCountMap[song.title] > maxCount) {
            maxCount = songCountMap[song.title];
            recommendedSong = song.title;
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

    // 1. 4D Array
    vector<vector<vector<vector<Song>>>> array4D(1, vector<vector<vector<Song>>>(1, vector<vector<Song>>(1, songs)));

    // 2. Linked List
    list<Song> linkedList(songs.begin(), songs.end());

    // 3. Binary Search Tree (BST)
    set<Song> bst(songs.begin(), songs.end());

    // 4. Hashmap + BST
    unordered_map<string, int> songCountMap;
    set<Song> bstForHashmap(songs.begin(), songs.end());

    // Ask user for age and preferred genre
    int userAge;
    string preferredGenre;
    cout << "Enter your age: ";
    cin >> userAge;
    cout << "Enter your preferred genre: ";
    cin.ignore();  // Ignore leftover newline from previous input
    getline(cin, preferredGenre);

    int maxCount;
    string recommendedSong;

    // Measure time for 4D Array
    auto start = chrono::high_resolution_clock::now();
    recommendSongs4DArray(array4D, preferredGenre, userAge, maxCount, recommendedSong);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> array4DTime = end - start;

    // Measure time for Linked List
    start = chrono::high_resolution_clock::now();
    recommendSongsLinkedList(linkedList, preferredGenre, userAge, maxCount, recommendedSong);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> linkedListTime = end - start;

    // Measure time for BST
    start = chrono::high_resolution_clock::now();
    recommendSongsBST(bst, preferredGenre, userAge, maxCount, recommendedSong);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> bstTime = end - start;

    // Measure time for Hashmap + BST
    start = chrono::high_resolution_clock::now();
    recommendSongsHashmapBST(songs, preferredGenre, userAge, maxCount, recommendedSong);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> hashmapBSTTime = end - start;

    // Measure time for BST-based Priority Queue
    start = chrono::high_resolution_clock::now();
    recommendSongsBSTPriorityQueue(songs, preferredGenre, userAge, maxCount, recommendedSong);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> bstPriorityQueueTime = end - start;

    // Output times
    cout << "4D Array recommendation time: " << array4DTime.count() << " seconds" << endl;
    cout << "Linked List recommendation time: " << linkedListTime.count() << " seconds" << endl;
    cout << "BST recommendation time: " << bstTime.count() << " seconds" << endl;
    cout << "Hashmap + BST recommendation time: " << hashmapBSTTime.count() << " seconds" << endl;
    cout << "BST-based Priority Queue recommendation time: " << bstPriorityQueueTime.count() << " seconds" << endl;

    // Output recommended song
    cout << "Recommended song: " << recommendedSong << " with " << maxCount << " occurrences." << endl;

    return 0;
}
