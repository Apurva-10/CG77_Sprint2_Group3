/************************************************************************************************

  The Oozy Snacks Bar Online Delivery System

  Filename -classes.cpp

  Description -This file consist of all the classes used in Oozy snack bar online delivery system


***********************************************************************************************/

#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;
bool valid_email(string s)      //validation check for email
{
    int fa=0,fb=0;
    if(!isalpha(s[0])) return false;
    for(int i=0;i<s.size()-2;i++)
    {
        if(s[i]==' ') return false;
        if(s[i]=='@') fa=i;
        if(s[i]=='.') fb=i;
    }
    if(fa < fb) return true;
    return false;
}

bool valid_length(string s,int m,int n)    //validation to check length of name,address,description
{
    if(s==" ")
        return false;
    if(s.size()<m || s.size()>n)
        return false;
    return true;
}

bool valid_phone(string s)       //validation to check phone number
{
    if(s==" " || s[0]=='0')
        return false;
    int cnt=0;
    for(int i=0;i<s.size();i++) if(isdigit(s[i])) cnt++;
    if(cnt==10 && cnt==s.size()) return true;
    return false;
}
bool valid_integer(string s)    //validation to check unit available and price
{
    int cnt=0;
    for(int i=0;i<s.size();i++)
    {
        if(s[i]==' ' || s[0]=='0') return false;
        if(isdigit(s[i])) cnt++;
    }
    if(cnt==s.size()) return true;
    return false;
}
using namespace std;
class details
{
	public :

    string name;
    long long int id;
    int set_name();
    int set_name(string s);
    int set_id(long long int i);
   long long int get_id();
    string get_name();
};
class product : public details     //product class to add new product
{

    int ua;
    float up;
    string desc;
public:
    product()
    {

    }
    product(long long int i, int uaa,float upp,string nm,string des)
    {
        id=i;
        ua=uaa;
        up=upp;
        name=nm;
        desc=des;
    }
    int set_up();
    int set_ua();
    int set_ua(int x);
    int set_desc();
    float get_up();
    int get_ua();
    string get_desc();
    int show_product();
};

class customer : public details        //customer class to add new customer
{

    string email,phone,pass;
public:
    customer()
    {

    }
    customer(long long int i,string nm,string mail, string ph,string p)
    {
        id=i;
        name=nm;
        email=mail;
        phone=ph;
        pass=p;
    }
    int set_email();
    int set_phone();
    int set_pass();
    long long int get_cid();
    string get_cname();
    string get_email();
    string get_phone();
    string get_pass();
    int show_customer();
    int getch();
    string pass_magic();
};

class order
{
    int ur;
    long long int pid;
    float up;
    string pname;
public:
    order(long long int id, float p, string nm, int req)
    {
        pid=id;
        up=p;
        pname=nm;
        ur=req;
    }
    int get_ur();
    long long int get_pid();
    float get_up();
    string get_pname();
};

class final_order               //final_order class to add final order
{
    long long int order_id;
    vector<order> items;       //container of order type
    long long int cid;
    string phone,cname;
    float dist;
    string status;
    float total=0;
public :
    final_order(long long int id,string ph,string name,int d,int x,vector <order> vord)
    {
        for(auto it : vord)
        {
            items.push_back(it);
            total+=(it.get_up() * it.get_ur());
        }
        cid=id;
        phone=ph;
        cname=name;
        dist=d;
        order_id=x+101;
        status="In progress";

    }
    int set_status(string s);
    string get_status();
    int show_final_order();
    long long int get_cid();
    string get_phone();
    string get_cname();
    float get_dist();
    long long int get_order_id();
    float get_total_amount();

};


int details :: set_id(long long int i)
{
    id=i;
    return 0;
}

int details :: set_name()
{
    string nm;
    while(1){
        cout<<"Enter Name(min 3 & max 10 characters)"<<endl;
        getline(cin, nm);
        if(valid_length(nm,3,10)){
                break;

        }
        cout<<"Invalid Input"<<endl;
        continue;
    }
    name=nm;
    return 0;
}
int details :: set_name(string s)
{
	name=s;
	return 0;
}
long long int details :: get_id()
{
    return id;
}

string details :: get_name()
{
    return name;
}


int product :: set_up()
{
    string price;
    while(1){
    cout<<"Enter Unit price"<<endl;
    getline(cin, price);
    if(valid_integer(price)==false){
       cout<<"Invalid Input"<<endl;
       continue;
    }
    else break;}
    stringstream ip(price);
    ip>>up;
    return 0;

}

int product :: set_ua()
{
    string unit;
    while(1){
        cout<<"Units Available"<<endl;
        getline(cin, unit);
        if(valid_integer(unit)==false ){
            cout<<"Invalid Input"<<endl;
            continue;
        }
        else break;
    }
    stringstream ip(unit);
    ip>>ua;
    return 0;
}

int product :: set_ua(int x)
{
    ua=x;
    return 0;
}

int product :: set_desc()
{

    string nm;
    while(1){
        cout<<"Enter Product Desc(min 3 & max 20 characters)"<<endl;
        getline(cin, nm);
        if(valid_length(nm,3,20)){
                break;

        }
        cout<<"Invalid Input"<<endl;
        continue;
    }
    desc=nm;
    return 0;
}


float product :: get_up()
{
    return up;
}

int product :: get_ua()
{
    return ua;
}

string product :: get_desc()
{
    return desc;
}

int product :: show_product()
{
        cout<<"***************************************************************"<<endl;
        cout<<"Product id : "<<get_id()<<endl;
        cout<<"Name       : "<<get_name()<<endl;
        cout<<"Description: "<<get_desc()<<endl;
        cout<<"Unit Price : "<<get_up()<<endl;
        cout<<"Units Available in stock : "<<get_ua()<<endl;
//	cout<<get_pid()<<"\t\t\t"<<get_pname()<<"\t\t"<<get_desc()<<"\t\t\t"<<get_up()<<"\t\t"<<get_ua()<<endl;
        cout<<"***************************************************************"<<endl<<endl;
        return 0;
}


int customer :: set_email()
{
    string nm;
    while(1){
        cout<<"Enter customer email(min 3 & max 20 characters)"<<endl;

        getline(cin, nm);

        transform(nm.begin(), nm.end(), nm.begin(), ::toupper);
        if(valid_length(nm,5,20) && valid_email(nm)){
                break;

        }
        cout<<"Invalid Input"<<endl;
        continue;

    }
    email=nm;
    return 0;
}

int customer :: set_phone()
{
    string nm;
    while(1){
        cout<<"Enter Valid phone number"<<endl;
        getline(cin, nm);
        if(valid_phone(nm)){
                break;

        }
        cout<<"Invalid Input"<<endl;
        continue;
    }
    phone=nm;
    return 0;
}

int customer :: set_pass()
{

    string ipt;
    while(1){
        cout<<"Enter Valid pass(min 3 & max 8 characters)"<<endl;
        ipt=pass_magic();
        if(valid_length(ipt,3,8)){
                break;

        }
        cout<<"Invalid Input"<<endl;
        continue;
    }
    pass=ipt;

    return 0;
}

int customer :: getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}

string customer :: pass_magic()
{
  const char BACKSPACE=127;
  const char RETURN=10;

  string password;
  unsigned char ch=0;
  while((ch=getch())!=RETURN)
    {
       if(ch==BACKSPACE)
         {
            if(password.length()!=0)
              {

                 cout <<"\b \b";
                 password.resize(password.length()-1);
              }
         }
       else
         {
             password+=ch;

                 cout <<'*';
         }
    }
  cout <<endl;
  return password;
}


string customer :: get_email()
{
    return email;
}

string customer :: get_phone()
{
    return phone;
}

string customer :: get_pass()
{
    return pass;
}

int customer :: show_customer()
{
        cout<<"***************************************************************"<<endl;
        cout<<"Customer id : "<<get_id()<<endl;
        cout<<"Name        : "<<get_name()<<endl;
        cout<<"Email       : "<<get_email()<<endl;
        cout<<"Phone number: "<<get_phone()<<endl;
        cout<<"***************************************************************"<<endl<<endl;
        return 0;
}

int order :: get_ur()
{
    return ur;
}

long long int order :: get_pid()
{
    return pid;
}

float order :: get_up()
{
    return up;
}

string order :: get_pname()
{
    return pname;
}

int final_order :: set_status(string s)
{
    status=s;
    return 0;
}

string final_order :: get_status()
{
    return status;
}

long long int final_order ::get_cid()
{
    return cid;
}

string final_order ::get_phone()
{
    return phone;
}

string final_order ::get_cname()
{
    return cname;
}

float final_order ::get_dist()
{
    return dist;
}

long long int final_order ::get_order_id()
{
    return order_id;
}

int final_order :: show_final_order()
{

    cout<<"*******************************************************************************************************************************************"<<endl;
    cout<<"Order ID : "<<get_order_id()<<"\t\t"<<"Order status : "<< get_status()<<endl;
    cout<<"Customer Name : "<<get_cname()<<"\t\t"<<"Customer Id : "<<get_cid()<<"\t\t"<<"Phone number : "<<get_phone()<<endl<<endl<<endl<<endl;
    cout<<"Item no."<<"\t\t"<<"Product ID : "<<"\t\t"<<"Product Name"<<"\t\t"<<"Quantity"<<"\t\t"<<"Unit Price"<<"\t\t"<<"Total price"<<endl;



    for(int j=0;j<items.size();j++)
    {
         cout<<j+1<<"\t\t\t"<<items[j].get_pid()<<"\t\t\t"<<items[j].get_pname()<<"\t\t\t\t"<<items[j].get_ur()<<"\t\t"<<items[j].get_up()<<"\t\t\t"<<items[j].get_up() * items[j].get_ur()<<endl;
    }
        cout<<"                                                                                                                                           Total Payable Amount : Rs. "<<total<<endl;

}

float final_order::get_total_amount()
{
    return total;
}
