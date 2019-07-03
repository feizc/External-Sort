using namespace std;
//判断字符串是否为数字。 
bool isdigital(const char *a){
	if((*a-'0')>9||(*a-'0')<0) return false;
	return true;
}
//用数组记录倍数和小数，加快运算。 
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
//判断是否有效数字。 
bool isvalid(char *s,int l){
	bool dotexist=false; //e,小数点只能出现1次。 
	bool eexist=false;
	int i=0;
	char *p=s;
	while(i<l){
		if(isdigital(p)||*p=='+'||*p=='-') {
			++p; ++i;
		}
		else if((*p=='E'||*p=='e')&&!eexist){
			++p; eexist=true; ++i;
		}else if(*p=='.'&&!dotexist&&!eexist){ //指数部分不能是小数。 
			++p; dotexist=true; ++i;
		}else{
			return false;
		}
	}
	if(i==l) return true;
	else return false;	
}
//字符串转化为浮点数。 
double aatof(char *s,int l){
	char *p=s;
	bool flag1=true,flag2=true; //记录小数和指数的正负。 
	double ans=0.0;
	int ltmp=0; //记录当前读取的长度，必须小于等于总长度l 
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
//浮点数转为字符串。 
char* dtoa(char* s,double number){
	int e_index=0,l=0,r=308;
	bool flag=true; //记录指数的正负 
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
	if(number==0.00){ //根据有效位数，四舍五入为0的。 
		*(s++)='0'; *(s++)='.';
		for(int i=0;i<9;i++) *(s++)='0';
		*(s++)='E'; *(s++)='+';
		for(int i=0;i<3;i++) *(s++)='0';
		*(s++)=0x0a;
		return s;
	}
	if(number>=1.0&&number<10.0) tmp=number;
	//此时，tmp在1-10之间，e_index保存的是指数
	tmp+=0.0000000005; //这样去尾后能保持四舍五入。 
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
