// Opencvtest.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include<fstream>
#include<string.h>
#include <time.h>
#include <math.h>

using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	unsigned seed;
    seed = (unsigned)time(NULL); // 取得時間序列
    srand(seed); // 以時間序列當亂數種子

	char theline [200] ;
	
	//read file
	fstream fin;
	fin.open("traindata.txt",ios::in);
	double data[1000][30];


	//initial
	for(int i=0; i<1000; i++){
		for(int j=0; j<30; j++){
			data[i][j] = 0 ;
		}
	}
	//分割字串存入二維陣列
	char *token = NULL;
	char *next_token = NULL;
	char seps[]   = " ,\t\n";
	int a=0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b = 0;
		token = strtok_s( theline, seps, &next_token);
		while (token != NULL){
			data[a][b] = atof(token);
            token = strtok_s( NULL, seps, &next_token);
			b++;
		}
		a++;
	}
	fin.close();

	double w[21];
	//initial
	for(int i=0; i<21; i++){
		w[i] = 0;
	}

	bool again = true;
	int T = 0;
	int number = 0;
	while(again){
		T++;
		// calculat deltaein
		
		double s = (-1)*data[number][20]*w[0]*1;
		double back[21];
		for(int j=0; j<21; j++){
			back[j] = 0;
		}
		back[0] = data[number][20];
		for(int j=0; j<20; j++){
			s = s + (-1)*data[number][20]*w[j+1]*data[number][j];
			back[j+1] = data[number][20]*data[number][j];
		}
		double thita = 1/(1+exp((-1)*s));
		

		if(T == 2000){
			again = false;
		}else{
			for(int i=0; i<21; i++){
				w[i] = w[i] + 0.001*thita*back[i];
			}
		}
		number++;
		if(number>=1000){
			number = number - 1000;
		}
	}
	//test
	fin.open("testdata.txt",ios::in);
	double test[3000][30];
	//initial
	for(int i=0; i<3000; i++){
		for(int j=0; j<30; j++){
			test[i][j] = 0 ;
		}
	}
	char *token2 = NULL;
	char *next_token2 = NULL;
	a = 0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b=0;
		token2 = strtok_s( theline, seps, &next_token2);
		while (token2 != NULL){
			test[a][b] = atof(token2);
            token2 = strtok_s( NULL, seps, &next_token2);
			b++;
		}
		a++;
	}

	double eout = 0;
	for(int i=0; i<3000; i++){
		double temp = w[0]*1;
		for(int j=0; j<20; j++){
			temp = temp + w[j+1]*test[i][j];
		}
		double h = 0;
		if(temp > 0){
			h = 1;
		}else{
			h = -1;
		}
		if(h == test[i][20]){

		}else{
			eout++;
		}
	}
	eout = eout/3000;
	cout<<eout<<endl;

	system("pause");
	return 0;
}