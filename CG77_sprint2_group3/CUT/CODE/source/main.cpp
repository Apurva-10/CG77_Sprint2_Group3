/************************************************************************
The Oozy Snacks Bar Online Delivery System 

Filename - main.cpp

Description - main file for oozy snacks bar online delivery syestem

*************************************************************************/
#include "header.h"
int main()
{
    file_to_cust();  //file to customer
    file_to_prod();  //file to product
    int (*fun_ptr_arr[])() = {customer_registration, new_order, show_all_orders, show_product,consult_order,show_delivery, begin_delivery, file_maintenance, show_order_by_min}; //pointer to function
    string choice;
    int ch;
    while(ch != 10)
    {
        cout<<"*****************************************************************************"<<endl;
        cout<<"The Oozy Snacks Bar Online Delivery System"<<endl;
        cout<<"*****************************************************************************"<<endl<<endl<<endl;
        cout<<"1. Customer Registration"<<endl;
        cout<<"2. New Order"<<endl;
        cout<<"3. Show all orders"<<endl;
        cout<<"4. Product Availability List"<<endl;
        cout<<"5. Consult order"<<endl;
        cout<<"6. Show Delivery List"<<endl;
        cout<<"7. Begin Delivery"<<endl;
        cout<<"8. File Maintenance (only for Employees)"<<endl;
        cout<<"9. Show order by min value"<<endl;
        cout<<"10. Exit"<<endl;
	//exceptional handling
        try{
            getline(cin,choice);
	    if(choice=="10") break;
            if(choice[0]==' ' || !isdigit(choice[0]) || choice.size()>1) throw choice;
            ch=stoi(choice);
            if(ch < 1 || ch >10)
            throw(ch);
            if(ch==10) break;
            (*fun_ptr_arr[ch-1])();
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
    cust_to_file(); //customer list to file
    prod_to_file(); //product list to file
    return 0;
}
