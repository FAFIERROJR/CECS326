#include <iostream>
#include <cstdlib>

using namespace std;

struct Arrays {
 char * charPtrArr[20];
 int intArr[20];
};

void deallocAll(struct Arrays);
void deleteChars(struct Arrays, int);
int getIndex();
int getMainChoice();
void initArrays(struct Arrays);
void listDealloc(struct Arrays);
void printChars(struct Arrays, int);
void printMainMenu();
void printSubMenu();
char getRandomUpperCaseChar();
void reInitCharPtr(struct Arrays, int);

main(){
	struct Arrays arrays;

	initArrays(arrays);

	int mainChoice= 0;
	int index = 0;
	int subChoice = 0;
	while(true){
		//Main menu
		mainChoice = getMainChoice();

		switch(mainChoice){
			case 1:
				index = getIndex();
				//Access a pointer
				if(arrays.charPtrArr[index] == NULL){
					reInitCharPtr(arrays, index);
				}

				//submenu
				printSubMenu();
				cin >> subChoice;

				//subSwitch
				switch(subChoice){
					case 1:
						printChars(arrays, index);
						break;
					case 2:
						deleteChars(arrays, index);
						break;
					case 3:
						break;
					default:
						break;
				}

				break;

			case 2:
				//list deallocated memory
				listDealloc(arrays);
				break;

			case 3:
				//dealloc all
				deallocAll(arrays);
				break;

			case 4:
				deallocAll(arrays);
				exit(0);
				break;

			default:
				break;
		}
	}

	return 0;
}

void deleteChars(struct Arrays a, int index){
	delete a.charPtrArr[index];
	a.charPtrArr[index] = NULL;
}

int getIndex(){
	int index = -1;
	do{
		cout << "Enter the index to access (0-19)" << endl;
		cin >> index;

		if(index < 0 || index > 19){
			cout<< "Invalid input. Try again" << endl;
		}
	}while(index < 0 || index > 19);

	return index;
}

void deallocAll(struct Arrays a){
	for(int i = 0; i < 20; i++){
		deleteChars(a, i);
	}
}

int getMainChoice(){
	int choice = -1;
	do{
		printMainMenu();
		cin >> choice;

		if(choice < 1 || choice > 4){
			cout<< "Invalid input. Try again" << endl;
		}
	}while(choice < 1 || choice > 4);

	return choice;
}

char getRandomUpperCaseChar(){
	return char(rand() % 26 + 65);
}

int getSubChoice(){
	int choice = -1;
	do{
		printSubMenu();
		cin >> choice;

		if(choice < 1|| choice > 4){
			cout<< "Invalid input. Try again" << endl;
		}
	}while(choice < 1 || choice > 4);

	return choice;
}

void initArrays(struct Arrays a){

	cout << "Initializing char array..." << endl;

	a.intArr[0] = 2900;

	for(int i = 1; i < 20; i++){
		a.intArr[i] = 2 * a.intArr[i - 1];
		cout << a.intArr[i] << " ";
	}
	cout << endl;

	for(int i = 0; i < 20; i++){
		a.charPtrArr[i] = new char[10];

		for(int j = 0; j < 10; j++){
			a.charPtrArr[i][j] = getRandomUpperCaseChar();
		}

	}
}

void listDealloc(struct Arrays a){
	cout << "List of deallocated char pointers (indices): " << endl;
	for(int i = 0; i < 20; i ){
		if(a.charPtrArr[i] == NULL){
			cout << i << ", ";
		}
	}
	cout << endl;
}

void printChars(struct Arrays a ,int index){
	for(int i = 0; i < 10; i ++){
		cout << a.charPtrArr[index][i];
	}
	cout << endl;
}
void printMainMenu(){
	cout << "1. Access a pointer\n"
		<< "2. List deallocated memory (index)\n"
		<< "3. Deallocate all memory\n"
		<< "4. Exit program" << endl;
}


void printSubMenu(){
	cout << "1. Print first 10 chars\n"
		<< "2. Delete  all chars at this pointer\n"
		<< "3. Return to Main menu" << endl;
}

void reInitCharPtr(struct Arrays a, int index){
	cout << "This char pointer was previously deleted. Reinitializing.." << endl;
	a.charPtrArr[index] = new char[a.intArr[index]];
	for(int i = 0; i < 10; i++){
		a.charPtrArr[index][i] = getRandomUpperCaseChar();
	}
}