#include "Component.h"

Component::Component(std::string file){
	this->file = file;
}

Component::~Component(){
}

void Component::addChild(std::string child){
	this->children.push_back(child);
}

std::string Component::getFile() const{
	return file;
}

std::vector<std::string> Component::getChildren() const{
	return children;
}
