#include <iostream>
#include <map>
#include <string>

class PhoneBook {
private:
    std::map<std::string, std::string> phoneBook;
    int CodeError = 0;

public:
    // Overloading the indexing operator
    std::string& operator[](const std::string& phoneNumber) {
        if (phoneBook.find(phoneNumber) == phoneBook.end()) {
            CodeError = 1; // Set error code if number is not found
            throw std::out_of_range("Number not found.");
        }
        CodeError = 0;
        return phoneBook[phoneNumber];
    }

    // Overloading the function call operator ()
    std::string operator()(const std::string& phoneNumber) {
        if (phoneBook.find(phoneNumber) == phoneBook.end()) {
            CodeError = 1; // Set error code if number is not found
            return "Error: number not found.";
        }
        CodeError = 0;
        return phoneBook[phoneNumber];
    }

    // Friend input stream operator
    friend std::istream& operator>>(std::istream& is, PhoneBook& pb) {
        std::string phone, fullName;
        std::cout << "Enter phone number: ";
        is >> phone;
        std::cin.ignore(); // To handle the newline character after entering the phone number
        std::cout << "Enter full name (last name, first name, middle name): ";
        std::getline(is, fullName);
        pb.phoneBook[phone] = fullName;
        return is;
    }

    // Friend output stream operator
    friend std::ostream& operator<<(std::ostream& os, const PhoneBook& pb) {
        for (const auto& entry : pb.phoneBook) {
            os << "Phone: " << entry.first << ", Name: " << entry.second << std::endl;
        }
        return os;
    }

    // Function to get error code
    int getErrorCode() const {
        return CodeError;
    }
};

int main() {
    PhoneBook pb;
    std::cin >> pb;

    std::cout << pb;

    try {
        std::cout << pb["1234567890"] << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << e.what() << " Error code: " << pb.getErrorCode() << std::endl;
    }

    std::cout << pb("1234567890") << std::endl;

    return 0;
}
