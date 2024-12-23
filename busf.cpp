#include<bits/stdc++.h>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include<process.h>
#include<conio.h>
#include<cstring>
using namespace std;
#define el "\n"
#define sp " "
int ticket_num();
int seat_validation(char busn[50], int seatn);
void search_ticket(int x);

/*Class of bus*/
class bus
{
	public:
	int bno;
	char driver[50];
	char source[50];
	char destination[50];
	char arrival_time[50];
	char departure_time[50];

		void createb();
		void showb();
		void showlist();
		char bus_no[50];
		int seat_no;

};


void bus::createb()							//To enter data in data members of class book
{
	int i;
	cout<<"\n\t\tEnter the details:-\n";
	cout<<"\n\t\tEnter bus registration number: ";
	char n[50];
	cin.getline(n,50);
	cin.getline(bus_no,50);
	for(i=0;bus_no[i]!='\0';i++)
	{
		if(bus_no[i]>='a'&&bus_no[i]<='z')
		bus_no[i]-=32;
	}
	cout<<"\n\t\tEnter Driver's Name: ";
	cin.getline(driver,50);
	cout<<"\n\t\tEnter Source location: ";
	cin.getline(source,50);
	cout<<"\n\t\tEnter destination location: ";
	cin>>destination;
	cout<<"\n\t\tEnter arrival time: ";
	cin>>arrival_time;
	cout<<"\n\t\tEnter departure time: ";
	cin>>departure_time;
	cout<<"\n\t\tEnter seats number: ";
	cin>>seat_no;
}


void bus::showb()											//To display the details of books
{
	cout<<"\n\t\tBus no.: "<<bus_no;
	cout<<"\n\t\tDriver: "<<driver;
	cout<<"\n\t\tSource: "<<source;
	cout<<"\n\t\tDestination: "<<destination;
	cout<<"\n\t\tArrival time: "<<arrival_time;
	cout<<"\n\t\tDeparture time: "<<departure_time;
	cout<<"\n\t\tNumber of seats "<<seat_no;
}


void bus::showlist()										//To display book details in list form
{
	cout<<"\n\t"<<bus_no<<"\t\t"<<driver<<"\t\t"<<source<<"\t"<<destination<<"\t"<<arrival_time<<"\t\t"<<departure_time<<"\t\t"<<seat_no;
}

/*Class for Book Ticket*/
class ticket
{
    public:
        char customer_name[25], customer_add[25];
        void createt();
        void showt();
        void showlist();
        void reserve();
        int ticket_no, seat_no;
        char bno[50];
};
void ticket::createt()                //To enter values to all data members of class student
{
	int i;
	system("CLS");
	cout<<"\n\t\tEnter the details:-\n";
	cout<<"\n\t\tEnter Passenger's Name: ";
	cin>>customer_name;
	cout<<"\n\t\tEnter Passenger's address: ";
	cin>>customer_add;
	cout<<"\n\t\tEnter seat number: ";
	cin>>seat_no;
}
//// display ticket
void ticket::showlist()
{
    cout<<"\n\t"<<ticket_no<<"\t\t"<<bno<<"\t"<<customer_name<<"\t\t"<<customer_add<<"\t\t"<<seat_no;
}

//To display the details of ticket
void ticket::showt()
{
   	cout<<"\n\t\tTicket No: "<<ticket_no;
	cout<<"\n\t\tBus No: "<<bno;
	cout<<"\n\t\tPassenger Name: "<<customer_name;
	cout<<"\n\t\tPassenger Address: "<<customer_add;
	cout<<"\n\t\tSeat No: "<<seat_no;
}

///book ticket
int book_ticket(char busn[50])
{
    ticket tkt;
    tkt.ticket_no = ticket_num();
    strcpy(tkt.bno, busn);
    system("pause");
    tkt.createt();
    if(seat_validation(busn, tkt.seat_no)>0)
    {
        ofstream outf("ticket.bin",ios::app|ios::binary);
        outf.seekp(0,ios::end);
        int x=outf.tellp()/sizeof(ticket);
        outf.write(reinterpret_cast<char *>(&tkt),sizeof(ticket));
        cout<<"\n\t\tRecord added successfully";
        outf.close();
    }
    return 0;

}
/// set ticket number
int ticket_num() {
    ticket tkt;
    int cnt = 1, condition =0;
    ifstream intf("ticket.bin", ios::in | ios::binary);
    intf.seekg(0, ios::beg);
    if (!intf)
        cout << "\n\t\tFile not found";
    else {
        while (intf.read(reinterpret_cast<char *>(&tkt), sizeof(ticket)))
		{
        	if(cnt==tkt.ticket_no)
        	{
        		condition++;
			}
			cnt++;
        }
        return tkt.ticket_no+1;
        intf.close();
    }
}



/*To delete a specific book record*/
void cancel_ticket(int tno) {
    ticket bk;
    int cnt = 0;
    ifstream intf("ticket.bin", ios::in | ios::binary);

    if (!intf) {
        cout << "\n\t\tFile not found";
        return;
    }

    ofstream outf("temp1.bin", ios::out | ios::binary);
    if (!outf) {
        cout << "\n\t\tUnable to create temporary file";
        intf.close();
        return;
    }

    while (intf.read(reinterpret_cast<char*>(&bk), sizeof(ticket))) {
        if (bk.ticket_no == tno)
            cnt++;
        else
            outf.write(reinterpret_cast<char*>(&bk), sizeof(ticket));
    }

    intf.close();
    outf.close();

    if (cnt == 0) {
        remove("temp1.bin");
        cout << "\n\t\tNo such record exists";
    } else {
        char c;
        search_ticket(tno);
        cout << "\tRecord found!!! Do you wanna delete it? (Y/N): ";
        cin >> c;
        if (c == 'Y' || c == 'y') {
            if (remove("ticket.bin") != 0) {
                cout << "\n\t\tError deleting original file";
            } else if (rename("temp1.bin", "ticket.bin") != 0) {
                cout << "\n\t\tError renaming temporary file";
            } else {
                cout << "\n\t\tRecord deleted successfully";
            }
        } else {
            remove("temp1.bin");
        }
    }
}



//// verify the seat available  or not
int seat_validation(char busn[50], int seatn)
{
    int con = 0; // confirmation counter
    bus bk;
    ticket tk;
	int cnt=0;
	ifstream intf("bus.bin",ios::in|ios::binary);
	intf.seekg(0,ios::beg);
	if(!intf)
    {
        	cout<<"\n\t\tFile error!!!";
        	return 1;
    }
	else
	{
		while(intf.read(reinterpret_cast<char *>(&bk),sizeof(bus)))
		{
		    if(strcmp(bk.bus_no,busn )==0)
			{
				cnt++;
				if(bk.seat_no<seatn)
                {
                    cout<<"\n\t\tSeat number should less than total number of bus seats";
                    break;
                }
                else
                {
                    con++;
                    break;
                }
				break;
			}
		}
	}
	intf.close();

    /// check the particular seat is booked or not
    cnt=0;
	ifstream intg("ticket.bin",ios::in|ios::binary);
	intg.seekg(0,ios::beg);
	if(!intg)
    {
        return 1;
    }
	else
	{
		while(intg.read(reinterpret_cast<char *>(&tk),sizeof(ticket)))
		{
		    if(strcmp(tk.bno,busn)==0 && tk.seat_no==seatn)
			{
                cout<<"\n\t\tThe particular seat is booked!!! Try another seat.\n";
                return 0;
                break;
			}
			cnt++;
		}
		if(cnt==0)
        {
            cout<<"\n\t\tError!!!";
            return 0;
        }
	}
	intg.close();
    return con;
}

/*Display the list of tickets*/
/*To display book records in list form*/
void list_ticket()
{
	ticket bk;
	ifstream intf("ticket.bin",ios::in|ios::binary);
	intf.seekg(0,ios::beg);
	if(!intf)
	cout<<"\n\t\tFile not found";
	else
	{
		cout<<"\n\t*****Tickets Details*****\n\n";
		cout<<"\n\tTicket No\tBus No \tPassanger \tAddress \tSeat No:";
		while(intf.read(reinterpret_cast<char *>(&bk),sizeof(ticket)))
		bk.showlist();
	}
	intf.close();
}
void search_ticket(int x)
{
    int cnt=0;
	ticket bk;
	ifstream intf("ticket.bin",ios::in|ios::binary);
	intf.seekg(0,ios::beg);
	if(!intf)
	cout<<"\n\t\tFile not found";
	else
	{
	    while(intf.read(reinterpret_cast<char *>(&bk),sizeof(ticket)))
		{
		    if(bk.ticket_no == x)
			{
				cnt++;
				cout<<"\n\t\tFILE FOUND!!!!";
				bk.showt();
				break;
			}
		}
	}
	intf.close();
}

/*To Write object of class bus in file*/
void write_bus()
{
	bus bk;
	ofstream outf("bus.bin",ios::app|ios::binary);
	outf.seekp(0,ios::end);
	int x=outf.tellp()/sizeof(bus);
	bk.createb();
	bk.showb();
	outf.write(reinterpret_cast<char *>(&bk),sizeof(bus));
	cout<<"\n\t\tRecord added successfully";
	outf.close();
}



/*To display book records in list form*/
void list_bus()
{
	bus bk;
	ifstream intf("bus.bin",ios::in|ios::binary);
	intf.seekg(0,ios::beg);
	if(!intf)
	cout<<"\n\t\tFile not found";
	else
	{
		cout<<"\n\t*****Bus Details*****\n\n";
		cout<<"\n\tBus no:\t\tDriver name: \t\tSource: \tDestination: \t Arrival time: \tDeparture time:\tSeat No:\n";
		cout<<"\t------------------------------------------------------------------------------------------------------------------\n";
		while(intf.read(reinterpret_cast<char *>(&bk),sizeof(bus)))
		bk.showlist();
	}
	intf.close();
}

/*To search for a specific book*/
int search_bus(char x[25])
{
	bus bk;
	int cnt=0;
	ifstream intf("bus.bin",ios::in|ios::binary);
	intf.seekg(0,ios::beg);
	if(!intf)
	cout<<"\n\t\tFile not found";
	else
	{
		while(intf.read(reinterpret_cast<char *>(&bk),sizeof(bus)))
		{
		    if(strcmp(bk.bus_no, x)==0)
			{
				cnt++;
				cout<<"\n\t\tFILE FOUND!!!!";
				bk.showb();
				return 1;
				break;
			}
		}
		if(cnt==0)
		cout<<"\n\t\tNo such record exists";
		return 0;
	}
	intf.close();
}
/*confirm bus

/*To modify the book records*/
void modify_bus(char x[50])
{
	bus bk;
	int cnt=0;
	fstream intf("bus.bin",ios::in|ios::out|ios::ate|ios::binary);
	intf.seekg(0,ios::beg);
	if(!intf)
	cout<<"\n\t\tFile not found";
	else
	{
		while(intf.read(reinterpret_cast<char *>(&bk),sizeof(bus)))
		{
			if(strcmp(bk.bus_no, x)==0)
			{
				cnt++;
				bk.createb();
				bk.showb();
				intf.seekp(intf.tellp()-sizeof(bus));
				intf.write(reinterpret_cast<char *>(&bk),sizeof(bus));
				cout<<"\n\t\tRecord Updated";
				break;
			}
		}
		if(cnt==0)
		cout<<"\n\t\tNo such record exists";
	}
	intf.close();
}



/*To delete a specific book record*/
void delete_bus(char x[50])
{
	bus bk;
	int cnt=0;
	ifstream intf("bus.bin",ios::in|ios::binary);
	intf.seekg(0,ios::beg);
	if(!intf)
	cout<<"\n\t\tFile not found";
	else
	{
		ofstream outf("temp1.bin",ios::app|ios::binary);
		while(intf.read(reinterpret_cast<char *>(&bk),sizeof(bus)))
		{
			if(strcmp(bk.bus_no, x)==0)
			cnt++;
			else
			outf.write(reinterpret_cast<char *>(&bk),sizeof(bus));
		}
		intf.close();
		outf.close();
		if(cnt==0)
		{
			remove("temp1.bin");
			cout<<"\n\t\tNo such record exists";
		}
		else
		{
            char c;
            cout<<"\n\tRecord found!!! Do you wanna delete it?(Y/N)";
            cin>>c;
            if(c=='Y'||c=='y')
            {
                remove("bus.bin");
                rename("temp1.bin","bus.bin");
                cout<<"\n\t\tRecord deleted successfully";
            }
		}
	}
}

// display list bus with ticket status
void bus_report() {
    bus bk;
    ticket tk;
    int booked_seat[100];
    ifstream intf("bus.bin", ios::in | ios::binary);
    ifstream intg("ticket.bin", ios::in | ios::binary);

    if (!intf) {
        cout << "\n\t\tFile not found: bus.bin";
        return;
    }
    if (!intg) {
        cout << "\n\t\tFile not found: ticket.bin";
        intf.close();
        return;
    }

    cout << "\n\t*****Bus Details*****\n\n";
    cout << "\nBus no:\tDriver name:\t\tSource:\tDestination:\t Arrival time:\tDeparture time:\tTotal Seat:\tBooked Seat No:";
    cout << "\n------------------------------------------------------------------------------------------------------------------\n";

    while (intf.read(reinterpret_cast<char*>(&bk), sizeof(bus))) {
        int i = 0;
        intg.clear();
        intg.seekg(0, ios::beg);

        while (intg.read(reinterpret_cast<char*>(&tk), sizeof(ticket))) {
            if (strcmp(bk.bus_no, tk.bno) == 0) {
                booked_seat[i] = tk.seat_no;
                i++;
            }
        }

        cout << "\n" << bk.bus_no << "\t" << bk.driver << "\t\t" << bk.source << "\t" << bk.destination << "\t" << bk.arrival_time << "\t\t" << bk.departure_time << "\t\t" << bk.seat_no << "\t";
        for (int j = 0; j < i; j++) {
            if (j > 0) cout << ",";
            cout << booked_seat[j];
        }
    }

    intf.close();
    intg.close();
}




/*Function that has features of Admin Menu*/
void admin_menu()
{
fine:
	system("PAUSE");
	system("CLS");
	int opt;
	char ad[50];
	cout<<"\n\n\n\t\t\t******BUS RECORD MENU******";
    cout<<"\n\n\t1.\tCREATE BUS RECORD";
    cout<<"\n\n\t2.\tDISPLAY ALL BUSES";
    cout<<"\n\n\t3.\tSEARCH BUS";
    cout<<"\n\n\t4.\tMODIFY BUS RECORD";
    cout<<"\n\n\t5.\tDELETE BUS RECORD";
    cout<<"\n\n\t6.\tBACK TO MAIN MENU";
    cout<<"\n\n\tPlease Enter Your Choice (1-6) ";
    cin>>opt;
    if(opt==1)
    {
    	system("CLS");
    	write_bus();
    	cout<<el;
    	goto fine;
	}
	else if(opt==2)
	{
		system("CLS");
    	list_bus();
    	cout<<el;
    	goto fine;
	}
	else if(opt==3)
	{
		system("CLS");
		cout<<"\n\n\n\t\tEnter the bus number:";
		cin>>ad;
		search_bus(ad);
		cout<<el;
		goto fine;
	}
	else if(opt==4)
	{
		system("CLS");
		cout<<"\n\n\n\t\tEnter the bus number:";
		cin>>ad;
		search_bus(ad);
		modify_bus(ad);
		cout<<el;
		goto fine;
	}
	else if(opt==5)
	{
		system("CLS");
		cout<<"\n\n\n\t\tEnter the bus number";
		cin>>ad;
		search_bus(ad);
		delete_bus(ad);
		cout<<el;
		goto fine;
	}
	else if(opt==6)
	return ;
	else
	{
		cout<<"\n\t\tEnter correct option";
		cout<<el;
		goto fine;
	}
}

/*Checks for correct password*/
//The password if predefined and has to be changed through the source code
//of application
bool passwords()
{
	int i=0;
	char ch,st[21],ch1[21]={"0000"};
	cout<<"\n\n\t\tEnter Password : ";
	while(1)
    {
    	ch=getch();
    	if(ch==13)
    	{
        	st[i]='\0';
        	break;
    	}
    	else if(ch==8&&i>0)
    	{
        	i--;
        	cout<<"\b \b";
    	}
    	else
    	{
    		cout<<"*";
    		st[i]=ch;
    		i++;
    	}
    }
    for(i=0;st[i]==ch1[i]&&st[i]!='\0'&&ch1[i]!='\0';i++);
    if(st[i]=='\0'&&ch1[i]=='\0')
    return 1;
    else
    return 0;
}

//Main function
int main()
{
	cout<<"\n\n\t\t\t*******************************************";
	cout<<"\n\t\t\t------------------------------------------";
	cout<<"\n\t\t\t\tBUS RESERVATION SYSTEM";
	cout<<"\n\t\t\t------------------------------------------";
	cout<<"\n\t\t\t*******************************************\n";
	bool a=passwords();
	if(!a)
	{
		for(int i=0;i<2;i++)
		{
			cout<<"\nWrong password";
			cout<<"\nYou have "<<2-i<<"attempts left";
			if(passwords())
			goto last;
			if(i==1)
			{
				cout<<"\n\n\n\t\t\t All attempts failed........";
				cout<<"\n\n\t\t\t Sorry, but you can't login";
				exit(0);
			}
		}
	}
	last:
		cout<<"\n\n";
	start:
		system("PAUSE");
		system("CLS");
		int opt;
		char ad[50];
		cout<<"\n\n\t\t\t------------------------------------------";
		cout<<"\n\t\t\t\tBUS RESERVATION SYSTEM";
		cout<<"\n\t\t\t------------------------------------------";
		cout<<"\n\n\t\t\tWhat do you want to do?";
		cout<<"\n\t\t\t1.\tRESERVE TICKET";
		cout<<"\n\t\t\t2.\tVIEW TICKET";
		cout<<"\n\t\t\t3.\tCANCEL TICKET";
		cout<<"\n\t\t\t4.\tBUS MENU";
		cout<<"\n\t\t\t5.\tExit";
		cout<<"\n\n Choose your option: ";
		cin>>opt;
		if(opt==1)
		{
		    t:
            system("CLS");
            bus_report();
			cout<<"\n\n\n\t\tEnter the bus number:";
            cin>>ad;
            system("CLS");
            if(search_bus(ad)>0)
            {
                book_ticket(ad);
            }
            else{
                goto t;
            }
            goto start;
		}
		else if(opt==2)
		{
			system("CLS");
			list_ticket();
			goto start;
		}
        else if(opt==3)
		{
		    int tn;
			system("CLS");
			cout<<"\n\n\n\t\tEnter ticket number:";
            cin>>tn;
			cancel_ticket(tn);
			goto start;
		}
		else if(opt==4)
		{
			admin_menu();
			goto start;
		}
		else if(opt==5)
		exit(0);
		else
		{
			cout<<"\n\t\tEnter correct option";
			goto start;
		}
}
