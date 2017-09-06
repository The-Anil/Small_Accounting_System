
//***************************************************************//
// HEADER FILE USED IN PROJECT                                   //
//***************************************************************//
#include<iostream>
#include<fstream>
#include<conio.h>
#include<ctype.h>
#include<iomanip>
#include<cstdlib>
#include<time.h>
using namespace std;


//***************************************************************//
// CLASS USED IN PROJECT                                         //
//***************************************************************//

class account
{
	int acno, deposit, withdraw;
	char name[50];
	char type;
	public:
	void create_account()
	{
	    time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

	    cout<<"\n\n==================================================\n";
		cout<<"************* NEW ACCOUNT ENTRY FORM *************\n";
		cout<<"==================================================\n\n";
		cout<<"\nEnter The account Number : ";
		cin>>acno;
		cout<<"\nEnter The Name of The account Holder : ";
		cin.ignore();
		cin.getline(name,50);
		cout<<"\nEnter Type of The account (C/S) \nC-->Current\tS-->Savings: \t";
		cin>>type;
		type=toupper(type);
		cout<<"\nEnter Initial amount\n>=500 for Saving \n>=1000 for current :\t";
		cin>>deposit;
		cout<<"\nDate of Opening The Account : "<<asctime (timeinfo);
		cout<<"\n\n\nCongratulations! Your Account Created Successfully ...........!!!!";
	}
	void show_account()
	{
		cout<<"\n\n============= ACCOUNT STATUS =============\n";
		cout<<"\nAccount No. : "<<acno;
		cout<<"\nAccount Holder Name : "<<name;
		cout<<"\nType of Account : "<<type;
		cout<<"\nBalance amount : "<<deposit;
	}
	void modify_account()
	{
		cout<<"\nAccount No. : "<<acno;
		cout<<"\nModify Account Holder Name : ";
		cin.ignore();
		cin.getline(name,50);
		cout<<"Modify Type of Account : ";cin>>type;
		cout<<"Modify Balance amount : ";cin>>deposit;
	}
	void dep(int x)
	{
		deposit+=x;
	}
	void draw(int x)
	{
		deposit-=x;
	}
	void report()
	{
		cout<<acno<<"\t"<<name<<"\t\t"<<type<<"\t\t"<<deposit<<endl;
	}
	int retacno()
	{
		return acno;
	}
	float retdeposit()
	{
		return deposit;
	}
	char rettype()
	{
		return type;
	}
};		//class ends here

//***************************************************************
// 	global declaration for stream object, object
//****************************************************************


 fstream fp;
 account ac;

//***************************************************************
// 	function to write in file
//****************************************************************

void write_account()
{
	fp.open("account.dat",ios::out|ios::app|ios::binary);
	ac.create_account();
	fp.write((char*)&ac,sizeof(account));
	fp.close();
}

//***************************************************************
// 	function to read specific record from file
//****************************************************************


void display_sp()
{
	int n;
	cout<<"\n\n============== BALANCE DETAILS ==============";
	cout<<"\n\nEnter the Account Number : ";
    cin>>n;
	int flag=0;
	fp.open("account.dat",ios::in);
	while(fp.read((char*)&ac,sizeof(account)))
	{
		if(ac.retacno()==n)
		{
			ac.show_account();
			flag=1;
		}
	}
	fp.close();
	if(flag==0)
		cout<<"\n\n........! Account Number does not exist !...........\n";
		cout<<"\n\n";
    system("pause");
}

//***************************************************************
// 	function to modify record of file
//****************************************************************

void modify_account()
{
	int no,found=0;
	cout<<"\n\n============== MODIFY RECORD ==============";
	cout<<"\n\nEnter the Account No. : ";
    cin>>no;
	fp.open("account.dat",ios::in|ios::out);
	while(fp.read((char*)&ac,sizeof(account)) && found==0)
	{
		if(ac.retacno()==no)
		{
			ac.show_account();
			cout<<"\n\n\n============== Enter the New Details ==============\n";
			ac.modify_account();
			int pos=-1*sizeof(ac);
			fp.seekp(pos,ios::cur);
			fp.write((char*)&ac,sizeof(account));
			cout<<"\n\n.........! Record Updated !.............\n\n";
			found=1;
		}
	}
	fp.close();
	if(found==0)
		cout<<"\n\n............! Record Not Found !..............\n\n";
		system("pause");
}

//***************************************************************
// 	function to delete record of file
//****************************************************************

void delete_account()
{
	int no;
	cout<<"\n\n============== Delete Record ==============";
	cout<<"\n\nEnter The Account No. : ";
    cin>>no;
	fp.open("account.dat",ios::in|ios::out);
	fstream fp2;
	fp2.open("Temp.dat",ios::out);
	fp.seekg(0,ios::beg);
	while(fp.read((char*)&ac,sizeof(account)))
	{
		if(ac.retacno()!=no)
		{
			fp2.write((char*)&ac,sizeof(account));
		}
	}
	fp2.close();
	fp.close();
	remove("account.dat");
	rename("Temp.dat","account.dat");
	cout<<"\n\n\t.............! Record Deleted !............\n\n";
	system("pause");
}

//***************************************************************
// 	function to display all accounts deposit list
//****************************************************************

void display_all()
{
	fp.open("account.dat",ios::in);
	if(!fp)
	{
		cout<<"ERROR!!! FILE COULD NOT BE OPEN ";
		getch();
		return;
	}
	cout<<"\n\n\t\t\tACCOUNT HOLDER LIST\n\n";
	cout<<"==========================================================================\n";
	cout<<"A/c no.\tNAME\t\tType\t\tBalance\n";
	cout<<"==========================================================================\n";
	while(fp.read((char*)&ac,sizeof(account)))
	{
		ac.report();
	}
	fp.close();
	cout<<"\n==========================================================================\n";
    system("pause");

}

//***************************************************************
// 	function to deposit and withdraw amounts
//****************************************************************

void deposit_withdraw(int option)
{
	int no,found=0,amt;
	cout<<"\n\n================== ACCOUNT TRANSCATION FORM ==================";
	cout<<"\n\nEnter The account No. : ";
	cin>>no;
	fp.open("account.dat",ios::in|ios::out);
	while(fp.read((char*)&ac,sizeof(account)) && found==0)
	{
		if(ac.retacno()==no)
		{
			ac.show_account();
			if(option==1)
			{
				cout<<"\n\nEnter The amount to DEPOSIT : ";
				cin>>amt;
				ac.dep(amt);
			}
			if(option==2)
			{
				cout<<"\n\nEnter The amount to WITHDRAW : ";
				cin>>amt;
				int bal=ac.retdeposit()-amt;
				if((bal<500 && ac.rettype()=='S') || (bal<1000 && ac.rettype()=='C'))
					cout<<"\n.............! Insufficient balance !............\n";
				else
					ac.draw(amt);
			}
			int pos=-1*sizeof(ac);
			fp.seekp(pos,ios::cur);
			fp.write((char*)&ac,sizeof(account));
			cout<<"\n\n.........! Record Updated !.............\n";
			found=1;
		}
	}
	fp.close();
	if(found==0)
		cout<<"\n\n............! Record Not Found !..............\n";
	system("pause");
}


//exit from the system.............................
void goout()
{
system("cls");
cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
cout<<"\t\t\t\t\t@@ _______________________________________________________________________________________ @@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                           THANKS FOR BEING A MEMBER OF-                               |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                             IIIT BANK OF ENGINEERS                                    |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                                       -always faithful to their customers             |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                                                       -:prepared by Anil Kumar Yadav  |@@\n";
cout<<"\t\t\t\t\t@@|_______________________________________________________________________________________|@@\n";
cout<<"\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n\n\n\t\t\t\t\t";
system("pause");
}
//***************************************************************
// 	INTRODUCTION FUNCTION
//****************************************************************

void intro()
{

cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
cout<<"\t\t\t\t\t@@ _______________________________________________________________________________________ @@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                                           		                                  |@@\n";
cout<<"\t\t\t\t\t@@|                         WELCOME TO THE ACCOUNTING SYSTEM OF -                         |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                              IIIT BANK OF ENGINEERS                                   |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                                          - on whom you can TRUST                      |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                                                                                       |@@\n";
cout<<"\t\t\t\t\t@@|                                                       -:prepared by Anil Kumar Yadav  |@@\n";
cout<<"\t\t\t\t\t@@|_______________________________________________________________________________________|@@\n";
cout<<"\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n\n\n\t\t\t\t\t";
system("pause");
system("cls");

}


//information about the bank...............option 8.....
void information()
{
        time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
    	ifstream file;
        file.open("about_bank.txt");
		if(!file)
		{
		cout<<"\nError while opening the file\n";system("pause");system("cls");;
		}
		else
		{
            cout<<"\n\n\t\t\t\t\t\t\t\t\t"<<asctime (timeinfo);cout<<"\n\n\n";
		    string line;
			while(file.good())
			{
			getline(file,line);
			cout<<line<<"\n\t\t";
			}
			cout<<"\n\n\t\t";
			system("pause");
            system("cls");

		}
}
//***************************************************************
// 	THE MAIN FUNCTION OF PROGRAM
//****************************************************************

int main()
{
time_t rawtime;
struct tm * timeinfo;

time ( &rawtime );
timeinfo = localtime ( &rawtime );

	char ch;
	intro();
	do
	{
		system("cls");
		be:
        cout<<"\n\n\t\t\t\t\t\t\t\t"<< asctime (timeinfo);
		cout<<"\n\n\n\t\t\t\t\t\t*******************";
		cout<<"\n\t\t\t\t\t\t*    MAIN MENU    *";
		cout<<"\n\t\t\t\t\t\t*******************\n\n\n";
		cout<<"\n\t\t\t\t\t\t***************************************";
		cout<<"\n\n\t\t\t\t\t\t01. OPEN A NEW ACCOUNT";
		cout<<"\n\n\t\t\t\t\t\t02. DEPOSIT AMOUNT";
		cout<<"\n\n\t\t\t\t\t\t03. WITHDRAW AMOUNT";
		cout<<"\n\n\t\t\t\t\t\t04. BALANCE ENQUIRY";
		cout<<"\n\n\t\t\t\t\t\t05. ALL ACCOUNT HOLDER LIST";
		cout<<"\n\n\t\t\t\t\t\t06. CLOSE AN ACCOUNT";
		cout<<"\n\n\t\t\t\t\t\t07. MODIFY AN ACCOUNT";
		cout<<"\n\n\t\t\t\t\t\t08. INFORMATION ABOUT THE BANK";
		cout<<"\n\n\t\t\t\t\t\t09. EXIT";
		cout<<"\n\n\t\t\t\t\t\t***************************************";
		cout<<"\n\n\n\n\t\t\t\t\t\tSelect Your Option (1-9) : \t";
		ch=getch();
		system("cls");
		switch(ch)
		{
			case '1': write_account();
				getch();
				break;
			case '2': deposit_withdraw(1);
				break;
			case '3': deposit_withdraw(2);
				break;
			case '4': display_sp();
				break;
			case '5': display_all();
				break;
			case '6': delete_account();
				break;
			case '7': modify_account();
				break;
            case '8': information();
                goto be;
                break;
			case '9': goout();
                exit(0);

			default : cout<<"\a";
		}
	}while(ch!='9');

return 0;
}

//***************************************************************
//		END OF PROJECT
//***************************************************************
