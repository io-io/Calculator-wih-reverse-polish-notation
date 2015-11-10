#include <iostream>
#include <fstream>
#include <iostream>
#include <cstddef>


using namespace std;

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

string postfix(string file/*, char* dict[30][2]*/) {
    string out = "";
    ifstream input(file);
    if (!input.is_open()) {
        cout << "File is not found.";
        input.close();
    } else {
        string infix;
        input >> infix;
        int end = infix.length() - 1;
        if (!((infix[end] >= 'a') || (infix[end] <= 'z') || (infix[end] == '(') || (infix[end] == ')'))) {
            cout << "The last symbol must be a number." << endl;
            system("pause");
            return 0;
        }
        int left = 0;
        int right = 0;
        for (int i = 0; i <= end; i++) {
            if (infix[i] == '(') {
                left++;
            } else if (infix[i] == ')') {
                right++;
            }
        }
        if (left != right) {
            cout << "Number of opening brackets and closing ones is unequal." << endl;
            system("pause");
            return 0;
        }
        stack *top;
        top->info='x';
        int NumberOfLetters=0;

        for (int num = 0; num <= end; num++) {
            if ((infix[num] >= 'a') && (infix[num] <= 'z')) {
                out += infix[num];
                /*bool contains=false;
                for (int i=1; i<=NumberOfLetters; i++){
                    if (infix[num]==*dict[i][1]){
                        contains=true;
                    }}
                    if (!contains){
                        *dict[NumberOfLetters+1][1]=infix[num];
                        NumberOfLetters++;
                    }*/
            }else {
            switch (infix[num]) {
                case '^':
                    while ((top->info == '^')) {
                        out += infix[num];
                    }
                    push(top, infix[num]);
                    break;
                case '*':
                    while ((top->info == '/') || (top->info == '*') || (top->info == '^')) {
                        out += pop(top);
                    }
                    push(top, infix[num]);
                    break;
                case '/':
                    while ((top->info == '/') || (top->info == '*') || (top->info == '^')) {
                        out += pop(top);
                    }
                    push(top, infix[num]);
                    break;
                case '+':
                    push(top, infix[num]);
                    break;
                case '-':
                    push(top, infix[num]);
                    break;
                case '(':
                    push(top, infix[num]);
                    break;
                case ')':
                    while (top->info != '(') {
                        out += pop(top);
                    }
                    pop(top);
                    break;
            }}
        }
        while (top->info!='x'){
            out += pop(top);
        }
    }
    return out;
}


int main() {
    /*char dict[30][2]={' '};*/
    string post=postfix("input.txt"/*, &dict[0][0]*/);
    //cout<<post<<endl;
    cout<<post<<endl;
    cout<<post<<endl;

    system("pause");

}