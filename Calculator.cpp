#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace {

struct CalculationRecord {
    std::vector<std::string> steps;
    double lastResult{};
};

// Prompt the user for a floating-point number and reject invalid input.
double promptForNumber(const std::string& label) {
    while (true) {
        std::cout << "Enter " << label << ": ";
        double value{};
        if (std::cin >> value) {
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid number. Please try again.\n";
    }
}

// Ask the user which arithmetic operation to perform; includes quit, reset, and memory options.
char promptForOperation() {
    while (true) {
        std::cout << "Choose operation (+, -, *, /, %, n for new, m for memory, q to quit): ";
        char op{};
        if (std::cin >> op) {
            switch (op) {
                case '+':  // Addition
                case '-':  // Subtraction
                case '*':  // Multiplication
                case '/':  // Division
                case '%':  // Modulo (floating-point remainder)
                case 'n':  // Start a new calculation (lowercase)
                case 'N':  // Start a new calculation (uppercase)
                case 'm':  // Show stored calculations (lowercase)
                case 'M':  // Show stored calculations (uppercase)
                case 'q':  // Quit (lowercase)
                case 'Q':  // Quit (uppercase)
                    return op;
                default:
                    break;
            }
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Unsupported operation. Please choose one of +, -, *, /, %, n, m, or q.\n";
    }
}

// Execute the arithmetic specified by op on lhs and rhs.
double calculate(double lhs, double rhs, char op) {
    switch (op) {
        case '+':  // Addition
            return lhs + rhs;
        case '-':  // Subtraction
            return lhs - rhs;
        case '*':  // Multiplication
            return lhs * rhs;
        case '/':  // Division with zero check
            if (rhs == 0.0) {
                throw std::runtime_error("Division by zero is undefined.");
            }
            return lhs / rhs;
        case '%':  // Floating-point modulo
            if (rhs == 0.0) {
                throw std::runtime_error("Modulo by zero is undefined.");
            }
            return std::fmod(lhs, rhs);
        default:
            throw std::runtime_error("Unsupported operation.");
    }
}

// Create a record with the initial value captured.
CalculationRecord createInitialRecord(double initialValue) {
    CalculationRecord record;
    record.lastResult = initialValue;
    std::ostringstream oss;
    oss << "Start: " << initialValue;
    record.steps.push_back(oss.str());
    return record;
}

// Append a textual description of the performed operation.
void appendStep(CalculationRecord& record,
                double previousResult,
                char op,
                double operand,
                double newResult) {
    std::ostringstream oss;
    oss << previousResult << " " << op << " " << operand << " = " << newResult;
    record.steps.push_back(oss.str());
    record.lastResult = newResult;
}

// Present stored calculations and optionally show their detailed steps.
void showHistory(const std::vector<CalculationRecord>& history) {
    if (history.empty()) {
        std::cout << "Memory is empty.\n";
        return;
    }

    while (true) {
        std::cout << "\nStored calculations:\n";
        for (std::size_t i = 0; i < history.size(); ++i) {
            std::cout << "  " << (i + 1) << ") Last result: " << history[i].lastResult << "\n";
        }
        std::cout << "Select calculation number to view (0 to return): ";

        int selection{};
        if (!(std::cin >> selection)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid selection. Please enter a number from the list.\n";
            continue;
        }

        if (selection == 0) {
            std::cout << "\n";
            break;
        }

        if (selection < 0 || static_cast<std::size_t>(selection) > history.size()) {
            std::cout << "Selection out of range. Try again.\n";
            continue;
        }

        const auto& record = history[static_cast<std::size_t>(selection) - 1];
        std::cout << "Calculation " << selection << " steps:\n";
        for (const auto& step : record.steps) {
            std::cout << "  " << step << "\n";
        }
        std::cout << "\n";
    }
}

}  // namespace

int main() {
    // Display a simple header.
    std::cout << "Simple Calculator\n";
    std::cout << "-----------------\n";

    // Seed the running result with the user's first number.
    double result = promptForNumber("the first number");
    std::cout << "Current result: " << result << "\n";

    std::vector<CalculationRecord> history;
    history.push_back(createInitialRecord(result));
    std::size_t currentRecordIndex = history.size() - 1;

    while (true) {
        char operation = promptForOperation();
        if (operation == 'q' || operation == 'Q') {
            break;
        }
        if (operation == 'n' || operation == 'N') {
            result = promptForNumber("the first number");
            std::cout << "Current result: " << result << "\n";
            history.push_back(createInitialRecord(result));
            currentRecordIndex = history.size() - 1;
            continue;
        }
        if (operation == 'm' || operation == 'M') {
            showHistory(history);
            continue;
        }

        // Gather the next operand and immediately compute the new result.
        double nextNumber = promptForNumber("the next number");
        double previousResult = result;

        try {
            // Compute and show the result of the selected operation.
            result = calculate(result, nextNumber, operation);
            appendStep(history[currentRecordIndex], previousResult, operation, nextNumber, result);
            std::cout << "Result: " << result << "\n";
        } catch (const std::exception& ex) {
            std::cout << "Error: " << ex.what() << "\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
