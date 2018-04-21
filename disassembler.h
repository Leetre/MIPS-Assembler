#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <qmap.h>
#include <QDebug>

class Disassembler
{
public:
    Disassembler();
    int Cases_contained(QString& Op);
    QString Discompile(QString& stringNeedCompile);
    QStringList Addn(QString& stringNeedCompile);
    QString Join_e(QStringList& rawAssembledStringList);
    QString InstructionToMipsCode(QString currentLine);
    QString convertToFabs(QString Bin);
    QString GetSubString(QString input, int beginposition, int endposition);
    QString convertBinToD(QString);
    QString convertBinToHex(QString BinString);
private:
    QString DisassembledString;

    QMap<QString, QString> KeyOp;
    QMap<QString, QString> Reg;

    QStringList STD_Instruction;
    QStringList TDA_Instruction;
    QStringList S_Instruction;
    QStringList STI_Instruction;
    QStringList STII_Instruction;
    QStringList J_Instruction;
    QStringList MipsresultList;
};

#endif // DISASSEMBLER_H
