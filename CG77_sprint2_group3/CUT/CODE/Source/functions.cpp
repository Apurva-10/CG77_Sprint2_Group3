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

struct type_c
{
    long long int id;
    char name[10],email[20],phone[10],pass[8];
};
struct type_p
{
    long long int id;
    char name[10],desc[20];
    int ua,up;
};
struct type_child_o
{
    int ur=0;
    long long int pid=-1;
    float up=0;
    char pname[10];
};
struct type_o
{
    long long int order_id=0;
    struct type_child_o items[3];
    long long int cid=0;
    char phone[10],cname[10];
    float dist=0;
    int status=0;
    float total=0;
};

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
int file_to_ord();
int ord_to_file();
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

    ifstream fr("customer");
			if(fr)
			{
			    struct type_c cst;
			    for(int i=0;i<10;i++) cst.name[i]='%';
			    for(int i=0;i<20;i++) cst.email[i]='%';
			    for(int i=0;i<8;i++) cst.pass[i]='%';
				fr.read((char *) &cst,sizeof(type_c));
				while(!fr.eof())
				{
					string name;
					for(int i=0;i<10;i++){if(cst.name[i]=='%') break; else  name+=cst.name[i];}
					string email="";
					for(int i=0;i<strlen(cst.email);i++){if(cst.email[i]=='%') break; else  email+=cst.email[i];}
					string phone="";
					for(int i=0;i<10;i++) phone+=cst.phone[i];
					string pass="";
					for(int i=0;i<8;i++){if(cst.pass[i]=='%') break; else pass+=cst.pass[i];}
					customer c(cst.id,name,email, phone,pass);
					cust.push_back(c);
					fr.read((char *) &cst,sizeof(type_c));
				}
				fr.close();
			}
			return 0;
}

/************************************************************************

  Function Name - cust_to_file

  Function Description - To write all the data of list to the file

************************************************************************/

int cust_to_file()
{
            fstream ff1;
            struct type_c tmp;
	    for(int i=0;i<10;i++) tmp.name[i]='%';
	    for(int i=0;i<20;i++) tmp.email[i]='%';
	    for(int i=0;i<8;i++) tmp.pass[i]='%';
			if(cust.size() != 0)
			{
				ff1.open("customer", ios::out);
				for(auto t : cust)
				{
					tmp.id=t.get_id();
					string name = t.get_name();
					for(int i=0;i<name.size();i++) tmp.name[i]=name[i];
					string email=t.get_email();
					for(int i=0;i<email.size();i++) tmp.email[i]=email[i];
					string pass=t.get_pass();
					for(int i=0;i<pass.size();i++) tmp.pass[i]=pass[i];
					string phone=t.get_phone();
					for(int i=0;i<phone.size();i++) tmp.phone[i]=phone[i];

					ff1.write((char *) &tmp , sizeof(type_c));
				}
				ff1.close();
			}
			return 0;

}
/************************************************************************

  Function Name - file_to_prod

  Function Description - To read all the data of file to the list

************************************************************************/

int file_to_prod()
{
    ifstream fr("product");
			if(fr)
			{
			    struct type_p prd;
			    for(int i=0;i<10;i++) prd.name[i]='%';
			    for(int i=0;i<20;i++) prd.desc[i]='%';
				fr.read((char *) &prd,sizeof(type_p));
				while(!fr.eof())
				{
					string name;
					for(int i=0;i<10;i++){if(prd.name[i]=='%') break; else  name+=prd.name[i];}
					string desc="";
					for(int i=0;i<20;i++){if(prd.desc[i]=='%') break; else  desc+=prd.desc[i];}
					product p(prd.id,prd.ua,prd.up,name,desc);
					prod.push_back(p);
					fr.read((char *) &prd,sizeof(type_p));
				}
				fr.close();
			}
			return 0;

}

/************************************************************************

  Function Name - prod_to_file

  Function Description - To write all the data of list to the file

************************************************************************/


int prod_to_file()
{
    fstream ff1;
            struct type_p tmp;
	    for(int i=0;i<10;i++) tmp.name[i]='%';
	    for(int i=0;i<20;i++) tmp.desc[i]='%';
			if(cust.size() != 0)
			{
				ff1.open("product", ios::out);
				for(auto t : prod)
				{
					tmp.id=t.get_id();
					string name = t.get_name();
					for(int i=0;i<name.size();i++) tmp.name[i]=name[i];
					string desc=t.get_desc();
					for(int i=0;i<desc.size();i++) tmp.desc[i]=desc[i];
                    tmp.ua=t.get_ua();
                    tmp.up=t.get_up();

					ff1.write((char *) &tmp , sizeof(type_p));
				}
				ff1.close();
			}
			return 0;

}

int file_to_ord()
{
    ifstream fr("order");
			if(fr)
			{
			    struct type_o ordt;
			    for(int i=0;i<10;i++) ordt.cname[i]='%';
			    for(int j=0;j<3;j++)for(int i=0;i<10;i++) ordt.items[j].pname[i]='%';

				fr.read((char *) &ordt,sizeof(type_o));
				while(!fr.eof())
				{
				    vector<order> child;
				    for(int i=0;i<3;i++)
                    {
                        if(ordt.items[i].pid == -1) break;
                        string pname="";
                        for(int j=0;j<10;j++)
                        {
                            if(ordt.items[i].pname[j] == '%') break;
                            else pname+=ordt.items[i].pname[j];
                        }
                        order ttt(ordt.items[i].pid, ordt.items[i].up,pname,ordt.items[i].ur);
                        child.push_back(ttt);

                    }
                    string cname="";
                    for(int i=0;i<10;i++)
                    {
                        if(ordt.cname[i]=='%') break;
                       cname+=ordt.cname[i];
                    }
					string phone="";
					for(int i=0;i<10;i++)  phone+=ordt.phone[i];
					string status;
					if(ordt.status==0) status="Delivered";
					else status="In progress";
					final_order ooo(ordt.cid,phone,cname,ordt.dist,ord.size(),child);
					ooo.set_status(status);
					ord.push_back(ooo);
					fr.read((char *) &ordt,sizeof(type_o));
				}
				fr.close();
			}
			return 0;

}

int ord_to_file()
{
    fstream ff1;
            struct type_o tmp;
	    for(int i=0;i<10;i++) tmp.cname[i]='%';
        for(int j=0;j<3;j++)for(int i=0;i<10;i++) tmp.items[j].pname[i]='%';
			if(ord.size() != 0)
			{
				ff1.open("order", ios::out);
				for(auto it : ord)
				{
					tmp.cid=it.get_cid();
					string cname = it.get_cname();
					for(int i=0;i<cname.size();i++) tmp.cname[i]=cname[i];
					tmp.order_id=it.get_order_id();
					string phone=it.get_phone();
					for(int i=0;i<10;i++) tmp.phone[i]=phone[i];
					tmp.dist=it.get_dist();
					tmp.total=it.get_total_amount();
					if(it.get_status() == "Delivered") tmp.status=0;
					else tmp.status=1;
					for(int i=0;i<it.items.size();i++)
                    {
                        tmp.items[i].ur=it.items[i].get_ur();
                        tmp.items[i].up=it.items[i].get_up();
                        tmp.items[i].pid=it.items[i].get_pid();
                        string pname=it.items[i].get_pname();
                        for(int j=0;j<it.items[i].get_pname().size();j++) tmp.items[i].pname[j]=pname[j];
                    }

					ff1.write((char *) &tmp , sizeof(type_o));
				}
				ff1.close();
			}
			return 0;

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
    for(auto it : cust) id=it.get_id();
    else id=1000;
    //to insert values
    tmp.set_id(id+1);
    tmp.set_name();
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
            if(pit->get_id()==vord[i].get_pid()) pit->set_ua(pit->get_ua()- vord[i].get_ur());   //to update stock details
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
                            if(pit->get_id()==pid)
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
                                    order tmp(pid, pit->get_up(), pit->get_name(), req);
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
                        final_order tmpord(it->get_id(),phone,it->get_name(), d,ord.size(),vord);
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
	if(delivery_list.size()==0)
	{cout<<"No more pending deliveries"<<endl;
		return 0;
	}
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
		    cout<<"No more pending orders left"<<endl;
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
	    id=it.get_id();
    }
    else id=100;
    tmp.set_id(id+1);
    tmp.set_name();
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
        if(it->get_id()==id)   //to check id present or not
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

                    it->set_name();
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
        if(it->get_id()==id){
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
                    it->set_name();
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
