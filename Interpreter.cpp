#include <bits/stdc++.h>
//#include "debug.h"
#define debug(...) 0
using namespace std;
#define int long long

map<string,string> variables;

set<string> keywords = {
	"mov",
	"mul",
	"add",
	"sub",
	"div",
	"cmp",
	"jmp",
	"lvl",
	"cin",
	"out",
	"ret",
	"rem"
};

string strip(string s)
{
	deque<char> a(s.begin(), s.end());
	while (a.front() == ' ' || a.front() == '\n') a.pop_front();
	while (a.back() == ' ' || a.back() == '\n') a.pop_back();
	s = "";
	for (auto it : a) {
		s += it;
	}
	return s;
}

bool is_valid_keyword(string cmd)
{
	return (keywords.find(cmd) != keywords.end());
}

string prettify(string s)
{
	string res;
	for (auto it : s) {
		if (it == ' ' || it == '\n') continue;
		res += it;
	}
	return res;
}

bool is_variable(string s)
{
	if (s.front() != '_' && !isalpha(s.front())) return false;
	for (auto it : s) {
		if (it == '_' || isalpha(it)) continue;
		return false;
	}
	return true;
}

bool is_string(string s)
{
	return (s.front() == '\"' && s.back() == '\"');
}

bool is_integer(string s)
{
	for (auto it : s) {
		if (isdigit(it) == false) return false;
	}
	return true;
}

vector<string> decode(string line)
{
	vector<string> tokens;
	tokens.push_back(line.substr(0, 3));

	assert(is_valid_keyword(tokens.back()));

	// debug(line);
	if (line == "ret") return tokens;

	string second_part = strip(line.substr(3, line.size()));
	if (!is_string(second_part)) second_part = prettify(second_part);

	if (tokens.back() == "jmp" || tokens.back() == "lvl" || tokens.back() == "cin" || tokens.back() == "out") {
		if (second_part.back() == ':') second_part.pop_back();
		tokens.push_back(second_part);
	}
	else {
		string first, second, third;
		int i = 0;
		while (i < second_part.size()) {
			if (second_part[i] == '>') break;
			if (second_part[i] == '<') break;
			if (second_part[i] == ',') break;
			if (second_part[i] == '=') break;
			first += second_part[i];
			i++;
		}

		second += second_part[i++];
		if (second_part[i] == '=') second += second_part[i++];

		assert(i < second_part.size());
		// debug(second, second_part);
		third = second_part.substr(i, second_part.size());
		// debug(third);
		tokens.push_back(first);
		// if (second[0] == '>' || second[0] == '<')
		tokens.push_back(second);
		tokens.push_back(third);
	}
	return tokens;
}

void mov_action(vector<string> instraction)
{
	assert(false == is_integer(instraction[1]));
	assert(false == is_string(instraction[1]));

	if (is_variable(instraction[3])) {
		variables[instraction[1]] = variables[instraction[3]];
	}
	else {
		variables[instraction[1]] = instraction[3];
	}
}

void mul_action(vector<string> instraction)
{
	assert(false == is_string(instraction[1]));
	assert(false == is_string(instraction[3]));

	if (is_variable(instraction[3])) {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) * atoll(variables[instraction[3]].c_str()));
	}
	else {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) * atoll(instraction[3].c_str()));
	}
}

void add_action(vector<string> instraction)
{
	assert(false == is_string(instraction[1]));
	assert(false == is_string(instraction[3]));
	// debug(instraction);
	if (is_variable(instraction[3])) {
		// debug(variables[instraction[1]]);
		// debug(variables[instraction[3]]);
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) + atoll(variables[instraction[3]].c_str()));
	}
	else {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) + atoll(instraction[3].c_str()));
	}
}

void sub_action(vector<string> instraction)
{
	assert(false == is_string(instraction[1]));
	assert(false == is_string(instraction[3]));

	if (is_variable(instraction[3])) {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) - atoll(variables[instraction[3]].c_str()));
	}
	else {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) - atoll(instraction[3].c_str()));
	}
}

void div_action(vector<string> instraction)
{
	assert(is_variable(instraction[1]));
	assert(false == is_string(instraction[3]));

	if (is_variable(instraction[3])) {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) / atoll(variables[instraction[3]].c_str()));
	}
	else {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) / atoll(instraction[3].c_str()));
	}
}

void rem_action(vector<string> instraction)
{
	assert(is_variable(instraction[1]));
	assert(false == is_string(instraction[3]));

	if (is_variable(instraction[3])) {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) % atoll(variables[instraction[3]].c_str()));
	}
	else {
		variables[instraction[1]] = to_string(atoll(variables[instraction[1]].c_str()) % atoll(instraction[3].c_str()));
	}
}

bool cmp_action(vector<string> instraction)
{
	debug(instraction);
	string first = (is_variable(instraction[1]) ? variables[instraction[1]] : instraction[1]);
	string second = (is_variable(instraction[3]) ? variables[instraction[3]] : instraction[3]);
    debug(first, second);

    if (is_integer(first) && is_integer(second)) {
        int f = atoll(first.c_str());
        int s = atoll(second.c_str());
        if(instraction[2] == ">") {
            return f > s;
        }
        else if (instraction[2] == ">=") {
            return f >= s;
        }
        else if (instraction[2] == "<") {
            return f < s;
        }
        else if (instraction[2] == "<=") {
            return f <= s;
        }
        else {
            return f == s;
        }
    }

	if(instraction[2] == ">") {
		return first > second;
	}
	else if (instraction[2] == ">=") {
		return first >= second;
	}
	else if (instraction[2] == "<") {
		return first < second;
	}
	else if (instraction[2] == "<=") {
		return first <= second;
	}
	else {
        return first == second;
	}
}

void jmp_action(vector<string> ins, vector<string> program, int &ip)
{
	assert(ins[0] == "jmp");
	debug(ins);
	for (int i = 0; i < program.size(); i++) {
		auto tmp_ins = decode(program[i]);
		if (tmp_ins[0] == "lvl" && tmp_ins[1] == ins[1]) {
			ip = i;
			return;
		}
	}
}

void output_action(vector<string> ins)
{
    //debug(ins);
    if (ins[1] == "NEW_LINE") {
        cout << endl;
        return;
    }
    deque<char> ans;
	if (is_variable(ins[1])) {
		ans = deque<char>(variables[ins[1]].begin(), variables[ins[1]].end());
	}
	else {
		ans = deque<char>(ins[1].begin(), ins[1].end());
	}
	if (ans.back() == '\"') ans.pop_back();
	if (ans.front() == '\"') ans.pop_front();
	for (auto it : ans) {
        cout << it;
	}
}

void input_action(vector<string> ins)
{
    //debug(ins[1]);
    //debug(is_variable(ins[1]));
	assert(is_variable(ins[1]));
	getline(cin, variables[ins[1]]);
	variables[ins[1]] = strip(variables[ins[1]]);
}

signed main(int argc, char *argv)
{
	ifstream inf("program.txt");

	vector<string> program;

	string input;

	while (getline(inf, input)) {
        //debug(input);
        if (input.size() == 0) continue;
        program.push_back(strip(input));
		// debug(strip(input));
		if (program.size() && program.back() == "end") {
			program.pop_back();
			break;
		}
	}

	// debug(program);

	for (int ip = 0; ip < program.size(); ip++) {
		auto ins = decode(program[ip]);

		//debug(program[ip]);
		if (is_valid_keyword(ins[0]) == false) break;

		if (ins[0] == "mul") {
			mul_action(ins);
		}
		else if (ins[0] == "mov") {
			mov_action(ins);
		}
		else if (ins[0] == "add") {
			add_action(ins);
		}
		else if (ins[0] == "sub") {
			sub_action(ins);
		}
		else if (ins[0] == "div") {
			div_action(ins);
		}
		else if (ins[0] == "cmp") {
			ip++;
			if (cmp_action(ins)) {
				debug("success");
				jmp_action(decode(program[ip]), program, ip);
			}
			else {
                debug("fail");
			}
		}
		else if (ins[0] == "jmp") {
			jmp_action(decode(program[ip]), program, ip);
		}
		else if (ins[0] == "lvl") {
			//do_nothing;
		}
		else if (ins[0] == "cin") {
			input_action(ins);
		}
		else if (ins[0] == "out") {
			output_action(ins);
		}
		else if (ins[0] == "rem") {
            rem_action(ins);
		}
		else {
			exit(0);
		}
	}
}
