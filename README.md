# Vehicle Rental Management System

## 🌟 Overview
The Vehicle Rental Management System is a C++ application designed to manage vehicle rentals efficiently. The system allows users to browse, rent, and return vehicles such as cars and bikes. It also features an admin interface for adding or removing vehicles and managing user information. This system leverages Object-Oriented Programming (OOP) principles for scalability and maintainability, providing a smooth user experience for both regular users and administrators.

## 🛠️ Features
### 🚗 Vehicle Management
- Manage a fleet of cars and bikes with unique vehicle IDs, models, and rental prices.

### 👥 User Authentication
- Supports user login with license IDs and passwords, including a special admin role for system management.

### 📅 Rental Process
- Users can rent available vehicles for a specified number of days and return them afterward.

### 💵 Cost Calculation
- Calculates total rental costs based on the duration and vehicle type.

### 🔍 Vehicle Filtering
- Filter vehicles based on price range and type (Car or Bike).

### 📝 Rental History
- View rental history showing detailed records of each transaction.

### 🔐 Admin Interface
- Add new vehicles (Cars or Bikes) to the system.
- Remove existing vehicles from the fleet.
- View all registered users in the system.

## 📚 Classes
### Vehicle (Base Class)
- Represents a generic vehicle, with attributes like vehicleID, model, rentalPricePerDay, and rental status.

### Car and Bike
- Inherit from Vehicle and provide specific implementations for cars and bikes.

### User
- Manages user information such as name, licenseID, password, and role (user/admin).
- Includes methods for authentication.

### RentalRecord
- Tracks individual rental transactions, including user information, vehicle details, rental duration, and total cost.

### CarRentalSystem
- Core class that manages users, vehicles, rental records, and the overall rental process. It provides methods for renting and returning vehicles, filtering vehicles, and managing users.

### RentalService
- The user interface class that interacts with the user and handles commands for renting vehicles, returning vehicles, and accessing the admin features.

## User Actions:
### View Available Vehicles
- Shows all vehicles that are currently available for rent.

### Rent a Vehicle
- Allows users to rent a car or bike for a specific number of days.

### Return a Vehicle
- Users can return a vehicle, and the system will calculate the rental cost based on the rental period.

### View Rental History
- Displays past rental records, including vehicle details, rental dates, and users.

### Filter Vehicles
- Allows users to filter available vehicles based on price range and type (Car or Bike).

### Admin Features
- Admins can log in and manage vehicles by adding or removing them from the system.
- Admins can also view all registered users.

## 🚀 Usage
### Build and Run the Project
To compile and execute the project, use a C++ compiler like `g++`. Run the following commands:

```bash
g++ -o car_rental Car_Rental_System.cpp
./car_rental
