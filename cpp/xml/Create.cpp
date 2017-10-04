#include <string>
#include <vector>

#include "Create.h"

Create::Create(std::string file){
	this->file = file;
}

Create::Create(std::string file, std::string templet){
	this->file = file;
	this->templet = templet;
}

Create::~Create(){
}

std::string Create::getFile() const{
	return file;
}

std::string Create::getTemplet()const{
	return templet;
}

bool Create::hasTemplet()const{
	return (templet.size() > 0);
}
