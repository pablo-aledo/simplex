/*
 * =====================================================================================
 * /
 * |     Filename:  solver.cpp
 * |
 * |  Description:  
 * |
 * |      Version:  1.0
 * |      Created:  05/30/2014 04:53:35 AM
 * |     Revision:  none
 * |     Compiler:  gcc
 * `-. .--------------------
 *    Y
 *    ,,  ,---,
 *   (_,\/_\_/_\     Author:   Pablo González de Aledo (), pablo.aledo@gmail.com
 *     \.\_/_\_/>    Company:  Universidad de Cantabria
 *     '-'   '-'
 * =====================================================================================
 */

#include <map>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>


using namespace std;



class MNumber
{
public:
        MNumber(float _m,float _r) :m(_m),r(_r) {};
        MNumber() :m(0),r(0) {};
        MNumber operator+(const MNumber& other);
        MNumber operator*(const MNumber& other);
        MNumber operator/(const MNumber& other);
        MNumber operator-(const MNumber& other);
        bool operator>(const MNumber& other);
        bool operator<(const MNumber& other);
        float m;
        float r;
private:
};

MNumber MNumber::operator+(const MNumber& other) { return MNumber( m+other.m, r+other.r );}
MNumber MNumber::operator-(const MNumber& other) { return MNumber( m-other.m, r-other.r );}



MNumber MNumber::operator*(const MNumber& other) {
	if( other.m == 0 )
		return MNumber( m*other.r, r*other.r );
	else if( m == 0 )
		return MNumber( r*other.m, r*other.r );

	else 
		return MNumber( m*other.r + r*other.m, r*other.r );

}

MNumber MNumber::operator/(const MNumber& other) {
	if( other.m == 0 )
		return MNumber( m/other.r, r/other.r );
	else if( m == 0 )
		return MNumber( other.m/r, other.r/r );

	assert(0 && "two m-numbers divided together");
}

bool MNumber::operator>(const MNumber& other) { 

	if(m > other.m)
		return true;
	else if (m < other.m)
		return false;
	else
		return r > other.r;

}

bool MNumber::operator<(const MNumber& other){

	if(m < other.m)
		return true;
	else if( m > other.m )
		return false;
	else
		return r < other.r;

}
















map<pair<int, int>,  MNumber> tabla;
int rows;
int cols;
map<int, MNumber> cost;
map<int, MNumber> costs_b;
set<int> out_of_base;
vector<int> in_base;
map<int, MNumber> cmj_cell;
int entering;
int leaving;
map<int, MNumber> coef;


void init_table(){

	MNumber tabla_aux[4][10] = {
		{ MNumber(9,0), MNumber(11,-1), MNumber(-10.0/3.0,-2)      , MNumber(2,0) , MNumber(4,-1), MNumber(2,-1     ), MNumber(3,5), MNumber(0,0), MNumber(0,0), MNumber(0,0) },
		{ MNumber(0,4), MNumber( 0, 6), MNumber(        0,-2      ), MNumber(0,1) , MNumber(0,-1), MNumber(0, 1     ), MNumber(0,2), MNumber(0,1), MNumber(0,0), MNumber(0,0) },
		{ MNumber(0,3), MNumber( 0, 2), MNumber(        0,-1.0/3.0), MNumber(0,-1), MNumber(0, 1), MNumber(0,1.0/2.0), MNumber(0,0), MNumber(0,0), MNumber(0,1), MNumber(0,0) },
		{ MNumber(0,2), MNumber( 0, 3), MNumber(        0,-1      ), MNumber(0,2),  MNumber(0, 4), MNumber(0,1.0/2.0), MNumber(0,1), MNumber(0,0), MNumber(0,0), MNumber(0,1) }
	};

	for ( unsigned int r = 0; r < 4; r++) {
		for ( unsigned int c = 0; c < 10; c++) {
			tabla[pair<int,int>(r,c)] = tabla_aux[r][c];
		}
	}

	cost[0] = MNumber(1,0);
	cost[1] = MNumber(1,0);
	cost[2] = MNumber(1,0);

	rows = 4;
	cols = 10;

	costs_b[1] = MNumber(0,1);
	costs_b[2] = MNumber(0,2);
	costs_b[3] = MNumber(0,0);
	costs_b[4] = MNumber(0,1);
	costs_b[5] = MNumber(0,1);
	costs_b[6] = MNumber(0,-5);
	costs_b[7] = MNumber(1,0);
	costs_b[8] = MNumber(1,0);
	costs_b[9] = MNumber(1,0);

	out_of_base.insert(1);
	out_of_base.insert(2);
	out_of_base.insert(3);
	out_of_base.insert(4);
	out_of_base.insert(5);
	out_of_base.insert(6);
	in_base.push_back(7);
	in_base.push_back(8);
	in_base.push_back(9);

}

void cmj(){

	for( set<int>::iterator it = out_of_base.begin(); it != out_of_base.end(); it++ ){
		
		MNumber sum = MNumber(0,0);
		int col = *it;
		for ( unsigned int j = 1; j < rows; j++) {
			sum = sum + cost[j] * tabla[pair<int,int>(j,col)];
		}
		cmj_cell[col] = sum + tabla[pair<int, int>(0,col)];
	}
}

string spaces(int n){
	char b[50] = {0};
	for ( unsigned int i = 0; i < n; i++) {
		//strcat(b, " ");
		strcat(b, " ");
	}

	return string(b);
}

#define N 14

string format(string a){
	string ret = a;
	while(ret.length() < N)
		ret = spaces(1) + ret;

	return ret;
}

string ftos(float i){
	stringstream i_ss;
	i_ss << setprecision(3) << i;
	return i_ss.str();
}

string print_mnumber(MNumber in){
	if(fabs(in.m) < 0.0001) return ftos(in.r);

	if(in.r > 0)
		return ftos(in.m) + "M+" + ftos(in.r);
	if(in.r < 0)
		return ftos(in.m) + "M" + ftos(in.r);
	return ftos(in.m) + "M";
}

string format(MNumber in){
	char b[20];
	sprintf(b, "%s", print_mnumber(in).c_str());
	string ret = string(b);
	while(ret.length() < N)
		ret = spaces(1) + ret;

	return ret;
}

string format_pivot(MNumber in){
	char b[20];
	sprintf(b, "(%s)", print_mnumber(in).c_str());
	string ret = string(b);
	while(ret.length() < N)
		ret = spaces(1) + ret;

	return ret;
}

string itos(int i){
	stringstream i_ss;
	i_ss << i;
	return i_ss.str();
}


void show(){

	printf("%s", format("").c_str());
	printf("%s", format("").c_str());
	for ( unsigned int c = 1; c < cols; c++) {
		printf("%s", format("x"+itos(c)).c_str());
	}
	printf("\n");
	
	for ( unsigned int r = 0; r < rows; r++) {

		if( r == 0 ){
			printf("%s|", format("").c_str());
		} else {
			printf("%s|", format(cost[r]).c_str() );
		}

		if( r > 0)
			printf("%s", format("x" + itos(in_base[r-1])).c_str());
		else
			printf("%s", format("").c_str());

		for ( unsigned int c = 0; c < cols; c++) {
			if(r == leaving && c == entering)
				printf("%s", format_pivot(tabla[pair<int, int>(r,c)]).c_str());
			else
				printf("%s", format(tabla[pair<int, int>(r,c)]).c_str());
		}

		if(r == 0){

		} else {
			printf("|%s", format(coef[r]).c_str());
		}


		printf("\n");
	}

	printf("%s|", format("").c_str());
	for ( unsigned int c = 0; c < cols; c++) {
		if( out_of_base.find(c) == out_of_base.end() ){
			printf("%s", format("").c_str());
		} else {
			printf("%s", format(cmj_cell[c]).c_str());
		}
	}



	printf("\n\n");
	printf("entering %d\n", entering);
	printf("leaving %d\n\n", leaving);
	printf("in_base:");
	for( vector<int>::iterator it = in_base.begin(); it != in_base.end(); it++ ){
		printf(" %d ", *it);
	} printf("\n");

	printf("out_of_base:");
	for( set<int>::iterator it = out_of_base.begin(); it != out_of_base.end(); it++ ){
		printf(" %d ", *it);
	} printf("\n");
	
	
	
	printf("===================================\n\n");
}

void select_entering(){

	int col;
	MNumber cont(0,0);
	MNumber max( -9999, -9999 );
	int i_max = 0;
	for( set<int>::iterator it = out_of_base.begin(); it != out_of_base.end(); it++ ){
		col = *it;
		cont = cmj_cell[col];
		if( cont > max){
			max = cont;
			i_max = col;
		}
	}

	entering = i_max;
	
}

void select_leaving(){

	MNumber cmin( 9999, 9999 );
	int imin;

	for ( unsigned int r = 1; r < rows; r++) {
		coef[r] = tabla[pair<int, int>(r,0)] / tabla[pair<int, int>(r,entering)];
		if( r == 1 ) coef[r] = coef[r] + MNumber(0,0.0001);
		if(coef[r] < MNumber(0,0)) continue;
		if(coef[r] < cmin){
			cmin = coef[r];
			imin = r;
		}
	}

	leaving = imin;
	
}

void pivot(){

	map<pair<int, int>,  MNumber> tabla_aux = tabla;

	MNumber pivot = tabla[pair<int,int>(leaving,entering)]; 

	for ( unsigned int c = 0; c < cols; c++) {
		tabla_aux[pair<int,int>(leaving,c)] = tabla[pair<int,int>(leaving,c)] / pivot;
	}

	vector<MNumber> col; for ( unsigned int r = 0; r < rows; r++) { col.push_back(tabla[pair<int,int>(r,entering)]); }

	for ( unsigned int r = 0; r < rows; r++) {
		for ( unsigned int c = 0; c < cols; c++) {

			if(r == leaving)
				continue;

			tabla_aux[pair<int,int>(r,c)] = tabla[pair<int,int>(r,c)] - tabla_aux[pair<int,int>(leaving,c)]*col[r];

		}
	}

	tabla = tabla_aux;


	out_of_base.erase(out_of_base.find(entering));
	out_of_base.insert(in_base[leaving-1]);

	in_base[leaving-1] = entering;

	cost[leaving] = costs_b[entering];


}

int main() {
	
	init_table();
	cmj();
	select_entering();
	select_leaving();
	show();

	pivot();
	cmj();
	select_entering();
	select_leaving();
	show();

	pivot();
	cmj();
	select_entering();
	select_leaving();
	show();

	return 0;
}
