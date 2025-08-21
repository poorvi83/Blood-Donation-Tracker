//BLOOD DONATION TRACKER
//FUNCTIONS :- insert, search, delete, display records
#include<iostream>
#include<fstream>
#include<vector>
#include <algorithm>
#include <windows.h>   // For colors and system("cls")
using namespace std;

class Donor
{
    private:
        string name, bloodGroup, city, contact, lastDonation;    //dd-mm-yyyy
    public:

        void inputInfo()
        {
            cin.ignore();    //to avoid newline input buffer
            cout<<"enter name: ";
            getline(cin, name);
            cout<<"enter blood Group: ";
            getline(cin, bloodGroup);
            cout<<"enter city: ";
            getline(cin, city);
            cout<<"enter contact: ";
            getline(cin, contact);
            cout<<"enter date of last donation (DD-MM-YYYY): ";
            getline(cin, lastDonation);
        }

        void displayDetails() const  //method doesn't modify data members
        {
            cout << "\n----------------------------------------------------";
        cout << "\nName: " << name
             << "\nBlood Group: " << bloodGroup
             << "\nCity: " << city
             << "\nContact: " << contact
             << "\nLast Donation Date: " << lastDonation;
        cout << "\n----------------------------------------------------\n";
        }

        string getBloodGroup() const {return bloodGroup;}
        string getCity() const {return city;}
        string getName() const {return name;}

        void updateContact(const string &newContact) 
            { contact = newContact; }

        void savetoFile()
        {
            ofstream out("donors.txt", ios::app);
            out << name << "|" << bloodGroup << "|" << city << "|"
            << contact << "|" << lastDonation << "\n";
            out.close();
        }
        static vector<Donor> loadFromFile()
        {
            vector<Donor> donors;
            ifstream in("donors.txt");
            string line;

            while(getline(in, line))
            {
                Donor d;
                size_t pos = 0;    //unsigned int
                string token;
                vector<string> fields;

                while((pos=line.find('|')) != string::npos)
                {
                    token= line.substr(0, pos);
                    fields.push_back(token);
                    line.erase(0, pos+1);
                }
                fields.push_back(line);

                if(fields.size() == 5)
                {
                    d.name=fields[0];
                    d.bloodGroup=fields[1];
                    d.city=fields[2];
                    d.contact=fields[3];
                    d.lastDonation=fields[4];
                    donors.push_back(d);
                }
            }
            in.close();
            return donors;
        }
};

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printHeader() {
    setColor(11); // Light cyan
    cout << "\n=========================================================\n";
    cout << "          BLOOD DONATION MANAGEMENT SYSTEM\n";
    cout << "=========================================================\n";
    setColor(7); // Reset
}

void searchDonor()
{
    string bg, city;
    cin.ignore();
    cout << "Enter Blood Group: ";
    getline(cin, bg);
    cout << "Enter City: ";
    getline(cin, city);

    vector<Donor> donors = Donor::loadFromFile();
    bool found=false;

    for(const auto &d : donors)
    {
        if(d.getBloodGroup()==bg  &&  d.getCity()==city)   //we are calling constant object here
        {
            d.displayDetails();
            found=true;
        }
    }
    if(!found) 
    {
        setColor(12);
        cout << "\nNo matching donors found.\n";
        setColor(7);
    }
}

void dispAllSorted()
{
    vector<Donor> donors= Donor::loadFromFile();
    sort(donors.begin(), donors.end(), [](const Donor &a, const Donor &b){
        return a.getName() > b.getName(); 
    });

    if (donors.empty()) {
        setColor(12);
        cout << "\nNo donor records available.\n";
        setColor(7);
        return;
    }

    for( const auto &d : donors)
        d.displayDetails();
}

// Function to delete a donor by name
void deleteDonorByName(const string &nameToDelete)
{
    vector<Donor> donors = Donor::loadFromFile();
    size_t originalSize = donors.size();

    donors.erase(remove_if(donors.begin(), donors.end(),
                           [&](const Donor &d) { return d.getName() == nameToDelete; }),
                 donors.end());

    if (donors.size() < originalSize) {
        ofstream file("donors.txt", ios::trunc); // overwrite file
        for (const auto &donor : donors) {
            file << donor.getName() << "|" 
                 << donor.getBloodGroup() << "|" 
                 << donor.getCity() << "|" 
                 << donor.getName() << "|"   // keep format consistent
                 << donor.getCity() << "\n";
        }
        file.close();
        setColor(10);
        cout << "Donor deleted successfully.\n";
        setColor(7);
    } 
    else 
    {
        setColor(12);
        cout << "No donor found with that name.\n";
        setColor(7);
    }
}


int main()
{
    int choice;
    string nametoDelete;
    do
    {
        system("cls");
        printHeader();
        setColor(14); // Yellow
        cout << "1. Add Donor\n";
        cout << "2. Search Donor\n";
        cout << "3. Display All Donors\n";
        cout << "4. Delete Donor Entry\n";
        cout << "5. Exit\n";
        setColor(7);
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:{ Donor d;
                    d.inputInfo();
                    d.savetoFile();
                    setColor(10);
                    cout << "\nDonor added successfully!\n";
                    setColor(7);
                    break; }  //create block scope in 'case'

            case 2: searchDonor();
                    break;

            case 3: dispAllSorted();
                    break;

            case 4: cout<<"enter name to delete";
                    cin.ignore();
                    getline(cin, nametoDelete);
                    deleteDonorByName(nametoDelete);
                    break;
            
            case 5: cout<<"exiting....";
                    exit(0);

            default: setColor(12);
                     cout << "Invalid choice! Try again.\n";
                     setColor(7);
        }
        system("pause");   //press any key to continue
    }while(1);
    
    return 0;
}


