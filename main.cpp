/*Написать калькулятор выражений, в которых используются переменные и
целые числа. В выражениях допустимы скобки любого уровня вложенности.
Множество допустимых операций: +, -, *, /, ^. Исходное выражение
считывается из входного файла, результат выводится в выходной файл.
Целочисленные значения переменных спросить у пользователя. Табличку
заданных значений переменных также вывести в файл.*/

#include <iostream>
#include <fstream>
#include <math.h>
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

struct stackInt {
    int info;
    struct stack *next;
};

template<class TemplateStack, class T>
void push(TemplateStack *&st, T symbol) {
    TemplateStack *top = new(TemplateStack);
    top->info=symbol;
    top->next=st;
    st=top;
}

template<class TemplateStack, class T>
T pop(TemplateStack *&st, T) {
    T out = st->info;
    TemplateStack *inter = new(TemplateStack);
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
                //просматриваем на вхождение этой переменной в словаре
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
            } else if ((infix[letter] >= '0') && (infix[letter] <= '9')) {
                out += infix[letter];
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
                        out += pop(top, int);
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
        dictionary[0].value = NumberOfVariables;
    }
    return out;
}

int calculate(string postfix, dict dictionary[]) {
    stackInt *top;

    int end = postfix.length() - 1;//последний элемент слова
    for (int i = 0; i <= end; i++) {
        if ((postfix[i] >= 'a') && (postfix[i] <= 'z')) {
            int j = 0;
            while (postfix[i] != dictionary[j].variable) {
                j++;
            }
            push(top, dictionary[j].value);
        } else if ((postfix[i] >= '0') && (postfix[i] <= '9')) {
            push(top, postfix[i]);
        } else {
            char inter = pop(top);
            int OperandLast = atoi(&inter);//операнд, стоящий вторым в постфиксной записи
            inter = pop(top);
            int OperandFirst = atoi(&inter);//операнд, стоящий первым в постфиксной записи
            switch (postfix[i]) {
                case '^':
                    push(top, pow(OperandFirst, OperandLast));
                    break;
                case '*':
                    push(top, OperandFirst * OperandLast);
                    break;
                case '/':
                    if (OperandLast == 0) {
                        cout << "You can`t divide by 0" << endl;
                        exit;
                    }
                    push(top, OperandFirst / OperandLast);
                    break;
                case '+':
                    int intermedium = OperandFirst + OperandLast;
                    cout << intermedium << endl;
                    push(top, intermedium);
                    break;
                case '-':
                    push(top, OperandFirst - OperandLast);
                    break;
            }
        }
    }
    int out = pop(top);
    return out;
}

int main() {
    dict dictionary[30];
///СПРОСИ КАК ПРАВИЛЬНО НАЗЫВАТЬ  ИТЕРАТОРЫ(СЧЁТЧИКИ)
    string post = postfix("input.txt", dictionary);

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
        output << dictionary[i].value;
    }

    output << " " << endl;
    output << " " << endl;
    cout << post << endl;

    //  output << calculate(post, dictionary);

    system("pause");

}