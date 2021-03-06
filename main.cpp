/*Написать калькулятор выражений, в которых используются переменные и
целые числа. В выражениях допустимы скобки любого уровня вложенности.
Множество допустимых операций: +, -, *, /, ^. Исходное выражение
считывается из входного файла, результат выводится в выходной файл.
Целочисленные значения переменных спросить у пользователя. Табличку
заданных значений переменных также вывести в файл.*/

#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

struct dict {
    char variable;
    int value;
};

struct stack {
    char info;
    struct stack *next;
};

struct stackInt {
    int info;
    struct stackInt *next;
};

template<class TemplateStack, class T>
void push(TemplateStack *&st, T symbol) {
    TemplateStack *top = new(TemplateStack);
    top->info = symbol;
    top->next = st;
    st = top;
}

template<class TemplateStack, class T>
void pop(TemplateStack *&st, T &out) {
    out = st->info;
    TemplateStack *inter = new(TemplateStack);
    inter = st->next;
    st = inter;
}

bool IsNumber(char *Symbol) {
    for (int i = 0; Symbol[i] != '\0'; i++) {
        if (((Symbol[i] < '0') || (Symbol[i] > '9')) && (Symbol[i] != '.')) {
            return false;
        }
    }
    return true;
}

string postfix(string file, dict dictionary[]) {
    string out = "";//постфиксная запись хранится здесь
    ifstream input(file);
    if (!input.is_open()) {
        cout << "File is not found.";
        input.close();
    } else {
        string infix;
        getline(input, infix);

        int end = infix.length() - 1;//последний элемент слова

        if ((infix[0] == '(') && (infix[end] == ')')) {
            cout << "USeless brackets on borders of infix notation." << endl;
            system("pause");
            return 0;
        }

        if (!((infix[end] >= 'a') && (infix[end] <= 'z') || (infix[end] == '(') || (infix[end] == ')') ||
              //проверка на последний элемент входной строки
              (infix[end] >= '0') && (infix[end] <= '9'))) {
            cout << "The last symbol must be a number or a letter or a bracket." << endl;
            system("pause");
            return 0;
        }
        int LeftBracket = 0;
        int RightBracket = 0;
        for (int i = 0; i <= end; i++) {
            if (infix[i] == '(') {
                LeftBracket++;
            } else if (infix[i] == ')') {
                RightBracket++;
            }
        }
        if (LeftBracket != RightBracket) {//количество скобок должно быть равно
            cout << "Number of opening brackets and closing ones is unequal." << endl;
            system("pause");
            return 0;
        }
        stack *top;
        push(top, 'X');//метка того, что стек закончился
        int NumberOfVariables = 0;//число переменных в инфиксной записи
        char inter;
        int NumberOfOperands = 0;//число операндов, т.е. количество переменных и чисел
        int NumberOfOperators = 0;

        infix += ' ';//уловка для проверки на пробелы между словами

        for (int letter = 0; letter <= end; letter += 2) {
            //проверка на последний элемент входной строки
            if (!((infix[letter] >= 'a') && (infix[letter] <= 'z') ||
                  (infix[letter] >= '(') && (infix[letter] <= '+') ||
                  (infix[letter] >= '/') && (infix[letter] <= '9') || (infix[letter] == '-'))) {
                cout <<
                    "The infix  notaion may contain only numbers, english letters and brackets on signifficant positions." <<
                    endl;
                system("pause");
                return 0;
            }

            if ((infix[letter] >= 'a') && (infix[letter] <= 'z')) {
                NumberOfOperands++;
                out += infix[letter];
                out += ' ';
                //просматриваем на вхождение этой переменной в словаре
                bool contains = false;
                for (int i = 1; i <= NumberOfVariables; i++) {
                    if (infix[letter] == dictionary[i].variable) {
                        contains = true;
                        break;
                    }
                }
                if (!contains) {
                    dictionary[NumberOfVariables + 1].variable = infix[letter];
                    NumberOfVariables++;
                }
            } else if ((infix[letter] >= '0') && (infix[letter] <= '9')) {
                NumberOfOperands++;
                string numbers;
                while ((infix[letter] >= '0') && (infix[letter] <= '9')) {
                    numbers += infix[letter];
                    letter++;
                }
                if (infix[letter] != ' ') {//элементы должны быть разделены только пробелом
                    cout << "Elements must be separated only by spaces." << endl;
                    system("pause");
                    return 0;
                }
                out += numbers + ' ';
                letter--;

                continue;
            } else {
                switch (infix[letter]) {
                    case '^':
                        NumberOfOperators++;
                        while ((top->info == '^')) {
                            out += infix[letter] + ' ';
                        }
                        push(top, infix[letter]);
                        break;
                    case '*':
                        NumberOfOperators++;
                        while ((top->info == '/') || (top->info == '*') || (top->info == '^')) {
                            pop(top, inter);
                            out += inter;
                            out += ' ';
                        }
                        push(top, infix[letter]);
                        break;
                    case '/':
                        NumberOfOperators++;
                        while ((top->info == '/') || (top->info == '*') || (top->info == '^')) {
                            pop(top, inter);
                            out += inter;
                            out += ' ';
                        }
                        push(top, infix[letter]);
                        break;
                    case '+':
                        NumberOfOperators++;
                        while ((top->info == '/') || (top->info == '*') || (top->info == '^') || (top->info == '+') ||
                               (top->info == '-')) {
                            pop(top, inter);
                            out += inter;
                            out += ' ';
                        }
                        push(top, infix[letter]);
                        break;
                    case '-':
                        NumberOfOperators++;
                        while ((top->info == '/') || (top->info == '*') || (top->info == '^') || (top->info == '+') ||
                               (top->info == '-')) {
                            pop(top, inter);
                            out += inter;
                            out += ' ';
                        }
                        push(top, infix[letter]);
                        break;
                    case '(':
                        push(top, infix[letter]);
                        break;
                    case ')':
                        while (top->info != '(') {
                            pop(top, inter);
                            out += inter;
                            out += ' ';
                        }
                        pop(top, inter);
                        break;
                }
                if (NumberOfOperators > NumberOfOperands) {
                    cout << "Number of operators is more then number of operands ." << endl;
                    system("pause");
                    return 0;
                }
            }
        }
        while (top->info != 'X') {
            pop(top, inter);
            out += inter;
            out += ' ';
        }
        dictionary[0].value = NumberOfVariables;
    }
    return out;
}

int calculate(string postfix, dict dictionary[]) {
    stackInt *top;

    int end = postfix.length() - 1;//последний элемент слова
    for (int i = 0; i <= end; i += 2) {
        if ((postfix[i] >= 'a') && (postfix[i] <= 'z')) {
            int j = 0;
            while (postfix[i] != dictionary[j].variable) {
                j++;
            }
            push(top, dictionary[j].value);
        } else if ((postfix[i] >= '0') && (postfix[i] <= '9')) {
            char numbers[50] = {' '};
            int j = 0;
            while ((postfix[i] >= '0') && (postfix[i] <= '9')) {
                numbers[j] = postfix[i];
                i++;
                j++;
            }
            //cout<<numbers<<endl;
            push(top, atoi(numbers));
            i--;
            continue;
        } else {
            int result;
            int OperandLast;
            pop(top, OperandLast);//операнд, стоящий вторым в постфиксной записи
            int OperandFirst;
            pop(top, OperandFirst);//операнд, стоящий первым в постфиксной записи

            switch (postfix[i]) {
                case '^':
                    result = pow(OperandFirst, OperandLast);
                    push(top, result);
                    break;
                case '*':
                    result = OperandFirst * OperandLast;
                    push(top, result);
                    break;
                case '/':
                    if (OperandLast == 0) {
                        cout << "You can`t divide by 0." << endl;
                        return 0;
                    }
                    result = OperandFirst / OperandLast;
                    push(top, result);
                    break;
                case '+':
                    result = OperandFirst + OperandLast;
                    push(top, result);
                    break;
                case '-':
                    result = OperandFirst - OperandLast;
                    push(top, result);
                    break;
            }
        }
    }
    int out;
    pop(top, out);
    return out;
}

int main() {
    dict dictionary[30];

    string post = postfix("input.txt", dictionary);

    for (int i = 1; i <= dictionary[0].value; i++) {
        cout << "Enter variable " << dictionary[i].variable << ":";
        char inter;
        cin >> inter;
        while (!(IsNumber(&inter))) {
            cout << "Variable can get only integer value." << endl;
            cin >> inter;
        }
        dictionary[i].value = atoi(&inter);
    }

    ofstream output;
    output.open("output.txt");

    //выводим переменные первой строкой
    for (int i = 1; i <= dictionary[0].value; i++) {
        output << setw(4);
        output << left;
        output << dictionary[i].variable;
    }
    output << " " << endl;//переходим на следующую строку
    //выводим значения переменных второй строкой
    for (int i = 1; i <= dictionary[0].value; i++) {
        output << setw(4);
        output << left;
        output << dictionary[i].value;
    }
    cout << post << endl;
    output << "The result is: " << calculate(post, dictionary);
}