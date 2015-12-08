/******************************Final Project*************************
*********************************Math 5370***************************
**********Iterative method for solving system of linear equation*****
*************Author: Kazi Md Masum Billah and Mary Mackay ***********
*/

#include "Gauss_header.h"

int main(){
	int routine, n;
	ifstream readfile;
	cout<<"Please choose from the following: "<<endl;
	cout<<"1 : Problem 1\n2 : Problem 2\n3 : Problem 3"<<endl;
	cin>>routine;

	if (routine == 1){
		readfile.open("problem1.dat");
		n = 0;
	}
	else if (routine == 2){
		readfile.open("problem2.dat");
		n = 0;	
	}
	else if (routine == 3){
		readfile.open("problem1.dat");
		double check;
		cout<<"What dimension shall we use to construct the matrix?  \nPlease choose a value between 1 and 10."<<endl;
		cin>>check;
		if ((check>0)&&(check<=10)&&(check == ceil(check))){
			n=check;
		}
		else {
			cout<<"You should have picked a dimension between 1 and 10!"<<endl;
			return 0;
		}
	}
	else {
		cout<<"You were supposed to pick 1, 2, or 3!"<<endl;
		return 0;
	}

  
  
  Matrix test(readfile, n);
  cout<<"We are solving the following matrices: "<<endl;
  test.print(test.A);
  test.print(test.b);
  

  clock_t t1, t2;
  t1 = clock();
  test.SolveLU();
  t1 = clock() - t1;

  cout<<"Using LU decomposition, it took: "<<t1<<" flops ("<<((float)t1)/CLOCKS_PER_SEC<<" seconds) for the calculation."<<endl;
  cout<<"and we get the following answer:"<<endl;
  test.print(test.y);
  test.print(test.x);
  cout<<"To solve this, we obtained and used the Lower and Upper matrices:"<<endl;
  test.print(test.L);
  test.print(test.U);


  cout<<endl;

  t2 = clock();
  test.SolveInverse();
  t2 = clock()-t2;

  cout<<"Using the inverse of A to solve, it took: "<<t2<<" flops ("<<((float)t2)/CLOCKS_PER_SEC<<" seconds) for the calculation."<<endl; 
  cout<<"and we get the following answer:"<<endl;
  test.print(test.y);
  cout<<"To solve this, we obtained and used the inverse matrix:"<<endl;
  test.print(test.inverse);

  return 0;
}
