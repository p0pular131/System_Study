#!/usr/bin/env python3

import sys

'''
opcode 기준 정리
0110111 -> lui (U)
0010111 -> auipc (U)
1101111 -> jal (J)
1100111 -> jalr (I)
1100011 -> beq, bne, blt, bge, bltu, bgeu (B)
0000011 -> lb, lh, lw, lbu, lhu (I)
0100011 -> sb, sh, sw (S) 
0010011 -> addi, slti, sltiu, xori, ori, andi (I)         
            slli, srli, srai (R)
0110011 -> add, sub, sll, slt, sltu, xor, srl, sra, or, and (R)
===================================================================
U format : 
    lui, auipc ( funct3 존재 x )
J format : 
    jal ( funct3 존재 x )
I format : 
    jalr, lb, lh, lw, lbu, lhu, addi, slti, sltiu, xori, ori, andi
R foramt : 
    slli, slri, srai, add, sub, sll, slt, sltu, xor, srl, sra, or, and
S format :
    sb, sh, sw
B format :
    beq, bne, blt, bge, bltu, bgeu

'''
OP_CODE = {
    0b0110111 : "U",
    0b0010111 : "U",
    0b1101111 : "J",
    0b1100111 : "I",
    0b1100011 : "B",
    0b0000011 : "I",
    0b0100011 : "S",
    0b0010011 : "IR",
    0b0110011 : "R"
}
# 각 format에서 funct 기준으로 함수 이름 분류
B = {
    0x0063 : "beq",
    0x1063 : "bne",
    0x4063 : "blt",
    0x5063 : "bge",
    0x6063 : "bltu",
    0x7063 : "bgeu"
}
S = {
    0x0023 : "sb",
    0x1023 : "sh",
    0x2023 : "sw"
}
I = {
    0x0067 : "jalr",
    0x0003 : "lb",
    0x1003 : "lh",
    0x2003 : "lw",
    0x4003 : "lbu",
    0x5003 : "lhu",
    0x0013 : "addi",
    0x2013 : "slti",
    0x3013 : "sltiu",
    0x4013 : "xori",
    0x6013 : "ori",
    0x7013 : "andi"
}
R = {
    0x1013 : "slli",
    0x5013 : "srli_srai",
    0x0033 : "add_sub", 
    0x1033 : "sll",
    0x2033 : "slt",
    0x3033 : "sltu",
    0x4033 : "xor",
    0x5033 : "srl_sra",
    0x6033 : "or",
    0x7033 : "and"
}

# addi, slti, sltiu, xori, ori, andi   I
# slli, srli, srai   R
IR = {
    0x0013 : "addi",
    0x2013 : "slti",
    0x3013 : "sltiu",
    0x4013 : "xori",
    0x6013 : "ori",
    0x7013 : "andi",
    0x1013 : "slli",
    0x5013 : "srli_srai"
}
FUNCT_3 = [0,1,2,3,4,5,6,7]
regi_info = [0 for _ in range(32)]
disas_list = []

N = int(sys.argv[-1])

class RISCV :
    def __init__(self) :
        self.bin_num = 0
        self.inst_num = 0
        self.func = None
        self.format = None
        self.op_code = 0
        self.funct3 = 0
        self.imm = 0
        self.rs1 = 0
        self.rs2 = 0
        self.rd = 0

def make_bit(arr_input) :
    # 32비트 숫자 생성
    result = 0
    for i in range(len(arr_input)) :
        result += arr_input[i] << 8*(i)
    return result

def make_inst_num(input) :
    result = hex(input)[2:]
    while(len(result)<=8) :
        result = "0" + result
    return result

def bit_convert_32to12(input) :
    result = 0
    input = bin(input)[2:]
    while(len(input)<12) :
        input = "0" + input
    temp = 1
    for i in range(len(input)) :
        result += temp*int(input[11-i])
        temp *= 2
        if(i==10) : temp *= -1
    return result

def bit_convert_32to13(input) :
    result = 0
    input = bin(input)[2:]
    while(len(input)<13) :
        input = "0" + input
    temp = 1
    for i in range(len(input)) :
        result += temp*int(input[12-i])
        temp *= 2
        if(i==11) : temp *= -1
    return result

def bit_convert_32to21(input) :
    result = 0
    input = bin(input)[2:]
    while(len(input)<21) :
        input = "0" + input
    temp = 1
    for i in range(len(input)) :
        result += temp*int(input[20-i])
        temp *= 2
        if(i==19) : temp *= -1
    return result

def bit_2compliment(input) :
    result = 0
    input = bin(input)[2:]
    while(len(input)<32) :
        input = "0" + input
    temp = 1
    for i in range(len(input)) :
        result += temp*int(input[31-i])
        temp *= 2
        if(i==30) : temp *= -1
    return result

# 일단 먼저 disassemble한 후에 n만큼 execution 진행
# disassemble 한 정보들은 클래스 형태로 리스트에 저장
def main() :
    path = sys.argv[1]
    with open(path,'rb') as file :
        data = file.read()
        # 4개 단위씩 끊어서 진행
        for i in range(len(data)//4) :
            curr = RISCV()
            bytes = data[4*i:4*(i+1)]
            curr.bin_num = make_bit(bytes)
            curr.inst_num = make_inst_num(curr.bin_num)
            curr.op_code = curr.bin_num & 0b1111111
            curr.format = OP_CODE[curr.op_code]
            # funct3가 없는 U,J형식과 아닌 형식 먼저 분리
            if(curr.format=="U" or curr.format=="J") :
                if(curr.op_code == 0b0110111) :
                    curr.func = "lui"
                elif(curr.op_code == 0b0010111) :
                    curr.func = "auipc"
                elif(curr.op_code == 0b1101111) :
                    curr.func = "jal"
                if(curr.func == "lui" or curr.func == "auipc") :
                    curr.rd = (curr.bin_num >> 7) & 0x1F
                    curr.imm = ((curr.bin_num >> 12) & 0xFFFFF) << 12
                    curr.imm = bit_2compliment(curr.imm)
                    print(f"inst {i}: {curr.inst_num} {curr.func} x{curr.rd}, {curr.imm}")
                elif(curr.func == "jal") :
                    curr.rd = (curr.bin_num >> 7) & 0x1F
                    curr.imm = (curr.bin_num & 0x7FE00000) >> 21
                    curr.imm <<= 1
                    curr.imm += (curr.bin_num & 0x100000) >> 9
                    curr.imm += (curr.bin_num & 0xFF000)
                    curr.imm += (curr.bin_num & 0x80000000) >> 11
                    curr.imm = bit_convert_32to21(curr.imm)
                    print(f"inst {i}: {curr.inst_num} {curr.func} x{curr.rd}, {curr.imm}")
            else :
                curr.funct3 = curr.bin_num & 0x707F

                if(curr.format == "IR") :
                    if(curr.funct3 == 0x1013 or curr.funct3 == 0x5013) :
                        curr.format = "R"
                    else :
                        curr.format = "I"

                if(curr.format=="B") :
                    curr.func = B[curr.funct3]
                    curr.rs2 = (curr.bin_num >> 20) & 0x1F
                    curr.rs1 = (curr.bin_num >> 15) & 0x1F
                    curr.imm = (curr.bin_num & 0xF00) >> 8
                    curr.imm <<= 1 # 마지막 0 추가
                    curr.imm += (curr.bin_num & 0x7E000000) >> 20
                    curr.imm += (curr.bin_num & 0x80) << 4
                    curr.imm += (curr.bin_num & 0x80000000) >> 19
                    curr.imm = bit_convert_32to13(curr.imm)
                    print(f"inst {i}: {curr.inst_num} {curr.func} x{curr.rs1}, x{curr.rs2}, {curr.imm}")

                elif(curr.format=="I") :
                    curr.func = I[curr.funct3]
                    if(len(curr.func) > 6) : #funct7을 바탕으로 분류
                        funct7 = (curr.bin_num >> 25) & 0b1111111
                        a,b = curr.func.split("_")
                        if(funct7 == 0b0000000) :
                            curr.func = a
                        else :
                            curr.func = b
                    curr.imm = (curr.bin_num & 0xFFF00000) >> 20
                    curr.imm = bit_convert_32to12(curr.imm)
                    curr.rs1 = (curr.bin_num >> 15) & 0x1F
                    curr.rd = (curr.bin_num >> 7) & 0x1F
                    if(curr.op_code == 0b11 or curr.func == "jalr") :
                        print(f"inst {i}: {curr.inst_num} {curr.func} x{curr.rd}, {curr.imm}(x{curr.rs1})")
                    else :
                        print(f"inst {i}: {curr.inst_num} {curr.func} x{curr.rd}, x{curr.rs1}, {curr.imm}")

                elif(curr.format=="S") :
                    curr.func = S[curr.funct3]
                    curr.rs2 = (curr.bin_num >> 20) & 0x1F
                    curr.rs1 = (curr.bin_num >> 15) & 0x1F
                    curr.imm = (curr.bin_num & 0xF80) >> 7
                    curr.imm += (curr.bin_num & 0xFE000000) >> 20
                    curr.imm = bit_convert_32to12(curr.imm)
                    print(f"inst {i}: {curr.inst_num} {curr.func} x{curr.rs2}, {curr.imm}(x{curr.rs1})")

                elif(curr.format=="R") :
                    curr.func = R[curr.funct3]
                    if(len(curr.func) > 6) : #funct7을 바탕으로 분류
                        funct7 = (curr.bin_num >> 25) & 0b1111111
                        a,b = curr.func.split("_")
                        if(funct7 == 0b0000000) :
                            curr.func = a
                        else :
                            curr.func = b
                    curr.rs2 = (curr.bin_num >> 20) & 0x1F
                    curr.rs1 = (curr.bin_num >> 15) & 0x1F
                    curr.rd = (curr.bin_num >> 7) & 0x1F
                    if(curr.func[-1]=='i') :
                        print(f"inst {i}: {curr.inst_num} {curr.func} x{curr.rd}, x{curr.rs1}, {curr.rs2}")
                    else :
                        print(f"inst {i}: {curr.inst_num} {curr.func} x{curr.rd}, x{curr.rs1}, x{curr.rs2}")
            disas_list.append(curr)


def int_to_hex_32bit(n):
    # 변환을 위해 32비트 마스크 적용
    mask = 0xFFFFFFFF
    # 입력값을 32비트로 변환
    hex_value = n & mask
    # 16진수 형식으로 변환하여 출력
    list_hex = hex(hex_value)
    print(list_hex[:2],end='')
    for _ in range(10-len(list_hex)) :
        print('0',end="")
    print(list_hex[2:])



def pregi() :
    for i in range(32) :
        print(f"x{i}: ",end="")
        int_to_hex_32bit(regi_info[i])

if __name__ == "__main__" :
    main()
    for i in range(len(disas_list)) :
        if i >= N :
            break
        inst = disas_list[i]
        # 입력 받은 N만큼만 연산 진행
        # unknown inst면 pass
        if inst == 0 :
            continue
        regi_info[0] = 0
        if inst.func == "add" :
            regi_info[inst.rd] = regi_info[inst.rs1] + regi_info[inst.rs2]
        elif inst.func == "sub" :
            regi_info[inst.rd] = regi_info[inst.rs1] - regi_info[inst.rs2]
        elif inst.func == "addi" :
            regi_info[inst.rd] = regi_info[inst.rs1] + inst.imm
        elif inst.func == "xor" :
            regi_info[inst.rd] = regi_info[inst.rs1] ^ regi_info[inst.rs2]
        elif inst.func == "or" :
            regi_info[inst.rd] = regi_info[inst.rs1] | regi_info[inst.rs2]
        elif inst.func == "and" :
            regi_info[inst.rd] = regi_info[inst.rs1] & regi_info[inst.rs2]
        elif inst.func == "xori" :
            regi_info[inst.rd] = regi_info[inst.rs1] ^ inst.imm
        elif inst.func == "ori" :
            regi_info[inst.rd] = regi_info[inst.rs1] | inst.imm
        elif inst.func == "andi" :
            regi_info[inst.rd] = regi_info[inst.rs1] & inst.imm
        elif inst.func == "slli" :
            regi_info[inst.rd] = regi_info[inst.rs1] << inst.rs2
        elif inst.func == "srli" :
            mask = 0xFFFFFFFF
            
            regi_info[inst.rd] = regi_info[inst.rs1] >> inst.rs2
        elif inst.func == "srai" :
            regi_info[inst.rd] = regi_info[inst.rs1] >> inst.rs2
        elif inst.func == "sll" : 
            regi_info[inst.rs2] = regi_info[inst.rs2] & 0b11111
            regi_info[inst.rd] = regi_info[inst.rs1] << regi_info[inst.rs2]
        elif inst.func == "srl" : # logical은 unsigned shift
            mask = 0xFFFFFFFF
            regi_info[inst.rs2] = regi_info[inst.rs2] & 0b11111
            regi_info[inst.rd] = (regi_info[inst.rs1] & mask) >> regi_info[inst.rs2]
        elif inst.func == "sra" :
            regi_info[inst.rs2] = regi_info[inst.rs2] & 0b11111
            regi_info[inst.rd] = regi_info[inst.rs1] >> regi_info[inst.rs2]
        elif inst.func == "slti" :
            if(regi_info[inst.rs1]<inst.imm) :
                regi_info[inst.rd] = 1
            else :
                regi_info[inst.rd] = 0
        elif inst.func == "slt" :
            if(regi_info[inst.rs1]<regi_info[inst.rs2]) :
                regi_info[inst.rd] = 1
            else :
                regi_info[inst.rd] = 0
        elif inst.func == "lui" :
            regi_info[inst.rd] += inst.imm
        # elif inst.func == "auipc" :

        # elif inst.func == "jalr" :

        # elif inst.func == "beq" :

        # elif inst.func == "bne" :

        # elif inst.func == "blt" :

        # elif inst.func == "bge" :

        # elif inst.func == "lw" :

        # elif inst.func == "sw" :

        regi_info[0] = 0
    pregi()
