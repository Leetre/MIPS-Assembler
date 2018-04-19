#include "disassembler.h"

Disassembler::Disassembler()
{
    KeyOp = QMap<QString, QString>({
        {"000000100000",     "add"},
        {"000000100001",    "addu"},
        {"001000",          "addi"},
        {"001001",         "addiu"},
        {"000000100100",     "and"},
        {"001100",          "andi"},
        {"000000100101",      "or"},
        {"001101",           "ori"},
        {"000000000000",     "sll"},
        {"000000000010",     "srl"},
        {"000000100010",     "sub"},
        {"000000100011",    "subu"},
        {"000000100110",     "xor"},
        {"001110",          "xori"},
        {"000000101010",     "slt"},
        {"000000101011",    "sltu"},
        {"001010",          "slti"},
        {"001001",         "sltiu"},
        {"000100",           "beq"},
        {"000101",           "bne"},
        {"000010",             "j"},
        {"000011",           "jal"},
        {"000000001000",      "jr"},
        {"100011",            "lw"},
        {"101011",            "sw"},
    });

    Reg = QMap<QString,QString>({
        {"00000","$zero"},
        {"00001","$at"},
        {"00010","$v0"},
        {"00011","$v1"},
        {"00100","$a0"},
        {"00101","$a1"},
        {"00110","$a2"},
        {"00111","$a3"},
        {"01000","$t0"},
        {"01001","$t1"},
        {"01010","$t2"},
        {"01011","$t3"},
        {"01100","$t4"},
        {"01101","$t5"},
        {"01110","$t6"},
        {"01111","$t7"},
        {"10000","$s0"},
        {"10001","$s1"},
        {"10010","$s2"},
        {"10011","$s3"},
        {"10100","$s4"},
        {"10101","$s5"},
        {"10110","$s6"},
        {"10111","$s7"},
        {"11000","$t8"},
        {"11001","$t9"},

        {"11010","$k0"},
        {"11011","$k1"},
        {"11100","$gp"},
        {"11101","$sp"},
        {"11110","$fp"},
        {"11111","$ra"}
     });

    STD_Instruction << "add"
                    << "addu"
                    << "and"
                    << "nor"
                    << "or"
                    << "sub"
                    << "subu"
                    << "xor"
                    << "slt"
                    << "sltu";
    TDA_Instruction << "sll"
                    << "srl";
    S_Instruction << "jr";
    STI_Instruction << "addi"
                    << "addiu"
                    << "andi"
                    << "ori"
                    << "xori"
                    << "slti"
                    << "sltiu"
                    << "beq"
                    << "bne";
    STII_Instruction << "sw"
                     << "lw";
    J_Instruction << "j"
                  << "jal";
}

QString Disassembler::Discompile(QString &stringNeedCompile)
{
    MipsresultList.clear();
    QStringList listOfStringNeedCompile = Addn(stringNeedCompile);
    QListIterator<QString> itr(listOfStringNeedCompile);
    while (itr.hasNext())
    {
        QString currentLine  = itr.next();
        QChar commentBegin = '#';
        int commentIndex = currentLine.indexOf(commentBegin, 0);
        QString disassembledLine = validInstructionToMipsCode(currentLine.left(commentIndex));
        if (!disassembledLine.isEmpty())
        {
            MipsresultList.append(disassembledLine);
        }
    }
    return getRawDisassembledString(MipsresultList);
}

QString Disassembler::GetSubString(QString input, int beginposition, int endposition)
{
    int i;
    QString result = "";
    for(i = beginposition; i < endposition; i++){
        result = result + input[i];
    }
    return result;
}

QString Disassembler::convertBinToD(QString BinString){
    bool ok;
    int Bin;
    QString resultstring;
    Bin = BinString.toInt(&ok, 2);
    resultstring = QString::number(Bin, 10);
    return resultstring;
}

QString Disassembler::convertBinToHex(QString BinString){
    bool ok;
    int Bin;
    QString resultstring;
    Bin = BinString.toInt(&ok, 2);
    resultstring = QString::number(Bin, 16);
    qDebug() << resultstring;
    return resultstring;
}

QStringList Disassembler::Addn(QString &stringNeedCompile)
{
    QChar sep = '\n';
    QStringList tokens = stringNeedCompile.split(sep);
    return tokens;
}

QString Disassembler::getRawDisassembledString(QStringList &rawDisassembledStringList)
{
    rawDisassembledString = rawDisassembledStringList.join("\n");
    return rawDisassembledString;
}

QString Disassembler::validInstructionToMipsCode(QString currentLine)
{
    QString MipsCode;
    QString SubString;
    if (currentLine.isEmpty()){
        return "";
    }
    SubString = GetSubString(currentLine, 0, 6);
    if(KeyOp.find(SubString) != KeyOp.end()){
        MipsCode = KeyOp[SubString];
        if(STI_Instruction.contains(KeyOp[SubString])){
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 11, 16)];
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 6, 11)];
            MipsCode = MipsCode + " " + convertBinToD(GetSubString(currentLine, 16, 32)) + ";";
        }
        else if(STII_Instruction.contains(KeyOp[SubString])){
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 11, 16)];
            MipsCode = MipsCode + " " + convertBinToD(GetSubString(currentLine, 16, 32)) + "(";
            MipsCode = MipsCode + Reg[GetSubString(currentLine, 6, 11)] + ")" +";";
        }
        else if(J_Instruction.contains(KeyOp[SubString])){
            MipsCode = MipsCode + " " + convertBinToHex(GetSubString(currentLine, 6, 32)) + ";";
        }
    }
    else{
        SubString = GetSubString(currentLine, 0, 6) + GetSubString(currentLine, 26, 32);
        MipsCode = KeyOp[SubString];
        if(STD_Instruction.contains(KeyOp[SubString])){
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 16, 21)];
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 6, 11)];
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 11, 16)] +";";
        }

        else if(TDA_Instruction.contains(KeyOp[SubString])){
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 16, 21)];
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 11, 16)];
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 21, 26)] + ";";
        }
        else if(S_Instruction.contains(KeyOp[SubString])){
            MipsCode = MipsCode + " " + Reg[GetSubString(currentLine, 6, 11)] + ";";
        }
    }
    return MipsCode;
}
