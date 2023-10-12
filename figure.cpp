#include "figure.h"
#include "ui_figure.h"
#include "choice.h"

figure::figure(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::figure)
{
    ui->setupUi(this);
}

figure::~figure()
{
    delete ui;
}

using namespace std;

class SDES {
private:
    // 初始置换表（IP）
    static const int IP[8];
    // 扩展置换表（EP）
    static const int EP[8];
    // P4置换表
    static const int P4[4];
    // 逆初始置换表（IP^-1）
    static const int IP_INV[8];
    // P10置换表
    static const int P10[10];
    // P8置换表
    static const int P8[8];
    // S-盒表
    static const int S0[4][4];
    static const int S1[4][4];

public:
    // 新函数：将ASCII字符映射为二进制数组
    static vector<int> asciiToBinary(const string& input);

    // 新函数：将二进制数组映射为ASCII字符
    static string binaryToAscii(const vector<int>& input);

    // 新函数：使用10位密钥加密ASCII编码字符串
    static string encryptAscii(const string& plaintext, const vector<int>& key);

    // 新函数：使用10位密钥解密ASCII编码字符串
    static string decryptAscii(const string& ciphertext, const vector<int>& key);

    // 新函数：将字符串向量连接成一个字符串
    static string join(const vector<string>& strings, const string& delimiter);

    //新函数：暴力破解
    static vector<int> bruteForceDecrypt(const vector<int>& ciphertext, const vector<int>& knownPlaintext);

    // 构造函数
    SDES() = default;

    // P4置换函数
    static vector<int> permuteP4(const vector<int>& input);

    // P10置换
    static vector<int> permuteP10(const vector<int>& key);

    // P8置换
    static vector<int> permuteP8(const vector<int>& key);

    // 扩展函数 EP
    static vector<int> expandEP(const vector<int>& input);

    // 两个位数组进行异或操作
    static vector<int> xorOperation(const vector<int>& a, const vector<int>& b);

    // S-盒替换
    static vector<int> sBox(const vector<int>& input, const int sBoxTable[4][4]);

    // F函数
    static vector<int> fFunction(const vector<int>& r, const vector<int>& subKey);

    // 循环左移
    static vector<int> circularLeftShift(const vector<int>& input);

    // 连接两个位数组
    static vector<int> concat(const vector<int>& a, const vector<int>& b);

    // 初始置换（IP）
    static vector<int> initialPermutation(const vector<int>& input);

    // 逆初始置换（IP^-1）
    static vector<int> inverseInitialPermutation(const vector<int>& input);

    // 密钥生成
    static vector<vector<int>> generateSubKeys(const vector<int>& key);

    // 使用10位密钥加密8位明文
    static vector<int> encrypt(const vector<int>& plaintext, const vector<int>& key);

    // 使用10位密钥解密8位密文
    static vector<int> decrypt(const vector<int>& ciphertext, const vector<int>& key);


};

// 初始化静态成员变量
const int SDES::IP[8] = { 2, 6, 3, 1, 4, 8, 5, 7 };
const int SDES::EP[8] = { 4, 1, 2, 3, 2, 3, 4, 1 };
const int SDES::P4[4] = { 2, 4, 3, 1 };
const int SDES::IP_INV[8] = { 4, 1, 3, 5, 7, 2, 8, 6 };
const int SDES::P10[10] = { 3, 5, 2, 7, 4, 10, 1, 9, 8, 6 };
const int SDES::P8[8] = { 6, 3, 7, 4, 8, 5, 10, 9 };
const int SDES::S0[4][4] = { {1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 0, 2} };
const int SDES::S1[4][4] = { {0, 1, 2, 3}, {2, 3, 1, 0}, {3, 0, 1, 2}, {2, 1, 0, 3} };

// 实现各种成员函数

// 实现新函数：暴力破解
vector<int> SDES::bruteForceDecrypt(const vector<int>& ciphertext, const vector<int>& knownPlaintext) {
    vector<int> potentialKey(10, 0);
    vector<int> decrypted;

    for (int key = 0; key < 1024; key++) {
        // 将密钥表示为二进制
        for (int i = 0; i < 10; i++) {
            potentialKey[i] = (key >> (9 - i)) & 1;
        }

        // 使用当前密钥尝试解密
        decrypted = decrypt(ciphertext, potentialKey);

        // 检查解密结果是否与已知的明文匹配
        if (decrypted == knownPlaintext) {
            return potentialKey;
        }
    }

    // 如果没有找到匹配的密钥，返回空向量
    return vector<int>();
}

// 实现新函数：将ASCII字符映射为二进制数组
vector<int> SDES::asciiToBinary(const string& input) {
    vector<int> binaryData;
    for (char c : input) {
        for (int i = 7; i >= 0; --i) {
            binaryData.push_back((c >> i) & 1);
        }
    }
    return binaryData;
}

// 实现新函数：将二进制数组映射为ASCII字符
string SDES::binaryToAscii(const vector<int>& input) {
    string asciiData;
    for (size_t i = 0; i < input.size(); i += 8) {
        char c = 0;
        for (int j = 7; j >= 0; --j) {
            c |= (input[i + 7 - j] << j);
        }
        asciiData += c;
    }
    return asciiData;
}

// 修改加密函数，接受ASCII字符串并返回ASCII字符串
string SDES::encryptAscii(const string& plaintext, const vector<int>& key) {
    vector<int> binaryPlaintext = asciiToBinary(plaintext);
    vector<string> encryptedGroups;

    // 分组长度为8位
    for (size_t i = 0; i < binaryPlaintext.size(); i += 8) {
        vector<int> group(binaryPlaintext.begin() + i, binaryPlaintext.begin() + i + 8);
        vector<int> encryptedGroup = encrypt(group, key);
        encryptedGroups.push_back(binaryToAscii(encryptedGroup));
    }

    return join(encryptedGroups, "");
}

// 修改解密函数，接受ASCII字符串并返回ASCII字符串
string SDES::decryptAscii(const string& ciphertext, const vector<int>& key) {
    vector<int> binaryCiphertext = asciiToBinary(ciphertext);
    vector<string> decryptedGroups;

    // 分组长度为8位
    for (size_t i = 0; i < binaryCiphertext.size(); i += 8) {
        vector<int> group(binaryCiphertext.begin() + i, binaryCiphertext.begin() + i + 8);
        vector<int> decryptedGroup = decrypt(group, key);
        decryptedGroups.push_back(binaryToAscii(decryptedGroup));
    }

    return join(decryptedGroups, "");
}

// 辅助函数：将字符串向量连接成一个字符串
string SDES::join(const vector<string>& strings, const string& delimiter) {
    string result;
    for (size_t i = 0; i < strings.size(); ++i) {
        result += strings[i];
        if (i < strings.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

//-----------------------分割线--------------------------//

    // P4置换函数
    vector<int> SDES::permuteP4(const vector<int>& input) {
        vector<int> output(4);
        for (int i = 0; i < 4; i++) {
            output[i] = input[P4[i] - 1];
        }
        return output;
    }

    // P10置换
    vector<int> SDES::permuteP10(const vector<int>& key) {
        vector<int> result(10);
        for (int i = 0; i < 10; i++) {
            result[i] = key[P10[i] - 1];
        }
        return result;
    }

    // P8置换
    vector<int> SDES::permuteP8(const vector<int>& key) {
        vector<int> result(8);
        for (int i = 0; i < 8; i++) {
            result[i] = key[P8[i] - 1];
        }
        return result;
    }

    // 扩展函数 EP
    vector<int> SDES::expandEP(const vector<int>& input) {
        vector<int> output(8);
        for (int i = 0; i < 8; i++) {
            output[i] = input[EP[i] - 1];
        }
        return output;
    }

    // 两个位数组进行异或操作
    vector<int> SDES::xorOperation(const vector<int>& a, const vector<int>& b) {
        vector<int> result(a.size());
        for (size_t i = 0; i < a.size(); i++) {
            result[i] = a[i] ^ b[i];
        }
        return result;
    }

    // S-盒替换
    vector<int> SDES::sBox(const vector<int>& input, const int sBoxTable[4][4]) {
        int row = input[0] * 2 + input[3];
        int col = input[1] * 2 + input[2];
        int value = sBoxTable[row][col];
        return { (value >> 1) & 1, value & 1 };
    }

    // F函数
    vector<int> SDES::fFunction(const vector<int>& r, const vector<int>& subKey) {
        vector<int> expandedR = expandEP(r);
        vector<int> xorResult = xorOperation(expandedR, subKey);
        vector<int> s0Input(xorResult.begin(), xorResult.begin() + 4);
        vector<int> s1Input(xorResult.begin() + 4, xorResult.end());

        vector<int> s0Output = sBox(s0Input, S0);
        vector<int> s1Output = sBox(s1Input, S1);

        vector<int> p4Input = concat(s0Output, s1Output);
        return permuteP4(p4Input);
    }

    // 循环左移
    vector<int> SDES::circularLeftShift(const vector<int>& input) {
        vector<int> result = input;
        int temp = result[0];
        for (size_t i = 0; i < input.size() - 1; i++) {
            result[i] = result[i + 1];
        }
        result[result.size() - 1] = temp;
        return result;
    }

    // 连接两个位数组
    vector<int> SDES::concat(const vector<int>& a, const vector<int>& b) {
        vector<int> result(a.size() + b.size());
        for (size_t i = 0; i < a.size(); i++) {
            result[i] = a[i];
        }
        for (size_t i = 0; i < b.size(); i++) {
            result[i + a.size()] = b[i];
        }
        return result;
    }

    // 初始置换（IP）
    vector<int> SDES::initialPermutation(const vector<int>& input) {
        vector<int> output(8);
        for (int i = 0; i < 8; i++) {
            output[i] = input[IP[i] - 1];
        }
        return output;
    }

    // 逆初始置换（IP^-1）
    vector<int> SDES::inverseInitialPermutation(const vector<int>& input) {
        vector<int> output(8);
        for (int i = 0; i < 8; i++) {
            output[i] = input[IP_INV[i] - 1];
        }
        return output;
    }

    // 密钥生成
    vector<vector<int>> SDES::generateSubKeys(const vector<int>& key) {
        vector<int> p10Result = permuteP10(key);
        vector<int> leftShift1 = circularLeftShift(vector<int>(p10Result.begin(), p10Result.begin() + 5));
        vector<int> leftShift2 = circularLeftShift(vector<int>(p10Result.begin() + 5, p10Result.end()));
        vector<int> leftShiftedKey = concat(leftShift1, leftShift2);
        vector<vector<int>> subKeys(2);
        subKeys[0] = permuteP8(leftShiftedKey); // k1
        leftShift1 = circularLeftShift(leftShift1);
        leftShift2 = circularLeftShift(leftShift2);
        leftShiftedKey = concat(leftShift1, leftShift2);
        subKeys[1] = permuteP8(leftShiftedKey); // k2
        return subKeys;
    }

    // 使用10位密钥加密8位明文
    vector<int> SDES::encrypt(const vector<int>& plaintext, const vector<int>& key) {
        vector<vector<int>> subKeys = generateSubKeys(key);
        vector<int> initialPermutationResult = initialPermutation(plaintext);
        vector<int> left(initialPermutationResult.begin(), initialPermutationResult.begin() + 4);
        vector<int> right(initialPermutationResult.begin() + 4, initialPermutationResult.end());
        vector<int> fResult = fFunction(right, subKeys[0]);
        vector<int> xorResult = xorOperation(left, fResult);
        vector<int> swapped = concat(right, xorResult);
        left = vector<int>(swapped.begin(), swapped.begin() + 4);
        right = vector<int>(swapped.begin() + 4, swapped.end());
        fResult = fFunction(right, subKeys[1]);
        xorResult = xorOperation(left, fResult);
        return inverseInitialPermutation(concat(xorResult, right));
    }

    // 使用10位密钥解密8位密文
    vector<int> SDES::decrypt(const vector<int>& ciphertext, const vector<int>& key) {
        vector<vector<int>> subKeys = generateSubKeys(key);
        vector<int> initialPermutationResult = initialPermutation(ciphertext);
        vector<int> left(initialPermutationResult.begin(), initialPermutationResult.begin() + 4);
        vector<int> right(initialPermutationResult.begin() + 4, initialPermutationResult.end());
        vector<int> fResult = fFunction(right, subKeys[1]);
        vector<int> xorResult = xorOperation(left, fResult);
        vector<int> swapped = concat(right, xorResult);
        left = vector<int>(swapped.begin(), swapped.begin() + 4);
        right = vector<int>(swapped.begin() + 4, swapped.end());
        fResult = fFunction(right, subKeys[0]);
        xorResult = xorOperation(left, fResult);
        return inverseInitialPermutation(concat(xorResult, right));
    }

void figure::on_pushButton_clicked()
{
    QString text1= ui->lineEdit->text();
    QString text2= ui->lineEdit_2->text();
    QString text3= ui->lineEdit_3->text();
    char*  ch;
    QByteArray ba = text1.toLatin1(); // must
    ch=ba.data();
    char*  ch2;
    QByteArray ba2 = text2.toLatin1(); // must
    ch2=ba2.data();
    char*  ch3;
    QByteArray ba3 = text3.toLatin1(); // must
    ch3=ba3.data();
    if(text1.length()==0)
    {
        if(text2.length()!=10)
        {
            QMessageBox::information(NULL, "错误", "你输入的秘钥长度不正确", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        else
        {
            for(int i=0;i<text2.length();i++)
            {
              if(ch2[i]!='0'&&ch2[i]!='1')
              {
                  QMessageBox::information(NULL, "错误", "你输入的秘钥不是二进制", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
              }
            }
            if(text3.length()!=8)
            {
                QMessageBox::information(NULL, "错误", "你输入的密文长度不正确", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
            else
            {
                bool w=true;
                for(int i=0;i<text3.length();i++)
                {
                  if(ch3[i]!='0'&&ch3[i]!='1')
                  {
                      w=false;
                      QMessageBox::information(NULL, "错误", "你输入的密文不是二进制", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                  }
                }
                if(w)
                {
                    vector<int> encrypted = {}; // 8位明文
                    vector<int> key = {}; // 10位密钥
                    for(int i=0;i<text3.length();i++)
                    {
                        encrypted.push_back(int(ch3[i])-48);
                    }
                    for(int i=0;i<text2.length();i++)
                    {
                        key.push_back(int(ch2[i])-48);
                    }
                    vector<int> decrypted = SDES::decrypt(encrypted, key);
                    stringstream ss;
                    string str;
                    copy(decrypted.begin(), decrypted.end(), ostream_iterator<int>(ss, ""));
                    str = ss.str();
                    QString qstr = QString::fromStdString(str);
                    ui->lineEdit->setText(qstr);
                }
            }
        }
    }
    else
    {
        if(text1.length()!=8)
        {
            QMessageBox::information(NULL, "错误", "你输入的明文长度不正确", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        else
        {
            for(int i=0;i<text1.length();i++)
            {
              if(ch[i]!='0'&&ch[i]!='1')
              {
                  QMessageBox::information(NULL, "错误", "你输入的明文不是二进制", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
              }
            }
            if(text2.length()!=10)
            {
                QMessageBox::information(NULL, "错误", "你输入的秘钥长度不正确", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            }
            else
            {
                bool w=true;
                for(int i=0;i<text2.length();i++)
                {
                  if(ch2[i]!='0'&&ch2[i]!='1')
                  {
                      w=false;
                      QMessageBox::information(NULL, "错误", "你输入的秘钥不是二进制", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                  }
                }
                if(w)
                {
                    vector<int> plaintext = {}; // 8位明文
                    vector<int> key = {}; // 10位密钥
                    for(int i=0;i<text1.length();i++)
                    {
                        plaintext.push_back(int(ch[i])-48);
                    }
                    for(int i=0;i<text2.length();i++)
                    {
                        key.push_back(int(ch2[i])-48);
                    }
                    vector<int> encrypted = SDES::encrypt(plaintext, key);
                    stringstream ss;
                    string str;
                    copy(encrypted.begin(), encrypted.end(), ostream_iterator<int>(ss, ""));
                    str = ss.str();
                    QString qstr = QString::fromStdString(str);
                    ui->lineEdit_3->setText(qstr);
                }
            }
        }
    }
}
void figure::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(e->x()>840&&e->x()<840+110&&e->y()>10&&e->y()<10+60)
        {
            this->close();
            choice *f=new choice();
            f->show();
        }
    }
}
