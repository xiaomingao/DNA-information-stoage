#ifdef _WIN32
#include"time.h"
#else
#include "sys/time.h"
#endif

class Timer{
	double t[10];
	double acc[10];
	double getCurrentTime(){

#ifdef _WIN32
		clock_t ct=clock();
		return ct*1e-3;
#else
		struct timeval ct;
		struct timezone tz;
		gettimeofday(&ct, &tz);
		return ct.tv_usec/1e+6+ct.tv_sec;
#endif
	}
public:
	Timer(){
		double ct=getCurrentTime();
		for(int i=0;i<10;++i) {t[i]=ct;acc[i]=0;}
	}
	void setTime(int id){
		t[id]=getCurrentTime();
	}
	double getTime(int id){
		double ct=getCurrentTime();
		double dif=ct-t[id];
		t[id]=ct;
		acc[id]+=dif;
		return dif;
	}
	void print(int id,const char* label){
		double ct=getCurrentTime();
		double dif=ct-t[id];
		t[id]=ct;
		printf("%s : %f s\n",label,dif);
	}
	void printAcc(int id,const char* label){
		printf("%s : %f s\n",label,acc[id]);
	}
};
