#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#define is_operator(c) (c == '+' || c == '-' || c == '/' || c == '*')

#define is_number(c) ((c >= '0' && c <= '9') || c == '.')

int op_preced(const char c){
	switch(c){
		case '*':
		case '/':
			return 2;
		case '+':
		case '-':
			return 1;
	}
	return 0;
}

double calc(double val1, double val2, const char op){
	switch(op){
		case '*':
			return (val1 * val2);
		case '/':
			return (val1 / val2);
		case '+':
			return (val1 + val2);
		case '-':
			return (val1 - val2);
	}
	return 0;
}

bool shunting_yard(const std::string &input, std::vector<std::string> &output){
	std::vector<char> stack;
	std::string temp;
	bool isDouble = false;
	char c;
	char chr;
	for(std::string::const_iterator it = input.begin(); it != input.end(); ++it){
		c = *it;
		if(c == ' '){
			continue;
		}else if(is_number(c)){
			if(c == '.'){
				if(!isDouble){
					isDouble = true;
				}else{
					std::cout << "ERROR: double value" << std::endl;
					return false;
				}
			}
			temp.push_back(c);
		}else if(is_operator(c)){
			if(!temp.empty()){
				output.push_back(temp);
				temp.clear();
				isDouble = false;
			}
			while(!stack.empty()){
				chr = stack.back();
				if(is_operator(chr) && (op_preced(c) <= op_preced(chr))){
					output.push_back(std::string(1, chr));
					stack.pop_back();
				}else{
					break;
				}
			}
			stack.push_back(c);
		}else if(c == '('){
			if(!temp.empty()){
				output.push_back(temp);
				temp.clear();
				isDouble = false;
			}
			stack.push_back(c);
		}else if(c == ')'){
			if(!temp.empty()){
				output.push_back(temp);
				temp.clear();
				isDouble = false;
			}
			bool pe = false;
			while(!stack.empty()){
				chr = stack.back();
				stack.pop_back();
				if(chr == '('){
					pe = true;
					break;
				}else{
					output.push_back(std::string(1, chr));
				}
			}
			if(!pe){
				std::cout << "Error: parentheses mismatched." << std::endl;
				return false;
			}
		}else{
			std::cout << "Unknown token: " << c << std::endl;
			return false;
		}
	}
	if(!temp.empty()){
		output.push_back(temp);
		temp.clear();
		isDouble = false;
	}
	while(!stack.empty()){
		chr = stack.back();
		stack.pop_back();
		if(chr == '(' || chr == ')'){
			std::cout << "Error: parentheses mismatched." << std::endl;
			return false;
		}
		output.push_back(std::string(1, chr));
	}
	return true;
}

bool execution_order(const std::vector<std::string> &input, double &result) {
	std::vector<double> stack;
	std::string temp;
	double val1;
	double val2;
	for(std::vector<std::string>::const_iterator it = input.begin(); it != input.end(); ++it){
		temp = *it;
		if(is_number(temp.at(0))){
			stack.push_back(atof(temp.data()));
		}else if(is_operator(temp.at(0))){
			if(stack.size() < 2) {
				std::cout << "ERROR operation quantity" << std::endl;
				return false;
			}
			val2 = stack.back();
			stack.pop_back();
			val1 = stack.back();
			stack.pop_back();
			stack.push_back(calc(val1, val2, temp.at(0)));
			std::cout << val1 << temp << val2  << "=" << stack.back() << std::endl;
		}
	}
	if(stack.size() == 1) {
		result = stack.at(0);
		std::cout << "Result: " << result << std::endl;
		return true;
	}
	return false;
}

bool calc_result(const std::string &expression, double &result){
	std::vector<std::string> vector;
	if(shunting_yard(expression, vector)){
		for(std::vector<std::string>::const_iterator it = vector.begin(); it != vector.end(); ++it){
			std::cout << (*it).data() << " ";
		}
		std::cout << std::endl;
		if(execution_order(vector, result)){
			return true;
		}
	}
	return false;
}

int main(int argc, char *argv[]){
	std::string input;
	double result = 0.0;
	while(true){
		input.clear();
		result = 0.0;
		std::cout << "please input your expression: " << std::endl;
		std::cin >> input;
		calc_result(input, result);
	}
	return 0;
}
