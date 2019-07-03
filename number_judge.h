using namespace std;
//�ж��ַ����Ƿ�Ϊ���֡� 
bool isdigital(const char *a){
	if((*a-'0')>9||(*a-'0')<0) return false;
	return true;
}
//�������¼������С�����ӿ����㡣 
double dp1[400],dp2[400];
void init(){
	dp2[0]=1.0;
	for(int i=1;i<400;i++){
		dp2[i]=0.1*dp2[i-1];
	}
	dp1[0]=1.0;
	for(int i=1;i<400;i++){
		dp1[i]=10.0*dp1[i-1];
	}
}
//�ж��Ƿ���Ч���֡� 
bool isvalid(char *s,int l){
	bool dotexist=false; //e,С����ֻ�ܳ���1�Ρ� 
	bool eexist=false;
	int i=0;
	char *p=s;
	while(i<l){
		if(isdigital(p)||*p=='+'||*p=='-') {
			++p; ++i;
		}
		else if((*p=='E'||*p=='e')&&!eexist){
			++p; eexist=true; ++i;
		}else if(*p=='.'&&!dotexist&&!eexist){ //ָ�����ֲ�����С���� 
			++p; dotexist=true; ++i;
		}else{
			return false;
		}
	}
	if(i==l) return true;
	else return false;	
}
//�ַ���ת��Ϊ�������� 
double aatof(char *s,int l){
	char *p=s;
	bool flag1=true,flag2=true; //��¼С����ָ���������� 
	double ans=0.0;
	int ltmp=0; //��¼��ǰ��ȡ�ĳ��ȣ�����С�ڵ����ܳ���l 
	if(*p=='-') {
		flag1=false;
		++p;++ltmp;
	}
	if(*p=='+') {
		++p; ++ltmp;
	}
	double number1=0.0,number2=0.0;
	int e_index=0;
	while(isdigital(p)&&ltmp<l){
		number1=number1*10.0+(*p-'0');
		++p; ++ltmp;
	}
	int tmp=1;
	if(*p=='.'){
		++p; ltmp++;
		while(isdigital(p)&&ltmp<l){
			number2+=dp2[tmp]*(double)(*p-'0');
			++p; ++ltmp;
			++tmp;
		}
	}
	if(*p=='E'||*p=='e'){
		p++; ltmp++;
		if(*p=='-'){
			flag2=false; p++; ltmp++;
		}
		if(*p=='+') {
		p++; ltmp++; }
		while(isdigital(p)&&ltmp<l){
			e_index=10*e_index+(*p-'0');
			p++; ltmp++;
		}
	}
	ans=number1+number2;
	ans=(flag1==true)?ans:-ans;
	ans=(flag2==true)?ans*dp1[e_index]:ans*dp2[e_index];
	return ans;	
}
//������תΪ�ַ����� 
char* dtoa(char* s,double number){
	int e_index=0,l=0,r=308;
	bool flag=true; //��¼ָ�������� 
	double tmp=0.0;
	if(number<0){
		number=-number;
		*s='-';
		++s;
	}
	if(number>=10.0){
		while(l<=r){
			e_index=l+(r-l)/2;
			tmp=number/dp1[e_index];
			if(tmp>=10.0) l=e_index+1;
			else if(tmp<1.0) r=e_index-1;
			else break;
		}
	}
	if(number<1.0){
		flag=false;
		while(l<=r){
			e_index=l+(r-l)/2;
			tmp=number*dp1[e_index];
			if(tmp>=10.0) r=e_index-1;
			else if(tmp<1.0) l=e_index+1;
			else break;
		}
	}
	if(number==0.00){ //������Чλ������������Ϊ0�ġ� 
		*(s++)='0'; *(s++)='.';
		for(int i=0;i<9;i++) *(s++)='0';
		*(s++)='E'; *(s++)='+';
		for(int i=0;i<3;i++) *(s++)='0';
		*(s++)=0x0a;
		return s;
	}
	if(number>=1.0&&number<10.0) tmp=number;
	//��ʱ��tmp��1-10֮�䣬e_index�������ָ��
	tmp+=0.0000000005; //����ȥβ���ܱ����������롣 
	*(s++)='0'+(int)tmp;
	*(s++)='.';
	for(int i=0;i<9;i++){
		tmp=tmp-(int)tmp;
		tmp*=10;
		*(s++)='0'+(int)tmp;
	}
	*(s++)='E';
	if(flag) *(s++)='+';
	else *(s++)='-';
	*(s++)='0'+e_index/100;
	e_index=e_index%100;
	*(s++)='0'+e_index/10;
	e_index=e_index%10;
	*(s++)='0'+e_index;
	*(s++)=0x0a;
	return s;
}
