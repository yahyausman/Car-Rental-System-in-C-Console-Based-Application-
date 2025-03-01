#include <iostream>
#include <string>
#include <fstream> // For file handling
using namespace std;

// initializing variables and arrays as globals to access throughout the code
const int totalCars = 15;
string cars[totalCars] = {"Honda Civic 2023", "Suzuki Alto 2018", "Toyota Vitz 2012", "Toyota Prius 2012", "Suzuki Swift 2012", "Toyota Corolla 2024", "Hyundai Elantra 2022", "KIA Sportage 2023", "Honda City 2018", "Land Cruiser V8 2019", "Nissan Note 2018", "Suzuki Alto 2022", "Toyota Mark X 2006", "Mercedez Benz C200 2013", "BMW 530i 2010"};
bool availability[totalCars] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
int prices[totalCars] = {5000, 2500, 3000, 5500, 4000, 6500, 8000, 7000, 8700, 5500, 6000, 7500, 9450, 7650, 8500};
string rentedBy[totalCars];
string carCondition[totalCars] = {"OK", "OK", "OK", "OK", "OK", "OK", "OK", "OK", "OK", "OK", "OK", "OK", "OK", "OK", "OK"};
int rentalDuration[totalCars] = {0};

// File handling functions
void loadData();
void saveData();

// Function prototypes
void viewAvailableCars();
void rentCar();
void returnCar();
void viewRentedCars();
void adminMenu();
void customerMenu();
void loginSystem();
int getValidatedInt(string prompt);
string getValidatedString(string prompt);

void loadData()
{
    ifstream inFile("car_rental_data.txt");
    if (inFile.is_open())
    {
        for (int i = 0; i < totalCars; i++)
        {
            inFile >> availability[i];
            inFile.ignore(); // clearing the input buffer after boolean availability
            getline(inFile, rentedBy[i], ' ');
            inFile >> rentalDuration[i] >> carCondition[i];
        }
        inFile.close();
    }
    else
    {
        // Initialize default data if file is not available
        for (int i = 0; i < totalCars; i++)
        {
            availability[i] = true;
            rentedBy[i] = "";
            carCondition[i] = "OK";
            rentalDuration[i] = 0;
        }
    }
}

void saveData()
{
    ofstream outFile("car_rental_data.txt");
    if (outFile.is_open())
    {
        for (int i = 0; i < totalCars; i++)
        {
            outFile << availability[i] << " " << rentedBy[i] << " " << rentalDuration[i] << " " << carCondition[i] << endl;
        }
        outFile.close();
    }
}

void viewAvailableCars()
{
    cout << "\nAvailable Cars:\n";
    for (int i = 0, j = 0; i < totalCars; i++, j++)
    {
        if (availability[i])
        {
            cout << j + 1 << ". " << cars[i] << " - PKR " << prices[i] << " per day\n";
        }
    }
    loadData();
}

void rentCar()
{
    int carIndex, days;
    string customerCNIC;
    string paymentMethod;
    string cardNumber;

    cout << "\nEnter the car number to rent: ";
    cin >> carIndex;
    carIndex--;   // Convert to 0-based index
    cin.ignore(); // to clear the inputs

    if (carIndex >= 0 && carIndex < totalCars)
    {
        if (availability[carIndex])
        {

            customerCNIC = getValidatedString("Enter your CNIC without dashes: ");
            while (customerCNIC.length() != 13)
            {
                cout << "Invalid CNIC ! ";
                customerCNIC = getValidatedString("Enter your CNIC without dashes: ");
            }

            days = getValidatedInt("Enter the number of days you want to rent: ");

            // Ask for payment method
            cout << "\nChoose payment method:\n";
            cout << "1. Cash\n";
            cout << "2. Card\n";
            int methodChoice = getValidatedInt("Enter your choice (1 or 2): ");

            if (methodChoice == 2)
            {
                cardNumber = getValidatedString("Enter your card number (16 digits): ");

                // Validate card number
                while (cardNumber.length() != 16)
                {
                    cout << "Invalid card number. Please enter a 16-digit card number.\n";
                    cardNumber = getValidatedString("Re-enter your credit card number: ");
                }

                cout << "Processing payment...\n";
                cout << "Payment successful. PKR " << prices[carIndex] * days << " has been deducted from your card.\n";
            }
            else if (methodChoice == 1)
            {
                cout << "Please pay PKR " << prices[carIndex] * days << " in cash when you pick up the car.\n";
            }
            else
            {
                cout << "Invalid payment method.\n";
                return; // Exit the function if an invalid method is chosen
            }

            // Complete the rental process
            availability[carIndex] = false;
            rentedBy[carIndex] = customerCNIC;
            rentalDuration[carIndex] = days;

            // Save the updated data to the file
            saveData();

            cout << "-----------------------------------------------------------------------------------\n";
            cout << "CNIC: " << customerCNIC << ", you have rented " << cars[carIndex] << " for " << days << " days.\n";
            cout << "Thank you for your payment!\n";
        }
        else
        {
            cout << "Sorry, this car is already rented by CNIC No." << rentedBy[carIndex] << ".\n";
        }
    }
    else
    {
        cout << "Invalid car number.\n";
    }
}

void returnCar()
{
    int carIndex;
    string verifyCNIC;
    string verifyCondition;

    carIndex = getValidatedInt("\nEnter the car serial number to return: ");
    carIndex--; // Convert to 0-based index

    if (carIndex >= 0 && carIndex < totalCars)
    {
        if (!availability[carIndex])
        { // Ensure the car is not available (i.e., it is rented)
            verifyCNIC = getValidatedString("Verify your CNIC: ");
            if (verifyCNIC == rentedBy[carIndex])
            { // Check if the name matches

                verifyCondition = getValidatedString("What is the condition of the car? (OK / Accidented): ");

                // Handle car condition
                if (verifyCondition == "accidented" || verifyCondition == "Accidented" || verifyCondition == "ACCIDENTED")
                {
                    cout << "You have been fined PKR " << 0.2 * prices[carIndex]
                         << " for damaging the property. Please visit the enterprise to return the car with the fine. Thank you!\n";
                }
                else if (verifyCondition == "OK" || verifyCondition == "ok" || verifyCondition == "Ok" || verifyCondition == "oK")
                {
                    cout << rentedBy[carIndex] << ", you have returned " << cars[carIndex] << ". Thank you!\n";
                }
                else
                {
                    cout << "Invalid car condition entered. Please try again.\n";
                    return; // Exit the function if invalid condition is provided
                }

                // Mark the car as returned and available
                availability[carIndex] = true; // Set availability to true (car is now available)
                rentedBy[carIndex] = "";       // Clear the customer name
                rentalDuration[carIndex] = 0;  // Reset the rental duration
                carCondition[carIndex] = "OK"; // Reset the car condition (assuming the car is fine after return)

                // Save the updated data to the file
                saveData();
            }
            else
            {
                cout << "Return Failed. The name doesn't match the renter.\n";
            }
        }
        else
        {
            cout << "This car is already available for rent.\n";
        }
    }
    else
    {
        cout << "Invalid car number.\n";
    }
}
void viewRentedCars()
{
    cout << "\nRented Cars:\n";
    for (int i = 0; i < totalCars; i++)
    {
        if (!availability[i])
        {
            cout << i + 1 << ". " << cars[i] << " - Rented by CNIC No.: " << rentedBy[i]
                 << " for " << rentalDuration[i] << " days.\n";
        }
    }
}

// Function to validate integer input
int getValidatedInt(string prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a valid integer.\n";
        }
        else
        {
            cin.ignore(1000, '\n');
            return value;
        }
    }
}

// Function to validate string input
string getValidatedString(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);

        if (value.empty())
        {
            cout << "Invalid input. Please enter a valid name.\n";
        }
        else
        {
            return value;
        }
    }
}

// Admin menu with full access
void adminMenu()
{
    int choice;

    do
    {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. View available cars\n";
        cout << "2. View rented cars\n";
        cout << "3. Return a car\n";
        cout << "4. Exit\n";
        choice = getValidatedInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            viewAvailableCars();
            break;
        case 2:
            viewRentedCars();
            break;
        case 3:
            returnCar();
            break;
        case 4:
            cout << "Exiting Admin Menu...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

// Customer menu
void customerMenu()
{
    int choice;

    do
    {
        cout << "\n--- Customer Menu ---\n";
        cout << "1. View available cars\n";
        cout << "2. Rent a car\n";
        cout << "3. Return a car\n";
        cout << "4. Exit\n";
        choice = getValidatedInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            viewAvailableCars();
            break;
        case 2:
            rentCar();
            break;
        case 3:
            returnCar();
            break;
        case 4:
            cout << "Exiting Customer Menu...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

// Simple login system to access admin or customer menus
void loginSystem()
{
    int role;
    cout << "----------------------------------------\n";
    cout << "   Welcome to the Car Rental System!\n";
    cout << "----------------------------------------\n";
    cout << "1. Admin Login\n";
    cout << "2. Customer Login\n";
    cout << "Enter your role: ";
    cin >> role;
    cin.ignore();

    if (role == 1)
    {
        string username, password;
        username = getValidatedString("Enter Admin username: ");
        password = getValidatedString("Enter Admin password: ");

        if (username == "admin" && password == "123")
        {
            cout << "Access granted. Welcome, Admin!\n";
            adminMenu();
        }
        else
        {
            cout << "Invalid username or password. Access denied.\n";
        }
    }
    else if (role == 2)
    {
        customerMenu();
    }
    else
    {
        cout << "Invalid role. Please try again.\n";
    }
}

// Main function
int main()
{
    loadData();    // Load car data from the file
    loginSystem(); // Start the login system

    return 0;
}
