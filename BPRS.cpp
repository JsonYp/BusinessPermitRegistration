#include <iostream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <random>
#include <regex>
#include <algorithm>
#include <vector>

using namespace std;

struct User {
    string username;
    string password;
    string email;
    string phoneNumber;
};

struct BusinessPermit {
    string BusinessName;
    string ownerName;
    string businessType;
    string businessHotline;
    string businessLocation;
    string permitNumber;
    time_t expirationDate;
    time_t deliveryDate;
};

bool isValidUsername(const string& username) {
    return !username.empty();
}

bool isValidPassword(const string& password) {
    regex passwordPattern("(?=.*[0-9])(?=.*[A-Za-z]).{8,}");
    bool valid =  regex_match(password, passwordPattern) &&
                  regex_search(password, regex("[!@#$%^&*()_+\\-=\\[\\]{};':\",.<>/?0-9]")) &&
                  regex_search(password, regex("[A-Z]")) &&
                  regex_search(password, regex("[0-9]"));
    if(!valid){
        cout << "Invalid password. Password must be at least 8 characters long, contain digits, a special character, and at least 1 uppercase letter\n";
    }
    return valid;
}

bool isValidEmail(const string& email) {
    return regex_match(email, regex("[a-zA-Z0-9_]+@[a-zA-Z]+\\.[a-zA-Z]+"));
}

bool isValidPhoneNumber(const string& phoneNumber) {
    return regex_match(phoneNumber, regex("\\d{11}"));
}

bool isValidPaymentChoice(int choice) {
    return choice >= 1 && choice <= 2;
}

bool isValidPermitChoice(char choice) {
    return choice >= '1' && choice <= '5';
}

time_t calculateDeliveryDate(time_t currentDate) {
    const int secondsPerDay = 24 * 60 * 60;
    const int workingDaysToAdd = 2;
    currentDate += workingDaysToAdd * secondsPerDay;
    tm* tmDate = localtime(&currentDate);
    if (tmDate->tm_wday == 0) // Sunday
        currentDate += secondsPerDay;
    else if (tmDate->tm_wday == 6) // Saturday
        currentDate += 2 * secondsPerDay;
    return currentDate;
}

void menu() {
    // Define the menu function according to your needs
}

int main() {
    menu:
    unordered_map<string, User> users;
    unordered_map<string, BusinessPermit> businessPermits;

    char choice;
    string dummy;
    int permitChoice;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100000, 999999);

    do {
        cout << "**********************************************************\n";
        cout << "Welcome to the Business Permit Registration Bureau Portal\n";
        cout << "**********************************************************\n";
        cout << "Please make a selection from the following options:\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit Program\n";
        cout << "Enter a Number: ";
        cin >> choice;

        cin.ignore();

        string username, password;

        switch (choice) {
            case '1': {
                User newUser;
                bool usernameTaken, validEmail, validPhoneNumber;

                do {
                    cout << "Enter username: ";
                    getline(cin, newUser.username);
                    usernameTaken = (users.find(newUser.username) != users.end()) || !isValidUsername(newUser.username);

                    if (usernameTaken) {
                        cout << "Invalid username or username already exists. Try a different one.\n";
                    }
                } while (usernameTaken);

                do {
                    cout << "Enter password: ";
                    getline(cin, newUser.password);
                } while (!isValidPassword(newUser.password));

                do {
                    cout << "Enter email: ";
                    getline(cin, newUser.email);
                    validEmail = isValidEmail(newUser.email);

                    if (!validEmail) {
                        cout << "Invalid email address. Please enter a valid email (e.g., example@gmail.com).\n";
                    }
                } while (!validEmail);

                do {
                    cout << "Enter phone number: ";
                    getline(cin, newUser.phoneNumber);
                    validPhoneNumber = isValidPhoneNumber(newUser.phoneNumber);

                    if (!validPhoneNumber) {
                        cout << "Invalid phone number. Please enter 11 digits only.\n";
                    }
                } while (!validPhoneNumber);

                users[newUser.username] = newUser;

                cout << "Registration successful!\n";
                break;
            }

            case '2': {
                cout << "Enter username: ";
                getline(cin, username);
                auto it = users.find(username);

                if (it == users.end()) {
                    cout << "Invalid username. Please register first.\n";
                    break;
                }

                string enteredPassword;
                cout << "Enter password: ";
                getline(cin, enteredPassword);

                if (it->second.password == enteredPassword && isValidPassword(enteredPassword)) {
                    cout << "\n";
                    cout << "**********************************************************\n";
                    cout << "Login successful. Welcome, " << username << "!\n";


                    // Loop for multiple actions before logging out
                    do {
                        cout << "Please select the type of service you require:\n";
                        cout << "1. Apply for a new business permit\n";
                        cout << "2. Renew business permit\n";
                        cout << "3. View Pending Permits\n";
                        cout << "4. Logout\n";
                        cout << "Enter a Number: ";

                        char permitChoice;
                        cin >> permitChoice;
                        cin.ignore();

                        if (!isValidPermitChoice(permitChoice)) {
                            cout << "Invalid choice. Please try again.\n";
                            continue;
                        }

                        switch (permitChoice) {

                            case '1': {
                                BusinessPermit newPermit;
                                cout << "Enter business name: ";
                                getline(cin, newPermit.BusinessName);
                                cout << "Enter business owner's name: ";
                                getline(cin, newPermit.ownerName);
                                cout << "Enter business type: ";
                                getline(cin, newPermit.businessType);
                                cout << "Enter business hotline: ";
                                getline(cin, newPermit.businessHotline);
                                cout << "Enter business location: ";
                                getline(cin, newPermit.businessLocation);
                                newPermit.expirationDate = time(0) + 31536000;
                                newPermit.deliveryDate = calculateDeliveryDate(time(0)); // Calculate delivery date
                                 
                                int paymentChoice;

                                do {
                                    cout << "Please pay the application fee of PHP:250. Please choose a payment method:\n";
                                    cout << "1. Online Bank\n";
                                    cout << "2. E-wallet\n";
                                    cout << "Enter a Number: ";
                                    cin >> paymentChoice;
                                    cin.ignore();
                                } while (!isValidPaymentChoice(paymentChoice));

                                if (paymentChoice == 1 || paymentChoice == 2) {
                                    cout << "Payment successful. Proceeding with the application...\n";
                                } else {
                                    cout << "Invalid choice. Please try again.\n";
                                    break;
                                }

                                cout << "Business permit granted to " << newPermit.BusinessName << " successfully!\n";
                                newPermit.permitNumber = to_string(dis(gen));
                                businessPermits[username] = newPermit;
                                cout << "with permit number: " << newPermit.permitNumber << "\n\n";
                                cout << "Delivery is scheduled for: " << ctime(&newPermit.deliveryDate);

                                break;
                            }

                            case '2': {
                                auto permitIt = businessPermits.find(username);

                                if (permitIt == businessPermits.end()) {
                                    cout << "You don't have an existing permit to renew. Please apply for a permit first.\n";
                                    break;
                                }

                                if (permitIt->second.expirationDate > time(0)) {
                                    cout << "Your business permit is still valid until " << ctime(&permitIt->second.expirationDate);
                                    break;
                                }

                                int paymentChoice;

                                do {
                                    cout << "Please choose a payment method:\n";
                                    cout << "1. Online Bank\n";
                                    cout << "2. Online E-wallet\n";
                                    cout << "Enter a Number: ";
                                    cin >> paymentChoice;
                                    cin.ignore();
                                } while (!isValidPaymentChoice(paymentChoice));

                                if (paymentChoice == 1 || paymentChoice == 2) {
                                    cout << "Payment successful. Proceeding with the renewal...\n";
                                } else {
                                    cout << "Invalid choice. Please try again.\n";
                                    break;
                                }

                                BusinessPermit newPermit;
                                cout << "Enter business name: ";
                                getline(cin, newPermit.BusinessName);
                                newPermit.expirationDate = time(0) + 31536000;
                                newPermit.deliveryDate = calculateDeliveryDate(time(0)); // Calculate delivery date
                                businessPermits[username] = newPermit;
                                cout << "Business permit for " << newPermit.BusinessName << " renewed successfully!\n";
                                cout << "Delivery is scheduled for: " << ctime(&newPermit.deliveryDate);

                                break;
                            }

                            case '3': {
                                cout << "Pending Permits:\n";
                                for (const auto& permit : businessPermits) {
                                    cout << "Permit Number: " << permit.second.permitNumber << ", Expiration Date: " << ctime(&permit.second.expirationDate);
                                    cout << "Delivery Date: " << ctime(&permit.second.deliveryDate) << endl;
                                }
                                break;
                            }

                            case '4': {
                                cout << "Logging out...\n";
                                goto menu;
                                break;
                            }

                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }

                    } while (permitChoice != '4');

                } else {
                    cout << "Login failed. Incorrect password or invalid password format.\n";
                }

                break;
            }
            
            case '3': {
                cout << "Exiting program...\n";
                return 0;
            }

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != '3');

    return 0;
}
