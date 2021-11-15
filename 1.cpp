#include <iostream>
#include <string>

using namespace std;

class Number;
ostream &operator << (ostream&, Number);
int numSize(int);
int charToInt(char);
char intToChar(int);
double pow(double,int);

class Number {
    string num;// (10)432[11] 15[6]
    int base;
    double num10;
    string intPart();
    string decPart();
    void initNum10();
    string numToStr(double);
public:
    Number(string = "0", int = 10);
    Number getIn10();
    Number toBase(int);
    friend ostream &operator << (ostream&, Number);
};
Number::Number(string num, int base){
    bool havePoint = 0;
    for(int i=num.size()-1; i>=0; i--) if(num[i] == '.') havePoint = 1;
    if( !havePoint ) this->num = num + ".0";
    else this->num = num;
    this->base = base;
    this->initNum10();
}
string Number::intPart(){
    return num.substr(0, num.find('.'));
}
string Number::decPart(){
    return num.substr(num.find('.')+1, num.size()-1);
}
void Number::initNum10(){//inicializa num10 y 
    // if( base == 10 ) return Number(num,10);
    this->num10 = 0;
	string ent = this->intPart();
	Number* aux = nullptr;
    /* Para parte entera */
	//3(12)4 13 ////3(132)4 143 ///(123456) (7654321)
	for( int i = ent.size()-1, j=0 ; i>=0; i--, j++ ){
		if( ent[i] != ')' )
			num10 += charToInt(ent[i]) * pow(base,j);//ent[i] * base^j
		else{
			string par = "";
			while( ent[--i] != '(' ) par += ent[i];//21 //231
			for(int l=0,k=par.size()-1; l < k ; l++, k--) swap(par[l],par[k]);//231 -> 132
			aux = new Number(par,10);
			num10 += (int)aux->num10 * pow(base, j);
			delete aux;
		}
	}
	/* Para parte decimal */
	string dec = this->decPart();
	if(dec != "0"){
        int decsize = dec.size();
        for( int i = 0, j = -1; i<decsize; i++, j-- ){//0.(11)13 12// 113 7
            if( dec[i] != '(')
                num10 += charToInt(dec[i]) * pow(base,j);
            else{// 0.(11)(10)1 12
                string par = "";
                while( dec[++i] != ')' ) par += dec[i];//11 //231
                aux = new Number(par,10);
                num10 += (int)aux->num10 * pow(base, j);
                delete aux;
            }
        }
    }
}
string Number :: numToStr(double num10){
    string num10s = "";
    int ent = (int)num10;
    int i=0,j=0;
    /*parte entera*/
    for( i=numSize(ent)-1, j = ent ; i >= 0 ; i--, ent/=10 )
        num10s += intToChar(ent % 10);
    for(int l=0,k=num10s.size()-1; l < k ; l++, k--) swap(num10s[l],num10s[k]);

    num10s+=".";

    /*parte decimal*/
    double forDec = pow(10, 6);
    // roundf(10);
    double dec = ((double)( (num10-(int)num10) * forDec ) ) / forDec;
    //0.01234567 //12345.67 //12345 //0.012345
    bool full0 = true;
    string aux = "";
    for( i=0; i<6 ; i++){
        dec *= 10;
        aux += intToChar((int)dec);
        dec = dec-(int)dec;
        if( aux[i] != '0') full0 = false;
    }
    if(full0) num10s += "0";
    else num10s += aux;
    return num10s;
}

Number Number::getIn10(){
    return Number(numToStr(this->num10),10);
}
Number Number::toBase(int newBase){
    /*parte entera*/
    int D, d, q, r, i;
    string cad = "";
    D = (int)num10; d = newBase; q =-1;
    if (D == d && newBase > 10) cad = "(10)";
    else{
        while( D >= d){
            q = D/d;
            r = D%d;
            D = q;
            if( numSize(r)<2 ){//24)12( -> 24)21( -> (12)42 //6 10
                cad += intToChar(r);
            }
            else {
                cad += ")";
                Number par = Number(numToStr(r),10);
                cad += par.intPart();
                for(int l=cad.size()-par.intPart().size(),k=cad.size()-1; l < k ; l++, k--) swap(cad[l],cad[k]);
                i += numSize(r);
                cad += "(";
            }
            //D/d = q;
        }
        if( q == -1 ) q = D;
        if( numSize(q)<2 )//24)12( -> 24)21( -> (12)42 //6 10
            cad += intToChar(q);
        else {
            cad += ")";
            Number par = Number(numToStr(q),10);
            cad += par.intPart();
            i = numSize(q);
            for(int l=cad.size()-i,k=cad.size()-1; l < k ; l++, k--) swap(cad[l],cad[k]);
            cad += "(";
        }
        for(int l=0, k=cad.size()-1; l < k ; l++, k--) swap(cad[l],cad[k]);
    }
    
    /*Parte decimal*/
    cad += ".";
    //0.2[10] -> 8
    double dec,e,f,aux;
    f = dec = num10 - (int)num10;//0.2
    i = 0;
    if( f != 0.0 ){
        string c = "";
        do {
            aux = f * newBase;//1.6 //4.8   //6.4   //3.2
            e = (int)aux;//1        //4     //6     //3
            f = aux - (int)aux;//0.6//0.8   //0.4   //0.2
            c += intToChar((int)e);//1463
            i++;
        } while( dec != f && i < 6 );//1
        cad += c;
        if( c.size() <= 6 ){
            cad += c.substr(0 , 6 - c.size());
        }
    } else {
        cad+="0";
    }
    return Number(cad,newBase);
}

ostream &operator << (ostream& o, Number n){
    o << n.num;
    return o;
}

int numSize(int a){//123
    if(!a) return 1;
    int count = 0;
    while(a){
        a = a/10;
        count++;
    }
    return count;
}
int charToInt(char a){
    if(a >= '0' && a <= '9') return (int)a-48;//ACII(0) = 48
    return -1;
}
char intToChar(int a){
    if( a >= 0 && a <= 9 ) return (char)a+'0';
    return '\0';
}
double pow(double a, int b){
    double val = a;
    if (b > 0){
        while( b-- > 1 )
            a *= val;
    }
    else if (b < 0){
        while( b++ < -1)
            a *= val;
        a = 1/a;
    }
    else return 1;
    return a;
}

int main(){
    // Number f = Number("133.121",15);
    // cout << f.toBase(10) << endl << endl;
    // Number g = Number("12(19).(12)",20);
    // cout << g.toBase(10) << endl << endl;

    // cout << endl;
    // Number test1 = Number("0.2",10);
    // cout << test1.toBase(2) << endl;
    // cout << test1.toBase(8) << endl;
    // cout << test1.toBase(16) << endl;

    // cout << endl;
    // Number test2 = Number("32.2",10);
    // cout << test2.toBase(2) << endl;
    // cout << test2.toBase(8) << endl;
    // cout << test2.toBase(16) << endl;

    // cout << endl;
    // Number test3 = Number("1010.110011",2);
    // cout << test3.toBase(2) << endl;
    // cout << test3.toBase(8) << endl;
    // cout << test3.toBase(16) << endl;
    
    // cout << endl;
    // Number test4 = Number("12.2",8);
    // cout << test4.toBase(2) << endl;
    // cout << test4.toBase(8) << endl;
    // cout << test4.toBase(16) << endl;
    
    // cout << endl;
    // Number test5 = Number("1(14).2",16);
    // cout << test5.toBase(2) << endl;
    // cout << test5.toBase(8) << endl;
    // cout << test5.toBase(16) << endl;
    
    system("pause>null");
    return 0;
}