#include<iostream>
#include<string>
using namespace std;
typedef struct tagnode
{
    string s;
    struct tagnode* left;
    struct tagnode* right;
}node;
node* taonode(string s)
{
    node* p = new node;
    if (p == NULL)
    {
        cout << "khong du bo nho";
        system("pause");
        exit(1);
    }
    p->s = s;
    p->left = p->right = NULL;
    return p;
}
typedef node* cay;
void taocay(cay& t)
{
    t = NULL;
}
int vitridau(string s, int j)
{
    for (int i = j + 1; i < s.size(); i++)
        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '(')
            return i;
    return -1;
}
int timngoac(string s, int i)
{
    int j, dem = 1;
    for (j = i + 1; j < s.size(); j++)
    {
        if (s[j] == '(') dem++;
        else if (s[j] == ')') dem--;
        if (dem == 0) return j;
    }
    return -1;
}
void xulyngoac(string& s)
{
    if (s[0] == '(')
    {
        int i = timngoac(s, 0);
        if (i == s.size() - 1)
        {
            s.erase(0, 1);
            s.erase(s.size() - 1, 1);
        }
    }
}
void doidau(string& c)
{
    int x = 0;
    do {
        x = vitridau(c, x);
        if (x != -1)
        {
            if (c[x] == '(') x = timngoac(c, x);
            else if (c[x] == '-' && c[x - 1] != '*' && c[x - 1] != '/') c[x] = '+';
            else if (c[x] == '+' && c[x - 1] != '*' && c[x - 1] != '/') c[x] = '-';
            //chỉ đổi dấu + thành - và dấu - thành cộng khi nó là dấu của 1 số hạng
            //nếu nó dấu của 1 thừa số thì dữ nguyên không thay đổi gì cả mặc dù
            //dấu của node của cây tức s[i] là dấu - vì dấu của thừa số không liên quan đến số hạng
        }
    } while (x != -1);
}
node* setcay(string s)
{
    xulyngoac(s);
    node* p;
    int k, j, x;
    string a, b, c;
    int i = vitridau(s, 0);//tìm vị trí dấu đầu tiên trong biểu thức, có 2 trường hợp, có dấu và không có dấu
    if (s[0] == '(') i = 0;
    if (i == -1)// trường hợp không có dấu
    {
        p = taonode(s);
        return p;
    }
    else//trường hợp có dấu, có 3 trường hợp nhỏ
    {
        if (s[i] == '+' || s[i] == '-')// dấu + hoặc -
        {
            a = s[i];
            p = taonode(a);
            b = s.substr(0, i);
            xulyngoac(b);
            c = s.substr(i + 1, s.size() - i - 1);
            xulyngoac(c);
            //đổi dấu các số hạng phía sau nếu s[i] là dấu -
            if (s[i] == '-') doidau(c);
            p->left = setcay(b);
            p->right = setcay(c);
        }
        else if (s[i] == '*' || s[i] == '/')//dấu * hoặc /
        {
            k = i;
            //tìm vị trí dấu + hoặc - phía sau dấu ngoặc nhưng nằm ngoài dấu ngoặc
            do {
                k = vitridau(s, k);
                if (k != -1)
                {
                    if (s[k] == '(') k = timngoac(s, k);
                    if (k > 0 && (s[k] == '+' || s[k] == '-') && (s[k - 1] != '*' && s[k - 1] != '/')) break;
                }
            } while (k != -1);
            if (k == -1)//nếu không có thì node sẽ là dấu * hoặc / tại vị trí i ban đầu
            {
                a = s[i];
                p = taonode(a);
                b = s.substr(0, i);
                xulyngoac(b);
                c = s.substr(i + 1, s.size() - i - 1);
                xulyngoac(c);
                p->left = setcay(b);
                p->right = setcay(c);
            }
            else//nếu tìm thấy thì node là dấu tìm thấy
            {
                a = s[k];
                p = taonode(a);
                b = s.substr(0, k);
                xulyngoac(b);
                c = s.substr(k + 1, s.size() - k - 1);
                xulyngoac(c);
                //doi dau +-
                if (s[k] == '-') doidau(c);
                p->left = setcay(b);
                p->right = setcay(c);
            }
        }
        else if (s[i] == '(')//dấu mở ngoặc (
        {
            j = timngoac(s, i);//tìm vị trí dấu đóng ngoặc

            if (s[j + 1] == '+' || s[j + 1] == '-')//sau dấu đóng ngoặc là dấu + hoặc - thì node là dấu + hoặc -
            {
                a = s[j + 1];
                p = taonode(a);
                b = s.substr(0, j + 1);
                xulyngoac(b);
                c = s.substr(j + 2, s.size() - j - 2);
                xulyngoac(c);
                //doi dau +-
                if (s[j + 1] == '-') doidau(c);
                p->left = setcay(b);
                p->right = setcay(c);
            }
            else if (s[j + 1] == '*' || s[j + 1] == '/')//nếu sau đóng ngoặc là * hoặc /
            {
                k = j + 1;
                //tiếp tục tìm dấu + - phía sau nó mà nằm ngoài tất cả dấu ngoặc
                do {
                    k = vitridau(s, k);
                    if (k != -1)
                    {
                        if (s[k] == '(') k = timngoac(s, k);
                        if (k > 0 && (s[k] == '+' || s[k] == '-') && (s[k - 1] != '*' && s[k - 1] != '/')) break;
                    }
                } while (k != -1);
                if (k == -1)//nếu không tìm thấy thì node là dấu */ tại trí j+1
                {
                    a = s[j + 1];
                    p = taonode(a);
                    b = s.substr(0, j + 1);
                    xulyngoac(b);
                    c = s.substr(j + 2, s.size() - j - 2);
                    xulyngoac(c);
                    p->left = setcay(b);
                    p->right = setcay(c);
                }
                else//nếu tìm thấy thì node là dấu tìm thấy
                {
                    a = s[k];
                    p = taonode(a);
                    b = s.substr(0, k);
                    xulyngoac(b);
                    c = s.substr(k + 1, s.size() - k - 1);
                    xulyngoac(c);
                    //doi dau +-
                    if (s[j + 1] == '-') doidau(c);
                    p->left = setcay(b);
                    p->right = setcay(c);
                }
            }
        }
    }
    return p;
}
long atol(string s)
{
    long x = 0, d = 1;
    int j = 0, k = 0;
    if (s[0] == '+' || s[0] == '-')
    {
        j = 1;
        if (s[0] == '-') k = 1;
    }
    for (int i = s.size() - 1; i >= j; i--)
    {
        switch (s[i])
        {
        case '0':x += 0 * d; break;
        case '1':x += 1 * d; break;
        case '2':x += 2 * d; break;
        case '3':x += 3 * d; break;
        case '4':x += 4 * d; break;
        case '5':x += 5 * d; break;
        case '6':x += 6 * d; break;
        case '7':x += 7 * d; break;
        case '8':x += 8 * d; break;
        case '9':x += 9 * d; break;
        }
        d *= 10;
    }
    if (k == 0)return x;
    return -x;
}
long tinh(cay t)
{
    long x;
    if (t)
    {
        if (t->s == "+") return tinh(t->left) + tinh(t->right);
        else if (t->s == "-") return tinh(t->left) - tinh(t->right);
        else if (t->s == "*") return tinh(t->left) * tinh(t->right);
        else if (t->s == "/") return tinh(t->left) / tinh(t->right);
        else return atol(t->s);
    }
    return 0;
}
int main()
{
    cay t;
    taocay(t);
    string s;
    cout << "nhap bieu thuc toan hoc cua cac so nguyen duong:\n";
    getline(cin, s);
    t = setcay(s);
    cout << "gia tri bieu thuc = " << tinh(t) << endl;
    system("pause");
}
