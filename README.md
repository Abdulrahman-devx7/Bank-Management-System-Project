# The Bank Management System
A simple and modular bank management system that simulates basic bank functionalities to interact with clients.
## 📝 The Objectives
- Practice clean code principles on a larger, closer to real-world scale.
- Utilize STLs (e.g., `unordered_map` for `O(1)` lookup by account number) for a more readable, efficient, and scalable code.
- Take procedural programming to its limits(e.g., scattered client-lookup logic and function overloading) to discover issues that would build up experience for a better understanding of  why OOP patterns exist.
- Put string manipulation functions into a real practice by integrating them File I/O.
## 🚀 Core Features
- A simple login page that authorizes access to the system by fetching existent users from a dedicated file
- Basic, user-friendly UI menus that gives the user full control over CRUD operations for client records, or UI-related utilities(i.e., finding a user by account number). 
- The ability to view a comprehensive table for either showing the data fields of all client in a table, or a smaller table showcasing clients' balances and their total sum at the bottom.
- Full control over adding clients, updating & deleting clients' data, and finding clients (CRUD) via the account number.
- A dedicated menu to manage users in the system with CRUD operations over the users and custom permission management for the bank utilities in the main menu
- A normalization of the username form in CRUD operations in the user management menu, which eliminates duplicate usernames differing in letter cases in the system. Thus, making unique usernames in the system and preventing trailing or leading spaces.
- The ability to do simple transactions (e.g., withdrawal and deposit) for any client via the account number
- A Permission verification of utility access for authentic users accessing the bank system. It shows a UI header indicating access denial and then a prompt to go to the menu if the user lacks access.
- A numerical validation that prevents crashes or infinite loops in input fields where a numerical value is expected.
- A protection of the base admin from deletion so the system remains accessible by the root account.
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
- A simple, scalable Finite State Machine(FSM) using `enum class enRunningState` in `StartBankSystem`
- A login screen function (`LoginScreen`) that acts as an initializer for the running state the bank system is going to start. 
- A modular design for `LoginScreen` that uses a Boolean for UI interactions and verifying the user's access via `VerifyLogin` before initializing the **starting parameters.** Additions regarding security or something else can be added to this function.
- A robust, centralized function (`RunBankServices`) that handles permission verification, rerouting to the utility, and unauthorized access in one place. Thus, Easing scalability by merely adding or manipulating existing functions for **new menu options or security measurements for the system.** 
- A separation of concerns led by the `RunBankServices` acting as the router using helper functions like `GetRequiredPermissionForMenuChoice` and `VerifyPermission`. Making the utilities in the switch statement of the main menu blind to any verification, merely handling the intended functionality.
- A utilization of bit masking for efficient, quick permission verification using `enum class` for its closed scope preventing accidental mixing with unrelated integers. Thus using the `static_cast` to verification and assignment of permissions to the user in CRUD.
- A heavy usage of function overloading for several logic, UI, and File I/O purposes (i.e, `CheckExistence`, `PrintInfoCard`, `LoadFromFile`, etc.)  
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

On launch, you'll be presented with a login screen asking for valid login credentials to login to the system.


1. Show Client List: View all existing clients in a formatted table.

2. Add New Client: Register a new client into the system.

3. Delete Client: Remove a client from the system (utilizes soft deletion).

4. Update Client Info: Modify a client's PIN, name, phone number, or balance.

5. Find Client: Search for a specific client card using their account number.

6. Transactions: Access the sub-menu to perform deposits, withdrawals, and view the total sum of all balances.

7. Manage Users: Access the administrative sub-menu to list, add, delete, update, and find system users.

8. Logout: Securely log out and return to the initial login screen.

### 🟢 Demo credentials (full acces)

1️⃣ Username: admin1

2️⃣ Password: master67

All client data is persisted to `clients.txt`, which acts as the system's
flat-file database. Deleted clients are flagged rather than removed, so
their records remain in the file for potential future auditing, but are
filtered out of every UI-facing view and CRUD operation.

The same goes to `users.txt` but merely for users accessing the system and their assigned permissions. The minor difference is that this file **hard-deletes** users from the system.