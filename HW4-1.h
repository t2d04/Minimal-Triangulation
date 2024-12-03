#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <limits>
#include <vector>

using namespace std;

class FILES {
public:
	int num;
	string ** filelist;

	FILES() {
		num = 0;
		filelist = nullptr;
	}

	FILES(string filename) {
		ifstream file(filename);
		if (!file.is_open()) {
			cout << "error: there is no such file" << endl;
			num = 0;
			filelist = nullptr;
			return;
		}

		file >> num;
		filelist = new string * [num];
		for (int i = 0; i < num; i++) {
			filelist[i] = new string[2];
		}

		for (int i = 0; i < num; i++) {
			file >> filelist[i][0] >> filelist[i][1];
		}
	}

	~FILES() {
		if (filelist) {
			for (int i = 0; i < num; i++)
				delete[] filelist[i];
			delete[] filelist;
		}
	}
};

struct POSITION {
	double x;
	double y;
};

struct CHORD {
	int i;
	int j;
};

class POLYGON {
public:
	int num;
	POSITION * dots;
	double ** distance;
	double** length;
	int** path;
	std::vector<CHORD> chords;

	POLYGON() {
		num = 0;
		dots = nullptr;
		distance = nullptr;
		length = nullptr;
		path = nullptr;
	}

	POLYGON(string filename) {
		ifstream file(filename);
		if (!file.is_open()) {
			cout << "error: there is no such file" << endl;
			num = 0;
			dots = nullptr;
			distance = nullptr;
			length = nullptr;
			path = nullptr;
			return;
		}

		file >> num;
		
		dots = new POSITION[num];

		distance = new double* [num];
		for (int i = 0; i < num; i++) {
			distance[i] = new double[num];
		}

		length = new double* [num];
		for (int i = 0; i < num; i++) {
			length[i] = new double[num];
		}

		path = new int* [num];
		for (int i = 0; i < num; i++) {
			path[i] = new int[num];
		}


		for (int i = 0; i < num; i++) {
			file >> dots[i].x >> dots[i].y;
		}
	}

	bool are_adjacent(int i, int j) {
		return (abs(i - j) == 1) || (i == 0 && j == num - 1) || (i == num - 1 && j == 0);
	}

	void get_distance() {
		if (!num) {
			cout << "error: no points" << endl;
			return;
		}

		for (int i = 0; i < num; i++) {
			for (int j = i + 1; j < num; j++) {
				distance[i][j] = sqrt(pow(dots[i].x - dots[j].x, 2) + pow(dots[i].y - dots[j].y, 2));
				distance[j][i] = distance[i][j];
			}
		}
	}

	void get_length() {
		if (!num) {
			cout << "error: no points" << endl;
			return;
		}

		for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				if (i == j || j == i + 1) {
					length[i][j] = 0;
				}
				else {
					length[i][j] = std::numeric_limits<double>::infinity();
				}
				path[i][j] = -1;
			}
		}

		for (int gap = 2; gap < num; gap++) {
			for (int i = 0; i < num - gap; i++) {
				int j = i + gap;
				for (int k = i + 1; k < j; k++) {
					double cost = length[i][k] + length[k][j] + distance[i][j];
					if (cost < length[i][j]) {
						length[i][j] = cost;
						path[i][j] = k;
					}
				}
			}
		}
	}

	void trace_chords(int i, int j) {
		if (path[i][j] == -1) return;

		int k = path[i][j];

		// Add chord between i and k if not adjacent
		if (!are_adjacent(i, k)) {
			chords.push_back({ std::min(i, k), std::max(i, k) });
		}

		// Add chord between k and j if not adjacent
		if (!are_adjacent(k, j)) {
			chords.push_back({ std::min(k, j), std::max(k, j) });
		}

		trace_chords(i, k);
		trace_chords(k, j);
	}

	void sort_chords() {
		for (size_t i = 1; i < chords.size(); ++i) {
			CHORD key = chords[i];
			int j = i - 1;

			while (j >= 0 && (chords[j].i > key.i || (chords[j].i == key.i && chords[j].j > key.j))) {
				chords[j + 1] = chords[j];
				--j;
			}
			chords[j + 1] = key;
		}
	}

    void get_chords() {
        if (!num) {
            cout << "error: no points" << endl;
            return;
        }

        chords.clear();
        trace_chords(0, num - 1);

		sort_chords();
	}


	void write(string filename) {
		ofstream file(filename);

		if (!file.is_open()) {
			cout << "error: cannot open file " << filename << endl;
			return;
		}

		file << fixed << setprecision(3);

		file << length[0][num - 1] << endl;

		for (const auto& chord : chords)
			file << chord.i << ' ' << chord.j << endl;

	}

	~POLYGON() {
		if (dots) delete[] dots;

		if (distance) {
			for (int i = 0; i < num; i++)
				delete[] distance[i];
			delete[] distance;
		}

		if (length) {
			for (int i = 0; i < num; i++)
				delete[] length[i];
			delete[] length;
		}

		if (path) {
			for (int i = 0; i < num; i++)
				delete[] path[i];
			delete[] path;
		}

	}
};