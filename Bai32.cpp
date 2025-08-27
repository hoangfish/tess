#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

class IObserver
{
public:
    virtual void update(vector<class Book> books) = 0;
    virtual ~IObserver() = default;
};

class LibraryItem
{
private:
    string name;

public:
    LibraryItem();
    void setName(const string &n);
    string getName() const;
    virtual void input();
    virtual void output() const;
};

class Book : public LibraryItem
{
private:
    string title;
    bool is_borrowed;
    string borrower_name;

public:
    Book(const string &book_title);
    string get_title() const;
    string get_borrower_name() const;
    bool get_is_borrowed() const;
    string borrow(const string &user_name);
    string return_to_library();
    void input() override;
    void output() const override;
};

class IUser : public IObserver
{
private:
    string name;
    vector<Book> books;

public:
    IUser(const string &user_name);
    void update(vector<Book> books) override;
    vector<Book> get_books();
    void display();
};

class Library
{
private:
    vector<Book> books;
    vector<IObserver *> observers;

public:
    Library();
    void add_book(const Book &book);
    void add_observer(IObserver *observer);
    void remove_observer(IObserver *observer);
    string edit(const string &book_title, const string &user_name, bool borrow_action);
    void notify_observers();
    string borrow_book(const string &book_title, const string &user_name);
    string release_book(const string &book_title);
};

LibraryItem::LibraryItem() : name("") {}

void LibraryItem::setName(const string &n) { name = n; }

string LibraryItem::getName() const { return name; }

void LibraryItem::input()
{
    cout << "Enter item name: ";
    cin.ignore(1);
    getline(cin, name);
}

void LibraryItem::output() const
{
    cout << "Item Name: " << name << endl;
}

Book::Book(const string &book_title) : LibraryItem(), title(book_title), is_borrowed(false), borrower_name("")
{
    setName(book_title);
}

string Book::get_title() const { return title; }

string Book::get_borrower_name() const { return borrower_name; }

bool Book::get_is_borrowed() const { return is_borrowed; }

string Book::borrow(const string &user_name)
{
    if (!is_borrowed)
    {
        is_borrowed = true;
        borrower_name = user_name;
        return "Book " + title + " has been borrowed by " + user_name + ".";
    }
    return "Book " + title + " is already borrowed.";
}

string Book::return_to_library()
{
    if (is_borrowed)
    {
        is_borrowed = false;
        borrower_name = "";
        return "Book " + title + " has been released.";
    }
    return "Book " + title + " is not borrowed.";
}

void Book::input()
{
    LibraryItem::input();
    title = getName();
}

void Book::output() const
{
    LibraryItem::output();
    cout << "Title: " << title << endl;
    cout << "Borrowed: " << (is_borrowed ? "Yes by " + borrower_name : "No") << endl;
}

IUser::IUser(const string &user_name) : name(user_name) {}

void IUser::update(vector<Book> books)
{
    this->books = books;
}

vector<Book> IUser::get_books()
{
    return books;
}

void IUser::display()
{
    cout << "Books for user " << name << ":\n";
    if (books.empty())
    {
        cout << "No books in the list\n";
        return;
    }
    bool has_unborrowed = false;
    for (const Book &book : books)
    {
        cout << "Book: " << book.get_title() << " | Borrowed: "
             << (book.get_is_borrowed() ? "Yes by " + book.get_borrower_name() : "No") << "\n";
        if (!book.get_is_borrowed())
        {
            has_unborrowed = true;
        }
    }
    if (!has_unborrowed)
    {
        cout << "No available (unborrowed) books\n";
    }
}

Library::Library() {}

void Library::add_book(const Book &book)
{
    books.push_back(book);
    notify_observers();
}

void Library::add_observer(IObserver *observer)
{
    observers.push_back(observer);
}

void Library::remove_observer(IObserver *observer)
{
    auto it = find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
    {
        observers.erase(it);
    }
}

string Library::edit(const string &book_title, const string &user_name, bool borrow_action)
{
    for (Book &book : books)
    {
        if (book.get_title() == book_title)
        {
            string result;
            if (borrow_action)
            {
                result = book.borrow(user_name);
            }
            else
            {
                result = book.return_to_library();
            }
            notify_observers();
            return result;
        }
    }
    return "Book " + book_title + " not found.";
}

void Library::notify_observers()
{
    for (IObserver *observer : observers)
    {
        observer->update(books);
    }
}

string Library::borrow_book(const string &book_title, const string &user_name)
{
    return edit(book_title, user_name, true);
}

string Library::release_book(const string &book_title)
{
    return edit(book_title, "", false);
}

int main()
{
    Library library;
    IUser user1("Alice");
    IUser user2("Bob");

    library.add_observer(&user1);
    library.add_observer(&user2);
    library.add_book(Book("The Great Gatsby"));
    library.add_book(Book("1984"));
    library.add_book(Book("To Kill a Mockingbird"));
    cout << "\nAdding books to library...\n";
    cout << "Books added: The Great Gatsby, 1984, To Kill a Mockingbird\n";

    cout << "\nUser Alice's view:\n";
    user1.display();

    cout << "\nUser Bob's view:\n";
    user2.display();

    cout << "\n"
         << library.borrow_book("1984", "Alice") << "\n";

    cout << "\nUser Alice's view after borrowing:\n";
    user1.display();

    cout << "\nUser Bob's view after borrowing:\n";
    user2.display();

    cout << "\n"
         << library.release_book("1984") << "\n";

    cout << "\nUser Alice's view after release:\n";
    user1.display();

    return 0;
}