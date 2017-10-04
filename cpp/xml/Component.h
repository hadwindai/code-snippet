
#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

#include <string>
#include <vector>

class Component{

public:
	Component(std::string file);
	virtual ~Component();
	void addChild(std::string child);
	std::string getFile() const;
	std::vector<std::string> getChildren() const;

private:
	std::string file;
	std::vector<std::string> children;
};

#endif