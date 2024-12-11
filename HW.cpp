#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

class Term
{
public:
    Term(float c = 0, int e = 0) : coef(c), exp(e) {}

    // ��^�Y�ƪ��ޥΡA���~���i�H�ק�
    float& getcoef()
    {
        return coef;
    }

    int getexp() const
    {
        return exp;
    }

private:
    float coef; // �Y��
    int exp; // ����
};

class Polynomial
{
public:
    Polynomial();  // �w�]�غc�l
    Polynomial(const Polynomial& p); // �ƻs�غc�l
    ~Polynomial(); // �Ѻc�l

    void addTerm(float coef, int exp); // �s�W�涵���A�X�֦P����
    Polynomial Add(const Polynomial& poly) const;  // �h�����[�k
    Polynomial Mult(const Polynomial& poly) const; // �h�������k
    float Eval(float x) const;  // �p��h�����b���w��x�U�����G
    void expandArray(); // �X�i�}�C���j�p
    void combineLikeTerms(); // �X�֦P����
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);

private:
    Term* termArray; // �D�s�����}�C�A�s�x�h�������C�@�����Y�ơ]coef�^�M���ơ]exp�^
    int capacity; // �}�C���e�q�A��ݭn��h�Ŷ��ɶi���X�i
    int terms; // ��e�h�������D�s���ƶq
};

// �w�]�غc�l
Polynomial::Polynomial() : termArray(nullptr), capacity(0), terms(0) {}

// �ƻs�غc�l
Polynomial::Polynomial(const Polynomial& p) : capacity(p.capacity), terms(p.terms)
{
    termArray = new Term[capacity]; // ���t�s���}�C
    for (int i = 0; i < terms; ++i)
    {
        termArray[i] = p.termArray[i]; // �ƻs��
    }
}

// �Ѻc�l�A����ʺA���t���O����
Polynomial::~Polynomial()
{
    delete[] termArray;
}

// �X�i�}�C���j�p
void Polynomial::expandArray()
{
    capacity = (capacity == 0) ? 1 : capacity * 2; // �p�G�e�q�� 0�A�h��l�Ƭ� 1�A�_�h½��
    Term* newArray = new Term[capacity];  // �Ыطs�}�C
    for (int i = 0; i < terms; ++i)
    {
        newArray[i] = termArray[i]; // �ƻs�{������
    }
    delete[] termArray;  // �����ª��}�C
    termArray = newArray;  // ��s termArray ���s�}�C
}

// �X�֦P�����A�N�ۦP���ƪ����X��
void Polynomial::combineLikeTerms()
{
    for (int i = 0; i < terms - 1; ++i)
    {
        for (int j = i + 1; j < terms; ++j)
        {
            // �`�N�o�̪� termArray[i] �M termArray[j] �ϥΪ��O�D const �ޥ�
            if (termArray[i].getexp() == termArray[j].getexp())
            {
                termArray[i].getcoef() += termArray[j].getcoef(); // �X�֦P���ƪ���
                for (int k = j; k < terms - 1; ++k)
                {
                    termArray[k] = termArray[k + 1]; // �᭱�������e��ɡA�ۦP���ƪ����A�X�᪺֫�ů�
                }
                --terms; // ��ֶ��ؼƶq
                --j; // ���s�ˬd�o�Ӧ�m
            }
        }
        // �p�G�X�֫�Y�Ƭ� 0�A�����Ӷ�
        if (termArray[i].getcoef() == 0)
        {
            for (int k = i; k < terms - 1; ++k)
            {
                termArray[k] = termArray[k + 1]; // �᭱�������e��ɡA�����Y�Ƭ�0������A�d�U���ů�
            }
            --terms; // ��ֶ��ؼƶq
            --i; // ���s�ˬd�o�Ӧ�m
        }
    }
    // �ˬd�̫�@���O�_�Y�Ƭ� 0
    if (terms > 0 && termArray[terms - 1].getcoef() == 0)
    {
        --terms;
    }
    // �����ƭ���(�j��p�ѥ��ܥk)�Ƨ�
    sort(termArray, termArray + terms, [](const Term& a, const Term& b)
    {
        return a.getexp() > b.getexp(); //���Ƥj���ƫe��
    });   // } ����Lambda��� // ); ���� std::sort �I�s
}

// �s�W�涵���A�æX�֦P����
void Polynomial::addTerm(float coef, int exp)
{
    if (coef != 0)
    {
        if (terms == capacity)
        {
            expandArray(); // �p�G�Ŷ������A�X�i�}�C
        }
        termArray[terms++] = Term(coef, exp); // �s�W�@��
        combineLikeTerms(); // �X�֦P����
    }
}

// �h�����[�k
Polynomial Polynomial::Add(const Polynomial& poly) const //poly�O(p2)
{
    Polynomial result = *this;  // �ϥη�e��H�Ӫ�l��result(�ƻs�غc�l�A�N��e���� *this(p1)�����e����ƻs��s����result��)
    for (int i = 0; i < poly.terms; ++i) //�C���`�����|�N p2 ����W�@���[�J��result
    {
        result.addTerm(poly.termArray[i].getcoef(), poly.termArray[i].getexp());  // �C���K�[��AaddTerm�|�B�z�O�_�ݭn�X�֦P����
    }
    return result;
}

// �h�������k
Polynomial Polynomial::Mult(const Polynomial& poly) const
{
    Polynomial result; //�ϥ�Polynomial���w�]�غc�l��l��result
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

// �p��h�����b���w�Ȫ����G
float Polynomial::Eval(float x) const
{
    float result = 0;
    for (int i = 0; i < terms; ++i)
    {
        result += termArray[i].getcoef() * pow(x, termArray[i].getexp());
    }
    return result;
}

// ��X�B��l����
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
        if (term.getcoef() == 0) continue; // ���L�Y�Ƭ� 0 ����
        if (i > 0 && term.getcoef() > 0) os << "+"; //�D�Ĥ@���B�Y�Ƥj��0

        if (term.getcoef() != 1 || term.getexp() == 0) os << term.getcoef(); // �ٲ��Y�Ƭ� 1 ����
        if (term.getexp() > 0)
        {
            os << "x";
            if (term.getexp() > 1) os << "^" << term.getexp(); // ���Ƥj�� 1 �����
        }
    }
    return os;
}

// ��J�B��l����
istream& operator>>(istream& is, Polynomial& p)
{
    int numTerms;
    cout << "��J�D�s�����ƶq: ";
    is >> numTerms;
    if (numTerms <= 0)
    {
        cout << "���~: �D�s���ƶq�����j��s�I" << endl;
        return is;
    }
    for (int i = 0; i < numTerms; ++i)
    {
        float coef;
        int exp;
        cout << "��J�Y�ƻP����: ";
        is >> coef >> exp;
        p.addTerm(coef, exp);
    }
    return is;
}

int main()
{
    Polynomial p1, p2;

    cout << "��J�Ĥ@�Ӧh����:" << endl;
    cin >> p1;

    cout << endl << "��J�ĤG�Ӧh����:" << endl;
    cin >> p2;

    Polynomial sum = p1.Add(p2);
    Polynomial prod = p1.Mult(p2);

    cout << "�Ĥ@�Ӧh����: " << p1 << endl;
    cout << "�ĤG�Ӧh����: " << p2 << endl;
    cout << "�M: " << sum << endl;
    cout << "�n: " << prod << endl;

    float x;
    cout << "�p��h������ (��J x ����): ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}