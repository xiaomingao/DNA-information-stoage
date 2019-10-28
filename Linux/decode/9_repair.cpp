//#include"windows.h"

#include"jpoly.h"
#include"jmatrix.h"
#include"info.h"
//#include"kernel.h"
#include"jtime.h"

void repair(char *file)
{
	MetaInfo meta(file);
	int k,n,p,blocksize,size,newextend,codlen;
	poly base = meta.base;
	poly base0 = meta.base0;
	k=meta.k;
	n=meta.n;
	p=meta.p;
	blocksize=meta.blocksize;
	codlen=meta.codlen;
	size=meta.size;
	//newextend=meta.codlen-meta.blocksize;

	FilesInfo finfo(file);


	//int m=n-k;

	TypeData **data2=(TypeData **)malloc(sizeof(TypeData *)*k);//存储变换后的

	TypeData **data=(TypeData **)malloc(sizeof(TypeData *)*k);//存储变换前的

	for(int i=0;i<k;++i) {

		data2[i]=(TypeData *)malloc(codlen*sizeof(TypeData));
		data[i]=(TypeData *)malloc(codlen*sizeof(TypeData));
	}

	FILE *fp;
	poly **mc=(poly**)malloc(sizeof(poly*)*k);
	

	TypeData *seed = (TypeData *)malloc(sizeof(TypeData));

	TypeData *addploy = (TypeData *)malloc(codlen * sizeof(TypeData));//用来循环的
	
	memset2(addploy, 0, codlen);///////randNN这个是随便设的一个非零数

	//poly **inv;
	MatEle ** inv= createMatrix(k,k);


	int package=meta.package;
	int numLow;
	int numHigh;
	Timer timer;
	
	int bagNum = ceil((float)size / (sizeof(TypeData)*blocksize*k));
	int seedSize = n*bagNum;

	int numS = 0;//用来记录解码后的数据


	for(int t=0;t<bagNum;++t){
		poly **tot = codMat(n, k, p, base);
	//	int livenum=0;
	//	int plc=0;
	//	int ilc=0;

	    timer.getTime(3);
    	    timer.setTime(1);
			//print(tot, k, k);

		for(int i=0;i<k;++i){

			fp=fopen(meta.filenames[finfo.cexist[t][i]],"r+b");
				
			//fseek(fp,t*package,SEEK_SET);

			fread(seed, sizeof(TypeData), 1, fp);

			numLow = *seed & 255;//低八位
			//printf("%d\n", numLow);
			//*seed >>= 8;
			//numHigh = *seed & 255;//高八位
			mc[i]=tot[mod(finfo.cexist[t][i],256)];
			for (int a = 0; a < numLow; ++a) {
				mc[i]=nextVector(mc[i], k);
			}
			memset2(data[i], 0, codlen);
			fread(data[i],sizeof(TypeData),blocksize,fp);	
			//print(data[i], blocksize);
			fclose(fp);
		}


     	timer.getTime(1);

	    timer.setTime(3);
		for(int i=0;i<k;++i){
			ker_encode(data[i],p);
		}
		printf("0--------------------\n");
		//print(mc, k, k);
		MatEle ** inv = createMatrix(k, k);
		inv=inverse(mc,base0,k);
		//print(inv, k, k);
		

        for(int i=0;i<k;++i){
            memset2(data2[i],0,codlen);
			print(data2[i], codlen);
			for(int j=0;j<k;++j){
				//print(data[j], codlen);
				addmul(data2[i],data[j], inv[i][j],p,base);
				//print(data2[i], codlen);
			}
			//print(data2[i], codlen);
        }


		//MatEle mcc = det(mc, k);
 		memset2(addploy, 0, codlen);
		memset2(addploy, randNN, blocksize);///////randNN这个是随便设的一个非零数
		
        /*for (int i = 0; i < numHigh; ++i) {
			addploy = nextVector(addploy);
		}
		//memadd(data2[mod(numHigh, k)], addploy, codlen);
		*/
		for (int i = 0; i < k; ++i) {
			memadd(data2[i], addploy, codlen);
		//	print(data2[i], codlen);
		}
        
	    timer.getTime(3);

	    timer.setTime(2);
		for (int i=0;i<k;++i)
		{

			//if(finfo.flag[i]!=0) continue;
			fp=fopen(meta.filenames[numS],"w+b");
	 //       print(data2[i], blocksize);
			//fwrite(data2[i],1,package,fp);
            fwrite(data2[i],sizeof(TypeData),blocksize,fp);
			
			fclose(fp);
			numS++;
		}
		printf("3--------------------\n");
		//MatEle mcc = det(mc, k);
		//poly **invmc;
		//invmc = mul(inv, mc);
	}



	double time=timer.getTime(0);
	double speed=size/time/1024/1024;
	cout<<"decode file: "<<file<<"\ntime: "<<time<<"s.\n";
	cout<<"speed: "<<speed<<"M/s.\n";
	timer.printAcc(1,"read");
	timer.printAcc(2,"write");
	timer.printAcc(3,"Calculate");
	//free(name);
	for(int i=0;i<k;++i) free(data[i]);
	for(int i=0;i<k;++i) free(data2[i]);
	free(data);
    free(data2);
    free(mc);
}

int main(int argc,char **argv){

	repair("./decode/decode_file");
 // 	if(argc<2) {
//		printf("Error, it should have a file name like \"repair filename\"\n");
//		return 0;
//	}
//	repair(argv[1]);
	return 0;
}
