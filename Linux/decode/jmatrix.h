#include"stdio.h"
#include"stdlib.h"
//#include<iostream>
//using namespace std;

#include"jpoly.h"

#define MatEle poly
#ifndef MatEle
#define MatEle double
//Arithmetic
MatEle add(MatEle a,MatEle b){return a+b;};
MatEle sub(MatEle a,MatEle b){return a-b;};
MatEle mul(MatEle a,MatEle b){return a*b;};
MatEle div(MatEle a,MatEle b){return a/b;};
MatEle mod(MatEle a,MatEle b){return int(a)%int(b);};
const double One=1;
const double Zero=0;
void print(const MatEle& a) {printf("%f ",a);}
bool isZero(const double& a) {return (a<1e-10 && a>-1e-10);}//

double __inverse(double a,double b,int a1,int a2,int b1,int b2){
	if(isZero(b)) {printf("Warning, base shouldn't be zero\n");return Zero;}
	if(isZero(a)) return b1;
	double d=mod(b,a);
	if(isZero(d)) return a1;
	double c=div(b,a);
	return __inverse(d,a,sub(b1,mul(a1,c)),sub(b2,mul(a2,c)),a1,a2);
}
double inverse(double a,double base){
	return base/a;
}
#endif

//Arithmetic
MatEle add(MatEle a,MatEle b);
MatEle sub(MatEle a,MatEle b);
MatEle mul(MatEle a,MatEle b);
MatEle div(MatEle a,MatEle b);
MatEle mod(MatEle a,MatEle b);

MatEle mul(MatEle x,MatEle y,MatEle z);
MatEle add(MatEle x,MatEle y,MatEle z);
MatEle mulmod(MatEle x,MatEle y,MatEle m);

void print(const MatEle& a);
bool isZero(const MatEle& a);
MatEle inverse(MatEle a,MatEle base);

//create a matrix with r rows and c columns
MatEle** createMatrix(int r,int c);

//create an identity matrix with n rows
MatEle** Identity(int n);

//delete the matrix
void release(MatEle** m,int r);

//inverse of a matrix with n rows
MatEle** inverse(MatEle **m,int n);

//output thr matrix
void print(MatEle **matrix,int r,int c);

//res = mat X vec , the mat has r rows and c columns
void multiply(MatEle res[],MatEle** mat,MatEle vec[],int r,int c);

//copy the sr-th row of src to the dr-th row of dst for c elements
void copyRow(MatEle **dst,int dr,MatEle **src,int sr,int c);


//res = mat X vec
void multiply(MatEle res[],MatEle** mat,MatEle vec[],int r,int c);
//determinant
MatEle det(MatEle** m,int n);
MatEle ** inverse(MatEle **m,int n);
MatEle codDet(MatEle **m,int r,int c,int n);
// O(n!)
MatEle** adjugate(MatEle **m,int n);

MatEle ** inverse2(MatEle **m,MatEle base,int n);

// O(n*n)
MatEle ** inverse(MatEle **m,MatEle base,int n);

MatEle** codedanduncodedmatrix(MatEle** codemat,int numcode,int *flag1,MatEle** Idenditymat,int numuncoded,int *flag2);
MatEle** createMatrix2(int r,int c,MatEle& base);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MatEle** createMatrix(int r,int c){
	MatEle** m=(MatEle**)malloc(sizeof(MatEle*)*r);
	for(int i=0;i<r;++i) {
		MatEle* t=(MatEle*)malloc(sizeof(MatEle)*c);
		for(int j=0;j<c;++j) t[j]=Zero;
		m[i]=t;
	}
	return m;
}
MatEle** Identity(int n){
	MatEle** m=(MatEle**)malloc(sizeof(MatEle*)*n);
	for(int i=0;i<n;++i)  {
		MatEle* t=(MatEle*)malloc(sizeof(MatEle)*n);
		for(int j=0;j<n;++j) t[j]=Zero;
		t[i]=One;
		m[i]=t;
	}
	return m;
}
void release(MatEle** m,int r){
	for(int i=0;i<r;++i) {
		free(m[i]);
	}
	free(m);
}
void print(MatEle **matrix,int r,int c){
	for(int i=0;i<r;++i){
		for(int j=0;j<c;++j) print(matrix[i][j]);
		printf("\n");
	}
	printf("\n");
}
void copyRow(MatEle **dst,int dr,MatEle **src,int sr,int c){
	for(int i=0;i<c;++i)
	dst[dr][i]=src[sr][i];
}

void exchange(MatEle **matrix,int i,int j,int c){
	if(i==j) return;
	MatEle temp;
	for(int k=0;k<c;++k){
		temp=matrix[i][k];
		matrix[i][k]=matrix[j][k];
		matrix[j][k]=temp;
	}
}
void scale(MatEle **matrix,int i,MatEle s,int c){
	for(int k=0;k<c;++k)
		matrix[i][k]=div(matrix[i][k],s);
}
void submul(MatEle **matrix,int target,int source,MatEle s,MatEle base,int c){
	if(target==source) return;
	for(int k=0;k<c;++k)
		matrix[target][k]=sub(matrix[target][k],mulmod(matrix[source][k],s,base));
}
void submul(MatEle **matrix,int target,int source,MatEle s,int c){
	if(target==source) return;
	for(int k=0;k<c;++k)
		matrix[target][k]=sub(matrix[target][k],mul(matrix[source][k],s));
}

int getNonzero(MatEle **matrix,int col,int c){
	for(int beginRow=col;beginRow<c;++beginRow){
		if(isZero(matrix[beginRow][col])==false) return beginRow;
	}
	return -1;
}

//res = mat X vec
void multiply(MatEle res[],MatEle** mat,MatEle vec[],int r,int c){
	int i,j;
	MatEle temp;
	for(i=0;i<r;++i){
		temp=Zero;
		for(j=0;j<c;++j)
			temp=add(temp,mul(mat[i][j],vec[j]));
		res[i]=temp;
	}
}
//determinant
MatEle det(MatEle** m,int n){
	if(n<2) return m[0][0];
	if(n==2) return sub(mul(m[0][0],m[1][1]),mul(m[1][0],m[0][1]));
	MatEle **t=(MatEle**)malloc(sizeof(MatEle*)*n);
	MatEle r=Zero,k;
	int j;
	for(int i=0;i<n;i++){
		for(j=0;j<i;j++) t[j]=m[j];
		for(j=i+1;j<n;j++) t[j-1]=m[j];
		k=mul(m[i][n-1],det(t,n-1));
		if((i+n)%2!=0) r=add(r,k);
		else r=sub(r,k);
	}
	free(t);
	return r;
}
MatEle ** inverse(MatEle **m,int n){
	MatEle ** matrix=createMatrix(n,n+n);
	MatEle ** res=createMatrix(n,n);
	int i;
	for(i=0;i<n;++i){
		for(int j=0;j<n;++j){
			matrix[i][j]=m[i][j];
			matrix[i][j+n]=Zero;
		}
		matrix[i][i+n]=One;
	}
	int r,ex,k;
	for(k=0;k<n;++k){
		ex=getNonzero(matrix,k,n+n);
		if(ex==-1) {printf("Inverse failure\n");exit(0);}
		exchange(matrix,k,ex,n+n);
		scale(matrix,k,matrix[k][k],n+n);	
		for(r=0;r<n;++r){
			if(r!=k)
			submul(matrix,r,k,matrix[r][k],n+n);
		}
	}
	for(i=0;i<n;++i){
		for(int j=0;j<n;++j){
			res[i][j]=matrix[i][j+n];
		}
	}
	release(matrix,n);
	return res;
}
void removeIJ(MatEle **des,MatEle **src,int r,int c,int n){
	int i,j,k,l;
	k=0;
	for(i=0;i<n;i++){
		l=0;
		if(i!=r){
			for(j=0;j<n;++j){
				if(j!=c)
				des[k][l++]=src[i][j];
			}
			k++;
		}
	}
}
MatEle codDet(MatEle **m,int r,int c,int n){
	MatEle **t=createMatrix(n-1,n-1);
	removeIJ(t,m,r,c,n);
	MatEle cd=det(t,n-1);
	release(t,n-1);
	if((r+c)%2==0)	return cd;
	else return sub(Zero,cd);
}
// O(n!*n*n)
MatEle** adjugate(MatEle **m,int n){
	MatEle ** res=createMatrix(n,n);
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			res[i][j]=codDet(m,j,i,n);
	return res;
}

MatEle ** inverse2(MatEle **m,MatEle base,int n){
	MatEle d=inverse(det(m,n),base);
	MatEle ** res=createMatrix(n,n);
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			res[i][j]=mulmod(codDet(m,j,i,n),d,base);
	return res;

/*	MatEle **dj=adjugate(m,n);
	for(inti=0;i<n;++i) for(int j=0;j<n;++j) dj[i][j]=mulmod(dj[i][j],d,base);
	return dj;*/
}

void scale(MatEle **matrix,int i,MatEle s,MatEle base,int c){
	MatEle d=inverse(s,base);
	//print(mul(d,s));
	for(int k=0;k<c;++k)
		matrix[i][k]=mod(mul(matrix[i][k],d),base);
}
// O(n*n)
MatEle ** inverse(MatEle **m,MatEle base,int n){
	MatEle ** matrix=createMatrix(n,n+n);
	MatEle ** res=createMatrix(n,n);
	if(n==1){
		res[0][0]=inverse(m[0][0],base);
		return res;
	}
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			matrix[i][j]=m[i][j];
			matrix[i][j+n]=Zero;
		}
		matrix[i][i+n]=One;
	}
	int r,ex,k;
	for(k=0;k<n;++k){
		ex=getNonzero(matrix,k,n);//返回matrix的第k列不为0的行位置 
		exchange(matrix,k,ex,n+n);//交换k和ex行 
		scale(matrix,k,matrix[k][k],base,n+n);//第k行首元素变为one	
	//	for (int i = 0; i<n; ++i) {
	//		print(matrix[k][i]);
	//	}
	//	printf("\n--------------------%d\n", k);
		for(r=0;r<n;++r){
			if(r!=k)
			submul(matrix,r,k,matrix[r][k],base,n+n);
        //    for(int i=0;i<n;++i){
         //       print(matrix[r][i]);
		 //   }
	//		printf("\n--------------------%d\n", r);
		}
		printf("%d--------------------\n", k);
		
	    
	}
	
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			res[i][j]=mod(matrix[i][j+n],base);
		}
	}
	release(matrix,n);
	return res;
}

MatEle** codedanduncodedmatrix(MatEle** codemat,int numcode,int *flag1,MatEle** Idenditymat,int numuncoded,int *flag2)
{
	int m = numcode+numuncoded;
	MatEle** mat= createMatrix(m,m);
	int i,j,k;
	for(i=0;i<numuncoded;i++)
		mat[i][flag2[i]] = Idenditymat[flag2[i]][flag2[i]];
	for(k=0;k<numcode,i<m;k++,i++)
		for(j=0; j<m; j++)
			mat[i][j] = codemat[flag1[k]][j];
	return mat;
}

MatEle mul(MatEle x,MatEle y,MatEle z) {return mul(mul(x,y),z);}
MatEle add(MatEle x,MatEle y,MatEle z) {return add(add(x,y),z);}

MatEle mulmod(MatEle x,MatEle y,MatEle m) {return mod(mul(x,y),m);}//*/
/*
poly mulmod(poly a,poly b,poly y){
	a=mod(a,y);
	b=mod(b,y);
	poly x=Zero;
	if(isZero(a) || isZero(b)) return x;
	if(isZero(y)) {printf("Error: divided by 0\n"); return x;}
	while(!isZero(b)){
		if(and1(b)) xorEq(x,a);
		shr(b,1);
		shl(a,1);
		a=mod(a,y);
	}
	mod(x,y);
	return x;
}//*/
MatEle** createMatrix2(int r,int c,MatEle& base){
	MatEle** m=(MatEle**)malloc(sizeof(MatEle*)*r);
	MatEle p;
	for(int i=0;i<r;++i) {
		MatEle* t=(MatEle*)malloc(sizeof(MatEle)*c);
		for(int j=0;j<c;++j) {p=One;shl(p,i*j);t[j]=mod(p,base);}
		m[i]=t;
	}
	return m;
}
MatEle** matdup(MatEle** mat,int r,int c){
	MatEle** m=(MatEle**)malloc(sizeof(MatEle*)*r);
	for(int i=0;i<r;++i) {
		MatEle* t=(MatEle*)malloc(sizeof(MatEle)*c);
		for(int j=0;j<c;++j) t[j]=mat[i][j];
		m[i]=t;
	}
	return m;	
}
