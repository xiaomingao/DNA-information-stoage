 
#include"jpoly.h"
#include"jmatrix.h"
#include"info.h"
//#include"kernel.h"
#include"jtime.h"


void decoder(char *file)
{
	MetaInfo meta(file);
	//FilesInfo finfo(file);

//	if(finfo.numRepairO > 0) {
//		printf("Some files are lost. Repair them first.\n");
//		return;
//	}


	int k,n,p,blocksize,size,codlen;	
	k=meta.k;
	n=meta.n;
	p=meta.p;
	blocksize=meta.blocksize;
	codlen=meta.codlen;
	size=meta.size;
	int package=meta.package;

	int i;
	
    int bagNum = ceil((float)size / (sizeof(TypeData)*blocksize*k));
	int seedSize = ceil((float)size / (sizeof(TypeData)*blocksize));//注意这个和repair里的不一样,这里是k个 
	
	
	TypeData *coding=(TypeData *)malloc(blocksize*sizeof(TypeData));
	//TypeData **data=(TypeData **)malloc(sizeof(TypeData *)*seedSize);
	//for(i=0;i<k;++i) data[i] = (TypeData *)malloc(codlen*sizeof(TypeData));

	FILE *ft;
	FILE *fp=fopen(file,"w+b");	
	
	

	
	
	Timer timer;
	for (i=0;i<seedSize;i++)
	{		
		if((ft=fopen(meta.filenames[i],"r+b"))==NULL)
		{
			printf("Some files are lost. Repair them first.\n");
			return;
		}
		fclose(ft);
	}


    for(i=0;i<seedSize;++i){
		if(size<=0) break;
	    timer.setTime(1);
		ft=fopen(meta.filenames[i],"r+b");

		fread(coding,sizeof(TypeData),blocksize,ft);//这是按照int读
		//fread(coding,1,sizeof(TypeData)*blocksize,ft);//这是按照字节读
		fclose(ft);
	    timer.getTime(1);
			
    	timer.setTime(2);
		if (size > sizeof(TypeData)*blocksize) {
            fwrite(coding,sizeof(TypeData),blocksize, fp);
		    print(coding, blocksize);
		}	
		else {
            fwrite(coding,1,size, fp);
		}
		size-=sizeof(TypeData)*blocksize;
	    timer.getTime(2);
		}
	
	fclose(fp);

	double time=timer.getTime(0);
	double speed=meta.size/time/1024/1024;
	cout<<"decode file: "<<file<<"\ntime: "<<time<<"s.\n";
	cout<<"speed: "<<speed<<"M/s.\n";
	timer.printAcc(1,"read");
	timer.printAcc(2,"write");
	timer.printAcc(3,"Calculate");
//	free(data);
	free(coding);
//	free(codingsum);
}
int main(int argc,char **argv){
	decoder(argv[1]);

  	//if(argc<2) {
	//	printf("Error, it should have a file name like \"decoder filename\"\n");
	//	return 0;
	//}
	//decoder(argv[1]);
	return 0;
}
