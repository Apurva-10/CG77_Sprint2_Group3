/************************************************************************
The Oozy Snacks Bar Online Delivery System

Filename - functions.cpp

Decsription - This file consist of all the functions
*************************************************************************/

#include <bits/stdc++.h>
#include <fstream>
#include "classes.cpp"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

vector<order> vord;                //container of order type
list<customer> cust;               //container of list type
vector<final_order> ord;           //container of final order type
stack<final_order> delivery_list;  //container of final prder type 
list<product> prod;                //container of product type

int delivered=0;
int customer_registration();
int checkout();
int new_order();
int show_all_orders();
int show_product();
int consult_order();
int show_delivery();
int begin_delivery();
int file_maintenance();
int add_product();
int modify_product();
int modify_customer();
int show_customer_list();
bool sortbydist(final_order &a,final_order &b);
int file_to_cust();
int cust_to_file();
int file_to_prod();
int prod_to_file();
string pass_magic();
int show_order_by_min();
string pass_magic();
int getch();
mutex mtx;                      //mutex for critical section

/************************************************************************

  Function Name - file_to_cust

  Function Description - To read all the data of file to the list

************************************************************************/

int file_to_cust()
{
    ifstream t("customer.txt"); 
    stringstream buffer;        //for taking input
    buffer << t.rdbuf();        //read buffer
    int cnt=0;
    string s,h="";
    while(getline(buffer,s)){
            cnt++;
    }
    t.close();
    if(cnt==0) return 0;
    cnt=cnt/5;                 
    ifstream x("customer.txt");
    while(cnt--)
    {
        int id;
        string name,mail,ph,p;
        x>>id;
        x.ignore();                   //for clearing buffer of interger type
        getline(x,name);
        getline(x,mail);
        getline(x,ph);
        x>>p;                    
        customer tmp(id,name,mail,ph,p);
        cust.push_back(tmp);

   }
    return 0;

}

/************************************************************************

  Function Name - cust_to_file

  Function Description - To write all the data of list to the file

************************************************************************/

int cust_to_file()                   
{
    ofstream t("customer.txt" , ios::trunc);     //ios::trunc-open stream in truncate mode
    string op="";
    for(auto it : cust)
    {
        t<<it.get_cid()<<"\n";
        t<<it.get_cname()<<"\n";
        t<<it.get_email()<<"\n";
        t<<it.get_phone()<<"\n";
        t<<it.get_pass()<<"\n";
    }
    t.close();

}

/************************************************************************

  Function Name - file_to_prod

  Function Description - To read all the data of file to the list

************************************************************************/

int file_to_prod()
{
    ifstream t("product.txt");
    stringstream buffer;                 //for reading buffer
    buffer << t.rdbuf();
    int cnt=0;
    string s,h="";
    while(getline(buffer,s)){
            cnt++;
    }
    t.close();
    if(cnt==0) return 0;
    cnt=cnt/5;
    ifstream x("product.txt");
    while(cnt--)
    {
        int id,ua,up;
        string nm,d;
        x>>id;
        x>>ua;
        x>>up;
        x.ignore();                     //to clean buffer
        getline(x,nm);

        getline(x,d);
        product tmp(id,ua,up,nm,d);
        prod.push_back(tmp);

    }
    return 0;

/************************************************************************

  Function Name - prod_to_file

  Function Description - To write all the data of list to the file

************************************************************************/

}
int prod_to_file()
{
    ofstream t("product.txt" , ios::trunc);
    string op="";
    for(auto it : prod)
    {
	//to write data line by line
        t<<it.get_pid()<<"\n";
        t<<it.get_ua()<<"\n";
        t<<it.get_up()<<"\n";
        t<<it.get_pname()<<"\n";
        t<<it.get_desc()<<"\n";
    }
    t.close();

}

/************************************************************************

  Function Name - customer_registration

  Function Description - To register customer for the first time

************************************************************************/

int customer_registration()
{
    customer tmp;
    long long int id;
    if(cust.size()>0)
    for(auto it : cust) id=it.get_cid();
    else id=1000;
    //to insert values
    tmp.set_cid(id+1);
    tmp.set_cname();
    tmp.set_phone();
    tmp.set_email();
    tmp.set_pass();
    cust.push_back(tmp);             //push all the data into the cust list
    tmp.show_customer();
    cout<<"Customer Added Successfully "<<endl;

    return 0;
}

/************************************************************************

  Function Name - checkout

  Function Description - To display msg for order confirmation and 
  updating products details

************************************************************************/

int checkout()
{

    for(int i=0;i<vord.size();i++)
    {
        for(list<product>::iterator pit = prod.begin();pit!=prod.end();++pit)   //to access complete list using iterator pit
        {
            if(pit->get_pid()==vord[i].get_pid()) pit->set_ua(pit->get_ua()- vord[i].get_ur());   //to update stock details
        }
    }
    cout<<"Order placed Successfully"<<endl;

    return 0;
}


/************************************************************************

  Function Name - new_order

  Function Description - To place order for maximum 3 products at a time

************************************************************************/

int new_order()
{
    if(prod.size()==0)
    {
        cout<<"Empty Stock list"<<endl;
        return 0;
    }
    int flag=0;
    string phone;
    cout<<"Enter Phone number"<<endl;
    cin>>phone;
    cin.ignore();          //to clear the buffer of string type
    for(list<customer>::iterator it = cust.begin();it!=cust.end();++it)
    {
            if(it->get_phone()==phone)
            {
                flag=1;
                cout<<"Enter password"<<endl;
                string pass;
                pass=pass_magic();          //password masking
                if(it->get_pass() != pass){
                    cout<<"Invalid Password"<<endl;
                    return 0;
                }
                vord.clear();          
                string choice;
                int ch=0;
                while(ch != 3 )
                {
                    cout<<"1. Continue Shopping"<<endl;
                    cout<<"2. Check Out"<<endl;
                    cout<<"3. Cancel and proceed to menu"<<endl;
		    //exception handling to validate '0' and ' '
                try{               
                    getline(cin,choice);
                    if(choice[0]==' ' || !isdigit(choice[0]) || choice.size()>1) throw choice;
                    ch=stoi(choice);
                    if(ch==1)
                    {
                        if(vord.size()==3)    //validation to order maximum 3 items
			{
                            cout<<"Item limit exceeded. You can either Checkout or cancel and proceed to main menu"<<endl;           
                            continue;
                        }
                        int pflag=0;
			string ppid;
			cout<<"Enter Product id"<<endl;
			getline(cin,ppid);
			try{
				if(ppid[0]==' ' || !isdigit(ppid[0]) || ppid[0] == '0') throw ppid;
			}
			catch(string s)
			{cout<<"Invalid Input Exception"<<endl;
				return 0;
			}
                        long long int pid;
                        pid=stoi(ppid);
                        for(list<product>::iterator pit = prod.begin();pit!=prod.end();++pit)
                        {
                            if(pit->get_pid()==pid)
                            {
                                pflag=1;
				string rreq;
                                int req;
                                cout<<"Enter units required"<<endl;
				try{
					getline(cin,rreq);
					if(rreq[0]==' ' || !isdigit(rreq[0]) || rreq[0]=='0') throw rreq;
				}
				catch(string s)
				{
					cout<<"Invalid input Exception"<<endl;
					return 0;
				
				}
				req=stoi(rreq);
                                if(pit->get_ua() >= req)
                                {
                                    order tmp(pid, pit->get_up(), pit->get_pname(), req);
                                    vord.push_back(tmp);    //to write data into the order vector
                                    cout<<"Item added to cart"<<endl;
                                }
                                else
                                    cout<<"Entered Quantity not Available"<<endl;
                            }
                        }
                        if(!pflag) cout<<"Product not found"<<endl;

                    }
                    else if(ch==2) {

                    float d;
                    while(1)
                    {   
                        cout<<"Enter distance between Snack bar and and your delivery location."<<endl;
                        cin>>d;
                        cin.ignore();
			if(d > 30) 
			{
				cout<<" Location not servicable. Order not placed."<<endl;
				return 0;
			}
                        if(d<=0){
                            cout<<"Invalid input"<<endl;
                        }
                        else break;
                    }
                    if(vord.size()==0)
                    {
                        cout<<"Empty Cart"<<endl;
                        return 0;
                    }
                        final_order tmpord(it->get_cid(),phone,it->get_cname(), d,ord.size(),vord);
                        ord.push_back(tmpord);   //to add data to the final order vector
                        checkout();  //to place order and exit
                        tmpord.show_final_order();  //order details with total payable amount
                        return 0;
                    }
                    else if(ch==3){  //to exit without ordering
                        return 0;
                    }
                    else throw(ch);

                }
                catch(string s)
        {
            cout<<"Invalid Input Exception\nPlease Try again."<<endl;
        }
        catch(int ch)
        {
            cout<<"Invalid Choice Exception\nPlease Try again."<<endl;
        }
            }


            }
    }
    if(!flag) cout<<"Phone number not registered. Please register through Customer registration option in main menu"<<endl;
    return 0;
}

/************************************************************************

  Function Name - show_all_orders

  Function Description - To show details of all the orders

************************************************************************/

int show_all_orders()
{
	string pass;
	cout<<"Enter employee password"<<endl;

	pass=pass_magic();
	if(pass != "test@123")
	{
		cout<<"Invalid password"<<endl;
		return 0;
	}
if(ord.size()==0)
{
	cout<<"Empty Order List"<<endl;
	return 0;
}
    for(int i=0;i<ord.size();i++)
    {
        ord[i].show_final_order();
    }
    return 0;
}

/************************************************************************

  Function Name - show_product

  Function Description - To show details of all the products

************************************************************************/

int show_product()
{
    if(prod.size()==0)
    {
        cout<<"Empty Stock list"<<endl;
        return 0;
    }
    cout<<"**********************************************************************************************************"<<endl;
    cout<<"                 \t\t Stock Availability List"<<endl;
    cout<<"**********************************************************************************************************"<<endl<<endl<<endl;
  //  cout<<"Product Id"<<"\t\t"<<"Name"<<"\t\t"<<"Description"<<"\t\t"<<"Unit Price"<<"\t\t"<<"unit Available"<<endl;
    for(auto it : prod){
        it.show_product();
    }
    return 0;
}

/************************************************************************

  Function Name - show_order_by_min

  Function Description - To display all the orders having total payable
                         more than the min value

************************************************************************/

int show_order_by_min()
{

	string pass;
cout<<"Enter Employee Password"<<endl;
pass=pass_magic();
if(pass != "test@123")
{
	cout<<"Invalid Password";
	return 0;
}
    if(ord.size()==0)
    {
        cout<<"Empty Order list"<<endl;
        return 0;
    }
    float id;
    cout<<"Enter Min order value"<<endl;
    cin>>id;
    cin.ignore();          //clear buffer
    for(auto it : ord)
    {
        if(it.get_total_amount()>=id)//to display all the orders having total payable more than min value
	{ 
            it.show_final_order();
        }
    }
    return 0;
}

/************************************************************************

  Function Name - consult_order

  Function Description - To show details of order with specific order id

************************************************************************/

int consult_order()
{
    int id;
    cout<<"Enter Order id"<<endl;
    cin>>id;
    cin.ignore();
    for(auto it : ord)
    {
        if(it.get_order_id()==id){
                id=-1;
            it.show_final_order();
        }
    }
    if(id != -1)
        cout<<"Order not found"<<endl;
    return 0;
}


/************************************************************************

  Function Name - show_delivery

  Function Description - To show delivery list

************************************************************************/

int show_delivery()
{
	
    if(ord.size()==0)
    {
        cout<<"Empty Order list"<<endl;
        return 0;
    }
    sort(ord.begin(),ord.end(),sortbydist);   //sort in ascending order of distance
    reverse(ord.begin(),ord.end());           //reverse to get smallest distance on top
    for(auto it : ord) if(it.get_status() == "In progress")delivery_list.push(it);
    reverse(ord.begin(),ord.end());
    for(auto it : ord) if(it.get_status() == "In progress")it.show_final_order();
    return 0;
}

/************************************************************************

  Function Name - begin_delivery

  Function Description - To start delivery of orders in descending order
                         of their distance

************************************************************************/

int begin_delivery()
{
	string pass;
	cout<<"Enter Employee password"<<endl;
	pass=pass_magic();
	if(pass != "test@123")
	{
		cout<<"Invalid password"<<endl;
		return 0;
	}
   show_delivery();
    if(delivery_list.size()==0)
    {
	    cout<<"No pending deliveries left"<<endl;
	    return 0;
    }
    while(1)
    {
        string choice;
        int ch;
        cout<<"1. Deliver next order"<<endl;
        cout<<"2. Return to main menu"<<endl;
	//exceptional handling
    try{
        getline(cin,choice);
        if(choice[0]==' ' || !isdigit(choice[0]) || choice.size()>1) throw choice;
        ch=stoi(choice);   //string to integer
        if(ch==1)
        {
            for(int i=0;i<ord.size();i++)
            {
                if(ord[i].get_order_id()==delivery_list.top().get_order_id())    //compare order id 
                {
                    ord[i].set_status("Delivered");
                    delivery_list.pop();          //remove delivered orders
                    break;
                }
            }
            delivered++;
	    if(delivery_list.size()==0)
	    {
		    cout<<"No more pending orders left";
		    return 0;
	    }
            show_delivery();
            continue;

        }
        else if(ch==2) return 0;
        else throw ch;
        }
        catch(string s)
        {
            cout<<"Invalid Input Exception\nPlease Try again."<<endl;
        }
        catch(int ch)
        {
            cout<<"Invalid Choice Exception\nPlease Try again."<<endl;
        }

    }
    return 0;
}

/************************************************************************

  Function Name - file_maintenance

  Function Description - To main the customer and product files

************************************************************************/

int file_maintenance()
{
    cout<<"Enter Employee Password"<<endl;
    string x=pass_magic();       //password masking
    if(x != "test@123")         //static password for employees
    {
        cout<<"Invalid Password"<<endl;
        return 1;
    }
    string choice;
    int ch=0;
    while(ch != 5)
    {
	//sub menu for file maintenance
        cout<<"***************************************************************************"<<endl;
        cout<<"1. Add product"<<endl;
        cout<<"2. Modify Stock"<<endl;
        cout<<"3. Modify Customer"<<endl;
        cout<<"4. Customer List"<<endl;
        cout<<"5. Main menu"<<endl;
    try{
        getline(cin,choice);
            if(choice[0]==' ' || !isdigit(choice[0]) || choice.size()>1) throw choice;
            ch=stoi(choice);   //string to int
        switch(ch)
        {
        case 1 :
		{
	   // thread creation
            thread t1(&add_product);
	    t1.join();//join thread
		}
            break;
        case 2:
	    {
            thread t2(&modify_product);
	    t2.join();
	    }
            break;
        case 3:
            modify_customer();
        case 4 :
            show_customer_list();
            break;
        case 5:
            break;
        default :
            throw(ch);
            break;

        }
    }
    catch(string s)
        {
            cout<<"Invalid Input Exception\nPlease Try again."<<endl;
        }
        catch(int ch)
        {
            cout<<"Invalid Choice Exception\nPlease Try again."<<endl;
        }

    }

}

/************************************************************************

  Function Name - add_product

  Function Description - To add new product 

************************************************************************/

int add_product()
{
    mtx.lock();	
    product tmp;
    long long int id;
    if(prod.size()>0)
    for(auto it : prod)
    {
	    id=it.get_pid();
    }
    else id=100;
    tmp.set_pid(id+1);
    tmp.set_pname();
    tmp.set_desc();
    tmp.set_ua();
    tmp.set_up();
    prod.push_back(tmp); //push data back to list
    tmp.show_product();
    cout<<"product added Successfully"<<endl;
    mtx.unlock();
    return 0;
}

/************************************************************************

  Function Name - modify_product

  Function Description - To edit details of the product by entering its 
                         product id

************************************************************************/

int modify_product()
{
    mtx.lock();	
    int id;
    cout<<"Enter Product ID"<<endl;
    cin>>id;
    cin.ignore();
    int flag=0;
    for(list<product>::iterator it = prod.begin();it!=prod.end();++it)   //to access complete list
    {
        if(it->get_pid()==id)   //to check id present or not
	{
            flag=1;
            string choice;
            int ch=0;
                cout<<"1. Modify name"<<endl;
                cout<<"2. Modify description"<<endl;
                cout<<"3. Modify Unit Price"<<endl;
                cout<<"4. Modify Units Available"<<endl;
                cout<<"5. Delete product"<<endl;
		cout<<"6. Back Menu"<<endl;
		//Exception Handling
            try{
                getline(cin,choice);
            if(choice[0]==' ' || !isdigit(choice[0]) || choice.size()>1) throw choice;
            ch=stoi(choice);

                switch(ch)
                {
                case 1 :

                    it->set_pname();
                    cout<<"Name modified"<<endl;
                    break;
                case 2:
                    it->set_desc();
                    cout<<"Description modified"<<endl;
                    break;
                case 3 :
                    it->set_up();
                    cout<<"Unit price modified"<<endl;
                    break;
                case 4 :
                    it->set_ua();
                    cout<<"Stock availability modified"<<endl;
                    break;
                case 5:
                    it=prod.erase(it);    //to delete product
                    break;
		case 6:
		    return 0;
                default :
                    throw ch;
                    break;
                }
            }
        catch(string s)
        {
            cout<<"Invalid Input Exception\nPlease Try again."<<endl;
        }
        catch(int ch)
        {
            cout<<"Invalid Choice Exception\nPlease Try again."<<endl;
        }


        }
    }
    if(flag==0){
        cout<<"Product ID not found"<<endl;
    }
    mtx.unlock();
    return 0;
}

/************************************************************************

  Function Name - modify_customer

  Function Description - To edit details of the customer by entering their 
                         customer id

************************************************************************/

int modify_customer()
{
    int id;
    cout<<"Enter customer ID"<<endl;
    cin>>id;
    cin.ignore();      //clear buffer
    int flag=0;
    for(list<customer>::iterator it = cust.begin();it!=cust.end();++it)   //access complete list using iterator it 
    {
        if(it->get_cid()==id){
            flag=1;
            string choice;
            int ch=0;
                cout<<"1. Modify name"<<endl;
                cout<<"2. Modify email"<<endl;
                cout<<"3. Modify Phone number"<<endl;
          try{      cout<<"4. Delete customer"<<endl;
		  cout<<"5. Back menu"<<endl;

            getline(cin,choice);
            if(choice[0]==' ' || !isdigit(choice[0]) || choice.size()>1) throw choice;
            ch=stoi(choice);
                switch(ch)
                {
                case 1 :
                    it->set_cname();
                    cout<<"Name modified"<<endl;
                    break;
                case 2:
                    it->set_email();
                    cout<<"email modified"<<endl;
                    break;

                case 3 :
                    it->set_phone();
                    cout<<"Phone number modified"<<endl;
                    break;

                case 4 :

                    it=cust.erase(it);            //to delete customer 
                    break;
                case 5:
                   return 0;
                default :
                    throw ch;
                    break;
                }
          }
          catch(string s)
        {
            cout<<"Invalid Input Exception\nPlease Try again."<<endl;
        }
        catch(int ch)
        {
            cout<<"Invalid Choice Exception\nPlease Try again."<<endl;
        }

        }
    }
    if(flag==0){
        cout<<"Customer ID not found"<<endl;
    }
    return 0;
}

/************************************************************************

  Function Name - show_customer_list

  Function Description - To show details of all customer

************************************************************************/

int show_customer_list()
{
    if(cust.size()==0)
    {
        cout<<"Empty Customer list"<<endl;
        return 0;
    }
    for(auto it : cust) it.show_customer();

    return 0;
}

/************************************************************************

  Function Name - sort_by_dist

  Function Description - To sort orders on the basis of distance

************************************************************************/

bool sortbydist(final_order &a,final_order &b)
{
    return (a.get_dist() < b.get_dist());
}


int getch() {
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


string pass_magic()
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


