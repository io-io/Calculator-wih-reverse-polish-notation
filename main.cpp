/*Написать калькулятор выражений, в которых используются переменные и
целые числа. В выражениях допустимы скобки любого уровня вложенности.
Множество допустимых операций: +, -, *, /, ^. Исходное выражение
считывается из входного файла, результат выводится в выходной файл.
Целочисленные значения переменных спросить у пользователя. Табличку
заданных значений переменных также вывести в файл.*/

#include <iostream>
#include <fstream>
#include <iostream>
#include <cstddef>
#include <iomanip>


using namespace std;

struct dict {
    char variable;
    int value;
};

struct stack{
    char info;
    struct stack *next;
};

void push(stack* &st, char symbol){
    stack *top=new(stack);
    top->info=symbol;
    top->next=st;
    st=top;
}

char pop(stack* &st){
    char out=st->info;
    stack *inter=new(stack);
    inter=st->next;
    st=inter;
    return out;
}

string postfix(string file, dict dictionary[]) {
    string out = "";//постфиксная запись хранится здесь
    ifstream input(file);
    if (!input.is_open()) {
        cout << "File is not found.";
        input.close();
    } else {
        string infix;
        input >> infix;
        int end = infix.length() - 1;//последний элемент слова
        if (!((infix[end] >= 'a') || (infix[end] <= 'z') || (infix[end] == '(') || (infix[end] == ')') ||
              (infix[end] >= '0') || (infix[end] <= '9'))) {
            cout << "The last symbol must be a number or a letter." << endl;
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
        if (LeftBracket != RightBracket) {
            cout << "Number of opening brackets and closing ones is unequal." << endl;
            system("pause");
            return 0;
        }
        stack *top;
        push(top, 'X');//метка того, что стек закончился
        int NumberOfVariables = 0;//число переменных в инфиксной записи

        for (int letter = 0; letter <= end; letter++) {
            if ((infix[letter] >= 'a') && (infix[letter] <= 'z')) {
                out += infix[letter];
                bool contains = false;
                for (int i = 1; i <= NumberOfVariables; i++) {
                    if (infix[letter] == dictionary[i].variable) {
                        contains=true;
                        break;
                    }}
                    if (!contains){
                        dictionary[NumberOfVariables + 1].variable = infix[letter];
                        NumberOfVariables++;
                    }
            }else {
                switch (infix[letter]) {
                case '^':
                    while ((top->info == '^')) {
                        out += infix[letter];
                    }
                        push(top, infix[letter]);
                    break;
                case '*':
                    while ((top->info == '/') || (top->info == '*') || (top->info == '^')) {
                        out += pop(top);
                    }
                        push(top, infix[letter]);
                    break;
                case '/':
                    while ((top->info == '/') || (top->info == '*') || (top->info == '^')) {
                        out += pop(top);
                    }
                        push(top, infix[letter]);
                    break;
                case '+':
                    push(top, infix[letter]);
                    break;
                case '-':
                    push(top, infix[letter]);
                    break;
                case '(':
                    push(top, infix[letter]);
                    break;
                case ')':
                    while (top->info != '(') {
                        out += pop(top);
                    }
                    pop(top);
                    break;
            }}
        }
        while (top->info != 'X') {
            out += pop(top);
        }
        dictionary[0].variable = NumberOfVariables;
    }
    return out;
}

int main() {
    dict dictionary[30];
    string post = postfix("input.txt", &dictionary[30]);
    for (int i = 1; i <= dictionary[0].value; i++) {
        cout << "Enter variable " << dictionary[i].variable << ":";
        cin >> dictionary[i].value;
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
        output << dictionary[i].variable;
    }

    cout<<post<<endl;

    system("pause");

}