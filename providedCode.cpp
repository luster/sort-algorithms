// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2013

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds one string
class Data {
public:
  string data;
  Data(const string &s) { data = s; }
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l) {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;

  ifstream input(filename.c_str());
  if (!input) {
    cerr << "Error: could not open " << filename << endl;
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    string line;
    getline(input, line);
    l.push_back(new Data(line));
  }

  input.close();
}

// Output the data to a specified input file
void writeDataList(const list<Data *> &l) {
  string filename;
  cout << "Enter name of output file: ";
  cin >> filename;

  ofstream output(filename.c_str());
  if (!output) {
    cerr << "Error: could not open " << filename << endl;
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << endl;

  // Write the data
  for (list<Data *>::const_iterator ipD = l.begin(); ipD != l.end(); ipD++) {
    output << (*ipD)->data << endl;
  }

  output.close();
}

void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  list<Data *> theList;
  loadDataList(theList);

  cout << "Data loaded.  Executing sort..." << endl;

  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds." << endl;

  writeDataList(theList);
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// declare global variables
list<Data *> counts [100000];
list<Data *> l2;
list<Data *> digits [10];
list<Data *> buckets [21];
list<Data *>::iterator it, tmpIter, holePos;
Data * countps [1000000];
char countns [1000000];
int idx1, idx2;
int decimalIdx = 20;
int endIdx;
int numBuckets;
int bucketNum;
int i, j;
int lengthNum;
int lengthList;
int tmp;
float tmp2;

// Comparison function for t1, t2
// If dec point is in same place, compare left-to-right.
// Otherwise, lower number has lower decimal place.
// Start at idx 20 since 90% of the time dec pt is at idx 20.
inline bool compare(const Data* x, const Data* y) {
	idx1=20;
	idx2=20;
	while ((x->data)[idx1] != '.') idx1--;
	while ((y->data)[idx2] != '.') idx2--;
	if (idx1 == idx2) return (x->data).compare((y->data)) < 0;
	else return (idx1 < idx2);
}

// Comparison function for t3
// Decimal point is at 1, 2, or 3, so string::find doesn't 
// have too much overhead
inline bool compare3(const Data* x, const Data* y) {
	idx1 = (x->data).find(".");
	idx2 = (y->data).find(".");
	if (idx1 == idx2) return (x->data).compare((y->data)) < 0;
	else return (idx1 < idx2);
}

// comp function if you know that elements are the same size
// this is used in the bucket sorts
inline bool comp2(const Data* x, const Data* y) {
	return (x->data).compare((y->data)) < 0;
}

// case 3: counting sort since only 1M possible numbers xxx.xxx
void countingSort2(list<Data *> &l) {
	for (it=l.begin(); it!=l.end(); ++it) {
		// index into array of counts and formatted #s by converting to int (tmp)
		tmp2 = (1000*atof(((*it)->data).c_str()));
		tmp = tmp2;
		countps[tmp]=*it;
		countns[tmp]++;
	}
	// push Data ptrs in order into new linked list
	for (i=0; i<=999999; i++) {
		while(countns[i]--) l2.push_back(countps[i]);
	}
	// replace list l with new list l2
	l.swap(l2);
}

// case 1, bucket sort
void bucketSort(list<Data *> &l) {
	numBuckets = 1000;
	for (it=l.begin(); it!=l.end(); ++it) {
		decimalIdx=20;
		while (((*it)->data)[decimalIdx] != '.') decimalIdx--;
		// endIdx is how many most significant digits to take depending on where 
		// the decimal place is
		endIdx = decimalIdx-17;
		if (endIdx<=0) counts[0].push_back(*it);
		else {
			bucketNum = atoi(((*it)->data).substr(0,endIdx).c_str());
			counts[bucketNum].push_back(*it);
		}
	}
	// sort 0s bucket using general compare function and append list
	counts[0].sort(compare);
	l2.splice(l2.end(),counts[0]);
	// sort the rest of the buckets using specific compare function and append 
	// to end
	for (j=1; j<=numBuckets; j++) {
		counts[j].sort(comp2);
		l2.splice(l2.end(),counts[j]);
	}
	// replace reference list
	l.swap(l2);
}

// case 2, bucket sort
void bucketSort2(list<Data *> &l) {
	numBuckets = 10000;
	for (it=l.begin(); it!=l.end(); ++it) {
		decimalIdx=20;
		while (((*it)->data)[decimalIdx] != '.') decimalIdx--;
		// endIdx is how many most significant digits to take depending on where 
		// the decimal place is
		endIdx = decimalIdx-16;
		if (endIdx<=0) counts[0].push_back(*it);
		else {
			bucketNum = atoi(((*it)->data).substr(0,endIdx).c_str());
			counts[bucketNum].push_back(*it);
		}
	}
	// sort 0s bucket using general compare function and append list
	counts[0].sort(compare);
	l2.splice(l2.end(),counts[0]);
	// sort the rest of the buckets using specific compare function and append 
	// to end
	for (j=1; j<=numBuckets; j++) {
		counts[j].sort(comp2);
		l2.splice(l2.end(),counts[j]);
	}
	// replace reference list
	l.swap(l2);
}

void insertionSort2(list<Data *> &l) {
	// start at 2nd position for insertion sort
	it = l.begin(); it++;
	for (it; it!=l.end(); ++it) {
		holePos = it;
		tmpIter = holePos; 
		tmpIter--;
		while (holePos!=l.begin() && (*holePos)->data < (*tmpIter)->data) {
			iter_swap(holePos,tmpIter);
			holePos--;
			tmpIter--;
		}
	}
}

void sortDataList(list<Data *> &l) {
	// Determine what dataset by length and size of list, choose algorithm 
	// accordingly
	lengthList = l.size();
	lengthNum = ((*(l.begin()))->data).length();
	it = l.begin();

	if (lengthList < 110001) bucketSort(l); // case 1
	else if (lengthNum < 10) countingSort2(l); // case 3
	else {
		string firstNum, secondNum;
		firstNum = (*it) -> data;
		advance(it,1);
		secondNum = (*it) -> data;

		if (firstNum.substr(0,8) == secondNum.substr(0,8)) insertionSort2(l); // case 4
		else bucketSort2(l); // case 2
	}
}
