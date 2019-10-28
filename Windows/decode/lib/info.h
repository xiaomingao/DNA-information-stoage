
#include <sys/stat.h>
#define MatEle poly
//#define TypeData char
#define TypeData unsigned int

#include"kernel.h"

class MetaInfo{
public:
	char * filename;
	int n,k,p;
//	int B;
	MatEle base,base0;
	char ** filenames;
	int blocksize;
	int codlen;
	int size;
	int package;
	int readins;

	MetaInfo(const char * filename0,int k0,int n0,int p0){
		n=n0;
		k=k0;
		p=p0;
		//num=num0;
		package=(p-1)/8;
		checkprime(p0);
		checkFile(filename0);
		filename=new char[strlen(filename0)];
		strcpy(filename,filename0);
		struct stat status;
		stat(filename, &status);
		size = status.st_size;
		setFileNames();


		int m=n-k;
		base=One;
		shl(base,p);
		base0=div(add(base,One),Poly(3));
		base=add(base,One);

		//package=package0;
		//int pack=package/((p-1)*sizeof(TypeData));
		blocksize=(p-1)/(8*sizeof(TypeData));//每一条有blocksize个int的有效信息
		codlen=blocksize+1;// 每一条长codlen个int
		//blocksize=pack*(p-1);
		//readins=size/(blocksize*sizeof(TypeData)*k)+1;
		readins=size/(blocksize*sizeof(TypeData)*k)+1;
		printf("%s %d %d %d %d %d\n",filename,size,k,n,p);

		printf("Size of DataType is %d\n",(int)sizeof(TypeData));
	}
	MetaInfo(const char * metafile){
		char name[200];
		sprintf(name,"%s_meta.txt",metafile);
		checkFile(name);
		FILE* meta=fopen(name,"r");
		if(meta==NULL) exit(0);
		fscanf(meta,"%s %d %d %d %d %d %d %d %d",name,&size,&k,&n,&p,&package,&readins,&blocksize,&codlen);
		checkprime(p);
		filename=new char[strlen(name)];
		strcpy(filename,name);
		printf("%s %d %d %d %d %d\n",filename,size,k,n,p);
		fclose(meta);
		setFileNames();

		base=One;
		shl(base,p);
		base0=div(add(base,One),Poly(3));
		base=add(base,One);

//		codlen/=sizeof(TypeData);
//		blocksize/=sizeof(TypeData);
	}
	void saveFile(){
		char name[200];
		sprintf(name,"%s_meta.txt",filename);
		FILE *meta=fopen(name,"w");
		fprintf(meta,"%s %d %d %d %d %d %d %d %d\n",filename,size,k,n,p,package,readins,blocksize,codlen);
		//fprintf(meta,"%d %d %d %d %d %d\n",k,n,p,blocksize,size,codlen-blocksize);
		fclose(meta);
	}
private:
	void setFileNames(){//很重要****//文件起名字，以后实际应用中可能需要改

		//int seedsize = ceil((float)size / (sizeof(TypeData)*blocksize*k));
		//int N = n*seedsize;
		int N = 20000;
		filenames=(char**)malloc(sizeof(char**)*N);//
		for(int i=0;i<N+1;++i) {
			filenames[i]=(char*)malloc(sizeof(char)*200);
			sprintf(filenames[i],"%s_cod%d",filename,i);
		}
	}
	void checkFile(const char* file){
		FILE *fp=fopen(file,"r+b");
		if (fp==NULL)
		{
			printf("%s is not exist.\n",file);
			exit(0);
		}
		fclose(fp);
	}
	bool isPrime(int p){
		if(p<=0) return false;
		if(p%2==0) return false;
		for(int i=3;i<p/2;++i) if(p%i==0) return false;
		return true;
	}
	int checkprime(int p){
		/*
		int list[]={ 3, 5, 11, 13, 19, 29, 37, 53, 59, 61, 67, 83, 101};
		int num=sizeof(list)/sizeof(int);
		for(int i=0;i<num;++i) if(list[i]==p && p>=n-k && p>=k) return 1;
		printf("Error, p = %d , p can only be one of the following:\n",p);
		for(int j=0;j<num;++j) if(list[j]>=n-k && list[j]>=k) printf("%d ",list[j]);
		printf("\n");
		exit(0);//*/
		if( isPrime(p)==false) {
			printf("Error, p = %d , p should be a prime :\n",p);
			//if(p<k) p=k;
			//if(p<n-k) p=n-k;
			while(isPrime(p)==false) p++;
			printf("Now, p = %d instead\n",p);
		}
		return 1;
	}
};
class FilesInfo{
public:
	int n, k, p;

	int cflag[20000],cexist[300][300];
	int flag[20000],olost[20000];
//	int numRepairO[1000];
	int numRepairC[1000];
	MatEle** mc;
	MatEle base;
	int lastLostO;

	FilesInfo(const MetaInfo& meta){

		FILE *fp;
		int goodCode[1000] = { 0 };
		n=meta.n;
		k=meta.k;
		p=meta.p;
		int size = meta.size;
		int blocksize = meta.blocksize;
		//num=meta.num;
		int bagNum = ceil((float)size / (sizeof(TypeData)*blocksize*k));
		int seedSize = n*bagNum;
		//lastLostO=-1;
		for (int a = 0; a < bagNum; ++a) {
	//		numRepairO[a] = k;
			numRepairC[a]=0;
		}
		for (int t = 0; t < bagNum; ++t) {
			for (int i=0;i<n;++i)
			{
		        //int j;
			    if ((fp=fopen(meta.filenames[t*256+i],"r+b"))!=NULL){//检查文件是否存在//以后实际应用中可能需要改，因为目前还不清楚生物组那边解码后的文件具体是什么格式的
				//**很重要//每一条链的地址我都保存到链里面了，所以到时候可能不需要这些，但是需要在链内查找和整理文件
				//cexist用来记录目前存在的c链,cexist[i]=j表示第i个存在的链是链j
				//cflag和flag[i]用来记录第i条链是否存在
				    //j = i / n;
			    	cexist[t][i] = t*256 + i;
			    	cflag[t*256 + i] = 1; flag[t*256 + i] = 1;
			     	printf("exist c %d %d\n",i, cexist[t][i]);
			    	fclose(fp);
			    }else {
				    flag[t*256 + i] = 0; cflag[t*256 + i] = 0;
			    	//j = i / n;
			     	numRepairC[t]++;
			    }
		    }
		}


		for (int a = 0; a < bagNum; ++a) {
            if(numRepairC[a]>n-k) {
			    printf("There is %d files broken, and could not repair.\n",numRepairC[a]);
			    exit(0);
		    }
		}

		base=meta.base;
		//mc=Filematrix(cflag,p,base,num);
		//print(mc,numRepairO,numRepairO);
	}
	FilesInfo(const MetaInfo& meta,int** sysMat,TypeData ** odata,int line){
		int i;
		FILE *fp;
		int goodCode[100] = { 0 };
		n=meta.n;
		k=meta.k;
		p=meta.p;
		//lastLostO=-1;
		int size = meta.size;
		int blocksize = meta.blocksize;

		int bagNum = ceil((float)size / (sizeof(TypeData)*blocksize*k));
		int seedSize = n*bagNum;
		//lastLostO=-1;
		for (int a = 0; a < bagNum; ++a) {
		//	numRepairO[a] = k;
			numRepairC[a] = 0;
		}

		//int j;
		for (int t = 0; t < bagNum; ++t) {
			for (int i = 0; i<n; ++i)
			{
				//int j;
				if ((fp = fopen(meta.filenames[t*256 + i], "r+b")) != NULL) {//检查文件是否存在//以后实际应用中可能需要改，因为目前还不清楚生物组那边解码后的文件具体是什么格式的
																		   //**很重要//每一条链的地址我都保存到链里面了，所以到时候可能不需要这些，但是需要在链内查找和整理文件
																		   //cexist用来记录目前存在的c链,cexist[i]=j表示第i个存在的链是链j
																		   //cflag和flag[i]用来记录第i条链是否存在
																		   //j = i / n;
					cexist[t][i] = t*256 + i;
					cflag[t*256 + i] = 1; flag[t*256 + i] = 1;
					printf("exist c %d %d\n", i, cexist[t][i]);
					fclose(fp);
				}
				else {
					flag[t*256 + i] = 0; cflag[t*256 + i] = 0;
					//j = i / n;
					numRepairC[t]++;
				}
			}
		}


		for (int a = 0; a < bagNum; ++a) {
			if (numRepairC[a]>n - k) {
				printf("There is %d files broken, and could not repair.\n", numRepairC[a]);
				exit(0);
			}
		}

		base = meta.base;

	}


	MatEle **Filematrix(int exist[], int p, MatEle base, int num){
		MatEle **ma=(MatEle**)malloc(sizeof(MatEle*)*k);
//		MatEle **mb = (MatEle**)malloc(sizeof(MatEle*)*n);

		poly **mb = totalMat(n, k, p, base, num);

		//MatEle p;
		int i=0;
		//	printf("+-+-+-+-+-++-+-+-+-+-\n");
		for(int t=0;t<n;++t){
			if(exist[t]==0) continue;
			ma[i] = (MatEle*)malloc(sizeof(MatEle)*k);
			for (int j = 0; j < k; ++j) {
				ma[i][j] = mb[t][j];
			}


			i++;
			if(i>=k) break;
		}
		//printf("+-+-+-+-+-++-+-+-+-+-\n");
		return ma;
	}
};

