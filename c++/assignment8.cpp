/* Assignment 8
programmer: Sebastian Crespo
testing all arithmetic operation between three fractions
date:11/12/2015*/
#include <iostream>
#include <cmath>
using namespace std;

class Rational
{
private:

  double numerator;
  double denominator;
public:
  Rational();
  Rational(int up,int down);

  void setfraction(int x, int y);
  void whole(int whole_number);
  void normalization(int a, int b);
  void reduce_fraction(int c, int d);
  int getNumerator();
  int getDenominator();
  Rational operator+(Rational& amount2);//my operators
  Rational operator-(Rational& amount2);
  Rational operator*(Rational& amount2);
  Rational operator/(Rational& amount2);
  bool operator>(Rational& amount2);
  bool operator>=(Rational& amount2);
  bool operator<=(Rational& amount2);
  bool operator<(Rational& amount2);
};
bool Rational::operator<(Rational& amount2)//amount 2 refers to the other class that I will be adding to my already existing one
{
  if((numerator/denominator)<(amount2.numerator/amount2.denominator))
    {return true;}
  else
    return false;
}
bool Rational::operator<=(Rational& amount2)
{
  if((numerator/denominator)<=(amount2.numerator/amount2.denominator))
    {return true;}
  else
    return false;
}
bool Rational::operator>=(Rational& amount2)
{
  if((numerator/denominator)>=(amount2.numerator/amount2.denominator))
    {return true;}
  else
    return false;
}
bool Rational::operator>(Rational& amount2)
{
  double first=(numerator/denominator);
  double sec=(amount2.numerator/amount2.denominator);
  if(first>sec)
    {return true;}
  else
    return false;
}
Rational Rational::operator/(Rational& amount2)
{
  int up_div=numerator*amount2.denominator;
  int down_div= denominator*amount2.numerator;
  return Rational(up_div,down_div);//it means that I set my values with a intial construct set in place to create a new class
}
Rational Rational::operator*(Rational& amount2)
{
  int up_multiply=numerator*amount2.numerator;
  int down_multiply= denominator*amount2.denominator;
  return Rational(up_multiply,down_multiply);
}
Rational Rational::operator-(Rational& amount2)
{
  int up_sub=(numerator*amount2.denominator)-(amount2.numerator*denominator);
  int down_sub=denominator*amount2.denominator;
  return Rational(up_sub,down_sub);
}
Rational::Rational(int up,int down)
{ 
  numerator=up;
  denominator=down;
}
Rational Rational::operator+(Rational& amount2)
{
  int up_add=(numerator*amount2.denominator)+(amount2.numerator*denominator);
  int down_add=denominator*amount2.denominator;
  return Rational(up_add,down_add);
}
int Rational:: getNumerator(){

  return numerator;
}
int Rational:: getDenominator(){

  return denominator;
}
Rational::Rational()
{
  numerator= 0;
  denominator=1;
}
void Rational::reduce_fraction(int c,int d)
{
  for(int i= c*d;i>1;i--)
    {
      if((c%i==0)&&(d%i==0))
	{
	  c/=i;
	  d/=i;
	}
    }
  numerator=c;
  denominator=d;
}

void Rational::setfraction(int x,int y)
{
  numerator=x;
  denominator=y;
}

void Rational::whole(int whole_number)
{
  numerator= whole_number;
  denominator=1;
}

void Rational::normalization(int a, int b)
{
  if(a==b)
    {
      numerator =1;
      denominator=1;
    }
  if(a<0 && b<0)
    {
      numerator=-1*a;
      denominator=-1*b;
    }
}

int main()
{
  Rational exp1, exp2,exp3, exp4;
  int num1,dem1,num2,dem2,num3,dem3,num4,dem4;
  char fraction;
  int m;
  cout<<"Enter a fraction in the format integer_numerator/integer_denominator"<<endl;//lot of repeating
  cin>>num1>>fraction>>dem1;
  if( fraction!='/')
    {cout<<" Bad input format for operator >>. Aborting!"<<endl;
      return 0;
    }
  exp1.setfraction(num1,dem1);
  exp1.normalization(num1,dem1);
  exp1.reduce_fraction(num1,dem1);
  cout<<"You have entered the equivalent of: "<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<endl;

 cout<<"Enter a fraction in the format integer_numerator/integer_denominator"<<endl;
  cin>>num2>>fraction>>dem2;
  if( fraction!='/')
    {cout<<" Bad input format for operator >>. Aborting!"<<endl;
      return 0;
    }
  exp2.setfraction(num2,dem2);
  exp2.normalization(num2,dem2);
  exp2.reduce_fraction(num2,dem2);
  cout<<"You have entered the equivalent of: "<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<endl;

 cout<<"Enter a fraction in the format integer_numerator/integer_denominator"<<endl;
  cin>>num3>>fraction>>dem3;
  if( fraction!='/')
    {cout<<" Bad input format for operator >>. Aborting!"<<endl;
      return 0;
    }
  exp3.setfraction(num3,dem3);
  exp3.normalization(num3,dem3);
  exp3.reduce_fraction(num3,dem3);
  cout<<"You have entered the equivalent of: "<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<endl;
  cout<<"Testing arithmetic and relational operator overloading"<<endl;

 exp4=exp1*exp2;
  num4=exp4.getNumerator();
  dem4=exp4.getDenominator();
  exp4.normalization(num4,dem4);
  exp4.reduce_fraction(num4,dem4);
  cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"*"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<"(" <<exp4.getNumerator()<<"/ "<<exp4.getDenominator()<<")"<<endl;

  exp4=exp1+exp2;
  num4=exp4.getNumerator();
  dem4=exp4.getDenominator();
  exp4.normalization(num4,dem4);
  exp4.reduce_fraction(num4,dem4);
  cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"+"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<"(" <<exp4.getNumerator()<<"/ "<<exp4.getDenominator()<<")"<<endl;


  exp4=exp1-exp2;
  num4=exp4.getNumerator();
  dem4=exp4.getDenominator();
  exp4.normalization(num4,dem4);
  exp4.reduce_fraction(num4,dem4);
  cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"-"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<"(" <<exp4.getNumerator()<<"/ "<<exp4.getDenominator()<<")"<<endl;



  exp4=exp1/exp2;
  num4=exp4.getNumerator();
  dem4=exp4.getDenominator();
  exp4.normalization(num4,dem4);
  exp4.reduce_fraction(num4,dem4);
  cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"/"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<"(" <<exp4.getNumerator()<<"/ "<<exp4.getDenominator()<<")"<<endl;

 m=(exp1<exp2);
 cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"<"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<m<<endl;
 m=(exp1<exp1);
 cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"<"<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"="<<m<<endl;

 m=(exp1<=exp2);
 cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"<="<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<m<<endl;
 m=(exp1<=exp1);
 cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"<="<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"="<<m<<endl;

 m=(exp1>exp2);
 cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<">"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<m<<endl;
 m=(exp1>exp1);
 cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<">"<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"="<<m<<endl;

 m=(exp1>=exp2);
 cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<">="<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<m<<endl;
 m=(exp1>=exp1);
 cout<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<">="<<"("<<exp1.getNumerator()<<"/"<<exp1.getDenominator()<<")"<<"="<<m<<endl;

 exp4=exp3*exp2;
  num4=exp4.getNumerator();
  dem4=exp4.getDenominator();
  exp4.normalization(num4,dem4);
  exp4.reduce_fraction(num4,dem4);
  cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"*"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<"(" <<exp4.getNumerator()<<"/ "<<exp4.getDenominator()<<")"<<endl;

  exp4=exp3+exp2;
  num4=exp4.getNumerator();
  dem4=exp4.getDenominator();
  exp4.normalization(num4,dem4);
  exp4.reduce_fraction(num4,dem4);
  cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"+"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<"(" <<exp4.getNumerator()<<"/ "<<exp4.getDenominator()<<")"<<endl;


  exp4=exp3-exp2;
  num4=exp4.getNumerator();
  dem4=exp4.getDenominator();
  exp4.normalization(num4,dem4);
  exp4.reduce_fraction(num4,dem4);
  cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"-"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<"(" <<exp4.getNumerator()<<"/ "<<exp4.getDenominator()<<")"<<endl;



  exp4=exp3/exp2;
  num4=exp4.getNumerator();
  dem4=exp4.getDenominator();
  exp4.normalization(num4,dem4);
  exp4.reduce_fraction(num4,dem4);
  cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"/"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<"(" <<exp4.getNumerator()<<"/ "<<exp4.getDenominator()<<")"<<endl;

 m=(exp3<exp2);
 cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"<"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<m<<endl;
 m=(exp3<exp3);
 cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"<"<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"="<<m<<endl;

 m=(exp3<=exp2);
 cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"<="<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<m<<endl;
 m=(exp3<=exp3);
 cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"<="<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"="<<m<<endl;

 m=(exp3>exp2);
 cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<">"<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<m<<endl;
 m=(exp3>exp3);
 cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<">"<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"="<<m<<endl;

 m=(exp3>=exp2);
 cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<">="<<"("<<exp2.getNumerator()<<"/"<<exp2.getDenominator()<<")"<<"="<<m<<endl;
 m=(exp3>=exp3);
 cout<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<">="<<"("<<exp3.getNumerator()<<"/"<<exp3.getDenominator()<<")"<<"="<<m<<endl;
  return 0;
}
