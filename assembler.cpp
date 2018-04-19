#include "assembler.h"

Assembler::Assembler()
{
    keywordOpCode = QMap<QString, QString>({
        {"add",     "000000ssssstttttddddd00000100000"},//R-STD
        {"addu",    "000000ssssstttttddddd00000100001"},//R-STD
        {"addi",    "001000ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"addiu",   "001001ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"and",     "000000ssssstttttddddd00000100100"},//R-STD
        {"andi",    "001100ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"div",     "000000sssssttttt0000000000011010"},//R-ST
        {"divu",    "000000sssssttttt0000000000011011"},//R-ST
        {"mult",    "000000sssssttttt0000000000011000"},//R-ST
        {"multu",   "000000sssssttttt0000000000011001"},//R-ST
        {"nor",     "000000ssssstttttddddd00000100111"},//R-STD
        {"or",      "000000ssssstttttddddd00000100101"},//R-STD
        {"ori",     "001101ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"sll",     "00000000000tttttdddddaaaaa000000"},//R-TDA
        {"sllv",    "000000ssssstttttddddd00000000100"},//R-STD
        {"sra",     "00000000000tttttdddddaaaaa000011"},//R-TDA
        {"srav",    "000000ssssstttttddddd00000000111"},//R-STD
        {"srl",     "00000000000tttttdddddaaaaa000010"},//R-TDA
        {"srlv",    "000000ssssstttttddddd00000000110"},//R-STD
        {"sub",     "000000ssssstttttddddd00000100010"},//R-STD
        {"subu",    "000000ssssstttttddddd00000100011"},//R-STD
        {"xor",     "000000ssssstttttddddd00000100110"},//R-STD
        {"xori",    "001110ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"lhi",     "01100100000tttttiiiiiiiiiiiiiiii"},//I-STI
        {"llo",     "01100000000tttttiiiiiiiiiiiiiiii"},//I-STI
        {"slt",     "000000ssssstttttddddd00000101010"},//R-STD
        {"sltu",    "000000ssssstttttddddd00000101011"},//R-STD
        {"slti",    "001010ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"sltiu",   "001001ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"beq",     "000100ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"bgtz",    "000111sssss00000iiiiiiiiiiiiiiii"},//I-SI
        {"blez",    "000110sssss00000iiiiiiiiiiiiiiii"},//I-SI
        {"bne",     "000101ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"j",       "000010iiiiiiiiiiiiiiiiiiiiiiiiii"},//J-I
        {"jal",     "000011iiiiiiiiiiiiiiiiiiiiiiiiii"},//J-I
        {"jalr",    "000000sssss00000ddddd00000001001"},//R-SD
        {"jr",      "000000sssss000000000000000001000"},//R-S
        {"lb",      "100000ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"lbu",     "100100ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"lh",      "100001ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"lhu",     "100101ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"lw",      "100011ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"sb",      "101000ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"sh",      "101001ssssstttttiiiiiiiiiiiiiiii"},//I-STI
        {"sw",      "101011ssssstttttiiiiiiiiiiiiiiii"},//I-STI
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
    R_STD_InstructionPatterns << "add"
                              << "addu"
                              << "and"
                              << "nor"
                              << "or"
                              << "sllv"
                              << "srav"
                              << "srlv"
                              << "sub"
                              << "subu"
                              << "xor"
                              << "slt"
                              << "sltu";
    R_TDA_InstructionPatterns << "sll"
                              << "sra"
                              << "srl";
    R_SD__InstructionPatterns << "jalr";
    R_S___InstructionPatterns << "jr";
    R_ST__InstructionPatterns << "div"
                              << "divu"
                              << "mult"
                              << "multu";
    I_STI_InstructionPatterns << "addi"
                              << "addiu"
                              << "andi"
                              << "ori"
                              << "xori"
                              << "lhi"
                              << "llo"
                              << "slti"
                              << "sltiu"
                              << "beq"
                              << "bne";
    I_STII_InstructionPatterns<< "sw"
                              << "lw";
    I_SI__InstructionPatterns << "bgtz"
                              << "blez";
    J_InstructionPatterns << "j"
                          << "jal";
}

QString Assembler::Compile(QString &stringNeedCompile)
{
    resultList.clear();
    QStringList listOfStringNeedCompile = Lexer(stringNeedCompile);
    QListIterator<QString> itr(listOfStringNeedCompile);
    while (itr.hasNext())
    {
        QString currentLine  = itr.next();
        QChar commentBegin = '#';
        int commentIndex = currentLine.indexOf(commentBegin, 0);
        QString assembledLine = validInstructionToMachineCode(currentLine.left(commentIndex));
        if (!assembledLine.isEmpty())
        {
            resultList.append(assembledLine);
        }
        // qDebug() << resultList;
    }
    return getRawAssembledString(resultList);
}

QStringList Assembler::Lexer(QString &stringNeedCompile)
{
    QChar sep = '\n';
    QStringList tokens = stringNeedCompile.split(sep);
    qDebug() << tokens;
    return tokens;
}

QString Assembler::getRawAssembledString(QStringList &rawAssembledStringList)
{
    rawAssembledString = rawAssembledStringList.join("\n");
    return rawAssembledString;
}

QString Assembler::validInstructionToMachineCode(QString currentLine)
{
    if (currentLine.isEmpty())
        return "";
    currentLine.replace("(", " ");
    currentLine.replace(")", " ");
    currentLine.replace(";", "");
    QString machineCode;
    QStringList tokens = currentLine.split(QRegExp("(\\s|,)"), QString::SkipEmptyParts);
    if(tokens.size()>4)
    {
        qDebug() << "Token size error!";
    }
    qDebug() << tokens;
    machineCode = keywordOpCode[tokens.at(0)];
    if(R_STD_InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "R_STD";
        convert_Reg_D(tokens.at(1), machineCode);
        convert_Reg_S(tokens.at(2), machineCode);
        convert_Reg_T(tokens.at(3), machineCode);
    }
    else if(R_TDA_InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "R_TDA";
        convert_Reg_D(tokens.at(1), machineCode);
        convert_Reg_T(tokens.at(2), machineCode);
        convert_Reg_A(tokens.at(3), machineCode);
    }
    else if(R_ST__InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "R_ST";
        convert_Reg_S(tokens.at(1), machineCode);
        convert_Reg_T(tokens.at(2), machineCode);
    }
    else if(R_SD__InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "R_SD";
        convert_Reg_D(tokens.at(1), machineCode);
        convert_Reg_S(tokens.at(2), machineCode);
    }
    else if(R_S___InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "R_S";
        convert_Reg_S(tokens.at(1), machineCode);
    }
    else if(I_STII_InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "II_STI";
        convert_Reg_T(tokens.at(3), machineCode);
        convert_Reg_S(tokens.at(1), machineCode);
        convert_Immediate(tokens.at(2), machineCode);
    }
    else if(I_STI_InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "I_STI";
        convert_Reg_T(tokens.at(2), machineCode);
        convert_Reg_S(tokens.at(1), machineCode);
        convert_Immediate(tokens.at(3), machineCode);
    }
    else if(I_SI__InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "I_SI";
        convert_Reg_S(tokens.at(1), machineCode);
        convert_Immediate(tokens.at(2), machineCode);
    }
    else if(J_InstructionPatterns.contains(tokens.at(0)))
    {
        qDebug() << "J";
        convert_Target(tokens.at(1), machineCode);
    }
    return machineCode;
}


void Assembler::convert_Reg_S(const QString& rs, QString& machineCodeNeedConvert)
{
    QMap<QString,QString>::iterator it = Register.find(rs);
    QString machineCodeOfRegS = QString(it->data());
    machineCodeNeedConvert.replace(6, 5, machineCodeOfRegS);
}

void Assembler::convert_Reg_T(const QString& rt, QString& machineCodeNeedConvert)
{
    QMap<QString,QString>::iterator it = Register.find(rt);
    QString machineCodeOfRegT = QString(it->data());
    machineCodeNeedConvert.replace(11, 5, machineCodeOfRegT);
}

void Assembler::convert_Reg_D(const QString& rd, QString& machineCodeNeedConvert)
{
    QMap<QString,QString>::iterator it = Register.find(rd);
    QString machineCodeOfRegD = QString(it->data());
    machineCodeNeedConvert.replace(16, 5, machineCodeOfRegD);
}

void Assembler::convert_Reg_A(const QString& sa, QString& machineCodeNeedConvert)
{
    QMap<QString,QString>::iterator it = Register.find(sa);
    QString machineCodeOfRegA = QString(it->data());
    machineCodeNeedConvert.replace(21, 5, machineCodeOfRegA);
}

void Assembler::convert_Immediate(const QString& imm, QString& machineCodeNeedConvert)
{
    QString machineCodeOfImmediate = QString("%1").arg(imm.toInt(), 16, 2, QChar('0'));
    machineCodeNeedConvert.replace(16, 16, machineCodeOfImmediate);
}

void Assembler::convert_Target(const QString& target, QString& machineCodeNeedConvert)
{
    bool ok;
    QString machineCodeOfTarget = QString("%1").arg(target.toInt(&ok, 16), 26, 2, QChar('0'));
    machineCodeNeedConvert.replace(6, 26, machineCodeOfTarget);
}
