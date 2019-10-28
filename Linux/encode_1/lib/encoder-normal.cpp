
#include"jpoly.h"
#include"jmatrix.h"
#include"info.h"
//#include"kernel.h"
#include"jtime.h"
#include"filter.h"

//#include <math.h> double floor(double x);


//typedef unsigned char byte;

//void encoder(char *file,int k,int n,int p)
void encoder(char *file)
{
	int k = KK;
	int n = NN;
	int p = PP;
	MetaInfo meta(file, k, n, p);
	int m = n - k;
	int blocksize, size, newextend, codlen;
	int package = meta.package;
	blocksize = meta.blocksize;
	codlen = meta.codlen;
	//newextend=meta.codlen-meta.blocksize;
	poly base = meta.base;
	poly base0 = meta.base0;
	size = meta.size;


	/////////////////////////////////////////////////�������Ŀ
	//int seedsize = 0;
	int seedhigh = 0;
	//seedsize = ceil((float)size / (sizeof(TypeData)*blocksize*k));
	int bagNum = ceil((float)size / (sizeof(TypeData)*blocksize*k));
	//	package=meta.package;


	TypeData **data = (TypeData **)malloc(sizeof(TypeData *)*k);//��ʼ����+��żУ��λ
	for (int i = 0; i < k; ++i) data[i] = (TypeData *)malloc(codlen * sizeof(TypeData));
	TypeData **data2 = (TypeData **)malloc(sizeof(TypeData *)*k);//�м���
	for (int i = 0; i < k; ++i) data2[i] = (TypeData *)malloc(codlen * sizeof(TypeData));
	TypeData **codingsum = (TypeData **)malloc(sizeof(TypeData *)*n);//����任��
	for (int i = 0; i < n; ++i) codingsum[i] = (TypeData *)malloc(codlen * sizeof(TypeData));
	//TypeData *=(TypeData *)malloc(codlen*sizeof(TypeData));

	////////////////////////////////////////////////////////////////////����ѭ��
	TypeData *addploy = (TypeData *)malloc(codlen * sizeof(TypeData));
	memset2(addploy, 0, codlen);
	//memset2(addploy, 27, blocksize);///////27=ACGU
	//int div = 0;
	//int remind = 1;
	TypeData *temp = (TypeData *)malloc(codlen * sizeof(TypeData));


	int numLow[NN] = { 0 };
	//int numHigh = 0;

	////////////////////////////////////////////////////////////////////
	//print(tot,n,k);

	Timer timer;
	//FILE *fop=fopen(file,"r+b");
	FILE *fp;


	/////////////////////////////////////////////////////////////////seed

	//poly ** tot1 = codMat(n, p, p, base);


	TypeData *seed = (TypeData *)malloc(sizeof(TypeData));//����
	//print(tot, n, p);
	//int high = 0;

	for (int t = 0; t < bagNum;++t) {

		FILE *fop = fopen(file, "r+b");
        fseek(fop, t*k*package, SEEK_SET);


		memset2(addploy, randNN, blocksize);///////randNN�����������һ��������

		//print(addploy, codlen);

		for (int i = 0; i < k; ++i) {
			memset2(data[i], 0, codlen);
			memset2(data2[i], 0, codlen);
		}
		//print(data[k - 1], codlen);
	    //////////////////////////////////////////////////////////////��ȡ�ļ�������żУ��λ
		for (int i = 0; i < k; ++i) {
			if(size < 0)break;

		    if (size > sizeof(TypeData)*blocksize) {
		        fread(data[i],1, sizeof(TypeData)*blocksize, fop);
 	//			print(data[i], codlen);
		        size -= sizeof(TypeData)*blocksize;
		    }
		    else {
		     	fread(data[i], 1, size, fop);
				size -= sizeof(TypeData)*blocksize;
		    }
	     }
		fclose(fop);

		for (int i = 0; i < k; ++i) {
		///////////////////////////////////��żУ��
            ker_encode(data[i],p);
			//print(data[i], codlen);
		}
        ///////////////////////////////////////////////////////////////////////////////////


		int filte1 = 0;


        int a;
		int num = 0;//�����ж��Ƿ�ǰ����������
		//numHigh = 0;//��¼����ѭ������
		while (num < n) {
			poly ** tot = codMat(n, k, p, base);//////��ʼ�����ɾ���
			num = 0;
			for (a = 0; a < k; ++a) {
	        	memcpy2(data2[a], data[a], codlen);
				//print(data[a], codlen);
			//	print(data2[a], codlen);
				//numLow[a] = 0;
				memadd(data2[a], addploy, codlen);//��һ���������ֹȫ0����
				//print(data2[a], codlen);
			}


			int filte2 = 0;
			//int numm = 0;

			for (a = 0; a < n && filte2 == 0; ++a) {

				numLow[a] = 0;//&& mod(numLow[a], n)
				int filte = 0;
				while (filte == 0 && numLow[a]<n+1) {
					memset2(codingsum[a], 0, codlen);
					//print(codingsum[a], codlen);

					for (int j = 0; j<k; ++j) {
						addmul(codingsum[a], data2[j], tot[a][j], p, base);//����任
						//print(codingsum[a], codlen);
					}
					//print(codingsum[a], codlen);
					if (filter(codingsum[a]) == 0) {//ɸѡ
						//tot[a]=nextVector(tot[a], k);
						tot[a] = nextVector(tot[a], k);
						numLow[a]++;
					}
					else
					{
						filte = 1;
						num++;
						//printf("%d \n", numLow[a]);
					}
				}

		/*		if (numLow[a] == n+1) {
					filte2 = 1;
					numHigh++;
					addploy=nextVector(addploy);//��ǰ��������ʣ�Ѱ����һ��
					//print(addploy, codlen);
				}
				*/

			}
			//num = a;
			printf("%d \n", num);
			//print(tot, n, k);

		}



		for (a = 0; a < n; ++a) {
			*seed = t * 256 * 256 + a * 256 + numLow[a];
			//printf("%d \n", *seed/256/256);
			//*seed = t;//��8λΪt��Ϊ��λ���к�
			//*seed <<= 8;
			//*seed ^= a;//�и�λ8Ϊa��Ϊ��λ���к�
            //*seed <<= 8;
			int nameF = t*256+a;

          //  *seed ^= numHigh;//�е�λ8λΪnumHigh��Ϊ��ѭ������
			//*seed <<= 8;
			//*seed ^= numLow[a];//��λ8λΪnum��ΪСѭ������

			fp = fopen(meta.filenames[nameF], "w+b");//����һ���ļ�filenames_nameF
            fwrite(seed, sizeof(TypeData), 1, fp);//�洢seed
			//fwrite(num,1, 1,fp);//�洢num
			fwrite(codingsum[a], sizeof(TypeData), blocksize, fp);//�洢
		//	print(seed, 1);
		//	print(codingsum[a], blocksize);
            fclose(fp);
		}



		//fwrite(codingsum,1, package,fp);

		timer.getTime(2);
		//print(tot, n, k);

	}



	meta.saveFile();
	double time=timer.getTime(0);
	double speed=size/time/1024/1024;
	cout<<"encode file: "<<file<<"\ntime: "<<time<<"s.\n";
	cout<<"speed: "<<speed<<"M/s.\n";
	timer.printAcc(1,"read");
	timer.printAcc(2,"write");
	timer.printAcc(3,"Calculate");
	for(int i=0;i<k;++i) free(data[i]);
	free(data);
	free(codingsum);
}


int main(int argc,char **argv){
	encoder(argv[1]);
//	if(argc<1)
//	{
//		printf("Please spicify the location of the encode file!");
//	}
//	else
//	{
//		if(argc<2)
//			{
//				encoder(argv[1]);
//				return 0;
//			}
//			else
//			{
//				printf("Less opt!");
//			}
//	}	
}
