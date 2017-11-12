#include<fstream>
#include<iostream>
#include<string>
#include<cassert>
#include<algorithm>
#include<map>
#include<vector>
using namespace std;

map<string, int> regs;
vector<vector<string>> res;
map<string, int> label;

int startAddress;
int pass = 1;
int isData;
/*struct variable {
	int type;
	int *value;
	string valuestr;
};*/
struct ARRAY {
	int address;
	int arr[32];
};
struct INT {
	int address;
	int value;
};
struct STRING {
	int address;
	string value;
};
map<string, INT> varint;
map<string, ARRAY>vararray;
map<string, STRING> varstr;
void  initRegs()
{
	regs["zero"] = 0;
	regs["at"] = 1;
	regs["gp"] = 28;
	regs["sp"] = 29;
	regs["fp"] = 30;
	regs["s8"] = 30;
	regs["ra"] = 31;
	for (int i = 0; i < 2; i++)
	{
		regs["v" + to_string(i)] = i + 2;
		regs["k" + to_string(i)] = i + 26;
	}	
	for (int i = 0; i < 4; i++)
		regs["a" + to_string(i)] = i + 4;
	for (int i = 0; i < 8; i++)
	{
		regs["t" + to_string(i)] = i + 8;
		regs["s" + to_string(i)] = i + 16;
	}
}
vector<string> split(const string &s, const string &seperator) {//根据特定字符对string进行分割
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}
int transferNumber(string s)
{
	if (s.find("0x") == string::npos)//十进制
	{
		return atoi(s.c_str());
	}
	else//十六进制
	{
		int count = s.length();
		int sum = 0;
		for (int i = count - 1; i >= 2; i--)//从十六进制个位开始，每位都转换成十进制  
		{
			if (s[i] >= '0'&&s[i] <= '9')//数字字符的转换  
			{
				sum += (s[i] - 48)*pow(16, count - i - 1);
			}
			else if (s[i] >= 'A'&&s[i] <= 'F')//字母字符的转换  
			{
				sum += (s[i] - 55)*pow(16, count - i - 1);
			}
			else if (s[i] >= 'a'&&s[i] <= 'f')
			{
				sum+= (s[i] - 87)*pow(16, count - i - 1);
			}
		}
		return sum;
	}
}
void readFile(string file)
{
	ifstream ifile;
	int proAddress=0;
	int proIntAddress=0;
	int dataAddress = 0;
	ifile.open(file.data());
	assert(ifile.is_open());
	string code;
	string sep = ", \n\t$()";
	while (getline(ifile, code))
	{
		vector<string> temp=split(code, sep);
		vector<string>::iterator it;
		if (temp[0] == ".data")
		{
			isData = 1;
			if (temp.size() > 1)
			{
				dataAddress = transferNumber(temp[1]);
			}
			continue;
		}
		if (temp[0] == ".text")
		{
			isData = 0;
			if (temp.size() > 1)
			{
				startAddress = transferNumber(temp[1]);
				proAddress = startAddress;
			}
			continue;
		}	
		if (isData == 1)//数据段处理
		{
			if (find(temp.begin(), temp.end(), ".word") != temp.end())
			{
				if (temp.size()==3)//single
				{
					temp[0] = temp[0].substr(0, temp[0].size() - 1);
					INT var;
					var.address = dataAddress;
					var.value = transferNumber(temp[2]);
					varint[temp[0]] = var;
					dataAddress += 4;
				}
				else//array
				{
					ARRAY ary;
					ary.address = dataAddress;
					for (int i = 0; i < temp.size() - 2; i++)
					{
						ary.arr[i]= transferNumber(temp[2+i]);
						dataAddress += 4;
					}
					temp[0] = temp[0].substr(0, temp[0].size() - 1);
					vararray[temp[0]] = ary;
				}
			}
			else if(find(temp.begin(), temp.end(), ".asciiz") != temp.end())
			{
				STRING str;
				str.value = temp[2];
				str.address = dataAddress;
				dataAddress += 4;
				temp[0] = temp[0].substr(0, temp[0].size() - 1);
				varstr[temp[0]] = str;
			}
		}
		else//代码段处理
		{
			//判断label
			if (temp[0][temp[0].size() - 1] == ':')
			{
				temp[0]=temp[0].substr(0, temp[0].size() - 1);
				label[temp[0]] = proAddress;
				if (temp.size() > 1)
				{
					temp.erase(temp.begin());
					res.push_back(temp);
				}
				proAddress += 4;
			}
			else if (temp[0]== "EXC_BASE")//中断处理程序
			{
				res.push_back(temp);
				proIntAddress += 4;
			}
			else
			{
				res.push_back(temp);
				proAddress += 4;
			}
		}
	}
}

vector<vector<string>> processCode(vector<vector<string>> input)
{
	vector<vector<string>> output;
	vector<string> temp;
	for (int i = 0; i < res.size(); i++)
	{
		for (int j = 0; j < res[i].size(); j++)
		{
			if (label.count(res[i][j]) > 0)
				temp.push_back(to_string(label[input[i][j]]));
			else if (regs.count(res[i][j]) > 0)
				temp.push_back(to_string(regs[input[i][j]]));
			else if (varint.count(res[i][j]) > 0)
				temp.push_back(to_string(varint[input[i][j]].address));
			else if (vararray.count(res[i][j]) > 0)
				temp.push_back(to_string(vararray[input[i][j]].address));
			else if (varstr.count(res[i][j]) > 0)
				temp.push_back(to_string(varstr[input[i][j]].address));
			else
				temp.push_back(input[i][j]);
		}
		output.push_back(temp);
		temp.clear();
	}
	return output;
}

int main()
{
	initRegs();
	readFile("test.asm");
	//cout << label.size();
	map<string, int>::iterator iter;
	/*for (iter = label.begin(); iter != label.end(); iter++)
	{
		cout << iter->first.size()<< iter->second<<endl;

	}*/
	
	vector<vector<string>> res1=processCode(res);
	for (int i = 0; i < res1.size(); i++)
	{
		for (int j = 0; j < res1[i].size(); j++)
			cout << res1[i][j]<<" ";
		cout << endl;
	}
}