#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QString>
#include <QVector>
#include <QMap>
#include <qmap.h>
#include <QDebug>
#include <QRegExp>

class Assembler
{

public:
    Assembler();
    int Case_contained(QString& Op);
    QString Compile(QString& stringNeedCompile);
    QStringList Split_enter(QString& stringNeedCompile);
    QString Join_enter(QStringList& rawAssembledStringList);
    QString GetSubstring(QString input, int beginposition);
    QString CompleteMachineCode(QString currentLine);
    QString convertTocomplement(int offset);
    void change_s(const QString& rs, QString& to_machinecode);
    void change_t(const QString& rt, QString& to_machinecode);
    void change_d(const QString& rd, QString& to_machinecode);
    void change_a(const QString& sa, QString& to_machinecode);
    void change_i(const QString& imm, QString& to_machinecode);
    void change_labelori(QString& thisLine, const QString& label, QString& to_machinecode);
    void change_target(const QString& target, QString& to_machinecode);

private:
    QString AssembledString;
    QMap<QString, QString> KeyCode;
    QMap<QString, QString> Register;
    QMap<QString, QString> LabelAddr;
    QMap<QString, int> InstructionAddr;

    QStringList STD_Instruction;
    QStringList TDA_Instruction;
    QStringList S___Instruction;

    QStringList STI_Instruction;
    QStringList STII_Instruction;

    QStringList J_Instruction;

    QStringList resultList;
};

#endif // ASSEMBLER_H
