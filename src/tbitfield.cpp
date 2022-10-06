// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);


TBitField::TBitField(int len)
{
    if (len >= 0) {
        BitLen = len;
        MemLen = (BitLen + (1 << 5  ) - 1) >> 5;
        pMem = new TELEM[MemLen];
        memset(pMem, 0, MemLen * sizeof(TELEM));
    }
    else{ throw length_error("negative size"); }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if (pMem != nullptr) {
        delete[]pMem;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n>BitLen)
        throw "Eror GetMemIndex";
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n>sizeof(TELEM) * 8)
        throw "Eror GetMemMask";
    return (1 << n);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen || n < 0)
        throw out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n>BitLen)
        throw "Eror ClrBit";
    int index = GetMemIndex(n);
    int number_bit = n - (n / (sizeof(TELEM) * 8)) * (sizeof(TELEM) * 8);
    pMem[index] &= ~(GetMemMask(number_bit));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw "Eror GetBit";
    int index = GetMemIndex(n);

    int number_bit = n - index * (sizeof(TELEM) * 8);
    if (pMem[index] & GetMemMask(number_bit))
        return 1;
    return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{

    if (pMem != nullptr) {
        delete[]pMem;
    }
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (*this == bf)
        return 0;
    return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int size = max(BitLen, bf.BitLen);
    TBitField res(size);
    int blocks = max(MemLen, bf.MemLen);
    for (int i = 0; i < blocks; i++) {
        res.pMem[i] = 0;
        if (i < MemLen)
            res.pMem[i] |= pMem[i];
        if (i < bf.MemLen)
            res.pMem[i] |= bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int len = max(BitLen, bf.BitLen);
    TBitField res(len);
    int size = max(MemLen, bf.MemLen);
    for (int i = 0; i < size; i++) {
        if (i < MemLen && i < bf.MemLen)
            res.pMem[i] = pMem[i] & bf.pMem[i];
        else
            res.pMem[i] = 0;
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen;i++) {
        res.pMem[i] = ~pMem[i];
    }
    int shift = BitLen & ((1 << 5) - 1);
    res.pMem[MemLen - 1] = (~pMem[MemLen - 1]) & ((1 << shift) - 1);
    
    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    int size;
    istr >> size;
    TBitField field(size);
    int value;
    for (int i = 0; i < size; i++) {

        istr >> value;

        if (value)

            field.SetBit(i);
    }
    bf = field;

    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    ostr << "size: " << bf.BitLen << '\n';
    for (int i = 0; i < bf.BitLen; i++) {
        cout << bf.GetBit(i) << ' ';
    }
    return ostr;
}