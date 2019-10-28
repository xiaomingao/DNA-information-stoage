#include"stdio.h"
#include"string.h"
#pragma once
#ifndef POLY_H
#define POLY_H
#define S 20


#ifdef _WIN32
	#define INT64 __int64
#else
	#define INT64 long long
#endif

struct poly{
	unsigned int v[S];
};
poly Poly(unsigned INT64 p);

const int polyLen=sizeof(poly)*8;
const poly Zero=Poly(0);
const poly One=Poly(1);

poly Polly(char * a,int n);
/*
#define willcarry(p) ((p.v[S-1]&0x80000000)!=0)
#define orEq1(i) (i.v[0]|=1)
#define and1(i)	(i.v[0]&1)
#define andEq1(i) (i.v[0]&=1)
*/
void xorEq(poly & a,poly & b);
poly _xor(poly & a,poly & b);


void shl(poly & p,int n);
void shr(poly & p,int n);
int getVector(const poly & p,int v[]);
bool isZero(const poly & p);
bool isOne(const poly & p){
	if(p.v[0]!=1) return false;
	for(int i=1;i<S;++i) if(p.v[i]) return false;
	return true;	
};

void print(const poly& p);
int getMaxBit(poly c);

//algorithm
poly add(poly a,poly b);

poly sub(poly a,poly b);
poly mul(poly a,poly b);

poly div(poly x,poly y);

poly mod(poly x,poly y);
poly __inverse(poly a,poly b,poly a1,poly a2,poly b1,poly b2);

poly inverse(poly a,poly base);
/*poly mul(poly x,poly y,poly z) ;/*{return mul(mul(x,y),z);}*/
poly add(poly x,poly y,poly z) ;/*{return add(add(x,y),z);}*/
poly mulmod(poly x,poly y,poly m) ;/*{return mod(mul(x,y),m);}*/

#ifdef TUPLE_H
Tuple divmod(poly x,poly y);
Tuple gcdEX(poly a,poly b,poly a1=One,poly a2=Zero,poly b1=Zero,poly b2=One);
#endif

poly gcd(poly a,poly b);
#endif

#include"jpoly.h"

poly Poly(unsigned INT64 p){
	poly t;
	t.v[0]=(unsigned int)p;
	t.v[1]=(unsigned int)(p>>32);
	for(int i=2;i<S;++i) t.v[i]=0;
	return t;
}

/*
poly Polly(char * a,int n){
	poly c=Zero;
	n--;
	while(n>=0){
		if(a[n]!=0) andEq1(c);
		shl(c,1);
		n--;
	}
	return c;
}*/

#define willcarry(p) ((p.v[S-1]&0x80000000)!=0)
#define orEq1(i) (i.v[0]|=1)
#define and1(i)	(i.v[0]&1)
#define andEq1(i) (i.v[0]&=1)

void xorEq(poly & a,poly & b){
	for(int i=0;i<S;++i) a.v[i]^=b.v[i];
}
poly _xor(poly & a,poly & b){
	poly c;
	for(int i=0;i<S;++i) c.v[i]=a.v[i]^b.v[i];
	return c;
}


void shl(poly & p,int n){
	for(;n>32;n-=32){
		for(int i=S-1;i>0;i--) p.v[i]=p.v[i-1];
		p.v[0]=0;
	}
	for(;n>0;n--){
		for(int i=S-1;i>0;i--) {
			p.v[i]<<=1;
		//	if(p.v[i-1]&0x80000000) 
			if((int)p.v[i-1]<0) 
				p.v[i]|=1;
		}
		p.v[0]<<=1;
	}
}
void shr(poly & p,int n){
	for(;n>32;n-=32){
		for(int i=0;i<S-1;i++) p.v[i]=p.v[i+1];
		p.v[S-1]=0;
	}
	for(;n>0;n--){
		for(int i=0;i<S-1;i++) {
			p.v[i]>>=1;
			if((p.v[i+1]&1)==1) 
				p.v[i]|=0x80000000;
		}
		p.v[S-1]>>=1;
	}
}
int getVector(const poly & p,int v[]){
	poly a=p;
	int c=0,i;
	int tv[32*S];
	for(i=0;i<S*32;++i){
		if(a.v[0]&1){
			tv[c]=i;
			c++;
		}
		shr(a,1);
	}
	for(i=c-1;i>=0;--i){
		v[i]=tv[c-i-1];//v[i]为从高位到低位第i个1的位置 
	}
	return c;//c为1的总数 
}
bool isZero(const poly & p){
	for(int i=0;i<S;++i) if(p.v[i]) return false;
	return true;
}
/*
bool le(const poly& a,const poly& b){
	for(int i=0;i<S;++i)	
		if(a.v[i]<b.v[i]) return false;
	return true;
}*/

void print(const poly& p){
	poly t=p;
	if(isZero(t)){
		printf("( $) \t");
		return;
	}	
	int i=0;
	printf("(");
	while(!isZero(t) && i<32*S){//
		if(and1(t)) printf(" %d",i);
		shr(t,1);
		i++;
	}
	printf(") \t");
}//*/
/*
void print(const poly& p){
	poly t=p;

	printf("(");
	if(isZero(t)){
		printf("0)\n");
		return;
	}	
	int i=0;
	while(!isZero(t) && i<32*S){//
		if(and1(t)) printf(" +x^%d",i);
		shr(t,1);
		i++;
	}
	printf(") ");
	printf("\n");
}//*/
int getMaxBit(poly c){
	if(isZero(c)) return 0;
	int i=0;
	while(!isZero(c)){
		shr(c,1);
		i++;
	}
	return i;
}

//algorithm
poly add(poly a,poly b){
	return _xor(a,b);
}
poly sub(poly a,poly b){
	return _xor(a,b);
}
poly mul(poly a,poly b){
	poly c=Zero;
	if(isZero(a) || isZero(b)) return c;
	while(!isZero(b)){
		if(and1(b)) xorEq(c,a);
		shr(b,1);
		shl(a,1);
	}
	return c;
}
poly div(poly x,poly y){
	if(isZero(x)) return Zero;
	if(isZero(y)) {printf("Error: divided by 0\n"); return Zero;}
	int pa=getMaxBit(x);
	int pb=getMaxBit(y);
	if(pa<pb) {return Zero;}

	shl(x,(polyLen-pa));
	shl(y,(polyLen-pb));
	int d=pa-pb;
	
	if(willcarry(x)) {xorEq(x,y);}
	shl(x,1);
	poly c=One;
	
	for(;d>0;d--){
		shl(c,1);
		if(willcarry(x)) {xorEq(x,y);orEq1(c);}
		shl(x,1);
	}
	return c;
}

poly mod(poly x,poly y){
	if(isZero(x)) return x;
	if(isZero(y)) {printf("Error: divided by 0\n"); return x;}
	int pa=getMaxBit(x);
	int pb=getMaxBit(y);
	if(pa<pb) {return x;}

	shl(x,(polyLen-pa));
	shl(y,(polyLen-pb));
	int d=pa-pb;
	
	if(willcarry(x)) {xorEq(x,y);}	
	for(;d>0;d--){
		shl(x,1);
		if(willcarry(x)) {xorEq(x,y);}	
	}
	shr(x,(polyLen-pb));
	return x;
}

int mod(int x, int y) {
	if (x==0) return x;
	if (y==0) { printf("Error: divided by 0\n"); return x; }
	
	//if (x<y) { return x; }

	while (x >= y) { x = x - y; }

	return x;
}

poly __inverse(poly a,poly b,poly a1,poly a2,poly b1,poly b2){
	if(isZero(b)) {printf("Warning, base shouldn't be zero\n");return Zero;}
	if(isZero(a)) return b1;
	poly d=mod(b,a);
	if(isZero(d)) return a1;
	poly c=div(b,a);
	return __inverse(d,a,sub(b1,mul(a1,c)),sub(b2,mul(a2,c)),a1,a2);
}
poly inverse(poly a,poly base){
	return __inverse(a,base,One,Zero,Zero,One);
}
/*
poly mul(poly x,poly y,poly z) {return mul(mul(x,y),z);}
poly add(poly x,poly y,poly z) {return add(add(x,y),z);}
poly mulmod(poly x,poly y,poly m) {return mod(mul(x,y),m);}*/

#ifdef TUPLE_H
Tuple divmod(poly x,poly y){
	poly q=Zero,r=Zero;
	if(isZero(x)) return Tu(q,r);
	if(isZero(y)) {printf("Error: divided by 0\n"); return Tu(q,r);}
	int pa=getMaxBit(x);
	int pb=getMaxBit(y);
	if(pa<pb) {return Tu(q,r);}

	shl(x,(polyLen-pa));
	shl(y,(polyLen-pb));
	int d=pa-pb;
	
	if(willcarry(x)) {xorEq(x,y);}
	shl(x,1);
	poly c=One;
	
	for(;d>0;d--){
		shl(c,1);
		if(willcarry(x)) {xorEq(x,y);orEq1(c);}
		shl(x,1);
	}
	q=c;
	shr(x,(polyLen-pb+1));
	r=x;
	return Tu(q,r);	
}
Tuple gcdEX(poly a,poly b,poly a1=One,poly a2=Zero,poly b1=Zero,poly b2=One){
	if(isZero(a)) return Tu(b1,b2);
	poly c=div(b,a);
	return gcdEX(mod(b,a),a,sub(b1,mul(a1,c)),sub(b2,mul(a2,c)),a1,a2);
}
#endif

poly gcd(poly a,poly b){
	if(isZero(a)) return b;
	return gcd(mod(b,a),a);
}
