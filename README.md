# calculator-app-thingy

Simple console calculator that handles decimal numbers. Start with an initial value, chain additions, subtractions, multiplications, divisions, or modulo operations, jump to a fresh calculation with `n`, recall stored work with `m`, and quit anytime with `q`.

## Build and run

```bash
g++ -std=c++17 -Wall -Wextra -o calculator Calculator.cpp
./calculator
```

Follow the prompts: enter the first number, pick an operator, enter the next number, and the updated result prints immediately. Press `q` when choosing an operator to exit.
Choose `n` when prompted for an operator to clear the running total and start a new calculation.
Press `m` to open memory, pick a numbered calculation, and review the full progression of its steps.
