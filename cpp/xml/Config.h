
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <string>
#include <vector>

class Create;
class Component;

class Config{

public:
	Config(std::string name);
	virtual ~Config();

	std::string getName() const;

	std::string getNumber() const;
	void setNumber(std::string number);

	std::string getPath() const;
	void setPath(std::string path);

	std::string getTemplet() const;
	void setTemplet(std::string templet);

	std::vector<std::string> getOpens() const;
	void addOpen(std::string open);

	std::vector<Create*> getCreates() const;
	void addCreate(std::string file);
	void addCreate(std::string file, std::string templet);

	std::vector<Component*> getAssembles() const;
	Component* addAssemble(std::string file);

	std::vector<Component*> getImports() const;
	Component* addImport(std::string file);
	
private:
	std::string name;
	std::string number;
	std::string path;
	std::string templet;
	
	std::vector<std::string> opens;
	std::vector<Create*> creates;
	std::vector<Component*> assembles;
	std::vector<Component*> imports;

};

#endif
