#include <iostream>
#include<fstream>
#include <conio.h>
#include <iomanip>
#include <string>
#include <windows.h>
#include <ctime>
#include <array>
using namespace std;

// current date/time based on current system
time_t now = time(0);
tm *ltm = localtime(&now); // getting the system time

string countries[] = {"PK - Pakistan", "BG - Bangladesh", "SA - Saudi Arabia", "AE - Dubai", "SL - Sri Lanka", "IR - Iraq", "JP - Japan", "CH - China"}; // listing the countries
const int WIDTH = 100; // declaring the Frame Width
const int MAX_FLIGHT_RECORDS = 100;
const int MAX_CUSTOMER_RECORDS = 100;
const int MAX_TICKET_RECORDS = 10000;
string allFlights[MAX_FLIGHT_RECORDS][71] = {}; // 70 seats + flight name
string allCustomers[MAX_CUSTOMER_RECORDS][5] = {}; // PP number + First Name + Last Name + Phone Number + email
string allTickets[MAX_TICKET_RECORDS][3] = {}; // TicketID + Flight Name + PP number

void booking();

void gotoxy(int x, int y){ // helps to move the causer to the exact point in the console
  cout<<"\e["+to_string(y+1)+";"+to_string(x+1)+"H";
}

void maxsc(){
    HWND Hwnd = GetForegroundWindow();
    ShowWindow(Hwnd, SW_MAXIMIZE);
}

void clearScreen(){
    cout<<"\033[2J\e[H";
    cout<<endl<<endl<<endl<<endl;
}

void textColor(int fgColorCode=15, int bgColorCode=0){ // change the console font color as per the color code pass on the parameter
	cout<<"\e[38;5;"+to_string(fgColorCode)+";48;5;"+to_string(bgColorCode)+"m";
}

string colorText(string txt, int txtColorCode, int bgColorCode=0, bool printcenter=1, int endFgColorCode=15, int endBgColorCode=16){ // return the colored text
	string out = "\e[38;5;"+to_string(txtColorCode)+";48;5;"+to_string(bgColorCode)+"m"+txt+"\e[38;5;"+to_string(endFgColorCode)+";48;5;"+to_string(endBgColorCode)+"m";
	if(printcenter){ // if the text uses for printCenter function: length of the text has to be managed
        out += "`"; // helps to split the text on printCenter function
        int len = out.length()-txt.length();
        if(len<10){
            out+="0";
        }
        out+=to_string(len); // extra length will be joint for reduce while using printCenter
	}
	return out;
}

string airlinesName = colorText("BILLU AIRLINES", 3);

string upper(string txt){
    for (char i : txt){i = char(toupper(i));}
    return txt;
}

string lower(string txt){
    for (char i : txt){i = char(tolower(i));}
    return txt;
}

void printCenter(string text, int width, string begin = " ", char fillBy = ' ', string end = " ", bool avoid_spaces=0){ // helps to print the text in center with border
	string temp;
	if (end==" "){
		end = begin;
	}
	if(avoid_spaces){
        temp = "";
        for(int i=0; i<text.length(); i++){
            if(text[i]!=' ')temp += text[i];
        }
        text = temp;
	}
	int beginRed = 0;
	int txtRed = 0;
	int endRed = 0;

	temp = "";
	for(int i=0; i<begin.length();){
        if(begin[i]=='`'){
            beginRed = (int(begin[++i])-48)*10 + int(begin[++i])-48-2;
            i++;
        }
        else temp += begin[i++];
	}
	begin = temp;

	temp = "";
	for(int i=0; i<text.length();){
        if(text[i]=='`'){
            txtRed = (int(text[++i])-48)*10 + int(text[++i])-48-1;
            i++;
            continue;
        }
        temp += text[i++];
	}
	text = temp;

	temp = "";
	for(int i=0; i<end.length();){
        if(end[i]=='`'){
            endRed = (int(end[++i])-48)*10 + int(end[++i])-48-2;
            i++;
            continue;
        }
        temp += end[i++];
	}
	end = temp;

	if (!((width-text.length()-begin.length()-end.length())%2)){ // if the output length is odd
        if (!text.length()){
            text += fillBy; // if text is empty; it should be filled with FILLBY
        }else{
            text += " ";
        }
	}
	cout<<begin
		<<string((width-text.length()+txtRed-begin.length()+beginRed-end.length()+endRed)/2, fillBy)
		<<text
		<<string((width-text.length()+txtRed-begin.length()+beginRed-end.length()+endRed)/2, fillBy)
		<<end
	<<endl;
}

string center(string txt, int width, string begin="", char filby=' ', string end=""){
    if(!((width - txt.length()-end.length()-begin.length())%2)){
        txt += filby;
    }
    if(end==""){
        end = begin;
    }
    return begin+string((width-txt.length()-end.length()-begin.length())/2, filby)+txt+string((width-txt.length()-end.length()-begin.length())/2, filby)+end;
}

void caution(string text="", int colorCode=125, int width=WIDTH, bool blink=1){
	gotoxy(0, 0);
	if (text!=""){
        textColor(colorCode);
        if(blink)cout<<"\e[5m";
        printCenter("", width, "+", '-');
        printCenter(text, width, "|");
        printCenter("", width, "+", '-');
        textColor(15);
        cout<<"\e[0m";
	}else{
        cout<<string(width, ' ')<<endl<<string(width, ' ')<<endl<<string(width, ' ')<<endl;
	}
	cout<<endl;
}

string csv_to_array(string fileName, int col){
    string out;
    static string rows;
    static fstream file;
    file.open(fileName, ios::in);
    if(!file){
        file.open(fileName, ios::out);
        file.close();
        file.open(fileName, ios::in);
    }
    getline(file, rows);
    col = 0;
    int i = 0;
    while(rows[i]=='\n'){
        if(rows[i]==','){
            col++;
        }
        else if(rows[i]!=' '){
                out[col]=rows[i];
        }
        i++;
    }
//        for(char& i: rows){
//            if(i==',') col++;
//            else if(i!=' ')out[row][col]=i;
//        }
//    }
    return out;

}

string flightName(int from, int to, string year, string month, string date){
    string name = "";
    for(int i=0; i<2; i++){
        name+=countries[from][i];
    };
    for(int i=0; i<2; i++){
            name+=countries[to][i];
        }
    name += to_string(from)+to_string(to)+"-" + year + month + date;
    return name;
}

string ticketId(string flightName, string seatNo){
    return flightName+seatNo;
}

void arrayUpdate(){
    string row_text;
    fstream flight, customer, ticket;
    flight.open("flightDetails.csv", ios::in);
    customer.open("customerDetails.csv", ios::in);
    ticket.open("ticketDetails.csv", ios::in);

    if(!flight){
        flight.open("flightDetails.csv", ios::out);
        flight.close();
        flight.open("flightDetails.csv", ios::in);
    }
    if(!customer){
        customer.open("customerDetails.csv", ios::out);
        customer.close();
        customer.open("customerDetails.csv", ios::in);
    }
    if(!ticket){
        ticket.open("ticketDetails.csv", ios::out);
        ticket.close();
        ticket.open("ticketDetails.csv", ios::in);
    }
    int row = 0;

    while(getline(flight, row_text)){
        int col = 0;  // for allFlights array
        string flightName="";
        int index = 0; // for row_text
        while(1){
            flightName += row_text[index];
            index++;
            if(row_text[index]==',')break;
        }
        allFlights[row][col] = flightName;
        for(index; index<row_text.length(); index++){
            if(row_text[index]==',')col++;
            else if(row_text[index]!=' ')allFlights[row][col] = row_text[index];
        }
        row++;
    }
    row = 0;
    flight.close();
    while(getline(customer, row_text)){
        int col = 0;  // for allCustomer array
        int index = 0; // for row_text
        string text = "";
        for(index; index<row_text.length(); index++){
            if(row_text[index]==','){
                allCustomers[row][col] = text;
                text = "";
                col++;
            }
            else if(row_text[index]!=' ')text += row_text[index];
        }
        row++;
    }
    row = 0;
    customer.close();
    while(getline(ticket, row_text)){
        int col = 0;  // for allTicket array
        string flightName="";
        int index = 0; // for row_text
        string text = "";
        for(index; index<row_text.length(); index++){
            if(row_text[index]==','){
                allTickets[row][col] = text;
                text = "";
                col++;
            }
            else if(row_text[index]!=' ')text += row_text[index];
        }
        row++;
    }
    ticket.close();

}

void csvUpdate(){
    fstream flight, customer, ticket;
    flight.open("flightDetails.csv", ios::out);
    customer.open("customerDetails.csv", ios::out);
    ticket.open("ticketDetails.csv", ios::out);

    // updating flight Details
    for (int i=0; i<MAX_FLIGHT_RECORDS; i++){
        if(allFlights[i][0]!=""){
            for(int j=0; j<71; j++){
                flight<<allFlights[i][j]<<", ";
            }
            flight<<endl;
        }
    }

    // updating customer details
    for (int i=0; i<MAX_CUSTOMER_RECORDS; i++){
        if(allCustomers[i][0]!=""){
            for(int j=0; j<5; j++){
                customer<<allCustomers[i][j]<<", ";
            }
            customer<<endl;
        }
    }

    // updating ticket details
    for(int i=0; i<MAX_TICKET_RECORDS; i++){
        if(allTickets[i][0]!=""){
            for(int j=0; j<3; j++){
                ticket<<allTickets[i][j]<<", ";
            }
            ticket<<endl;
        }
    }

    flight.close();
    customer.close();
    ticket.close();


}

int available_seats(string flightName){
    arrayUpdate();
    int no_of_seats = 70;
    for (int i=0; i<MAX_FLIGHT_RECORDS; i++){
        if(allFlights[i][0]==flightName){
            for(int j=1; j<71; j++){
                if(allFlights[i][j]=="1")no_of_seats--;
            }
            return no_of_seats;
        }
    }
    return no_of_seats;
}

void showSeats(int start_x, int start_y, string flight_name, bool recently_added[70]){
    int x = start_x, y = start_y;
    gotoxy(x, y++);
    arrayUpdate();
    bool seats[70]={};
    int width = 48;
    printCenter("| "+flight_name+" |", width, "+", '-');
    for(int i=0; i<32; i++){
        gotoxy(x, y++);
        printCenter("", width, "|");
    }
    gotoxy(x, y++);
    printCenter("", width, "+", '-');
    gotoxy(start_x, start_y+3);
    y = 3;
    bool flight_name_not_available = true;
    char available_seat = 255, unavailable_seat = 254;


    printSeats:
        for(int i=0; i<MAX_FLIGHT_RECORDS; i++){
            if(allFlights[i][0]==flight_name){
                flight_name_not_available = false;
                for(int j=0; j<70; j++){
                    if(!(j%7)){
                        gotoxy(start_x, y++);
                        printCenter("", width, "|"); // blank line
                        gotoxy(start_x, y++); // next line
                        cout<<"| ";
                        for(int k=0; k<7; k++){
                            cout<<"  ";
                            char seatAlphabet = (j/7)+65; // j/7 returns the row number [1-A, 2-B...]
                            cout<<seatAlphabet<<"0"<<k;
                            if (k==1 || k==4){cout<<"   ";}
                        }
                        cout<<"   |";
                        gotoxy(start_x+2, y++); // next line
                    }
                    cout<<"  [";
                    if(allFlights[i][j+1]=="1"){
                        if(recently_added[j])cout<<colorText(string(1, unavailable_seat), 51, 0, 0);
                        else cout<<colorText(string(1, unavailable_seat), 124, 0, 0);
                    }else{cout<<available_seat;}
                    cout<<"]";
                    if(j%7==1 || j%7==4){cout<<"   ";}
                }
                break;
            }
        }
    if(flight_name_not_available){
        for(int i=0; i<MAX_FLIGHT_RECORDS; i++){
            if(allFlights[i][0]==""){
                allFlights[i][0] = flight_name;
                for(int j=1; j<71; j++){
                    allFlights[i][j] = "0";
                }
                csvUpdate();
                goto printSeats;
            }
        }
    }
}

int seatNoToIndex(string seat_no){
    int out = ((seat_no[0]-65)*7)+seat_no[2]-48;
    return out;
}

string indexToSeatNo(int index){
    string out = "";
    out += char(index/7+65);
    out += "0";
    out += char(index%7+48);
    return out;
}

int userBooked(string flight_name, string pp_no){
    arrayUpdate();
    for(int i=0; i<MAX_TICKET_RECORDS; i++){
        if(allTickets[i][1]==flight_name && allTickets[i][2]==pp_no) return i;
        if(allTickets[i][0]=="") break;
    }
    return 0;
}

int seatBooked(string flight_name, int seat_no){
    for(int i=0; i<MAX_FLIGHT_RECORDS; i++){
        if(allFlights[i][0]==flight_name && allFlights[i][seat_no+1]=="1") return true;
        else if(allFlights[i][0]=="") return false;
    }
    return false;

}

int alreadyRegistered(string pp_no){
    for(int i=0; i<MAX_CUSTOMER_RECORDS; i++){
        if(allCustomers[i][0]==pp_no) return i+1;
        if(allCustomers[i][0] == ""){
            caution("breaked", 125, 60);
            return 0;
        }
    }
}

string chrToStr(char in[]){
    string out="";
    int index=0;
    while(in[index]){
        out += in[index];
        index++;
    }
    return out;
}

void printTicket(string pp_no, string flightName){
    const int WIDTH = 99;
    string seatNo;
    arrayUpdate();
    string name = "";
    for(int i=0; i<MAX_CUSTOMER_RECORDS; i++){
        if(allCustomers[i][0]==pp_no){
            name = allCustomers[i][1] + " " + allCustomers[i][2];
            break;
        }
        if(allCustomers[i][0]=="") {
            caution(to_string(i));
        }
    }
    for(int i=0; i<MAX_TICKET_RECORDS; i++){
        if(allTickets[i][1]==flightName && allTickets[i][2]==pp_no){
            seatNo = string(1, allTickets[i][0][15])+string(1, allTickets[i][0][16])+string(1, allTickets[i][0][17]);
        }
    }
    string from = countries[stoi(string(1, flightName[4]))];
    string to = countries[stoi(string(1, flightName[5]))];
    string date = string(1, flightName[7])+string(1, flightName[8])+string(1, flightName[9])+string(1, flightName[10])+" - "+string(1, flightName[11])+string(1, flightName[12])+" - "+string(1, flightName[13])+string(1, flightName[14]);
    string barcode;
    string barcodeNo;
    string flg;

    if(flg.length()%2==1)flg += " ";
    if(to.length()%2==0)to += " ";
    if(from.length()%2==0)from = " " + from;
    int val[4] = {186, 219, 221, 222}; // bar code chars
    srand(time(0));
    for (int i=0; i<25; i++){
        int index = rand() % 4;
        barcode += char(val[index]);
        if(i%2) barcodeNo += " ";
        else barcodeNo += to_string(rand()%9);
    }

    int frameColor = 30;
    int frameFontColor = 16;
    int bodyColor = 15;
    int bodyFontColor = 16;
    textColor(16, 23);
    printCenter("", WIDTH, "+", '-');
    printCenter(colorText(from+" ==> "+to, 15, 23, 1, 16, 23), WIDTH, "|");
    printCenter("", WIDTH, "|", '-', "|");

    printCenter("", WIDTH, colorText("|           _                 |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', "|");
    printCenter("", WIDTH, colorText("|          | \\                |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', "|");
    printCenter("", WIDTH, colorText("|          |  \\               |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor)+string(10, ' ')+"Passport No. : "+pp_no, ' ', "|");
    printCenter("", WIDTH, colorText("|          |   \\              |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', "|");
    printCenter("", WIDTH, colorText("|  ___     |    \\             |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor)+string(10, ' ')+"Name         : "+name, ' ', "|");
    printCenter("", WIDTH, colorText("|  \\  \\____|     \\_________   |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', "|");
    printCenter("", WIDTH, colorText("|   \\                      \\  |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor)+string(10, ' ')+"Date         : "+date, ' ', "|");
    printCenter("", WIDTH, colorText("|   /  ____       _________/  |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', "|");
    printCenter("", WIDTH, colorText("|  /__/    |     /            |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor)+string(10, ' ')+"Seat No.     : "+seatNo, ' ', "|");
    printCenter("", WIDTH, colorText("|          |    /             |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', "|");
    printCenter("", WIDTH, colorText("|          |   /              |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', barcode+"   |");
    printCenter("", WIDTH, colorText("|          |  /               |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', barcode+"   |");
    printCenter("", WIDTH, colorText("|          |_/                |", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', barcode+"   |");
    printCenter("", WIDTH, colorText("|"+string(29, ' ')+"|", frameFontColor, frameColor, 1, bodyFontColor, bodyColor), ' ', barcodeNo+"   |");
    printCenter("", WIDTH, colorText("+", frameFontColor, frameColor, 1, frameFontColor, frameColor), '-', "+");
    textColor();
    cout<<endl;

    Sleep(1000); // to generate different qr by time bcz srand(time(0))
}

void whatNext(){
    printCenter("", WIDTH, "+", '-');
    printCenter("", WIDTH, "|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"+---+"+string(15, '-')+"+", ' ', "+---+"+string(15, '-')+"+"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"| 1 |Book Ticket    |", ' ', "| 2 |Exit           |"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"+---+"+string(15, '-')+"+", ' ', "+---+"+string(15, '-')+"+"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|");
    printCenter("", WIDTH, "+", '-');
    printCenter("+"+string(3, '-')+"+", WIDTH, "|");
	printCenter("|"+string(3, ' ')+"|", WIDTH, "|");
	printCenter("+"+string(3, '-')+"+", WIDTH, "|");
    printCenter("", WIDTH, "+", '-');
    char choice; // declaring input choice
	cout<<"\e[3F|"<<string(46, ' ')<<"| ";
	choice = getch();

	while(choice<49 || choice>50){ // checking the range
        choice = getch();
    }
    switch (choice){
        case 49:{booking();break;}
        case 50:{cout<<"\033[x";}
    }
}

void cancelBooking(){
//    const int WIDTH = 150;
        string customer[100][6]{};
        string txt, pp;
        int row = 0;
        int column = 0;
        bool ppNotAvailable = true;
        int totalLines = 0;
        bool removingIndexes[100];
        string newCustomers[100][6] = {};
        cout<<endl<<endl;
        const int FRAME_WIDTH = 51;
        printCenter(center("", FRAME_WIDTH,"+",'-'), WIDTH);
        printCenter(center("Cancel Booking", FRAME_WIDTH,"|",' '), WIDTH);
        printCenter(center("", FRAME_WIDTH,"+",'-'), WIDTH);
        printCenter(center("", FRAME_WIDTH+1,"|  +---|Passport No. |",'-', "+  |"), WIDTH);
        printCenter(center("", FRAME_WIDTH+1,"|  |"), WIDTH);
        printCenter(center("", FRAME_WIDTH,"|  +", '-', "+  |"), WIDTH);
        printCenter(center("", FRAME_WIDTH,"+",'-'), WIDTH);
        printCenter("", WIDTH, "+", '-');
        cout<<"\033[f"<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
//        printCenter("", 30, "|",' ', "|  |");

        textColor(10); cin>>pp; textColor(7);
        pp = upper(pp);
        while(ppNotAvailable){
            for(int i=0; i<MAX_TICKET_RECORDS; i++){
                if(allCustomers[i][0]=="")break;
                if(allCustomers[i][2]==pp){
                    ppNotAvailable = false;
//                    cout<<"\033[f \033[K"<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
                    break;
                }
            }
            if(ppNotAvailable){
                textColor(12);
                cout<<"\033[f \033[K";
                printCenter("No tickets has been booked under this Passport No. "+pp, WIDTH);
                textColor(7);
                cout<<endl<<endl<<endl<<endl<<endl<<endl;
                printCenter("|  |"+string(40, ' ')+"|  |", WIDTH, "|", ' ', "|");
                cout<<"\033[F";
                printCenter("", 30, "|", ' ', "|  |");
                textColor(10); cin>>pp; textColor(7);
                pp = upper(pp);
            }
        }
        printCenter("|----+"+string(41, '-')+"|", WIDTH, "|", ' ', "|");
        printCenter("| No |"+string(17, ' ')+"Details"+string(17, ' ')+"|", WIDTH, "|", ' ', "|");
        printCenter("|----+"+string(41, '-')+"|", WIDTH, "|", ' ', "|");
        pp = upper(pp);
        row = 0;
        for(auto & i : allCustomers){
            if(i[0].empty()){break;}
            if(i[0]==pp){for(int j=0; j<6; j++){customer[row][j] = i[j];}row ++;}
        }
        for(bool & removingIndex : removingIndexes){removingIndex = false;}
        for(int ii=0; ii<100; ii++){
            string no;
            if(customer[ii][0].empty()){break;}
            cout<<"\033[f"<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
            for(int i=0; i<100; i++){
                string cnclFlgNm, cnclYr, cnclMnth, cnclDt, cnclFrmCntry, cnclToCntry, cnclStNo, line1, line2, line3;
                if(customer[i][0].empty()){break;}
                for(int j=0; j<customer[i][4].length(); j++){
                    if(customer[i][4][j]=='-'){
                        for(int k=j+1; k<j+5; k++){cnclYr += customer[i][4][k];}
                        for(int k=j+5; k<j+7; k++){ cnclMnth += customer[i][4][k];}
                        for(int k=j+7; k<j+9; k++){cnclDt += customer[i][4][k];}
                        break;
                    }
                    cnclFlgNm += customer[i][4][j];
                }
                switch(cnclFlgNm[0]+0){
                    case 74:{cnclFrmCntry = "Japan"; break;}
                    case 75:{cnclFrmCntry = "Korea"; break;}
                    case 67:{cnclFrmCntry = "China"; break;}
                    case 66:{cnclFrmCntry = "Bangladesh"; break;}
                    case 83:{cnclFrmCntry = "Sri Lanka"; break;}
                    case 80:{cnclFrmCntry = "Pakistan"; break;}
                    case 73:{cnclFrmCntry = "Iran"; break;}
                    case 79:{cnclFrmCntry = "Oman"; break;}
                    case 69:{cnclFrmCntry = "Egypt"; break;}
                    case 76:{cnclFrmCntry = "Libya"; break;}
                }
                switch(cnclFlgNm[1]+0){
                    case 74:{cnclToCntry = "Japan"; break;}
                    case 75:{cnclToCntry = "Korea"; break;}
                    case 67:{cnclToCntry = "China"; break;}
                    case 66:{cnclToCntry = "Bangladesh"; break;}
                    case 83:{cnclToCntry = "Sri Lanka"; break;}
                    case 80:{cnclToCntry = "Pakistan"; break;}
                    case 73:{cnclToCntry = "Iran"; break;}
                    case 79:{cnclToCntry = "Oman"; break;}
                    case 69:{cnclToCntry = "Egypt"; break;}
                    case 76:{cnclToCntry = "Libya"; break;}
                }
                cnclStNo = customer[i][5];
                line1 = cnclFrmCntry + " ==> " + cnclToCntry + " (" + cnclFlgNm + ")";
                line2 = "Date: " + cnclYr + "-" + cnclMnth + "-" + cnclDt;
                line3 = "Seat No.: " + cnclStNo;
                if(line1.length()%2==0){line1 += " ";}
                if(line2.length()%2==0){line2 += " ";}
                if(line3.length()%2==0){line3 += " ";}
                string lineNo = to_string(i+1);
                if(lineNo.length()==1){lineNo = "0"+lineNo;}
                if(removingIndexes[i]){textColor(12);}
                cout<<"\033[K";
                printCenter("|    |"+string((41-line1.length())/2, ' ')+line1+string((41-line1.length())/2, ' ')+"|", WIDTH, "|", ' ', "|");
                cout<<"\033[K";
                printCenter("| "+lineNo+" |"+string((41-line2.length())/2, ' ')+line2+string((41-line2.length())/2, ' ')+"|", WIDTH, "|", ' ', "|");
                cout<<"\033[K";
                printCenter("|    |"+string((41-line3.length())/2, ' ')+line3+string((41-line3.length())/2, ' ')+"|", WIDTH, "|", ' ', "|");
                if(removingIndexes[i])textColor(7);
                cout<<"\033[K";
                printCenter("|----+"+string(41, '-')+"|", WIDTH, "|", ' ', "|");
                totalLines = i+1;
            }
            if(ii!=0){
                printCenter("|  +---|Cancel more tickets? (Y/N) |"+string(8, '-')+"+  |", WIDTH, "|", ' ', "|");
                printCenter("|  |"+string(40, ' ')+"|  |", WIDTH, "|", ' ', "|");
                cout<<"\033[F";
                printCenter("", 30, "|", ' ', "|  |");
                cin>>no;
                while(upper(no)!="N" && upper(no)!="Y"){
                    cout<<"\033[F \033[F";
                    printCenter("|  +---|Cancel more tickets? (Y/N) |"+string(8, '-')+"+  |", WIDTH, "|", ' ', "|");
                    printCenter("|  |"+string(40, ' ')+"|  |", WIDTH, "|", ' ', "|");
                    cout<<"\033[F";
                    printCenter("", 30, "|", ' ', "|  |");
                    cin>>no;
                }
                if(upper(no)=="N"){break;}
                else{cout<<"\033[F"<<"\033[F";}
            }
            printCenter("|  +---|No. |"+string(31, '-')+"+  |", WIDTH, "|", ' ', "|");
            printCenter("|  |"+string(40, ' ')+"|  |", WIDTH, "|", ' ', "|");
            printCenter("|  +"+string(40, '-')+"+  |", WIDTH, "|", ' ', "|");
            printCenter("+"+string(46, '-')+"+", WIDTH, "|", ' ', "|");
            printCenter("", WIDTH, "+", '-', "+");
            cout<<"\033[F"<<"\033[F"<<"\033[F"<<"\033[F";
            printCenter("", 30, "|", ' ', "|  |");
            textColor(12); cin>>no; textColor(7);
            while (true) {
                try{
                    while(stoi(no)>totalLines || stoi(no)<1 || removingIndexes[stoi(no)-1]){
                        cout<<"\033[F";
                        printCenter("|  |"+string(40, ' ')+"|  |", WIDTH, "|", ' ', "|");
                        cout<<"\033[F";
                        printCenter("", 30, "|", ' ', "|  |");
                        textColor(12); cin>>no; textColor(7);
                    }break; }
                catch (std::invalid_argument){}
                cout<<"\033[F";
                printCenter("|  |"+string(40, ' ')+"|  |", WIDTH, "|", ' ', "|");
                cout<<"\033[F";
                printCenter("", 30, "|", ' ', "|  |");
                textColor(12); cin>>no; textColor(7);
            }
            removingIndexes[stoi(no)-1] = true;
        }
        cout<<endl<<endl<<endl;
        row = 0;
        for(auto & custom : allCustomers){
            if(custom[0].empty()){break;}
            if(custom[0]!=pp){for(int j=0; j<6; j++){newCustomers[row][j] = custom[j];}row++;}
        }
        for (int i=0; i<100; i++){
            if(allCustomers[i][0].empty()){break;}
            if(!removingIndexes[i]){for(int j=0; j<6; j++){newCustomers[row][j] = allCustomers[i][j];}row++;}
            else{
                for(auto & flight : allFlights){
                    if(flight[0].empty()){break;}
                    if(flight[0]==customer[i][4]){int stNo = (customer[i][5][0]-65)*7 + customer[i][5][2] - 47;flight[stNo] = "0";}
                }
            }
        }
}

void bookTickets(int no_of_passangers, string flight_name){
    const int width = 60;
    bool recently_booked_seats[70] = {};
    string recently_booked_PP[5] = {};
    for(int i=0; i<no_of_passangers; i++){
        string pp_no, f_name, l_name, phone_no, email;
        int seatNo;
        int index = 0; // for input iteration
        clearScreen();
        printCenter("| 0"+to_string(i+1)+" |", width, "+", '-');
        printCenter(airlinesName, width, "|");
        printCenter("", width, "+", '-');
        printCenter("", width, "|");
        printCenter("|Passport Number|", width, "|    +", '-', "+    |");
        printCenter("", width, "|    |", ' ', "|    |");
        printCenter("", width, "|    +", '-', "+    |");
        printCenter("", width, "|");
        printCenter("|First Name|", width, "|    +", '-', "+    |");
        printCenter("", width, "|    |", ' ', "|    |");
        printCenter("", width, "|    +", '-', "+    |");
        printCenter("", width, "|");
        printCenter("|Last Name|", width, "|    +", '-', "+    |");
        printCenter("", width, "|    |", ' ', "|    |");
        printCenter("", width, "|    +", '-', "+    |");
        printCenter("", width, "|");
        printCenter("|Phone Number|", width, "|    +", '-', "+    |");
        printCenter("", width, "|    |", ' ', "|    |");
        printCenter("", width, "|    +", '-', "+    |");
        printCenter("", width, "|");
        printCenter("|Email|", width, "|    +", '-', "+    |");
        printCenter("", width, "|    |", ' ', "|    |");
        printCenter("", width, "|    +", '-', "+    |");
        printCenter("", width, "|");
        printCenter("+-|Seat No|-+", width, "|");
        printCenter("|"+string(11, ' ')+"|", width, "|");
        printCenter("+"+string(11, '-')+"+", width, "|");
        printCenter("", width, "|");
        printCenter("", width, "+", '-');
        showSeats(width+5, 1, flight_name, recently_booked_seats);

        getPP:
            index = 0;
            arrayUpdate();
            char temp_pp_no[50] = {};
            while(1){
                gotoxy(0, 9);
                printCenter(temp_pp_no, width, "|    |", ' ', "|    |", 1);
                gotoxy(29+(index+1)/2, 9);
                char temp = toupper(getch());
                caution("", 125, width);
                if(temp=='\b' && index>=0){
                    temp_pp_no[--index] = '\0' ;
                    index--; // reducing the value only if it grater than 0
                }
                else if(temp==13){// if user press enter
                    if(index>0)break; // if it's not a first character
                    caution("Passport number cannot be empty", 125, width);
                    continue;
                }
                else if(temp==' ')continue;
                else{
                    temp_pp_no[index] = temp;
                }
                index++;
            }
            pp_no = chrToStr(temp_pp_no);
        if(userBooked(flight_name, pp_no)){
            caution("This user is already booked for this flight! ", 3, width, 0);
            goto getPP;
        }
        if(alreadyRegistered(pp_no)){
            caution("Already Registered user", 125, width, 0);
            int index_of_customer = alreadyRegistered(pp_no)-1;
            f_name = allCustomers[index_of_customer][1];
            l_name = allCustomers[index_of_customer][2];
            phone_no = allCustomers[index_of_customer][3];
            email = allCustomers[index_of_customer][4];
            gotoxy(0, 13);
            printCenter(f_name, width, "|    |", ' ', "|    |", 1);
            gotoxy(0, 17);
            printCenter(l_name, width, "|    |", ' ', "|    |", 1);
            gotoxy(0, 21);
            printCenter(phone_no, width, "|    |", ' ', "|    |", 1);
            gotoxy(0, 25);
            printCenter(email, width, "|    |", ' ', "|    |", 1);
        }
        else{
            getfName:
                char f_name[50] = {'\0'};
                index = 0;
                while(1){
                    gotoxy(0, 13);
                    printCenter(f_name, width, "|    |", ' ', "|    |", 1);
                    gotoxy(29+(index+1)/2, 13);
                    char temp = toupper(getch());
                    caution("", 125, width);
                    if(temp=='\b' && index>=0){
                        if(!index)goto getPP;
                        f_name[--index] = ' ' ;
                        index--;
                    }
                    else if(temp==13){// if user press enter
                        if(index)break; // if it's not a first character
                        caution("First Name cannot be empty", 125, width);
                        continue;
                    }
                    else if(temp==' ')continue;
                    else{
                        f_name[index] = temp;
                    }
                    index++;
                }

            getlName:
                char l_name[50] = {};
                index = 0;
                while(1){
                    gotoxy(0, 17);
                    printCenter(l_name, width, "|    |", ' ', "|    |", 1);
                    gotoxy(29+(index+1)/2, 17);
                    char temp = toupper(getch());
                    caution("", 125, width);
                    if(temp=='\b' && index>=0){
                        if(!index)goto getfName;
                        l_name[--index] = ' ' ;
                        index--;
                    }
                    else if(temp==13)break;// if user press enter
                    else if(temp==' ')continue;
                    else{
                        l_name[index] = temp;
                    }
                    index++;
                }

            getPhoneNo:
                char phone_no[50] = {};
                index = 0;
                while(1){
                    gotoxy(0, 21);
                    printCenter(phone_no, width, "|    |", ' ', "|    |", 1);
                    gotoxy(29+(index+1)/2, 21);
                    char temp = toupper(getch());
                    caution("", 125, width);
                    if(temp=='\b' && index>=0){
                        if(!index)goto getlName;
                        phone_no[--index] = ' ' ;
                        index--;
                    }
                    else if(temp==13){// if user press enter
                        if(index)break; // if it's not a first character
                        caution("Phone number cannot be empty", 125, width);
                        continue;
                    }
                    else if((temp!='+' && temp<48) || temp>57 || temp==' ')continue; // accepting only numbers and '+'
                    else phone_no[index] = temp;
                    index++;
                }

            getEmail:
                char email[50] = {};
                index = 0;
                int validMail = 0;
                while(1){
                    gotoxy(0, 25);
                    printCenter(email, width, "|    |", ' ', "|    |", 1);
                    gotoxy(29+(index+1)/2, 25);
                    char temp = tolower(getch());
                    caution("", 125, width);
                    if(temp=='@')validMail=index+1;
                    if(temp=='\b' && index>=0){
                        if(!index)goto getPhoneNo;
                        email[--index] = ' ' ;
                        index--;
                        if(index==validMail)validMail=0;
                    }
                    else if(temp==13){// if user press enter
                        if(index && validMail)break; // if it's not a first character and input contain '@'
                        if(!validMail)caution("Invalid Email", 125, width);
                        else caution("Email cannot be empty", 125, width);
                        continue;
                    }
                    else if(temp==' ')continue;
                    else{
                        email[index] = temp;
                    }
                    index++;
                }
            for(int i=0; i<MAX_CUSTOMER_RECORDS; i++){
                if(allCustomers[i][0]==""){
                    allCustomers[i][0] = pp_no;
                    allCustomers[i][1] = f_name;
                    allCustomers[i][2] = l_name;
                    allCustomers[i][3] = phone_no;
                    allCustomers[i][4] = email;
                    csvUpdate();
                    break;
                }
            }
        }
        getSeatNo:
            char seat_no[4] = {};
            index = 0;
            while(1){
                gotoxy(0, 29);
                printCenter(seat_no, width, "|"+string(22, ' ')+"|", ' ', "|"+string(22, ' ')+"|", 1);
                if(index==3)break;
                gotoxy(29+(index+1)/2, 29);

                char temp = toupper(getch());
                caution("", 125, width);
                if(temp=='\b' && index){
//                    if(!index)goto getlName;
                    seat_no[--index] = ' ' ;
                    index--;
                }
                else if(temp==13){// if user press enter
                    if(index)break; // if it's not a first character
                    else caution("seat cannot be empty", 125, width);
                    continue;
                }
                else if(temp==' ')continue;
                else{
                    seat_no[index] = temp;
                }
                index++;
            }
            seatNo = seatNoToIndex(chrToStr(seat_no));

        if(seatNoToIndex(seat_no)>=70 || seatNoToIndex(seat_no)<0 || seat_no[1]!='0' || seat_no[2]>54){
            caution("Invalid selection", 125, width);
            goto getSeatNo;
        }

        if(seatBooked(flight_name, seatNo)){
            caution(chrToStr(seat_no)+", Already booked", 125, width);
            goto getSeatNo;
        }

        for (int i=0; i<MAX_FLIGHT_RECORDS; i++){
            if(allFlights[i][0]==flight_name){
                allFlights[i][seatNo+1] = "1";
            }
        }

        for(int i=0; i<MAX_TICKET_RECORDS; i++){
            if(allTickets[i][0]==""){
                allTickets[i][0] = ticketId(flight_name, seat_no);
                allTickets[i][1] = flight_name;
                allTickets[i][2] = pp_no;
                csvUpdate();
                break;
            }
        }
        recently_booked_seats[seatNo] = 1;
        recently_booked_PP[i] = pp_no;
    }
    clearScreen();
    for(int i=0; i<5; i++){
        if(recently_booked_PP[i]=="")break;
        printTicket(recently_booked_PP[i], flight_name);
    }
    whatNext();

}

void booking(){
    bool flight_seats[70]={}; // status of each seats in the specific flight
    bool recently_booked_seats[70] = {};
    int from, to, no_of_passengers;
    string year="0000", month="00", date="00", flight_name;
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (1){
        clearScreen();
        printCenter("", WIDTH, "+", '-');
        printCenter(airlinesName, WIDTH, "|");
        printCenter("", WIDTH, "+", '-');
        printCenter("", WIDTH, "|");
        printCenter("=== BOOK YOU TICKETS ===", WIDTH, "|");
        printCenter("", WIDTH, "|");
        printCenter("", WIDTH, "|  +---|Country Codes|", '-', "+  |");
        printCenter("", WIDTH, "|  |");
        for (int i=0; i<8; i=i+2){
            printCenter("", WIDTH, "|  |"+string(10, ' ')+string(1, i+1+48)+". "+countries[i], ' ', string(20, ' ')+string(1, i+2+48)+". "+countries[i+1]+string(WIDTH/2-countries[i+1].length()-20, ' ')+"|  |");
        }
        printCenter("", WIDTH, "|  |");
        printCenter("", WIDTH, "|  +", '-', "+  |");
        printCenter("", WIDTH, "|");
        printCenter("", WIDTH, "|  +---|From: |---------------+", ' ', "+---|To: |-----------------+  |");
        printCenter("", WIDTH, "|  |"+string(26, ' ')+"|", ' ', "|"+string(26, ' ')+"|  |");
        printCenter("", WIDTH, "|  +"+string(26, '-')+"+", ' ', "+"+string(26, '-')+"+  |");
        printCenter("", WIDTH, "|");
        printCenter("", WIDTH, "|  +---|Date: |-----+", ' ', "+-|No. Passengers: |-+  |");
        printCenter("", WIDTH, "|  |                |", ' ', "|                    |  |");
        cout<<"\e[F|  | "+colorText(" yyyy ", 8, 0, 0)+colorText(" mm ", 8, 0, 0)+colorText(" dd ", 8, 0, 0)+" |\n";
        printCenter("", WIDTH, "|  +----------------+", ' ', "+--------------------+  |");
        printCenter("", WIDTH, "|");
        printCenter("", WIDTH, "+", '-');

        getFrom:
            caution();
            gotoxy(4, 20);
            cout<<string(26, ' ');
            gotoxy(4, 20);
            from = getch();
            while(from<49 || from>56){
                caution("Please enter valid input only [1 - 8]");
                gotoxy(4, 20);
                from = getch();
                caution();
                gotoxy(4, 20);
            }
            from -= 49;
            cout<<countries[from]<<endl;

        getTo:
            caution();
            gotoxy(WIDTH-31, 20);
            cout<<string(26, ' ');
            gotoxy(WIDTH-31, 20);
            to = getch();
            while(to<49 || to>56 || to==from+49){
                if (to == '\b') {
                    cout<<string(26, ' ');
                    goto getFrom;
                }
                if(to==from) caution("Invalid selection!!!");
                else caution("Please enter valid input only [1 - 8]");
                gotoxy(WIDTH-31, 20);
                to = getch();
                caution();
                gotoxy(WIDTH-31, 20);
            }
            to -= 49;
            cout<<countries[to]<<endl;

        getYear:
            gotoxy(6, 24);
            cout<<colorText("yyyy ", 8, 0, 0)+colorText(" mm ", 8, 0, 0)+colorText(" dd ", 8, 0, 0)+" |\n";
            for(int i=0; i<4;){
                gotoxy(6+i, 24);
                char temp = getch();
                if(!i && temp=='\b'){
                    i--;
                    cout<<colorText("yyyy ", 8, 0, 0)+colorText(" mm ", 8, 0, 0)+colorText(" dd ", 8, 0, 0)+" |\n";
                    goto getTo;
                }else if(temp=='\b'){
                    i--;
                    gotoxy(6+i, 24);
                    cout<<colorText("y", 8, 0, 0);
                    gotoxy(6+i, 24);
                }else if(temp>=48 && temp<=57){
                    year[i] = temp;
                    cout<<temp;
                    i++;
                }else{
                    caution("Invalid Input");
                    continue;
                }
                caution();
            }

        if(stoi(year)<(1900+ltm->tm_year)){ // checking the year [if it is less then current year program get the year again]
            caution("Invalid Year");
            goto getYear;
        }

        if(!(stoi(year)%4)) months[1] = 29; // if the year is a leap year, february should have 29 days

        getMonth:
            gotoxy(12, 24);
            cout<<colorText("mm ", 8, 0, 0)+colorText(" dd ", 8, 0, 0)+" |\n";
            for(int i=0; i<2;){
                gotoxy(12+i, 24);
                char temp = getch();
                if(!i && temp=='\b'){
                    i--;
                    cout<<colorText("mm ", 8, 0, 0)+colorText(" dd ", 8, 0, 0)+" |\n";
                    goto getYear;
                }else if(temp=='\b'){
                    i--;
                    gotoxy(12+i, 24);
                    cout<<colorText("m", 8, 0, 0);
                    gotoxy(12+i, 24);
                }else if(temp>=48 && temp<=57){
                    month[i] = temp;
                    cout<<temp;
                    i++;
                }else{
                    caution("Invalid Input");
                    continue;
                }
                caution();
            }

        if(!stoi(month) || (stoi(year)==(1900+ltm->tm_year) && stoi(month)<(1+ltm->tm_mon)) || stoi(month)>12){
            caution("Invalid Month");
            goto getMonth;
        }

        getDate:
            gotoxy(16, 24);
            cout<<colorText("dd ", 8, 0, 0)+" |\n";
            for(int i=0; i<2;){
                gotoxy(16+i, 24);
                char temp = getch();
                if(!i && temp=='\b'){
                    i--;
                    cout<<colorText("dd ", 8, 0, 0)+" |\n";
                    goto getMonth;
                }else if(temp=='\b'){
                    i--;
                    gotoxy(16+i, 24);
                    cout<<colorText("d", 8, 0, 0);
                    gotoxy(16+i, 24);
                }else if(temp>=48 && temp<=57){
                    date[i] = temp;
                    cout<<temp;
                    i++;
                }else{
                    caution("Invalid Input");
                    continue;
                }
                caution();
            }

        if (!stoi(date) || (stoi(year)==(1900+ltm->tm_year) && stoi(month)==(1+ltm->tm_mon) && stoi(date)<(ltm->tm_mday)) || stoi(date)>months[stoi(month)-1]){
            caution("Invalid Date");
            goto getDate;
        }
    }

        flight_name = flightName(from, to, year, month, date);
        int no_of_available_seats = available_seats(flight_name);
        caution(to_string(no_of_available_seats)+" Seats Available", 41, WIDTH, 0);
        showSeats(WIDTH+5, 1, flight_name, recently_booked_seats);
        gotoxy(85, 24);
        no_of_passengers = getch();
        while(no_of_passengers<49 || no_of_passengers >53 || no_of_passengers-48>no_of_available_seats){
            if(no_of_passengers  =='\b'){
                caution();
                showSeats(WIDTH+5, 1, "", recently_booked_seats);
                goto getDate;
            }
            if(no_of_passengers>53 && no_of_passengers<58) caution("Maximum 5 at once");
            else if(no_of_passengers-48>no_of_available_seats) caution("Sorry! only "+to_string(no_of_available_seats)+" available", 3, WIDTH, 0);
            else caution("Invalid Input");
            gotoxy(85, 24);
            no_of_passengers = getch();
        }
        no_of_passengers -= 48;
        cout<<no_of_passengers;
        caution();
        bookTickets(no_of_passengers, flight_name);
}

void mainMenu(){
//================Printing Main Manu=====================
    clearScreen();
    caution("Set console font size to 12 for best experience!");
	printCenter("", WIDTH, "+", '-');
	printCenter(airlinesName, WIDTH, "|");
	printCenter("", WIDTH, "+", '-');
	printCenter("", WIDTH, "|");
	printCenter("Welcome to " + airlinesName, WIDTH, "|");
	printCenter("", WIDTH, "|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"+---+"+string(15, '-')+"+", ' ', "+---+"+string(15, '-')+"+"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"| 1 |Book Tickets   |", ' ', "| 2 |Cancel Booking |"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"+---+"+string(15, '-')+"+", ' ', "+---+"+string(15, '-')+"+"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"+---+"+string(15, '-')+"+", ' ', "+---+"+string(15, '-')+"+"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"| 3 |MyBookings     |", ' ', "| 4 |Report an Issue|"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|"+string((WIDTH/2-20)/2, ' ')+"+---+"+string(15, '-')+"+", ' ', "+---+"+string(15, '-')+"+"+string((WIDTH/2-20)/2, ' ')+"|");
	printCenter("", WIDTH, "|");
	printCenter("+---+"+string(15, '-')+"+", WIDTH, "|");
	printCenter("| 5 |Exit           |", WIDTH, "|");
	printCenter("+---+"+string(15, '-')+"+", WIDTH, "|");
	printCenter("", WIDTH, "|");
	printCenter("", WIDTH, "+", '-');
	printCenter("+"+string(3, '-')+"+", WIDTH, "|");
	printCenter("|"+string(3, ' ')+"|", WIDTH, "|");
	printCenter("+"+string(3, '-')+"+", WIDTH, "|");
	printCenter("", WIDTH, "+", '-');
//=======================================================
	char choice; // declaring input choice
	cout<<"\e[3F|"<<string(46, ' ')<<"| ";
	choice = getch();
//=======================================================
    while(choice<49 || choice>53){ // checking the range
    //============Returning to the exact input location=============
        caution("Invalid input");
        gotoxy(49, 24);
        choice = getch();
    }
    switch (choice){
        case 49:{booking();break;}
        case 50:{}
        case 51:{}
        case 52:{}
        case 53:{cout<<"\033[x";}
    }
}





int main(){
	maxsc();
	Sleep(400);
//    mainMenu();
    booking();
//    bookTickets(2, "BGSL14-20250101");
////
//    printTicket("N10108512", "BGSL14-20250101");
//    printTicket("N10108520", "SLPK40-20240307");

//    seatNoToIndex("B01");
//mainMenu();
//cancelBooking();
}
