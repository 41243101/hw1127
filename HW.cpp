#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

class Term
{
public:
    Term(float c = 0, int e = 0) : coef(c), exp(e) {}

    // 返回係數的引用，讓外部可以修改
    float& getcoef()
    {
        return coef;
    }

    int getexp() const
    {
        return exp;
    }

private:
    float coef; // 係數
    int exp; // 指數
};

class Polynomial
{
public:
    Polynomial();  // 預設建構子
    Polynomial(const Polynomial& p); // 複製建構子
    ~Polynomial(); // 解構子

    void addTerm(float coef, int exp); // 新增單項式，合併同類項
    Polynomial Add(const Polynomial& poly) const;  // 多項式加法
    Polynomial Mult(const Polynomial& poly) const; // 多項式乘法
    float Eval(float x) const;  // 計算多項式在指定值x下的結果
    void expandArray(); // 擴展陣列的大小
    void combineLikeTerms(); // 合併同類項
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);

private:
    Term* termArray; // 非零項的陣列，存儲多項式中每一項的係數（coef）和指數（exp）
    int capacity; // 陣列的容量，當需要更多空間時進行擴展
    int terms; // 當前多項式的非零項數量
};

// 預設建構子
Polynomial::Polynomial() : termArray(nullptr), capacity(0), terms(0) {}

// 複製建構子
Polynomial::Polynomial(const Polynomial& p) : capacity(p.capacity), terms(p.terms)
{
    termArray = new Term[capacity]; // 分配新的陣列
    for (int i = 0; i < terms; ++i)
    {
        termArray[i] = p.termArray[i]; // 複製項
    }
}

// 解構子，釋放動態分配的記憶體
Polynomial::~Polynomial()
{
    delete[] termArray;
}

// 擴展陣列的大小
void Polynomial::expandArray()
{
    capacity = (capacity == 0) ? 1 : capacity * 2; // 如果容量為 0，則初始化為 1，否則翻倍
    Term* newArray = new Term[capacity];  // 創建新陣列
    for (int i = 0; i < terms; ++i)
    {
        newArray[i] = termArray[i]; // 複製現有的項
    }
    delete[] termArray;  // 釋放舊的陣列
    termArray = newArray;  // 更新 termArray 為新陣列
}

// 合併同類項，將相同指數的項合併
void Polynomial::combineLikeTerms()
{
    for (int i = 0; i < terms - 1; ++i)
    {
        for (int j = i + 1; j < terms; ++j)
        {
            // 注意這裡的 termArray[i] 和 termArray[j] 使用的是非 const 引用
            if (termArray[i].getexp() == termArray[j].getexp())
            {
                termArray[i].getcoef() += termArray[j].getcoef(); // 合併同指數的項
                for (int k = j; k < terms - 1; ++k)
                {
                    termArray[k] = termArray[k + 1]; // 後面的項往前填補，相同指數的項，合併後的空缺
                }
                --terms; // 減少項目數量
                --j; // 重新檢查這個位置
            }
        }
        // 如果合併後係數為 0，移除該項
        if (termArray[i].getcoef() == 0)
        {
            for (int k = i; k < terms - 1; ++k)
            {
                termArray[k] = termArray[k + 1]; // 後面的項往前填補，移除係數為0的項後，留下的空缺
            }
            --terms; // 減少項目數量
            --i; // 重新檢查這個位置
        }
    }
    // 檢查最後一項是否係數為 0
    if (terms > 0 && termArray[terms - 1].getcoef() == 0)
    {
        --terms;
    }
    // 按指數降序(大到小由左至右)排序
    sort(termArray, termArray + terms, [](const Term& a, const Term& b)
    {
        return a.getexp() > b.getexp(); //指數大的排前面
    });   // } 關閉Lambda函數 // ); 結束 std::sort 呼叫
}

// 新增單項式，並合併同類項
void Polynomial::addTerm(float coef, int exp)
{
    if (coef != 0)
    {
        if (terms == capacity)
        {
            expandArray(); // 如果空間不足，擴展陣列
        }
        termArray[terms++] = Term(coef, exp); // 新增一項
        combineLikeTerms(); // 合併同類項
    }
}

// 多項式加法
Polynomial Polynomial::Add(const Polynomial& poly) const //poly是(p2)
{
    Polynomial result = *this;  // 使用當前對象來初始化result(複製建構子，將當前物件 *this(p1)的內容完整複製到新物件result中)
    for (int i = 0; i < poly.terms; ++i) //每次循環都會將 p2 的單獨一項加入到result
    {
        result.addTerm(poly.termArray[i].getcoef(), poly.termArray[i].getexp());  // 每次添加後，addTerm會處理是否需要合併同類項
    }
    return result;
}

// 多項式乘法
Polynomial Polynomial::Mult(const Polynomial& poly) const
{
    Polynomial result; //使用Polynomial的預設建構子初始化result
    for (int i = 0; i < terms; ++i)
    {
        for (int j = 0; j < poly.terms; ++j)
        {
            result.addTerm(termArray[i].getcoef() * poly.termArray[j].getcoef(),
                termArray[i].getexp() + poly.termArray[j].getexp());
        }
    }
    return result;
}

// 計算多項式在指定值的結果
float Polynomial::Eval(float x) const
{
    float result = 0;
    for (int i = 0; i < terms; ++i)
    {
        result += termArray[i].getcoef() * pow(x, termArray[i].getexp());
    }
    return result;
}

// 輸出運算子重載
ostream& operator<<(ostream& os, const Polynomial& p)
{
    if (p.terms == 0)
    {
        os << "0";
        return os;
    }
    for (int i = 0; i < p.terms; ++i)
    {
        Term& term = p.termArray[i];
        if (term.getcoef() == 0) continue; // 跳過係數為 0 的項
        if (i > 0 && term.getcoef() > 0) os << "+"; //非第一項且係數大於0

        if (term.getcoef() != 1 || term.getexp() == 0) os << term.getcoef(); // 省略係數為 1 的項
        if (term.getexp() > 0)
        {
            os << "x";
            if (term.getexp() > 1) os << "^" << term.getexp(); // 指數大於 1 時顯示
        }
    }
    return os;
}

// 輸入運算子重載
istream& operator>>(istream& is, Polynomial& p)
{
    int numTerms;
    cout << "輸入非零項的數量: ";
    is >> numTerms;
    if (numTerms <= 0)
    {
        cout << "錯誤: 非零項數量必須大於零！" << endl;
        return is;
    }
    for (int i = 0; i < numTerms; ++i)
    {
        float coef;
        int exp;
        cout << "輸入係數與指數: ";
        is >> coef >> exp;
        p.addTerm(coef, exp);
    }
    return is;
}

int main()
{
    Polynomial p1, p2;

    cout << "輸入第一個多項式:" << endl;
    cin >> p1;

    cout << endl << "輸入第二個多項式:" << endl;
    cin >> p2;

    Polynomial sum = p1.Add(p2);
    Polynomial prod = p1.Mult(p2);

    cout << "第一個多項式: " << p1 << endl;
    cout << "第二個多項式: " << p2 << endl;
    cout << "和: " << sum << endl;
    cout << "積: " << prod << endl;

    float x;
    cout << "計算多項式值 (輸入 x 的值): ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}