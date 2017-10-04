#include <string>
#include <vector>

#include "Create.h"
#include "Component.h"
#include "Config.h"

Config::Config(std::string name){
	this->name = name;
}

Config::~Config(){
	for(std::vector<Create*>::iterator it = creates.begin(); it != creates.end(); ++it){ delete *it;}
	for(std::vector<Component*>::iterator it = assembles.begin(); it != assembles.end(); ++it){ delete *it;}
	for(std::vector<Component*>::iterator it = imports.begin(); it != imports.end(); ++it){ delete *it;}
}

std::string Config::getName() const{
	return name;
}

std::string Config::getNumber() const{
	return number;
}

void Config::setNumber(std::string number){
	this->number = number;
}

std::string Config::getPath() const{
	return path;
}

void Config::setPath(std::string path){
	this->path = path;
}

std::string Config::getTemplet() const{
	return templet;
}

void Config::setTemplet(std::string templet){
	this->templet = templet;
}

std::vector<std::string> Config::getOpens() const{
	return opens;
}

void Config::addOpen(std::string open){
	this->opens.push_back(open);
}

std::vector<Create*> Config::getCreates() const{
	return creates;
}

void Config::addCreate(std::string file){
	addCreate(file, "");
}

void Config::addCreate(std::string file, std::string templet){
	Create *create = new Create(file, templet);
	this->creates.push_back(create);
}

std::vector<Component*> Config::getAssembles() const{
	return assembles;
}

Component* Config::addAssemble(std::string file){
	Component *assemble = new Component(file);
	assembles.push_back(assemble);
	return assemble;
}

std::vector<Component*> Config::getImports() const{
	return imports;
}

Component* Config::addImport(std::string file){
	Component *import = new Component(file);
	imports.push_back(import);
	return import;
}

