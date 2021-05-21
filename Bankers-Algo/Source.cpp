#include <iostream>
#include <vector>
#include <string>
#include <numeric>     
using namespace std;



//for 1D vectors
vector<int> oneDInputInit(int num,string name,int resourceNumber) {

	vector<int> arr(num);
	int input;
	cout<<"Enter " << name << " Matrix: "<<endl;

	for (int i = 0; i < resourceNumber; i++) {
		cout << "R" << i<<" ";
	}
	cout << endl;
	for (int i = 0; i < num; i++) {
		cin >> input;
		arr[i] = input;
	}
	return arr;

}



vector<vector<int>> twoDInputInit(int processNumber, int resourceNumber, string matrixName ) {
	vector<vector<int>> arr(processNumber,vector<int>(resourceNumber));
	int input = 0;

	cout << "Enter Rows of "<< matrixName << " Matrix: " << endl;
	cout << "  ";
	for (int i = 0; i < resourceNumber; i++) {
		cout << "R" << i<<" ";
	}
	cout << endl;
	for (int i = 0; i < processNumber; i++) {
		cout << "P" << i << " ";
		for (int j = 0; j < resourceNumber; j++) {
			cin >> input;
			arr[i][j] = input;
		}
	}

	return arr;
}



int main() {
	// process & resource numebr
	string type = " ";

	while (type != "exit") {
label:		cout << "Enter safety or request or exit: " << endl;
		cin >> type;
		if (type == "safety") {
			int processNumber;
			cout << "Enter Number of Processes: ";
			cin >> processNumber;

			int resourceNumber;
			cout << "Enter Number of Resources: ";
			cin >> resourceNumber;


			//Table MATRICES
			vector<int> processArr(processNumber);
			for (int i = 0; i < processNumber; i++) {
				processArr[i] = i;
			}


			vector<int> finishArr(processNumber, 0);
			vector <vector<int>> allocatedArr = twoDInputInit(processNumber, resourceNumber, "Allocation");
			vector<vector<int>> maxArr = twoDInputInit(processNumber, resourceNumber, "MAX");
			vector<int> availableArr = oneDInputInit(resourceNumber,"Available",resourceNumber);


			//Need Array Calculation
			vector<vector<int>> neededArr(processNumber, vector<int>(resourceNumber));
			for (int i = 0; i < processNumber; i++) {
				for (int j = 0; j < resourceNumber; j++) {
					neededArr[i][j] = maxArr[i][j] - allocatedArr[i][j];
				}
			}


			cout << endl << "NEED MATRIX " << endl;
			cout << "  ";
			for (int i = 0; i < resourceNumber; i++) {
				cout << "R" << i << " ";
			}
			cout << endl;
			for (int i = 0; i < processNumber; i++) {
				cout << "P" << i << " ";
				for (int j = 0; j < resourceNumber; j++) {
					cout << neededArr[i][j];
					cout << " ";
				}
				cout << endl;
			}

			//Safe State Algo:
			vector<int> temp(resourceNumber, 0);
			int counter = 0;
			//check if zero >> Process Not Finished Yet
			while (accumulate(finishArr.begin(), finishArr.end(), 0) != processNumber) {
				if (counter < processNumber) {
					for (int i = 0; i < processNumber; i++) {
						for (int j = 0; j < resourceNumber; j++) {
							temp[j] = availableArr[j] - neededArr[i][j];
							if (temp[j] >= 0) {
								temp[j] = 1;
							}

						}
						if (accumulate(temp.begin(), temp.end(), 0) == resourceNumber && finishArr[i] == 0) {
							cout << "P" << processArr[i] << "  ";
							for (int k = 0; k < resourceNumber; k++) {
								availableArr[k] += allocatedArr[i][k];
							}
							finishArr[i] = 1;
						}

						fill(temp.begin(), temp.end(), 0);
					}
					counter++;
				}
				else {
					cout << "NO,the System is not safe" << endl;
					break;

				}
			}
			if (accumulate(finishArr.begin(), finishArr.end(), 0) == processNumber) {
				cout << "Yes, System is Safe " << endl;
			}
		}

		else if (type == "request") {
			int processNumber;
			cout << "Enter Number of Processes: ";
			cin >> processNumber;

			int resourceNumber;
			cout << "Enter Number of Resources: ";
			cin >> resourceNumber;


			//Table MATRICES
			vector<int> processArr(processNumber);
			for (int i = 0; i < processNumber; i++) {
				processArr[i] = i;
			}

			//Request input:
			int processRequestNumber;
			cout << "Enter process request number 0 1 2 ..." << endl;
			cin >> processRequestNumber;

			vector<int> requestArr = oneDInputInit(resourceNumber,"Request",resourceNumber);

			vector<int> finishArr(processNumber, 0);
			vector <vector<int>> allocatedArr = twoDInputInit(processNumber, resourceNumber, "Allocation");
			vector<vector<int>> maxArr = twoDInputInit(processNumber, resourceNumber, "MAX");
			vector<int> availableArr = oneDInputInit(resourceNumber,"Available Matrix",resourceNumber);


			//Need Array Calculation
			vector<vector<int>> neededArr(processNumber, vector<int>(resourceNumber));
			for (int i = 0; i < processNumber; i++) {
				for (int j = 0; j < resourceNumber; j++) {
					neededArr[i][j] = maxArr[i][j] - allocatedArr[i][j];
				}
			}



			bool flag_needed = false;
			bool flag_available = false;
			for (int i = 0; i < resourceNumber; i++) {
				if (requestArr[i] > neededArr[processRequestNumber][i]) {
					flag_needed = true;
					cout << " Request is denied, exceed Max limit" << endl;
					goto label;
				}
				if (requestArr[i] > availableArr[i]) {
					flag_available = true;
					cout << "Available Array can`t satisfy the request " << endl;
					goto label;
				}

			}

			for (int i = 0; i < resourceNumber; i++)
			{
				availableArr[i] -= requestArr[i];
				neededArr[processRequestNumber][i] -= requestArr[i];
				allocatedArr[processRequestNumber][i] += requestArr[i];
			}

			//PRINT NEEDED ARRAY
			cout << endl << "NEED MATRIX " << endl;
			cout << "  ";
			for (int i = 0; i < resourceNumber; i++) {
				cout << "R" << i << " ";
			}
			cout << endl;
			for (int i = 0; i < processNumber; i++) {
				cout << "P" << i << " ";
				for (int j = 0; j < resourceNumber; j++) {
					cout << neededArr[i][j];
					cout << " ";
				}
				cout << endl;
			}



			//Safe State Algo:

			vector<int> temp(resourceNumber, 0);
			int counter = 0;
			bool flag = true;
			//check if zero >> Process Not Finished Yet
			while (accumulate(finishArr.begin(), finishArr.end(), 0) != processNumber) {
				if (counter < processNumber) {
					for (int i = 0; i < processNumber; i++) {
						for (int j = 0; j < resourceNumber; j++) {
							temp[j] = availableArr[j] - neededArr[i][j];
							if (temp[j] >= 0) {
								temp[j] = 1;
							}

						}
						if (accumulate(temp.begin(), temp.end(), 0) == resourceNumber && finishArr[i] == 0) {
							if (flag) {
								cout << "P" << processRequestNumber << "req " ;
								flag = false;
							}
							cout << "P" << processArr[i] << "  ";
							for (int k = 0; k < resourceNumber; k++) {
								availableArr[k] += allocatedArr[i][k];
							}
							finishArr[i] = 1;
						}

						fill(temp.begin(), temp.end(), 0);
					}
					counter++;
				}
				else {
					cout << "NO, system is not safe --- REQUEST DENIED" << endl;
					break;

				}
			}
			if (accumulate(finishArr.begin(), finishArr.end(), 0) == processNumber) {
				cout << "Yes, System is Safe " << endl;
			}
			


		}

		else {
			cout << "enter valid input: " << endl;
		}
	}

	return 0;
}