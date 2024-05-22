#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;


class expression {
public:
    int unknown;
    double exp;
    expression() { unknown = 0; exp = 0; }
    void s_z() { unknown = 0; exp = 0; }

};


bool com_un(expression& a, expression& b) {
    return a.unknown > b.unknown;
}


bool com_st(expression& a, expression& b) {
    return a.unknown < b.unknown;
}

ostream& operator<<(ostream& stream, expression& a) {
    if (a.unknown == 0) return stream << a.exp;
    if (a.unknown == 1) {
        if (a.exp != 1) return stream << a.exp << " * x";
        else return stream << "x";
    }
    if (a.exp != 1) return stream << a.exp << " * x^" << a.unknown;
    else return stream << "x^" << a.unknown;
}


bool is_correct(string& str) {
    stack<char> prove;
    for (char c : str) {
        if (c == '(') {
            prove.push(c);
        }
        else if (c == ')') {
            if (prove.empty() || prove.top() != '(') {
                return false;
            }
            prove.pop();
        }
    }
    return prove.empty();
}



string multi_skob(string& str1, string& str2) {
    char o_s1 = '+'; char o_s2 = '+';
    if (str1[0] == '-') { o_s1 = '-'; str1.replace(0, 1, ""); }
    if (str2[0] == '-') { o_s2 = '-'; str2.replace(0, 1, ""); }

    string res, num, num1;
    for (char c : str1 + "+") {

        if (c == '+' || c == '-') {

            for (char c1 : str2 + "+") {

                if (c1 == '+' || c1 == '-') {
                    if (o_s1 == '-' && o_s2 == '-') o_s2 = '+';
                    else if (o_s1 == '-' && o_s2 == '+') o_s2 = '-';
                    else if (o_s1 == '+' && o_s2 == '-') o_s2 = '-';
                    else o_s2 = '+';
                    res = o_s2 + num + "*" + num1 + res;
                    num1 = "";
                    o_s2 = c1;
                }
                else num1 += c1;

            }

            o_s1 = c;
            num = "";

        }
        else num += c;
    }
    if (res[0] == '+') res.replace(0, 1, "");
    return res;
}


string sub_skob(string& str1) {
    char o_s1 = '+';
    if (str1[0] == '-') { o_s1 = '-'; str1.replace(0, 1, ""); }

    string res, num;
    for (char c : str1 + "+") {
        if (c == '+' || c == '-') {
            if (o_s1 == '+') res = "-" + num + res;
            else res = "+" + num + res;
            o_s1 = c;
            num = "";
        }
        else num += c;
    }
    if (res[0] == '+') res.replace(0, 1, "");
    return res;
}


string add_skob(string& str1, string& str2) {
    string res;
    char s_o = '+';
    if (str1[0] == '-') { s_o = '-'; str1.replace(0, 1, ""); }
    res = str2 + s_o + str1;
    if (res[0] == '+') res.replace(0, 1, "");
    return res;
}


string to_power_skob(string& str1, string& str2) {
    int col = stoi(str1);
    if (col == 0) return "1";
    string res = str2;
    for (int i = 1; i < col; i++) {
        res = multi_skob(str2, res);
    }
    return res;
}


bool math_ex(stack<string>& stack_n, stack<string>& stack_o) {
    if (stack_o.empty()) return false;
    string str = stack_n.top(); stack_n.pop();
    string str_o = stack_o.top(); stack_o.pop();
    string res;
    if (str_o == "+") {
        res = add_skob(str, stack_n.top()); stack_n.pop();
        stack_n.push(res);
        return true;
    }
    else if (str_o == "*") {
        res = multi_skob(str, stack_n.top()); stack_n.pop();
        stack_n.push(res);
        return true;
    }
    else if (str_o == "-") {
        res = sub_skob(str);
        stack_n.push(res);
        stack_o.push("+");

        return true;
    }
    return false;
}


int get_rang(string c) {
    if (c == "*") return 2;
    if (c == "+" || c == "-") return 1;
    return 0;
}

bool is_valid_expression(const string& str) {
    stack<char> brackets;

    for (char c : str) {
        if (!(c == 'x' || c == '+' || c == '-' || c == '*' || c == '(' || c == ')' || isdigit(c) || isspace(c))) {
            return false;
        }
    }
    return true;
}


int main() {
    do {
        setlocale(LC_ALL, "rus");
        cout << "Введите уравнение для упрощения или 00 для выхода:" << endl;
        string ep; getline(cin, ep);
        cout << endl;
        if (ep == "00") break;

        if (!is_valid_expression(ep)) {
            cout << "Введенное выражение содержит недопустимые символы!" << endl;
            return 0;
        }

        string op;
        for (char c : ep) if (c == '(' || c == ')') op += c;
        if (is_correct(op)) cout << "Упрощённое уравнение:" << endl << endl;
        else { cout << "Вы допустили ошибку!" << endl; return 0; }

        stack<string> stack_oper, stack_num;
        string num;
        ep = "(" + ep + ")";
        for (char c : ep + " ") {
            if (c >= '0' && c <= '9') {
                num += c;
                continue;
            }
            if (num != "") {
                stack_num.push(num);
                num = "";
            }
            if (c == '*' || c == '-' || c == '+') {
                if (stack_oper.empty()) {
                    num = c;
                    stack_oper.push(num);
                    num = "";
                    continue;
                }
                if (!stack_oper.empty() && stack_oper.top() == "-") {
                    math_ex(stack_num, stack_oper);
                }
                string p; p = c;
                if (!stack_oper.empty() && get_rang(p) > get_rang(stack_oper.top())) {
                    num = p;
                    stack_oper.push(num);
                    num = "";
                    continue;
                }
                if (!stack_oper.empty() && get_rang(p) <= get_rang(stack_oper.top())) {
                    while (!stack_oper.empty()) {
                        if (stack_oper.top() == "*") math_ex(stack_num, stack_oper);
                        else break;
                    }
                    if (stack_oper.top() == "-") math_ex(stack_num, stack_oper);
                    num = p;
                    stack_oper.push(num);
                    num = "";
                    continue;
                }

            }
            if (c == '(') {
                num = c;
                stack_oper.push(num);
                num = "";
                continue;
            }
            if (c == ')') {
                while (stack_oper.top() != "(") {
                    math_ex(stack_num, stack_oper);
                }
                stack_oper.pop();
                continue;
            }
            if (c >= 'a' && c <= 'z') {
                num = c;
                stack_num.push(num);
                num = "";
                continue;
            }
        }

        ep = stack_num.top(); stack_num.pop();
        ep = "(" + ep + ")";

        vector<expression> result;
        stack<expression> stack_n; stack<char> stack_o;
        num = ""; expression item;
        if (ep[1] == '-') { num = "-"; ep.replace(1, 1, ""); }
        for (char c : ep) {
            if (c >= '0' && c <= '9' || c == '.') {
                num += c;
                continue;
            }
            if (num != "" && num != "-") {
                item.exp = stod(num);
                item.unknown = 0;
                stack_n.push(item);
                num = "";
                item.s_z();
            }
            if (c == '-') {
                num = "-";
                stack_o.push('+');
                continue;
            }
            if (c == '+' || c == '*') {
                stack_o.push(c);
                continue;
            }
            if (c >= 'a' && c <= 'z') {
                if (num == "-") { item.exp = -1; num = ""; }
                else item.exp = 1;
                item.unknown = 1;
                stack_n.push(item);
                item.s_z();
                continue;
            }
            if (c == '(') {
                stack_o.push(c);
                continue;
            }
            if (c == ')') {
                item.s_z();
                item = stack_n.top(); stack_n.pop();
                while (stack_o.top() != '(') {
                    if (stack_o.top() == '+') {
                        result.push_back(item);
                        item = stack_n.top(); stack_n.pop();
                    }
                    else {
                        item.exp *= stack_n.top().exp;
                        item.unknown += stack_n.top().unknown;
                        stack_n.pop();
                    }
                    stack_o.pop();
                }
                result.push_back(item);
                stack_o.pop();
            }

        }



        sort(begin(result), end(result), com_un);

        item.s_z(); result.push_back(item);
        item = result[0];
        bool flag = true;
        for (int i = 1; i < result.size(); i++) {
            if (item.unknown == result[i].unknown) {
                item.exp += result[i].exp;
            }
            if (item.unknown != result[i].unknown || i == result.size() - 1) {
                if (item.exp == 0) { item = result[i]; continue; }
                if (item.exp < 0) {
                    item.exp = abs(item.exp);
                    cout << " - " << item;
                    flag = false;
                }
                else {
                    if (flag) {
                        cout << item;
                        flag = false;
                    }
                    else {
                        cout << " + " << item;
                    }
                }
                item = result[i];
            }
        } cout << endl;
    }
    while (true);

    return 0;
}