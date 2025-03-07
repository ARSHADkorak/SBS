#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <conio.h>  // For password hiding
using namespace std;

class head {
public:
    int totalitems;
    vector<string> Iname;
    vector<float> Qty;
    vector<float> price;
    vector<int> vatprice;
    vector<int> tprice;
    vector<string> unit;  // Stores unit type (kg/g/pcs)
    void input();
    void output();
};

class vat : public head {
public:
    void vatcal();
    void outputs();
};

// Function to set console colors
void setConsoleColors() {
    system("color F1");  // White background, Blue text
}

// Function to set text color
void setTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Function to print header with blue background
void printHeader() {
    setTextColor(31); // Blue background, white text
    cout << "\n\n\t\t\t------------------------------";
    cout << "\n\t\t\t SHOP BILLING MANAGEMENT SYSTEM ";
    cout << "\n\t\t\t------------------------------\n";
    setTextColor(15); // Reset to default
}

void head::input() {
    system("CLS");
    printHeader();
    setTextColor(14); // Yellow text
    cout << "\nEnter number of items: ";
    cin >> totalitems;
    Iname.resize(totalitems);
    Qty.resize(totalitems);
    price.resize(totalitems);
    tprice.resize(totalitems);
    unit.resize(totalitems);
    setTextColor(15); // Reset

    for (int i = 0; i < totalitems; i++) {
        cout << "\nEnter name of item " << i + 1 << ": ";
        cin >> Iname[i];

        cout << "Is the item measured in (1) Pieces or (2) Weight (kg/g)? Enter choice (1/2): ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            unit[i] = "pcs";
        } else {
            unit[i] = "kg"; // Default to kg
        }

        cout << "Enter quantity (" << unit[i] << "): ";
        cin >> Qty[i];

        cout << "Enter price per " << unit[i] << ": ";
        cin >> price[i];

        tprice[i] = Qty[i] * price[i];
    }
}

void head::output() {
    ifstream infile("COUNT.TXT");
    int a = 0;
    if (infile) infile >> a;
    infile.close();

    ofstream outfile("COUNT.TXT");
    a += 1;
    outfile << a;
    outfile.close();

    ofstream history("HIS.TXT", ios::app);
    history << "\nBill No.: " << a << "\n";
    history << "------------------------------------------------------------------------" << "\n";

    setTextColor(11); // Cyan
    cout << "\nName of Item\t\tQuantity\t\tUnit\t\tPrice\t\tTotal Price\n";
    setTextColor(15);
    for (int i = 0; i < totalitems; i++) {
        history << "Name: " << Iname[i] << " Qty: " << Qty[i] << unit[i] << " Price: " << tprice[i] << "\n";
        setTextColor(10); // Green text
        cout << Iname[i] << "\t" << Qty[i] << "\t" << unit[i] << "\t" << price[i] << "\t" << tprice[i] << '\n';
        setTextColor(15); // Reset
    }
    history.close();
}

void vat::vatcal() {
    input();
    vatprice.resize(totalitems);
    for (int i = 0; i < totalitems; i++) {
        vatprice[i] = tprice[i] + (tprice[i] * (price[i] <= 100.00 ? 0.03 : 0.1));
    }
}

void vat::outputs() {
    output();
    float cash = 0, sum = 0, qty = 0, sumt = 0;
    for (int i = 0; i < totalitems; i++) {
        sumt += tprice[i];
        sum += vatprice[i];
        qty += Qty[i];
    }
    setTextColor(14); // Yellow text
    cout << "\nTotal:";
    cout << "\n------------------------------------------------------------------------------";
    setTextColor(15);
    cout << "\n\tTotal Quantity = " << qty;
    cout << "\t\tSubtotal = " << sumt;
    cout << "\tWith VAT = " << sum;
    cout << "\n------------------------------------------------------------------------------";
    setTextColor(15);
    do {
        cout << "\n\n\t\t\t****PAYMENT SUMMARY****\n";
        cout << "\n\t\t\tTotal cash given: ";
        cin >> cash;
    } while (cash < sum && (cout << "\n\t\t\tCash given is less than total amount!!!"));
    setTextColor(12); // Red for change
    cout << "\n\t\t\tTotal cash repaid: " << cash - sum << '\n';
    setTextColor(15);
}

int passwords() {
    char pass[4];
    cout << "\n\n\n\n\n\t\t\tENTER THE PASSWORD: ";
    for (int i = 0; i < 4; i++) {
        pass[i] = getch();
        cout << '*';
    }
    pass[4] = '\0';
    return strcmp(pass, "MARS") == 0;
}

int main() {
    setConsoleColors();
    vat obj;
    char opt;
    int attempts = 2;

    // Password protection
    if (!passwords()) {
        while (attempts-- > 0) {
            cout << "\nWrong password, try again.";
            if (passwords()) goto menu;
        }
        cout << "\n\n\t\t\tAll attempts failed... Exiting.";
        return 0;
    }

menu:
    while (true) {
        system("CLS");
        printHeader();
        setTextColor(13); // Purple text
        cout << "\n\t1. New Entry\n\t2. View Previous Entries\n\t3. Exit\n";
        setTextColor(15);
        cout << "\nEnter your option: ";
        cin >> opt;

        switch (opt) {
            case '1':
                obj.vatcal();
                obj.outputs();
                break;

            case '2': {
                ifstream fin("HIS.TXT", ios::in);
                char ch;
                while (fin.get(ch)) {
                    cout << ch;
                }
                fin.close();
                system("PAUSE"); // Wait before returning to menu
                break;
            }

            case '3':
                cout << "\n\n\t\t\tExiting... Thank you!\n";
                exit(0);

            default:
                cout << "\nInvalid option! Please try again.";
                system("PAUSE");
        }
    }

    return 0;
}
