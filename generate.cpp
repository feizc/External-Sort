#include<random>
#include<iostream>
#include<ctime>
#include<fstream>
#include<cfloat>
#include<algorithm>
#include<vector>
#include<string>
#include<stdio.h>

using namespace std;
long long n;//������Ŀ���� 
int m;//���ɷǷ���Ŀ���� 
string s="abcdefghijklmnopqrstuvwxyz!#%*-+?=ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"; //�Ƿ���Ŀ�������ѡ������� 
const int mem_size=1000000; //�ֽ����ﵽmem_size������һ���ļ�д�롣 

//��double�͸�����תΪ�ַ����� 
char* transdouble(char* s,double number,int &l){
	if(number<0){
		*(s++)='-';
		number=-number;
		l++;
	}
	int tmp=(int)number;
	*(s++)='0'+tmp; l++;
	*(s++)='.'; l++;
	number=number-tmp;
	number*=10;
	tmp=(int)number;
	*(s++)='0'+tmp;	l++;
	while(tmp!=0&&l<10){
		number=number-tmp;
		number=number*10;
		tmp=(int)number;
		*(s++)='0'+tmp;
		l++;
	}
	return s;
}

//��int������תΪ�ַ�������Χ-308~308��������ָ���� 
char* transint(char* s,int number,int &l){
	if(number<0){
		*(s++)='-';
		number=-number;
		l++;
	}
	if(number>=100){
		*(s++)='0'+number/100;
		number=number%100;
		*(s++)='0'+number/10;
		number=number%10;
		*(s++)='0'+number;
		l=l+3;
		return s;
	}else if(number<100&&number>=10){
		*(s++)='0'+number/10;
		number=number%10;
		*(s++)='0'+number;
		l=l+2;
		return s;
	}else{
		*(s++)='0'+number;
		l++;
		return s;
	}
}

int main()
{
	cout<<"��������Ҫ���������Ŀ������n��"<<endl;
	cin>>n;
	cout<<"�������������ķǷ���Ŀ����m��"<<endl; 
	cin>>m;
	while(m>n){
		cout<<"�Ƿ���Ŀ������������������������m��"<<endl;
		cin>>m;
	}
	vector<long long> a;
	time_t start,end;
	start=clock();
	FILE *fp;
	fp=fopen("M1.txt","wb"); 
	//����ָ����Χ�ľ���������� 
	default_random_engine e(time(0));
	uniform_real_distribution<double> u(-9.0,9.0);
	uniform_int_distribution<long long> u2(1,n-1);
	uniform_int_distribution<int> u3(1,69);
	uniform_int_distribution<int> u4(-307,307);
	//����m�����ظ�������,����ȷ������Ƿ���Ŀ��λ�á� 
	while(a.size()<m){
		long long v=u2(e);
		bool flag=true;
		for(int i=0;i<a.size();i++){
			if(a[i]!=v) continue;
			else{
				flag=false;
				break;
			}
		}
		if(flag){
			a.push_back(v);
		}
	}
	sort(a.begin(),a.end());
	int j=0;
	cout<<"��ʼ������������԰���"<<endl;
	char mem[mem_size+50];
	char *p=&mem[0];
	int bytle=0,curlen=0;
	double number1;
	int number2;
	int chushu=n/10; int yushu=0;
	for(int i=0;i<n;i++){
		curlen=0;
		if(j<m&&i==a[j]){
			curlen=8;
			cout<<"���ɵ�"<<j+1<<"���Ƿ���"<<endl; 
			for(int m=0;m<8;m++){
				int tmp=u3(e);
				*(p++)=s[tmp];
			}
			bytle+=curlen;
			*(p++)=0x0a;
			bytle++;
			j++;
		}else{
			number1=u(e);
			p=transdouble(p,number1,curlen);
			*(p++)='e'; curlen++;
			number2=u4(e);
			p=transint(p,number2,curlen);
			bytle+=curlen;
			*(p++)=0x0a;
			bytle++;
		}
		if(bytle>mem_size){
			fwrite(&mem[0],sizeof(char),bytle,fp);
			bytle=0;
			p=&mem[0];
		}
		if(i%chushu==0){
			yushu=i/chushu;
			cout<<"�����������"<<yushu+1<<"0%"<<endl; 
		} 
			
	}
	if(bytle){
		fwrite(&mem[0],sizeof(char),bytle,fp);
	} 
	fclose(fp);
	end=clock();
	cout<<"����������ɣ��ܹ�����ʱ�䣺"<<(end-start)/1000.0<<" S"<<endl;
	system("pause");
	return 0;	
}

