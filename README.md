# The Bank Management System
A simple and modular bank management system that simulates basic bank functionalities to interact with clients.
## 📝 The Objectives
- Practice clean code principles on a larger, closer to real-world scale.
- Utilize STLs (e.g., `unordered_map` for `O(1)` lookup by account number) for a more readable, efficient, and scalable code.
- Take procedural programming to its limits(e.g., scattered client-lookup logic and function overloading) to discover issues that would build up experience for a better understanding of  why OOP patterns exist.
- Put string manipulation functions into a real practice by integrating them File I/O.
## 🚀 Core Features
- Basic, user-friendly UI menus that gives the user full control over CRUD operations for client records, or UI-related utilities(i.e., finding a user by account number). 
- The ability to view a comprehensive table for either showing the data fields of all client in a table, or a smaller table showcasing clients' balances and their total sum at the bottom.
- Full control over adding clients, updating & deleting clients' data, and finding clients (CRUD) via the account number.
- The ability to do simple transactions (e.g., withdrawal and deposit) for any client via the account number
- A numerical validation that prevents crashes or infinite loops in input fields where a numerical value is expected.
## ⚙️ Architecture and engineering choices
- A state-driven solution that uses Enums with switch cases to route the user to different sub-programs depending on the input.
- Dedicated Enums for the two menus in the system: the main menu, and the transactions menu, to separate the switch routers in each menu.
- Passing encapsulated variables via structs by reference to improve memory efficiency with `O(1)` space efficiency.
- A robust utilization of the `std::unordered_map` DSA for fast lookups averaging `O(1)` for CRUD utilities and finding specific clients.
- Usage of `std::unordered_map` iterators in functions like `VerifyBalanceForWithdraw`, `UpdateClientData`, etc. to change the balance value directly via the passed iterator.
- Usage of the `std::string` and its methods for string manipulation functions to extract and load data from and to files.
- A thorough abstraction of UI functionalities and core business logic of CRUD.
- A soft deletion that flips the Boolean flag of the deletion status, making it invisible to UI or CRUD utilities. Whilst keeping the deleted client's record persistent in the `clients.txt` file.
- Functions tailored to make deleted clients invisible: `FindClientByAccountNumber`
  checks the deletion flag and returns `.end()` if it's set, treating a
  soft-deleted client as "not found" for every lookup-based operation.
- `GetVisibleClients` applies the same principle when preparing the vector
  used by list/balance display utilities.
## 📦 Installation & Build

This project has no external dependencies — just a C++17-compatible
compiler (e.g., g++, MSVC via Visual Studio).

```
git clone https://github.com/Abdulrahman-devx7/Bank-Management-System-Project.git
cd Bank-Management-System-Project
g++ -std=c++17 Bank-Management-System.cpp -o bank
./bank
```

> **Note:** This project was originally developed on Windows and uses
> `system("cls")` / `system("pause")` for screen handling. On Linux/macOS,
> these calls will print a harmless "command not found" message instead
> of clearing the screen or pausing — functionality isn't affected.

## ▶️ Usage

On launch, you'll be presented with a main menu offering:

1. View all clients (table view)
2. View client balances (with total sum)
3. Add a new client
4. Update an existing client's data
5. Delete a client (soft delete — see below)
6. Deposit / withdraw funds for a client
7. Exit

All client data is persisted to `clients.txt`, which acts as the system's
flat-file database. Deleted clients are flagged rather than removed, so
their records remain in the file for potential future auditing, but are
filtered out of every UI-facing view and CRUD operation.