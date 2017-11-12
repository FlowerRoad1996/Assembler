#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<fstream>
#include<string>
using namespace std;
map<string, string> func;
map<string, string> op;
string transferR1(vector<string> str);
string transferR2(vector<string> str);
vector<string> R1 = {
	"add","addu","sub","subu","and","or","xor","nor","slt","sltu",
	"sll","srl","sra","sllv","srlv","srav"
};
vector<string> R2 = { "mult","multu","div","divu" };
vector<string> R3 = { "mfhi","mflo" };
vector<string> R4 = { "mthi","mtlo" };
vector<string> R5 = { "mfc0","mtc0" };
vector<string> R6 = { "sll","srl","sra" };
vector<string> R7 = { "jr" };
vector<string> R8 = { "jalr" };
vector<string> R9 = { "break","syscall","eret" };

vector<string> I1 = {
	"addi","addiu","andi","ori","xori","slti","sltiu","beq","bne"
};
vector<string> I2 = {
	"lb","lbu","lh","lhu","sb","sh","lw","sw"
};
vector<string> I3 = {
	"bgez","bgtz","blez","bltz","bgezal","bltzal"
};
vector<string> I4 = {
	"lui"
};
vector<string> J1 = {
	"j","jal"
};
string turnBi(int n,int size)
{
	string s = "";
	for (int i = n; i; i = i / 2)
		s = s + (i % 2 ? '1' : '0');
	while (s.size()<size)
		s += '0';
	std::reverse(s.begin(), s.end());
	return s;
}
void initMap()
{
	func["add"] = "100000";
	func["addu"] = "100001";
	func["sub"] = "100010";
	func["subu"] = "100011";
	func["and"] = "100010";
	func["mult"] = "011000";
	func["multu"] = "011001";
	func["div"] = "011010";
	func["divu"] = "011011";
	func["mfhi"] = "010000";
	func["mflo"] = "010010";
	func["mthi"] = "010001";
	func["mtlo"] = "010011";
	func["or"] = "100101";
	func["xor"] = "100110";
	func["nor"] = "100111";
	func["slt"] = "101010";
	func["sltu"] = "101011";
	func["sll"] = "000000";
	func["srl"] = "000010";
	func["sra"] = "000011";
	func["sllv"] = "000100";
	func["srlv"] = "000110";
	func["srav"] = "000111";
	func["jr"] = "001000";
	func["jalr"] = "001001";
	func["break"] = "001101";
	func["syscall"] = "001100";
	func["eret"] = "011000";
	func["bgez"] = "00001";
	func["bgtz"] = "00000";
	func["blez"] = "00000";
	func["bltz"] = "00000";
	func["bgezal"] = "10001";
	func["bltzal"] = "10000";
}
void initOp()
{
	op["mfc0"] = "00000";
	op["mtc0"] = "00100";//区别在后5位
	op["eret"] = "010000";
	op["addi"] = "001000";
	op["addiu"] = "001001";
	op["andi"] = "001100";
	op["ori"] = "001101";
	op["xori"] = "001110";
	op["lui"] = "001111";
	op["lb"] = "100000";
	op["lbu"] = "100100";
	op["lh"] = "100001";
	op["lhu"] = "100101";
	op["sb"] = "101000";
	op["sh"] = "101001";
	op["lw"] = "100011";
	op["sw"] = "101011";
	op["beq"] = "000100";
	op["bne"] = "000101";
	op["bgez"] = "000001";
	op["bgtz"] = "000111";
	op["blez"] = "000110";
	op["bltz"] = "000001";
	op["bgezal"] = "000001";
	op["bltzal"] = "000001";//区别在rt5位
	op["slti"] = "001010";
	op["sltiu"] = "001011";
	op["j"] = "000010";
	op["jal"] = "000011";
}


string transferR1(vector<string> str)
{
	string res;
	for (int pos=0; pos < 6; pos++)
	{
		res+= '0';
	}
	res += turnBi(atoi(str[2].c_str()),5);
	res += turnBi(atoi(str[3].c_str()),5);
	res += turnBi(atoi(str[1].c_str()),5);
	for (int pos = 0; pos < 5; pos++)
	{
		res += '0';
	}
	res += func[str[0]];
	return res;
}
string transferR2(vector<string> str)
{
	string res;
	for (int pos = 0; pos < 6; pos++)
	{
		res += '0';
	}
	res += turnBi(atoi(str[1].c_str()),5);
	res += turnBi(atoi(str[2].c_str()),5);
	for (int pos = 0; pos < 10; pos++)
	{
		res += '0';
	}
	res += func[str[0]];
	return res;
}
string transferR3(vector<string> str)
{
	string res;
	for (int pos = 0; pos < 16; pos++)
	{
		res += '0';
	}
	res += turnBi(atoi(str[1].c_str()), 5);
	for (int pos = 0; pos <5; pos++)
	{
		res += '0';
	}
	res += func[str[0]];
	return res;
}
string transferR4(vector<string> str)
{
	string res;
	for (int pos = 0; pos < 6; pos++)
	{
		res += '0';
	}
	res += turnBi(atoi(str[1].c_str()), 5);
	for (int pos = 0; pos <15; pos++)
	{
		res += '0';
	}
	res += func[str[0]];
	return res;
}
string transferR5(vector<string> str)
{
	string res;
	res += "010000";
	res += op[str[0]];
	res += turnBi(atoi(str[1].c_str()), 5);
	res += turnBi(atoi(str[2].c_str()), 5);
	for (int pos = 0; pos <8; pos++)
	{
		res += '0';
	}
	res += turnBi(atoi(str[3].c_str()), 3);
	return res;
}
string transferR6(vector<string> str)
{
	string res;
	for (int pos = 0; pos <11; pos++)
	{
		res += '0';
	}
	res += turnBi(atoi(str[2].c_str()), 5);
	res += turnBi(atoi(str[1].c_str()), 5);
	res += turnBi(atoi(str[3].c_str()), 5);
	res += func[str[0]];
	return res;
}
string transferR7(vector<string> str)
{
	string res;
	for (int pos = 0; pos <6; pos++)
	{
		res += '0';
	}
	res += turnBi(atoi(str[1].c_str()), 5);
	for (int pos = 0; pos <15; pos++)
	{
		res += '0';
	}
	res += func[str[0]];
	return res;
}
string transferR8(vector<string> str)
{
	string res;
	for (int pos = 0; pos <6; pos++)
	{
		res += '0';
	}
	res += turnBi(atoi(str[2].c_str()), 5);
	for (int pos = 0; pos <5; pos++)
	{
		res += '0';
	}
	res += turnBi(atoi(str[1].c_str()), 5);
	res += func[str[0]];
	return res;
}

string transferI1(vector<string> str)
{
	string res;
	res += op[str[0]];
	res += turnBi(atoi(str[2].c_str()), 5);
	res += turnBi(atoi(str[1].c_str()), 5);
	res += turnBi(atoi(str[3].c_str()), 16);
	return res;
}

string transferI2(vector<string> str)
{
	string res;
	res += op[str[0]];
	res += turnBi(atoi(str[3].c_str()), 5);
	res += turnBi(atoi(str[1].c_str()), 5);
	res += turnBi(atoi(str[2].c_str()), 16);
	return res;
}
string transferI3(vector<string> str)
{
	string res;
	res += op[str[0]];
	res += turnBi(atoi(str[1].c_str()), 5);
	res += func[str[0]];
	res += turnBi(atoi(str[2].c_str()), 16);
	return res;
}
string transferI4(vector<string> str)
{
	string res;
	res += op[str[0]];
	res += turnBi(atoi(str[1].c_str()), 5);
	res += func[str[0]];
	res += turnBi(atoi(str[2].c_str()), 16);
	return res;
}
string transferJ1(vector<string> str)
{
	string res;
	res += op[str[0]];
	res += turnBi(atoi(str[1].c_str()), 26);
	return res; 
}
string transfer(vector<string> str)
{
	string insName = str[0];
	return transferR2(str);
}
/*int main()
{
	vector<string> instru = { "mult","1","2" };
	initMap();
	string res=transfer(instru);
	cout << res;
}*/