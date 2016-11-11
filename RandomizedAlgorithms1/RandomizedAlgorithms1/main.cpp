#include <iostream>
#include <chrono>
#include <vector>
#include <math.h>
#include <iomanip>

using namespace std;
using namespace std::chrono;


void fillArrayWithRandoms(vector<int> &v, const bool no_repetitions = true)
{
	bool already_exist = false;
	//Give current time as seed for the random number generator.
	srand(time(NULL));

	for (int i = 0; i < v.size();)
	{
		int rand_numb = rand()%v.size() + 1;

		if (no_repetitions)
		{
			//Check for duplicates, only insert and iterate if no duplicates are found.
			if (find(v.begin(), v.begin() + i, rand_numb) == v.begin() + i)
			{
				v[i] = rand_numb;
				i++;
			}
		}
		else
		{
			v[i] = rand_numb;
			i++;
		}

	}
}

void insertionSort(vector<int> &v, const int start_pos, const int end_pos)
{
	for (int i = start_pos; i < end_pos; i++)
		for (int j = i; j > 0; j--)
			if (v[j] < v[j - 1])
			{
				auto temp = v[j];
				v[j] = v[j - 1];
				v[j - 1] = temp;
			}
			else
			{
				break;
			}
	/*cout << endl << "Insertionsort ran: ";
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << ",";
	cout << " and median is: " << v[v.size()/2] << " because " << v.size() << "/2 = " << v.size()/2 <<endl;*/
}

int quickSelect(vector<int> &v, const int k, const int piv = 0)
{
	int n = 3; //Size for wich to run insertionSort and pick the median.

	if (v.size() < n)
	{
		insertionSort(v, 0, v.size());
		return v[k - 1];
	}
	else
	{
		int pivot = v[piv];
		vector<int> left;
		vector<int> right;
		for (int i = 0; i < v.size(); i++)
			if(i != piv)
				if (v[i] < pivot)
					left.push_back(v[i]);
				else
					right.push_back(v[i]);

		// Print stuff =====
		/*cout << endl << "Pivot: " << pivot << ", k: " << k << endl << "Left: ";
		for (int i = 0; i < left.size(); i++)
			cout << left[i] << ", ";
		cout << endl << "Right: ";
		for (int i = 0; i < right.size(); i++)
			cout << right[i] << ", ";
		cout << endl;*/
		//==================

		if (left.size() + 1 == k)
			return pivot;
		else if (left.size() >= k)
			return quickSelect(left, k);
		else
			return quickSelect(right, k - left.size() - 1);
	}

}

int medianOfMedians(vector<int> &v)
{
	vector<int> medians;
	int i = 0;
	for (; i < v.size() - 5; i += 5)
	{
		vector<int> subvector(v.begin() + i, v.begin() + i + 5);
		medians.push_back(quickSelect(subvector, (subvector.size() / 2) + 1));
	}
	//cout << endl << "Mediansss: ";
	//for (int k = 0; k < medians.size(); k++)
	//	cout << medians[k] << ", ";
	//cout << endl;

	if (i < v.size())
	{
		vector<int> subvector(v.begin() + i, v.end());
		medians.push_back(quickSelect(subvector, subvector.size() / 2));
	}
	int pivot_number = quickSelect(medians, (medians.size() / 2) + 1);
	int pivot_index = find(v.begin(), v.end(), pivot_number) - v.begin();
	//cout << "Pivot for quickselect = " << pivot_number;
	//cout << " and pivot index = " << pivot_index << endl;
	return quickSelect(v, v.size() / 2 + 1, pivot_index);
}

int monteCarloMedian(const vector<int> &v)
{
	//1
	vector<int> R;
	while (R.size() <= pow(v.size(), 3.0 / 4.0))
		R.push_back(v[rand() % v.size()]);

	//2
	insertionSort(R, 0, R.size());

	//3
	int d = R[(1.0 / 2.0)*pow(v.size(), 3.0 / 4.0) - sqrt(v.size())];
	int u = R[(1.0 / 2.0)*pow(v.size(), 3.0 / 4.0) + sqrt(v.size())];

	//4
	vector<int> C;
	int l_d = 0;
	int l_u = 0;

	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] < d)
			l_d++;
		else if (v[i] > u)
			l_u++;
		else
			C.push_back(v[i]);
	}

	//5, 6, 7 - Check if the algorithm has failed and needs to run again.
	if (l_d > v.size() / 2.0 || l_u > v.size() / 2.0 || C.size() > 4.0*pow(v.size(), 3.0/4.0))
		return monteCarloMedian(v);

	//8
	insertionSort(C, 0, C.size());

	//cout << endl << "Special algorithm!" << endl;
	//for (int i = 0; i < C.size(); i++)
	//	cout << C[i] << ", ";
	//cout << endl;

	//9
	return C[floor(v.size()/2.0 - l_d)];
}

int main(void)
{

	int median = -1;
	int correct_median = -1;
	int vector_length = 100;
	int iterations = 50;

	vector<float> times1;
	vector<float> times2;
	vector<float> times3;

	for(int i = 0; i < iterations; i++)
	{
		cout << i << "/" << iterations << ", " << vector_length << " elements." << endl;
		//Create array with random numbers.
		vector<int> rand_vector1;
		rand_vector1.resize(vector_length);
		fillArrayWithRandoms(rand_vector1);
		vector<int> rand_vector2(rand_vector1.begin(), rand_vector1.end());
		vector<int> rand_vector3(rand_vector1.begin(), rand_vector1.end());

		//Perform quickselect and measure time consumption.
		high_resolution_clock::time_point start_time = high_resolution_clock::now();
		median = quickSelect(rand_vector1, rand_vector1.size() / 2 + 1, rand()%25);
		auto duration1 = duration_cast<microseconds>(high_resolution_clock::now() - start_time).count();
		times1.push_back(duration1 / 1000000.0);
		//cout << "Median by quickselect: " << median << endl;
		//cout << "Running time: " << duration1 << " microseconds." << endl << endl;

		//Perform median of medians and measure time consumption.
		start_time = high_resolution_clock::now();
		median = medianOfMedians(rand_vector2);
		auto duration2 = duration_cast<microseconds>(high_resolution_clock::now() - start_time).count();
		times2.push_back(duration2 / 1000000.0);
		//cout << "Median by 'Median of medians': " << median << endl;
		//cout << "Running time: " << duration2 << " microseconds." << endl << endl;

		//Perform Monte Carlo algorithm and measure time consumption.
		start_time = high_resolution_clock::now();
		median = monteCarloMedian(rand_vector3);
		auto duration3 = duration_cast<microseconds>(high_resolution_clock::now() - start_time).count();
		times3.push_back(duration3 / 1000000.0);
		//cout << "Median by Monte Carlo algorithm: " << median << endl;
		//cout << "Running time: " << duration3 << " microseconds." << endl << endl;

		//Use insertion sort to get the true median of the vector.	
		insertionSort(rand_vector1, 0, rand_vector1.size());
		//cout << endl << "Sorted vector:" << endl;
		//for (int i = 0; i < rand_vector1.size(); i++)
		//	cout << rand_vector1[i] << ", ";
		correct_median = rand_vector1[rand_vector1.size() / 2];
		//cout << endl << "Median in sorted vector (correct median): " << correct_median << endl;
		//cout << "Vector length: " << vector_length << endl;

		vector_length += 100;
	}

	for (int i = 0; i < times1.size(); i++)
		cout << times1[i] << setw(15) << times2[i] << setw(15) << times3[i] << endl;


	//Program exits after user press enter.
	std::getchar();
	
	return 0;
}