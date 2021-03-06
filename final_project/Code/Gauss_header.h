/* This is header file for Gauss.cpp. */

#ifndef gauss_header
#define gauss_header

#include<iostream>
#include<cmath>
#include<fstream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

using namespace std;

class Matrix
{

 private:
  struct setmat{
  	double mat[20][20];
  	int rows;
  	int cols;
  	string name;
  }; //For all matrices to be used
  int rows;
  void setmatrixproperties(int rows); //Initializes matrices, part of constructor
  void computeLU(); //Lower and Upper matrices
  void constructGaussMat(int step); //Setup a matrix for Gaussian Elimination.  Form depends on what we're doing, saves to matrix "Gauss".
  void computeGauss(); //Perform Gaussian elimination on whatever matrix is saved as "Gauss"
  void findinverse(); //Alternative method to solve by finding matrix inverse
  

 public:
  Matrix(ifstream &matfile, int n);
  setmat A, b, L, U, Gauss, y, x, inverse, setup;
  void print(setmat toprint);
  void SolveLU();  //First method
  void SolveInverse();  //Second method

  
};

Matrix::Matrix(ifstream &matfile, int n){//Constructor
	/*This sets up A and b and also sets up the sizes for all the rest of the matrices to be used.
	It requires two inputs, the readfile and an integer.  If an integer of 0 is given, the code 
	will read through the input file and ignore the integer.  If anything other than 0 is given, 
	it will set up the matrix for part c and completely ignore the input file.  Sort of like a switch.*/
	if (n==0){ //Read from the file to get the data
  		string line;
  		int counter(0);
  		if (matfile.is_open()){
    		while(getline(matfile,line)){
      			if (counter==0){
					matfile>>rows;  //Read in the number of rows which is the first data line
					setmatrixproperties(rows);
					counter++;
      			}
      			else if (counter<pow(A.rows, 2)+1){
					for (int i=0; i<A.rows; i++){
	  					for (int j=0; j<A.rows; j++){
	    					matfile>>A.mat[i][j];  //Read in the A matrix values
	    					counter++;
	  					}
					}
      			}
      			else {
					for(int i=0; i<b.rows; i++){
	  					matfile>>b.mat[i][0];  //Read in the b matrix values
	  					counter++;
					}
      			}
     		}   
 			matfile.close();
  		}
  		else {cout<<"Unable to open file"<<endl;}
  	}
  	else {  //Set up the alternative A and b matrices if an n is given
  		for (int i=0; i<n; i++){
  			for (int j=0; j<n; j++){ 
  				if (j==n-1) {A.mat[i][j]=1;}
  				else if (i==j) {A.mat[i][j]=0.1;}
  				else if (i>j) {A.mat[i][j]=-1;}
  				else {A.mat[i][j]=0;}
  			}
  			b.mat[i][0]=2.1-i;

  			setmatrixproperties(n);
  		}
  		b.mat[A.rows-1][0]=2-A.rows;
  	}
}


void Matrix::setmatrixproperties(int rows){
	/*Initialize all the characteristics of the matrices we will be using.
	The size depends on how big the matrix is, so this function is called 
	from the constructor, once it knows how many rows to use.*/
	A.rows = rows; 	A.cols = rows; 	A.name = "A";
	b.rows = rows; 	b.cols = 1; 		b.name = "b";
	Gauss.rows = rows; Gauss.cols = rows+1; Gauss.name = "Gauss";
	L.rows = rows; 	L.cols = rows; 	L.name = "L";
	U.rows = rows; 	U.cols = rows; 	U.name = "U";
	y.rows = rows; 	y.cols = 1; 		y.name = "y";
	x.rows = rows; 	x.cols = 1; 		x.name = "x";
	setup.rows = rows; setup.cols = 2*rows; setup.name = "Matrix for Part c";	
	inverse.rows = rows; inverse.cols = rows; inverse.name = "Inverse matrix";		
}


void Matrix::print(setmat toprint){  //Public command to print matrices
  	cout<<toprint.name<<" = "<<endl;
  	for (int i=0; i<toprint.rows; i++){
    	for (int j=0; j<toprint.cols; j++){
      		cout<<toprint.mat[i][j]<<"\t";
    	}
    	cout<<endl;
  	}
  	cout<<endl;
  	cout<<endl;
}

void Matrix::SolveLU(){
  //Consider Ax=b.  Replace A with LU, so we have LUx=b.  Let y=Ux, so Ly=b.  We first solve for y.  Then we solve for x with Ux=y
  	computeLU(); //This calculates the lower and upper matrices
  	y.cols = 1; //Reset their size in case they got changed somewhere else.  Should only be 1 column
  	x.cols = 1;
  	for (int i=0; i<L.rows; i++){ 
  		y.mat[i][0]=b.mat[i][0];  //Set the y value to the b
  		for (int j=0; j<i; j++){
  			y.mat[i][0] -= L.mat[i][j]*y.mat[j][0];  //Do the A value times the y value for each row and subtract from b
  		}
  		y.mat[i][0] /= L.mat[i][i];  //Divide by the coefficient of the y value and we have y
  	}

  	for (int i=U.rows-1; i>=0; i--){ //Solve upper matrix whcih is Ux = y, using what we got before
  		x.mat[i][0]=y.mat[i][0];  //Set the x values to the y values
  		for (int j=U.rows-1; j>i; j--){ 
  		//For this one, we'll start at the bottom instead because it's the upper matrix, but it's the same process as before. 
  			x.mat[i][0] -= U.mat[i][j]*x.mat[j][0]; 
  		}
  		x.mat[i][0] /= U.mat[i][i];
  	}
}

void Matrix::computeLU(){  //Compute the Lower and Upper matrices
  	double factor;
		for (int i=0; i<A.rows; i++){ //Initialize the matrices
			for (int j=0; j<A.cols; j++){
				if (i==j){
					L.mat[i][j]=1;  //Diagonal is ones
				}
				else{
					L.mat[i][j]=0; //Then initialize all the other numbers to zero.
				}
				U.mat[i][j]=A.mat[i][j]; //Begin with the upper matrix as A
			}
		}

	for (int index=0; index<A.cols-1; index++){  //Upper Matrix
		for (int i=index+1; i<A.rows; i++){
			factor = -U.mat[i][index]/U.mat[index][index];  //Find factor to make 0 in upper matrix for each element
 			for (int j=0; j<A.cols; j++){
				U.mat[i][j]=factor*U.mat[index][j]+U.mat[i][j]; //Use it to make that element 0 and multiply it by everything on that row
				L.mat[i][index]=-factor;  //The factor used on U forms L
			}
		}
	}
}

void Matrix::SolveInverse(){
	/*This function first sets up the matrix augmented with an identity matrix of the same dimension.
	It then uses Gaussian elmination to find the inverse of the matrix.  It then multiplies that inverse by
	the b matrix so we can get the answers.*/
	findinverse();  //Calls findinverse() which calls computegauss() 
	double tempsum = 0;
	y.cols=1;
	for (int i=0; i<A.rows; i++){ //This part multiplies the inverse we obtained by b
		for (int j=0; j<A.cols; j++){
			tempsum += Gauss.mat[i][j+A.cols]*b.mat[j][0];
		}
		y.mat[i][0]=tempsum;
		tempsum = 0;
	}
}

void Matrix::findinverse(){
	//This function begins by setting up the matrix with the identity matrix of the same dimension.  It then calls the computegauss() function to actually solve it
	Gauss.cols = 2*A.cols;
	y.cols = A.cols;
	for (int i=0; i<A.rows; i++){  //Put A on there first
		for (int j=0; j<A.rows; j++){
			Gauss.mat[i][j]=A.mat[i][j];  
		}
	}
	for (int i=0; i<A.rows; i++){  //Then construct the identity matrix
		for (int j=A.rows; j<2*A.rows; j++){
			if (i+A.rows==j){Gauss.mat[i][j]=1; }
			else {Gauss.mat[i][j]=0; }
		}
	}
	computeGauss();  //Solve it
	for (int i=0; i<A.rows; i++){
		for (int j=0; j<A.cols; j++){ //Then save it where we can use it later if we need to
			inverse.mat[i][j]=Gauss.mat[i][j+A.cols];
		}
	}
}

void Matrix::computeGauss(){
	//This does Gaussian elmination on a given matrix.  It first goes through and gets all the zeros, then it goes back and gets the ones in the diagonal
	double factor;
	for(int index=0; index<A.cols; index++){ 
    	for (int i=0;  i<Gauss.rows; i++){
      	if (index!=i){ //Go through and find the factor needed to make each term zero, then subtract from everything on that row
				factor = Gauss.mat[i][index]/Gauss.mat[index][index];
				for (int j=0; j<=Gauss.cols; j++){
	  				Gauss.mat[i][j]=Gauss.mat[i][j]-factor*Gauss.mat[index][j];
				}
      	}
    	}
  	}
  	for (int a=0; a<Gauss.rows; a++){
    	double factor=1/Gauss.mat[a][a];  //Figure out what we would divide by to make the diagonal ones, then divide everything on that row by that number
    	for (int b=0; b<=Gauss.cols; b++){
      		Gauss.mat[a][b]=Gauss.mat[a][b]*factor;
    	}
  	}

  	for (int i=0; i<y.rows; i++){ //Pull the values from the final matrix
  		for (int j=0; j<y.cols; j++){
  			y.mat[i][j]=Gauss.mat[i][j+A.cols];	
  		}
  	}
}
#endif
