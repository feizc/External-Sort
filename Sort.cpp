#include <iostream>
#include <string>
#include <algorithm>
#include <time.h>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <float.h> 
#include "number_judge.h"

using namespace std;
char txt_input[50],txt_output[50]; //�����������ļ���ַ�� 
const int mem_size=1000000; //ÿ�ζ�ȡ�ֽ���->�ڴ�ռ䡣 
long long total_number=0,ilnumber=0; //���򸡵����������Ƿ���Ŀ������ 
vector<double> sortnum;  
int index=0; //������ʱ�ļ��ĸ�����
char filename[64]; 
const double minDouble=DBL_MIN;
const double maxDouble=DBL_MAX;

//��ȡ��������ļ���ַ�� 
void read_param(){
	FILE *fp;
	fp=fopen("Sort.param","rb");
	char tmp1[1024],tmp2[1024];
	fgets(tmp1,1024,fp);
	fgets(tmp2,1024,fp);
	memset(txt_input,0,sizeof(txt_input));
	memset(txt_output,0,sizeof(txt_output));
	int i=0;
	int l1=0;
	while(tmp1[i]!='=') ++i;
	++i;
	while(tmp1[i]!=';'){
		txt_input[l1++]=tmp1[i++];
	}
	int l2=0;
	i=0;
	while(tmp2[i]!='=') ++i;
	++i;
	while(tmp2[i]!=';'){
		txt_output[l2++]=tmp2[i++];
	}
	cout<<"�ļ�·��������ȡ"<<endl;
	cout<<"�����ļ���ַ��"; 
	for(int i=0;i<l1;i++) cout<<txt_input[i];
	cout<<endl;
	cout<<"����ļ���ַ��";
	for(int i=0;i<l2;i++) cout<<txt_output[i];
	cout<<endl;
	fclose(fp);
}

//������д����ʱ�ļ��� 
void write_data(){
	FILE *stream;
	sprintf(filename,"temp%d.txt",index);
	stream=fopen(filename,"wb");
	int lt=sortnum.size();
	total_number+=lt;
	cout<<"��ʼд���"<<index+1<<"����ʱ�ļ�"<<endl; 
	fwrite(&sortnum[0],1,lt*sizeof(double),stream);
	fclose(stream);
	index++;
	cout<<"д����ɣ�"<<endl; 
}

//�ֿ��ȡ���ݡ� 
void read_data(){
	FILE *fp1;
	fp1=fopen(txt_input,"rb");
	if(fp1==NULL) cout<<"�ļ������ڻ���·������"<<endl;
	char mem[mem_size+50];
	int length=mem_size;
	double number=0.0;
	bool isnumber=false;
	char *start,*cur;
	int step=0,len=0,count=0;	
	fseek(fp1,0,SEEK_SET);
	cout<<"��ʼ��ȡ����"<<endl;
	while(length==mem_size){
		length=fread(mem+step,sizeof(char),mem_size,fp1);
		mem[length+step]=0;
		count++;
		start=mem;
		cur=mem;
		while(true){
			while(*cur!=0&&*cur!='\n') cur++;
			len=cur-start;
			if(*cur!=0) cur++;
			else{
				if(*(cur-1)!='\n'){
					step=len;
					for(int i=0;i<step;i++) mem[i]=*(start+i);
					if(length!=mem_size){
						if(isvalid(start,len)){
							number=aatof(start,len);
							sortnum.push_back(number);
						}else ilnumber++;
					}
				}else{
					step=0;
				}
				break;
			}
			if(isvalid(start,len)){
				number=aatof(start,len);
				sortnum.push_back(number);
			}else ilnumber++;
			start=cur;
		}
		if(count==100){
			cout<<"���ַǷ���Ŀ�� "<<ilnumber<<endl;
			sort(sortnum.begin(),sortnum.end());
			cout<<"д����ʱ�ļ�"<<endl;
			write_data();
			sortnum.clear();
			count=0;
		}		
	}	
	if(count){
			cout<<"���ַǷ���Ŀ�� "<<ilnumber<<endl;
			sort(sortnum.begin(),sortnum.end());
			cout<<"д����ʱ�ļ�"<<endl;
			write_data();
			sortnum.clear();
			count=0;
		}	
	fclose(fp1);
	cout<<"�ܹ��Ƿ���Ŀ��"<<ilnumber<<endl;
}

void adjust(int &fcount,int* &root,double* &leaf,int i){
	int t=(i+fcount)/2;
	int s=i;
	while(t>0){
		if(leaf[root[t]]<leaf[s]) swap(s,root[t]);
		t=t/2;
	}
	root[0]=s;
} 

void createtree(double* arr[],int &fcount,int* &root,double* &leaf){
	for(int i=0;i<fcount;++i) leaf[i]=arr[i][0];
	leaf[fcount]=minDouble;
	for(int i=0;i<fcount;++i) root[i]=fcount;
	for(int i=fcount-1;i>=0;--i) adjust(fcount,root,leaf,i);
}

void merge(){
	int fcount=index;
	cout<<"�ܹ���"<<fcount<<"����ʱ�ļ���"<<endl;
	char writebuf[mem_size+50];
	char *wstart=&writebuf[0];
	FILE *fp2;
	fp2=fopen(txt_output,"wb");
	if(fp2==NULL) cout<<"д���ļ���ʧ�ܣ�"<<endl;
	else cout<<"д���ļ��򿪳ɹ���"<<endl; 
	FILE* tmp[fcount];
	double **arr=new double*[fcount];
	int array_number[fcount];
	
	for(int i=0;i<fcount;i++){
		sprintf(filename,"temp%d.txt",i);
		tmp[i]=fopen(filename,"rb");
		arr[i]=new double[mem_size+50];		
	}
	
	for(int i=0;i<fcount;++i) {
		array_number[i]=fread(arr[i],sizeof(double),mem_size,tmp[i]);
	}
	double *leaf=new double[fcount+1];
	int *root=new int[fcount];
	createtree(arr,fcount,root,leaf);
	
	int* findex=new int[fcount];
	for(int i=0;i<fcount;i++) findex[i]=0;
	int writebytle=0,curlen=0;
	int s=0;
	while(true){
		s=root[0];
		if(leaf[s]==maxDouble) break;
		curlen=(leaf[s]<0)?17:16;
		if(writebytle+curlen<mem_size){
			wstart=dtoa(wstart,leaf[s]);
			writebytle+=curlen;
			writebuf[writebytle++]=0x0a;
		}else{
			fwrite(&writebuf[0],sizeof(char),writebytle,fp2);
			writebytle=0;
			wstart=&writebuf[0];
			wstart=dtoa(wstart,leaf[s]);
			writebytle+=curlen;
			writebuf[writebytle++]=0x0a;	
		}
		++findex[s];
		if(findex[s]<array_number[s]) arr[s][0]=arr[s][findex[s]];
		else {
			arr[s][0]=maxDouble;
			findex[s]=0;
		}
		leaf[s]=arr[s][0];
		adjust(fcount,root,leaf,s);
	}
	if(writebytle) {
		fwrite(&writebuf[0],sizeof(char),writebytle,fp2);
	}
	for(int i=0;i<fcount;++i) fclose(tmp[i]);
	fclose(fp2);
	delete[] findex;
    delete[] leaf;
    delete[] root;
    delete[] arr;	 
}

int main(){
	time_t start,end;
	start=clock();
	init();
	read_param();
	read_data();
	merge();
	end=clock();
	cout<<"�ⲿ������ɣ��ܹ�����ʱ�䣺"<<(end-start)/1000.0<<" ��"<<endl;
	system("pause");
	return 0;
} 
