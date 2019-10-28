

#ifndef SOLVE_H
#define  SOLVE_H
#include <iostream>
#include <fstream>
#include <time.h>
#include <limits.h>
#include <math.h>

using namespace std;

#define KK 252 
#define NN 256 
#define PP 257

#define randNN 195

#endif

void memset2(TypeData *des, TypeData num, int length){
	for(int i=0;i<length;++i) des[i]=num;
}
void memcpy2(TypeData *des, TypeData *src, int length){
	for(int i=0;i<length;++i) des[i]=src[i];
}
void memadd(TypeData *des, TypeData *src, int length) {
	for (int i = 0; i<length; ++i) des[i] ^= src[i];
}
void print(TypeData *des, int length) {
	for (int i = 0; i<length; ++i) printf("%u ", des[i]);
	printf("\n ");
}



TypeData* nextVector(TypeData *des) {

	poly c = Zero;
	int k = 0;
	int len = (PP - 1) / (8 * sizeof(TypeData));
	for (int i = 0; i<len; ++i) {
		c.v[i] = des[i];
	}
	k=bool(c.v[len - 1] & 0x80000000);
	c = mul(c, Poly(2));
	c.v[0] ^= k;
	for (int i = 0; i<len ; ++i) {
		des[i] = c.v[i];
	}
	return des;
}



MatEle** eleMat(int k){
	MatEle ** ma = (MatEle **)malloc(sizeof(MatEle *)*k);
	for (int i=0;i<k;i++)
	{
		ma[i]=(MatEle *)malloc(sizeof(MatEle)*k);
		for(int j=0;j<k;++j){
			ma[i][j]=Zero;
		}
		ma[i][i]=One;
	}
	return ma;
}




/*定义单位矩阵以及其变换-sc*/
int** unitMat(int k){

    srand(time(0));//用时间作种子，使每次随即的数字不一样

    int *rand_sort = new int[k];
    for(int i = 0; i < k; i++){
        rand_sort[i] = i;
    }
    for(int i=k-1; i >= 0; i--){
        int temp_index = rand()%(i+1);
        int temp_value = rand_sort[i];
        rand_sort[i] = rand_sort[temp_index];
        rand_sort[temp_index] = temp_value;
    }
    
    

    int **ma = new int*[k];
    for(int i = 0; i < k; i++)
    {
        ma[i] = new int[k];
        int temp_index = rand_sort[i];
        for(int j = 0; j < k; j++){
            if(j == temp_index){
                ma[i][j] = 1;
            }
            else{
                ma[i][j] = 0;
            }
        }
    }
    return ma;
}

/*定义单位矩阵以及其变换-sc*/
int* unitVector(int k, int n){

    int *sort = new int[k];
    for(int i = 0; i < k; i++){
        sort[i] = i;
    }

    int num=0;
    int temp=0;
    int flag=0;
   if (num<n){
		for(int i=0;i<k;i++){//排序算法 
	        for(int j=1;j<k;j++){
	    	    temp = sort[i];
	    	    sort[i] = sort[j];
	    	    sort[j] = temp;
	    	
                num++;
                if (num==n){
				    flag=1;
				    break;
			    } 	
            }
            if (flag==1) break;
            temp = sort[0];
            for(int j=0;j<k-1;j++){
			    sort[j]=sort[j+1];
		    }
		    sort[k-1]=temp;
		    num++;
		    if (num==n) {
			    flag=1;
			    break;
		    }	 	
        }
    } 	
   
   return sort;
}

/*定义范德蒙矩阵和变换之后的单位矩阵的相乘运算-sc*/
MatEle** mulMat(MatEle** ma,int n,int k,int unit_vector[]){
    MatEle ** mul_ma = (MatEle **)malloc(sizeof(MatEle *)*n);
    for (int i=0;i<n;i++)
	{
		mul_ma[i]=(MatEle *)malloc(sizeof(MatEle)*k);
		for(int j=0;j<k;j++){
			mul_ma[i][j] = ma[i][unit_vector[j]];
		}
	}
	//free(mul_ma);
	return mul_ma;
}




MatEle** codMat(int n,int k, int p0, MatEle base){
	MatEle ** ma = (MatEle **)malloc(sizeof(MatEle *)*n);
	MatEle p;
	for (int i=0;i<n;i++)
	{
		ma[i]=(MatEle *)malloc(sizeof(MatEle)*k);
		for(int j=0;j<k;++j){
				p=One;shl(p,mod((i+1)*j,p0));ma[i][j]=mod(p,base);
		}
	}
	/*调用变换后的单位矩阵*/
//	int ** unit_mat = unitMat(k);
	/*for(int i=0; i < k; i++){
        for(int j=0; j < k; j++){
            cout << unit_mat[i][j];
        }
        cout << endl;
	}*/
    /*范德蒙矩阵和变换后的单位矩阵相乘*/
//	MatEle** mul_ma = mulMat(ma,n,k,unit_mat);
	//print(mul_ma,n,k);

//	return mul_ma;
	return ma;
}

MatEle* nextVector(MatEle* vector, int k) {
	MatEle p;
	p = vector[0];
	for (int i = 1; i<k; i++){
		vector[i - 1] = vector[i];
	}
	vector[k - 1] = p;
	return vector;
}


MatEle** totalMat(int n, int k, int p0, MatEle base, int num) {
	MatEle ** ma = (MatEle **)malloc(sizeof(MatEle *)*n);
	MatEle p;
	for (int i = 0; i<n; i++)
	{
		ma[i] = (MatEle *)malloc(sizeof(MatEle)*k);
		for (int j = 0; j<k; ++j) {
			p = One; shl(p, mod(i*j, p0)); ma[i][j] = mod(p, base);
		}
	}
//	int k=time;
//	for ()
	/*调用变换后的单位矩阵*/
	int * unit_mat = unitVector(k,num);
	/*范德蒙矩阵和变换后的单位矩阵相乘*/
	MatEle** mul_ma = mulMat(ma,n,k,unit_mat);
//	print(mul_ma,n,k);
	free(ma);
	return mul_ma;
	//return ma;
}


/*void totalMat(MatEle** ma,int n,int k, int p0, MatEle base){
	MatEle p;
	for (int i=0;i<n;i++)
	{
		for(int j=0;j<k;++j){
				p=One;shl(p,mod(i*j,p0));ma[i][j]=mod(p,base);
		}
	}

	/*调用变换后的单位矩阵*/
//	int ** unit_mat = unitMat(k);
/*	for(int i=0; i < k; i++){
        for(int j=0; j < k; j++){
            cout << unit_mat[i][j];
        }
        cout << endl;
	}
    /*范德蒙矩阵和变换后的单位矩阵相乘*/
/*	ma = mulMat(ma,n,k,unit_mat);

	return;
}*/

void ker_encode(TypeData o[],int p){
	//TypeData *a=&(o[(p-1)*ext]);
	
	//int i,iext;
	//for(i=0;i<ext;++i) a[i]=0;
	//for(i=0;i<p-1;++i){
	//	iext=i*ext;
	//	for(int j=0;j<ext;j++,iext++){
	//		a[j]^=o[iext];
	//	}
	//}
	int lo=(p-1)/(8*sizeof(TypeData));//一个int有32=8*4位
	//TypeData *data=(TypeData *)malloc(codlen*sizeof());
    TypeData *a=&(o[lo]);
    for (int i=0;i<lo;++i){
		*a^=o[i];
	}
    for (int i=0;i<8*sizeof(TypeData);++i){		
		if(*a&1){
			*a^=0x00000002;
		} 
		*a>>=1;
	}
}


void addmul(TypeData des[],TypeData src[],poly e, int p, poly base){
	//addmul(codingsum,     data[j],      tot[a][j], p*newextend, newextend)
	poly c=Zero;
	int len =(p-1)/(8*sizeof(TypeData));
	for(int i=0;i<len+1;++i){
		c.v[i]=src[i];
	}
	c=mul(c,e);
	c=mod(c,base);
	for(int i=0;i<len+1;++i){
		des[i]^=c.v[i];
	}	
}

int reduceInv(MatEle **inv,const MatEle& base,int k){
	if(isZero(base)) return -1;
	int count,l=-1,min=100000000;
	int i,j;
	int nz,v[32*S];
	int m=getVector(base,v)/2;
	for(i=0;i<k;++i){
		count=0;
		for(j=0;j<k;++j){
			nz=getVector(inv[i][j],v);
			if(nz>m) {
				inv[i][j]=sub(base,inv[i][j]);
				count+=m+m+1-nz;
			}else count+=nz;
		}
		if(count==1 && isOne(inv[i][i])==true) continue;
		if(count<min) {min=count;l=i;/*printf("ll %d , min %d \n",l,min);*/}
	//	printf("%d , min %d \n",count,min);
	}
	return l;
}
