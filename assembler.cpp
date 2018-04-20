#include "assembler.h"

Assembler::Assembler()
{
    KeyCode = QMap<QString, QString>({
        {"add",     "000000ssssstttttddddd00000100000"},
        {"addu",    "000000ssssstttttddddd00000100001"},
        {"addi",    "001000ssssstttttiiiiiiiiiiiiiiii"},
        {"addiu",   "001001ssssstttttiiiiiiiiiiiiiiii"},
        {"and",     "000000ssssstttttddddd00000100100"},
        {"andi",    "001100ssssstttttiiiiiiiiiiiiiiii"},
        {"nor",     "000000ssssstttttddddd00000100111"},
        {"or",      "000000ssssstttttddddd00000100101"},
        {"ori",     "001101ssssstttttiiiiiiiiiiiiiiii"},
        {"sll",     "00000000000tttttdddddaaaaa000000"},
        {"srl",     "00000000000tttttdddddaaaaa000010"},
        {"sub",     "000000ssssstttttddddd00000100010"},
        {"subu",    "000000ssssstttttddddd00000100011"},
        {"xor",     "000000ssssstttttddddd00000100110"},
        {"xori",    "001110ssssstttttiiiiiiiiiiiiiiii"},
        {"slt",     "000000ssssstttttddddd00000101010"},
        {"sltu",    "000000ssssstttttddddd00000101011"},
        {"slti",    "001010ssssstttttiiiiiiiiiiiiiiii"},
        {"sltiu",   "001001ssssstttttiiiiiiiiiiiiiiii"},
        {"beq",     "000100ssssstttttiiiiiiiiiiiiiiii"},
        {"bne",     "000101ssssstttttiiiiiiiiiiiiiiii"},
        {"j",       "000010iiiiiiiiiiiiiiiiiiiiiiiiii"},
        {"jal",     "000011iiiiiiiiiiiiiiiiiiiiiiiiii"},
        {"jr",      "000000sssss000000000000000001000"},
        {"lw",      "100011ssssstttttiiiiiiiiiiiiiiii"},
        {"sw",      "101011ssssstttttiiiiiiiiiiiiiiii"},
    });

    Register = QMap<QString,QString>({
        {"$zero","00000"},
        {"$at","00001"},
        {"$v0","00010"},
        {"$v1","00011"},
        {"$a0","00100"},
        {"$a1","00101"},
        {"$a2","00110"},
        {"$a3","00111"},
        {"$t0","01000"},
        {"$t1","01001"},
        {"$t2","01010"},
        {"$t3","01011"},
        {"$t4","01100"},
        {"$t5","01101"},
        {"$t6","01110"},
        {"$t7","01111"},
        {"$s0","10000"},
        {"$s1","10001"},
        {"$s2","10010"},
        {"$s3","10011"},
        {"$s4","10100"},
        {"$s5","10101"},
        {"$s6","10110"},
        {"$s7","10111"},
        {"$t8","11000"},
        {"$t9","11001"},

        {"$k0","11010"},
        {"$k1","11011"},
        {"$gp","11100"},
        {"$sp","11101"},
        {"$fp","11110"},
        {"$ra","11111"}
     });
    STD_Instruction << "add" << "addu" << "and"
                    << "nor" << "or" << "sub"
                    << "subu" << "xor" << "slt"
                    << "sltu";
    TDA_Instruction << "sll" << "srl";
    S___Instruction << "jr";
    STI_Instruction << "addi" << "addiu" << "andi"
                    << "ori" << "xori" << "slti"
                    << "sltiu" << "beq" << "bne";
    STII_Instruction << "sw" << "lw";
    J_Instruction << "j" << "jal";
}

QString Assembler::Compile(QString &CompileString)
{
    bool ok;
    resultList.clear();
    QStringList CompileList = Split_enter(CompileString);
    QListIterator<QString> iterator(CompileList);
    while (iterator.hasNext())
    {
        QString thisLine  = iterator.next();
        QChar comment = '#';
        int commentIndex = thisLine.indexOf(comment, 0);
        QString assembledLine = CompleteMachineCode(thisLine.left(commentIndex));
        if (!assembledLine.isEmpty())
        {
            assembledLine = QString("%1").arg(assembledLine.toLongLong(&ok, 2), 8, 16, QChar('0'));
            resultList.append(assembledLine);
        }
    }
    return Join_enter(resultList);
}

int Assembler::Case_contained(QString& Op){
    int Case = -1;
    if(STD_Instruction.contains(Op)){
        Case = 0;
    }
    else if(TDA_Instruction.contains(Op)){
        Case = 1;
    }
    else if(S___Instruction.contains(Op)){
        Case = 2;
    }
    else if(STI_Instruction.contains(Op)){
        Case = 3;
    }
    else if(STII_Instruction.contains(Op)){
        Case = 4;
    }
    else if(J_Instruction.contains(Op)){
        Case = 5;
    }
    return Case;
}

QStringList Assembler::Split_enter(QString &CompileString)
{
    QChar sep = '\n';
    QStringList my_stringlist = CompileString.split(sep);
    return my_stringlist;
}

QString Assembler::Join_enter(QStringList &AssembledStringList)
{
    AssembledString = AssembledStringList.join("\n");
    return AssembledString;
}

QString Assembler::CompleteMachineCode(QString thisLine)
{
    int Case;
    QString Op;
    QString machineCode;
    if (thisLine.isEmpty()){
        return "";
    }
    thisLine.replace("(", " ");
    thisLine.replace(")", " ");
    thisLine.replace(";", "");
    QStringList my_stringlist = thisLine.split(QRegExp("(\\s|,)"), QString::SkipEmptyParts);
    Op = my_stringlist.at(0);
    Case = Case_contained(Op);
    machineCode = KeyCode[my_stringlist.at(0)];
    switch(Case)
    {
        case 0:
            change_d(my_stringlist.at(1), machineCode);
            change_s(my_stringlist.at(2), machineCode);
            change_t(my_stringlist.at(3), machineCode);
            break;
        case 1:
            change_d(my_stringlist.at(1), machineCode);
            change_t(my_stringlist.at(2), machineCode);
            change_a(my_stringlist.at(3), machineCode);
            break;
        case 2:
            change_s(my_stringlist.at(1), machineCode);
            break;
        case 3:
            change_t(my_stringlist.at(1), machineCode);
            change_s(my_stringlist.at(2), machineCode);
            change_i(my_stringlist.at(3), machineCode);
            break;
        case 4:
            change_t(my_stringlist.at(1), machineCode);
            change_s(my_stringlist.at(3), machineCode);
            change_i(my_stringlist.at(2), machineCode);
            break;
        case 5:
            change_target(my_stringlist.at(1), machineCode);
            break;
    }
    return machineCode;
}

void Assembler::change_s(const QString& rs, QString& to_machinecode)
{
    QString RegSCode = Register[rs];
    to_machinecode.replace(6, 5, RegSCode);
}

void Assembler::change_t(const QString& rt, QString& to_machinecode)
{
    QString RegTCode = Register[rt];
    to_machinecode.replace(11, 5, RegTCode);
}

void Assembler::change_d(const QString& rd, QString& to_machinecode)
{
    QString RegDCode = Register[rd];
    to_machinecode.replace(16, 5, RegDCode);
}

void Assembler::change_a(const QString& sa, QString& to_machinecode)
{
    QString RegACode = Register[sa];
    to_machinecode.replace(21, 5, RegACode);
}

void Assembler::change_i(const QString& imm, QString& to_machinecode)
{
    QString ImmediateCode = QString("%1").arg(imm.toInt(), 16, 2, QChar('0'));
    to_machinecode.replace(16, 16, ImmediateCode);
}

void Assembler::change_target(const QString& target, QString& to_machinecode)
{
    bool ok;
    QString TargetCode = QString("%1").arg(target.toInt(&ok, 16), 26, 2, QChar('0'));
    to_machinecode.replace(6, 26, TargetCode);
}
