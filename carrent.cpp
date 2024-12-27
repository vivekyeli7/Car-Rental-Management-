#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <unordered_map> 

using namespace std;

// Base class for vehicles
class Vehicle {
protected:
    string vehicleID;
    string model;
    bool isRented;
    double rentalPricePerDay;

public:
    Vehicle(string id, string mdl, double price)
        : vehicleID(id), model(mdl), isRented(false), rentalPricePerDay(price) {}

    virtual void displayInfo() const {
        cout << "\nVehicle ID: " << vehicleID
             << "\nModel: " << model
             << "\nPrice per day: $" << rentalPricePerDay
             << "\nStatus: " << (isRented ? "Rented" : "Available" ) << endl;
    }

    bool isAvailable() const {
        return !isRented;
    }

    string getModelName() const { 
        return model;
    } 

    void rent() {
        isRented = true;
    }

    void returnVehicle() {
        isRented = false;
    }

    double getRentalPrice() const {
        return rentalPricePerDay;
    }

    string getID() const {
        return vehicleID;
    }
};

// Derived class for Car
class Car : public Vehicle {
public:
    Car(string id, string mdl, double price)
        : Vehicle(id, mdl, price) {}

    void displayInfo() const override {
        cout << "Car Information:\n";
        Vehicle::displayInfo();
    }
};

// Derived class for Bike
class Bike : public Vehicle {
public:
    Bike(string id, string mdl, double price)
        : Vehicle(id, mdl, price) {}

    void displayInfo() const override {
        cout << "Bike Information:\n";
        Vehicle::displayInfo();
    }
};

// Vehicle Factory class
class VehicleFactory {
public:
    static Vehicle* createVehicle(const string& type, const string& id, const string& model, double price) {
        if (type == "Car") {
            return new Car(id, model, price);
        } else if (type == "Bike") {
            return new Bike(id, model, price);
        }
        return nullptr; // or throw an exception for invalid type
    }
};

// User class for managing user information
class User {
private:
    string name;
    string licenseID;
    string password;
    string role;

public:
    User(string uname, string license, string pwd, string r)
        : name(uname), licenseID(license), password(pwd), role(r) {}

    string getName() const {
        return name;
    }

    string getLicenseID() const {
        return licenseID;
    }

    bool authenticate(const string& enteredPassword) const {
        return password == enteredPassword;
    }

    string getRole() const {
        return role;
    }
};

// Class to track rental records
class RentalRecord {
private:
    User user;
    Vehicle* vehicle;
    string rentalDate;
    int rentalDuration;

public:
    RentalRecord(const User& usr, Vehicle* veh, const string& date, int duration)
        : user(usr), vehicle(veh), rentalDate(date), rentalDuration(duration) {}

    void displayRecord() const {
        cout << "\n--- Rental Record ---"
             << "\nUser: " << user.getName() << " (License ID: " << user.getLicenseID() << ")"
             << "\nVehicle ID: " << vehicle->getID() << " (Model: " << vehicle->getModelName() << ")"
             << "\nRental Date: " << rentalDate
             << "\nDuration: " << rentalDuration << " days"
             << "\nTotal Cost: $" << (rentalDuration * vehicle->getRentalPrice()) << endl;
    }

    Vehicle* getVehicle() const {
        return vehicle;
    }

    string getVehicleID() const {
        return vehicle->getID();
    }
};

// Main system to handle rental operations
class CarRentalSystem {
private:
     vector<User> users;
    vector<Vehicle*> vehicles;
    vector<RentalRecord> rentalRecords;
    unordered_map<string, Vehicle*> vehicleMap;
    
public:
    void addUser(const User& user) {
        users.push_back(user);
    }

    void addVehicle(Vehicle* vehicle) {
        // Check if vehicle ID already exists
        if (vehicleMap.find(vehicle->getID()) != vehicleMap.end()) {
            cout << "Vehicle ID " << vehicle->getID() << " already exists. Vehicle not added.\n";
            delete vehicle; 
            return;
        }

        vehicles.push_back(vehicle);
        vehicleMap[vehicle->getID()] = vehicle; 
        cout << "Vehicle " << vehicle->getID() << " added successfully.\n";
    }

    void removeVehicle(const string& vehicleID) {
        auto it = find_if(vehicles.begin(), vehicles.end(),
                          [&vehicleID](Vehicle* v) { return v->getID() == vehicleID; });

        if (it != vehicles.end()) {
            vehicleMap.erase(vehicleID); 
            delete *it;  
            vehicles.erase(it);  
            cout << "Vehicle " << vehicleID << " removed successfully.\n";
        } else {
            cout << "Vehicle ID not found.\n";
        }
    }

    void rentVehicle(const string& userName, const string& userLicenseID, const string& vehicleID, const string& rentalDate, int rentalDays) {
        User user(userName, userLicenseID, "defaultPassword", "user"); // Default password
        bool userExists = false;

        for (const auto& u : users) {
            if (u.getLicenseID() == userLicenseID) {
                userExists = true;
                user = u; // Update user to existing one
                break;
            }
        }

        if (!userExists) {
            addUser(user); // Register new user
            cout << "New user registered: " << userName << endl;
        }

        for (auto& vehicle : vehicles) {
            if (vehicle->getID() == vehicleID && vehicle->isAvailable()) {
                vehicle->rent();
                rentalRecords.emplace_back(user, vehicle, rentalDate, rentalDays);
                cout << "Vehicle " << vehicleID << " rented successfully!\n";
                return;
            }
        }

        cout << "Vehicle not available or does not exist.\n";
    }

    bool isValidDate(const string& date) {
        // Implement date validation check if date is in YYYY-MM-DD format
        if (date.length() != 10) return false;
        if (date[4] != '-' || date[7] != '-') return false;

        string yearStr = date.substr(0, 4);
        string monthStr = date.substr(5, 2);
        string dayStr = date.substr(8, 2);

        int year = stoi(yearStr);
        int month = stoi(monthStr);
        int day = stoi(dayStr);

        // Check if year, month, and day are valid
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;

        return true;
    }

    void returnVehicle(const string& vehicleID, int actualRentalDays) {
        for (auto& record : rentalRecords) {
            if (record.getVehicleID() == vehicleID) {
                double totalCost = actualRentalDays * record.getVehicle()->getRentalPrice();
                record.getVehicle()->returnVehicle(); // Mark vehicle as available
                cout << "Vehicle returned successfully!\n";
                cout << "Total cost: $" << totalCost << endl;
                return;
            }
        }

        cout << "No rental record found for this vehicle.\n";
    }

    void viewAvailableVehicles() const {
        cout << "\n--- Available Vehicles ---\n";
        for (const auto& vehicle : vehicles) {
            if (vehicle->isAvailable()) {
                vehicle->displayInfo();
            }
        }
    }

    void viewRentalHistory() const {
        cout << "\n--- Rental History ---\n";
        for (const auto& record : rentalRecords) {
            record.displayRecord();
        }
    }

    void filterVehicles(double minPrice, double maxPrice, const string& type) const {
        cout << "\n--- Filtered Vehicles ---\n";
        for (const auto& vehicle : vehicles) {
            if (vehicle->getRentalPrice() >= minPrice && vehicle->getRentalPrice() <= maxPrice) {
                if (type == "Car" && dynamic_cast<Car*>(vehicle)) {
                    vehicle->displayInfo();
                } else if (type == "Bike" && dynamic_cast<Bike*>(vehicle)) {
                    vehicle->displayInfo();
                }
            }
        }
    }

    User* authenticateUser(const string& licenseID, const string& password) {
        for (auto& user : users) {
            if (user.getLicenseID() == licenseID && user.authenticate(password)) {
                return &user;
            }
        }
        return nullptr;
    }

    void viewAllUsers() const {
        cout << "\n--- All Registered Users ---\n";
        for (const auto& user : users) {
            cout << "Name: " << user.getName() << ", License ID: " << user.getLicenseID() << ", Role: " << user.getRole() << endl;
        }
    }
};

// User interface class for regular users
class RentalService {
private:
    CarRentalSystem system;

public:
    void setup() {
        system.addUser(User("Admin", "A123", "admin123", "admin"));
        system.addUser(User("John Doe", "L8901", "password123", "user"));
        system.addVehicle(VehicleFactory::createVehicle("Car", "C100", "Toyota Camry", 50));
        system.addVehicle(VehicleFactory::createVehicle("Bike", "B200", "Yamaha YZF", 30));
        system.addVehicle(VehicleFactory::createVehicle("Car", "C300", "Honda Accord", 60));
    }

    void showMenu() {
        while (true) {
            cout << "\n--- Car Rental System ---\n";
            cout << "1. View Available Vehicles\n";
            cout << "2. Rent a Vehicle\n";
            cout << "3. Return a Vehicle\n";
            cout << "4. View Rental History\n";
            cout << "5. Filter Vehicles\n";
            cout << "6. Admin Login\n";
            cout << "7. Exit\n";
            cout << "Select an option: ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                system.viewAvailableVehicles();
                break;
            case 2:
                rentVehicle();
                break;
            case 3:
                returnVehicle();
                break;
            case 4:
                system.viewRentalHistory();
                break;
            case 5:
                filterVehicles();
                break;
            case 6:
                adminLogin();
                break;
            case 7:
                cout << "Exiting the system. Goodbye!\n";
                return;
            default:
                cout << "Invalid option. Please try again.\n";
            }
        }
    }

    void rentVehicle() {
         string userName, userLicenseID, vehicleID, rentalDate;
         int rentalDays;
     
         try {
             cout << "Enter your name: ";
             cin.ignore();
             getline(cin, userName);
             cout << "Enter your license ID: ";
             cin >> userLicenseID;
     
             cout << "Enter vehicle ID to rent: ";
             cin >> vehicleID;
     
             cout << "Enter rental date (YYYY-MM-DD): ";
             cin >> rentalDate;
             while (!system.isValidDate(rentalDate)) {
                 cout << "Invalid date format. Please enter in YYYY-MM-DD format: ";
                 cin >> rentalDate;
             }
     
             cout << "Enter number of rental days: ";
             cin >> rentalDays;
             if (cin.fail() || rentalDays <= 0) {
                 throw invalid_argument("Rental days must be a positive integer.");
             }
     
             system.rentVehicle(userName, userLicenseID, vehicleID, rentalDate, rentalDays);
         } catch (const invalid_argument& e) {
             cout << "Error: " << e.what() << "\n";
             cin.clear();  // Clear input stream state.
             cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore erroneous input.
         } catch (const exception& e) {
             cout << "An unexpected error occurred: " << e.what() << "\n";
         }
     }
     
    void returnVehicle() {
         string vehicleID;
         int actualRentalDays;
     
         try {
             cout << "Enter vehicle ID to return: ";
             cin >> vehicleID;
     
             cout << "Enter actual number of rental days: ";
             cin >> actualRentalDays;
             if (cin.fail() || actualRentalDays <= 0) {
                 throw invalid_argument("Rental days must be a positive integer.");
             }
     
             system.returnVehicle(vehicleID, actualRentalDays);
         } catch (const invalid_argument& e) {
             cout << "Error: " << e.what() << "\n";
             cin.clear();  
             cin.ignore(numeric_limits<streamsize>::max(), '\n');  
         } catch (const exception& e) {
             cout << "An unexpected error occurred: " << e.what() << "\n";
         }
     }
     
    void adminLogin() {
         string licenseID, password;
     
         try {
             cout << "Enter Admin License ID: ";
             cin >> licenseID;
             cout << "Enter Admin Password: ";
             cin >> password;
     
             User* adminUser = system.authenticateUser(licenseID, password);
             if (adminUser == nullptr || adminUser->getRole() != "admin") {
                 throw runtime_error("Invalid credentials. Admin login failed.");
             }
             
             cout << "Admin login successful!\n";
             adminMenu();
         } catch (const runtime_error& e) {
             cout << "Error: " << e.what() << "\n";
         } catch (const exception& e) {
             cout << "An unexpected error occurred: " << e.what() << "\n";
         }
     }

    void filterVehicles() {
        double minPrice, maxPrice;
        string type;

        cout << "Enter minimum price: ";
        cin >> minPrice;
        cout << "Enter maximum price: ";
        cin >> maxPrice;
        cout << "Enter vehicle type (Car/Bike): ";
        cin >> type;

        transform(type.begin(), type.end(), type.begin(), ::tolower);
        if (type == "car") {
            type = "Car";
        } else if (type == "bike") {
            type = "Bike";
        } else {
            cout << "Invalid vehicle type entered.\n";
            return;
        }

        system.filterVehicles(minPrice, maxPrice, type);
    }

    void adminMenu() {
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Add Vehicle\n";
            cout << "2. Remove Vehicle\n";
            cout << "3. View All Users\n";
            cout << "4. Logout\n";
            cout << "Select an option: ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                addVehicle();
                break;
            case 2:
                removeVehicle();
                break;
            case 3:
                system.viewAllUsers();
                break;
            case 4:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid option. Please try again.\n";
            }
        }
    }

    void addVehicle() {
        string vehicleID, model, type;
        double price;

        cout << "Enter vehicle ID: ";
        cin >> vehicleID;
        cout << "Enter vehicle model: ";
        cin.ignore();
        getline(cin, model);
        cout << "Enter vehicle type (Car/Bike): ";
        cin >> type;
        cout << "Enter rental price per day: ";
        cin >> price;

        Vehicle* newVehicle = VehicleFactory::createVehicle(type, vehicleID, model, price);
        if (newVehicle != nullptr) {
            system.addVehicle(newVehicle);
        } else {
            cout << "Invalid vehicle type. Vehicle not added.\n";
        }
    }

    void removeVehicle() {
        string vehicleID;
        cout << "Enter vehicle ID to remove: ";
        cin >> vehicleID;
        system.removeVehicle(vehicleID);
    }
};

int main() {
    RentalService service;
    service.setup();
    service.showMenu();
    return 0;
}
