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

    STD_Instruction << "add" << "addu" << "and"
                    << "nor" << "or" << "sub"
                    << "subu" << "xor" << "slt"
                    << "sltu";
    TDA_Instruction << "sll" << "srl";
    S_Instruction << "jr";
    STI_Instruction << "addi" << "addiu" << "andi"
                    << "ori" << "xori" << "slti"
                    << "sltiu" << "beq" << "bne";
    STII_Instruction << "sw" << "lw";
    J_Instruction << "j" << "jal";
}

QString Disassembler::Discompile(QString &CompileString)
{
    bool ok;
    QString validthisline;
    MipsresultList.clear();
    QStringList listOfCompileString = Addn(CompileString);
    QListIterator<QString> itr(listOfCompileString);
    while (itr.hasNext())
    {
        QString thisLine = itr.next();
        QChar commentBegin = '#';
        int commentIndex = thisLine.indexOf(commentBegin, 0);

        validthisline = thisLine.left(commentIndex);
        validthisline = QString("%1").arg(validthisline.toLongLong(&ok, 16), 32, 2, QChar('0'));

        QString disassembledLine = InstructionToMipsCode(validthisline);
        if (!disassembledLine.isEmpty())
        {
            MipsresultList.append(disassembledLine);
        }
    }
    return Join_e(MipsresultList);
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
    return resultstring;
}

QStringList Disassembler::Addn(QString &CompileString)
{
    QChar sep = '\n';
    QStringList my_stringlist = CompileString.split(sep);
    return my_stringlist;
}

QString Disassembler::Join_e(QStringList &DisassembledStringList)
{
    DisassembledString = DisassembledStringList.join("\n");
    return DisassembledString;
}

int Disassembler::Cases_contained(QString& Op){
    int Case = -1;
    if(STD_Instruction.contains(Op)){
        Case = 0;
    }
    else if(TDA_Instruction.contains(Op)){
        Case = 1;
    }
    else if(S_Instruction.contains(Op)){
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

QString Disassembler::InstructionToMipsCode(QString thisLine)
{
    int Case;
    QString MipsCode;
    QString SubString;
    if (thisLine.isEmpty()){
        return "";
    }
    SubString = GetSubString(thisLine, 0, 6);
    if(KeyOp.find(SubString) != KeyOp.end()){
        MipsCode = KeyOp[SubString];
        Case = Cases_contained(MipsCode);
        switch(Case)
        {
            case 3:
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 11, 16)];
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 6, 11)];
                MipsCode = MipsCode + " " + convertBinToD(GetSubString(thisLine, 16, 32)) + ";";
                break;
            case 4:
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 11, 16)];
                MipsCode = MipsCode + " " + convertBinToD(GetSubString(thisLine, 16, 32)) + "(";
                MipsCode = MipsCode + Reg[GetSubString(thisLine, 6, 11)] + ")" +";";
                break;
            case 5:
                MipsCode = MipsCode + " " + convertBinToHex(GetSubString(thisLine, 6, 32)) + ";";
                break;
        }
    }
    else{
        SubString = GetSubString(thisLine, 0, 6) + GetSubString(thisLine, 26, 32);
        MipsCode = KeyOp[SubString];
        Case = Cases_contained(MipsCode);
        switch(Case)
        {
            case 0:
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 16, 21)];
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 6, 11)];
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 11, 16)] +";";
                break;
            case 1:
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 16, 21)];
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 11, 16)];
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 21, 26)] + ";";
                break;
            case 2:
                MipsCode = MipsCode + " " + Reg[GetSubString(thisLine, 6, 11)] + ";";
                break;
        }
    }
    return MipsCode;
}
