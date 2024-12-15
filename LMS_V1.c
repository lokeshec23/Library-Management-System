#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // For setting console color

#define MAX_BOOKS 100
#define MAX_STRING 100

// Structure for book details
typedef struct {
    char title[MAX_STRING];
    char author[MAX_STRING];
    char publisher[MAX_STRING];
    int year;
    int available;
} Book;

// Function prototypes
void login();
void addBook();
void searchBook();
void borrowBook();
void returnBook();
void exitSystem();

// File name to store book data
const char *BOOKS_FILE = "books.txt";

int main() {
    int choice;

    // Set console text color to blue
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    login();

    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Search Book\n");
        printf("3. Borrow Book\n");
        printf("4. Return Book\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline character from input buffer

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                searchBook();
                break;
            case 3:
                borrowBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                exitSystem();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void login() {
    char id[MAX_STRING], password[MAX_STRING];

    printf("\nLibrary Management System - Login\n");
    while (1) {
        printf("\nEnter ID: ");
        scanf("%s", id);
        printf("\nEnter Password: ");
        scanf("%s", password);

        if (strcmp(id, "admin") == 0 && strcmp(password, "240802116") == 0) {
            printf("\n*****Login successful!*****\n");
            break;
        } else {
            printf("Invalid ID or password. Please try again.\n");
        }
    }
}

void addBook() {
    FILE *file = fopen(BOOKS_FILE, "a");
    if (!file) {
        printf("Error opening file for adding books.\n");
        return;
    }

    Book newBook;
    printf("\nEnter book title: ");
    fgets(newBook.title, MAX_STRING, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // Remove newline character
    printf("Enter author name: ");
    fgets(newBook.author, MAX_STRING, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0; // Remove newline character
    printf("Enter publisher: ");
    fgets(newBook.publisher, MAX_STRING, stdin);
    newBook.publisher[strcspn(newBook.publisher, "\n")] = 0; // Remove newline character
    printf("Enter publication year: ");
    scanf("%d", &newBook.year);
    getchar(); // Clear newline character from input buffer
    newBook.available = 1; // Mark as available

    fprintf(file, "%s|%s|%s|%d|%d\n", newBook.title, newBook.author, newBook.publisher, newBook.year, newBook.available);
    fclose(file);

    printf("Book added successfully!\n");
}

void searchBook() {
    FILE *file = fopen(BOOKS_FILE, "r");
    if (!file) {
        printf("Error opening file for searching books.\n");
        return;
    }

    char title[MAX_STRING];
    printf("\nEnter book title to search: ");
    fgets(title, MAX_STRING, stdin);
    title[strcspn(title, "\n")] = 0; // Remove newline character

    Book book;
    int found = 0;
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%d|%d\n", book.title, book.author, book.publisher, &book.year, &book.available) != EOF) {
        if (strcmp(book.title, title) == 0) {
            printf("\nBook Found:\nTitle: %s\nAuthor: %s\nPublisher: %s\nYear: %d\nAvailable: %s\n", 
                   book.title, book.author, book.publisher, book.year, book.available ? "Yes" : "No");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nNot Available\n");
    }

    fclose(file);
}

void borrowBook() {
    FILE *file = fopen(BOOKS_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening file for borrowing books.\n");
        return;
    }

    char title[MAX_STRING];
    printf("\nEnter book title to borrow: ");
    fgets(title, MAX_STRING, stdin);
    title[strcspn(title, "\n")] = 0; // Remove newline character

    Book book;
    int found = 0;
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%d|%d\n", book.title, book.author, book.publisher, &book.year, &book.available) != EOF) {
        if (strcmp(book.title, title) == 0 && book.available) {
            printf("\nBook borrowed successfully!\n");
            book.available = 0;
            found = 1;
        }
        fprintf(tempFile, "%s|%s|%s|%d|%d\n", book.title, book.author, book.publisher, book.year, book.available);
    }

    if (!found) {
        printf("\nBook not available for borrowing.\n");
    }

    fclose(file);
    fclose(tempFile);

    remove(BOOKS_FILE);
    rename("temp.txt", BOOKS_FILE);
}

void returnBook() {
    FILE *file = fopen(BOOKS_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening file for returning books.\n");
        return;
    }

    char title[MAX_STRING];
    printf("\nEnter book title to return: ");
    fgets(title, MAX_STRING, stdin);
    title[strcspn(title, "\n")] = 0; // Remove newline character

    Book book;
    int found = 0;
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%d|%d\n", book.title, book.author, book.publisher, &book.year, &book.available) != EOF) {
        if (strcmp(book.title, title) == 0 && !book.available) {
            printf("\nBook returned successfully!\n");
            book.available = 1;
            found = 1;
        }
        fprintf(tempFile, "%s|%s|%s|%d|%d\n", book.title, book.author, book.publisher, book.year, book.available);
    }

    if (!found) {
        printf("\nBook not found or already available.\n");
    }

    fclose(file);
    fclose(tempFile);

    remove(BOOKS_FILE);
    rename("temp.txt", BOOKS_FILE);
}

void exitSystem() {
    printf("\nExiting the system. Goodbye!\n");
    exit(0);
}

