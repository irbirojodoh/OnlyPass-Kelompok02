# OnlyPass - Secure Password Manager 🔒

A command-line password management solution with basic encryption and account protection features. Developed as a final project for programming fundamentals course.

## Key Features ✨

- **Account Management**
  - Secure user registration/login
  - Master password protection
  - Account deletion capability
- **Password Operations**
  - Encrypted password storage (Caesar cipher)
  - Password strength analyzer
  - Random password generator
  - Password history tracking
  - Search/filter functionality
- **Security Features**
  - Multi-layer encryption
  - Automatic data persistence
  - Masked password input
- **Advanced Functionality**
  - Parallel sorting with OpenMP
  - Linked list storage system
  - JSON-like data formatting

## Installation 💻

**Requirements**:
- Windows OS (uses Windows-specific headers)
- C compiler with OpenMP support (GCC/MSVC recommended)

**Build Instructions**:
```bash
gcc -o OnlyPass main.c -fopenmp
```

## Usage 🚀

1. **Main Menu**:
```
================ OnlyPass ================
| 1. Login       3. Help     5. Delete   |
| 2. Sign Up     4. Exit                 |
==========================================
```

2. **Account Creation**:
- Enter name, account ID, and master password
- System validates password strength

3. **Password Management**:
```
================ Welcome ================
| 1. Save Password     4. Change Pass   |
| 2. View Passwords    5. Help          |
| 3. Search Passwords  6. Logout        |
=========================================
```

**Example Password Entry**:
```c
Name: Email Account
ID: personal.gmail.com
Password: ************
Strength: Very Strong
```

## File Structure 📁

- `akun.dat`: Encrypted password storage
- `Count.dat`: Account counter
- `main.c`: Core program logic

## Security Considerations 🔐

⚠️ **Important Security Notes**:
- Uses simple Caesar cipher (educational purposes only)
- Not suitable for real-world sensitive data
- Master password stored with basic encryption
- Random number generation uses `rand()`

**Recommended Security Improvements**:
1. Implement AES encryption
2. Add salt to password hashes
3. Use cryptographic-grade RNG
4. Implement brute-force protection

## Technical Details ⚙️

**Data Structures**:
```c
struct Node {
    char dataPwd[100];
    char namaAkun[30];
    char idAkun[30];
    int key;
    int countPwd;
    List password[50];
    struct Node* next;
};
```

**Key Algorithms**:
- Parallel Quicksort (OpenMP)
- Case-insensitive search
- Password strength calculator
- ASCII-based encryption/decryption

**Dependencies**:
- `<omp.h>` for parallel processing
- `<conio.h>` for input handling
- `<time.h>` for RNG seeding

## Limitations 🚧

- Windows platform dependency
- Limited to 50 passwords/account
- No password sharing capability
- Basic error handling
- No cloud sync functionality

## Contributing 🤝

We welcome improvements in:
- Security implementations
- Cross-platform support
- UI enhancements
- Additional storage formats

Please follow standard C coding practices and include documentation for any changes.

## License 📄

MIT License - Free for educational and non-commercial use. Not recommended for production environments.
