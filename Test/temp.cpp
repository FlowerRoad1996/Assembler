#include<iostream>
#include<string>
#include<map>
#include<fstream>
#include<algorithm>
#include<cstdlib>
#include<vector>
using namespace std;


/*char* parse_token(char* s1, char* s2,char *delim,char **out_char)
{
	int len;
	char *ptr, *tptr, *token;
	len = strspn(s1, s2);
	ptr = s1 + len;
	tptr = strpbrk(ptr, s2);
	if (tptr == NULL)
		return NULL;
	len = tptr - ptr;
	if (delim != NULL)
		*delim = *tptr;
	*out_char = tptr + 1;
	token = (char *)malloc(len + 1);
	if (token == NULL)
		return NULL;
	memcpy(token, ptr, len);
	token[len] = (char) 0;
	return token;
}*/
#include <vector>
#include <string>
#include <iostream>
using namespace std;

vector<string> split(const string &s, const string &seperator) {
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
				sum += (s[i] - 87)*pow(16, count - i - 1);
			}
		}
		return sum;
	}
}
int main()
{
	/*string s = "addi $t0,$t0,4 ";
	char* p = const_cast<char*>(s.c_str());
	char *intoken;
	int j = 0;
	char intchar[20][40] = { 0 };
	while (1) {
		intoken = parse_token(p, " \n\t$,", NULL, &p);
		if (intoken == NULL || *intoken == '#') {
			if (intoken != NULL)
				free(intoken);
			break;
		}
		strcpy(intchar[j], intoken);
		j++;
	}
	for (int i = 0; i < j+1; i++)
		cout << intchar[i]<<endl;*/
	
	/*string code = "lw $v0, buf($t0)";
	for (int i = 0; i < code.size(); i++)
	{
		vector<string> temp;
		size_t end = code.find(' ');
		string s = code.substr(0, end);
		temp.push_back(s);
		while (end < code.size())
		{
			while (code[end] == ' ' || code[end] == '$'||code[end] == ','|| code[end]=='('||code[end]==')')
				end++;
			//end--;
 			size_t start = end;
			while (code[end] != ','&&code[end] != '$'&&code[end]!=' '&&end<code.size()&& code[end] != '(' && code[end] != ')')
				end++;
			string str = code.substr(start, end - start);;
			if(end!=start)
				temp.push_back(str);
		}
	}*/

	/*string a = "addi $t0,$t0,4 ";
	string b = " $,#";
	vector<string> v=split(a,b);
	for (int i = 0; i < v.size(); i++)
		cout << v[i]<<endl;*/
	cout << transferNumber("0x10000");
}


