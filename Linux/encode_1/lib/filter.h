#pragma once

#define TypeData unsigned int


bool filter(TypeData o[]){
	TypeData c[128];
	//int len =8;
	TypeData p;
	for(int i=0;i<8;++i){
		p=o[i];
		for(int j=0;j<16;++j){
			c[16*i+15-j]=p&0x00000003;
			p>>=2;
		}	
	}
 	for(int i=0;i<125;++i){
		if(c[i]==0&& c[i+1]==0&& c[i+2]==0&& c[i+3]==0)return false;//AAAA
		else if(c[i]==1&& c[i+1]==1&& c[i+2]==1&& c[i+3]==1)return false;//CCCC
		else if(c[i]==2&& c[i+1]==2&& c[i+2]==2&& c[i+3]==2)return false;//GGGG
		else if(c[i]==3&& c[i+1]==3&& c[i+2]==3&& c[i+3]==3)return false;//UUUU
	}

    if(c[122]==0&& c[123]==1&& c[124]==2&& c[125]==0&& c[126]==3&& c[127]==1)return false;//    ACGAUC
 	int au=0;
    for(int i=0;i<128;++i){
		if (c[i]==0||c[i]==3)au++;
	}
	//float h;
    //h=(float)au/128;
	if (au<0.35*128|| au>0.65*128) 	return false;
	
	return true;	
};