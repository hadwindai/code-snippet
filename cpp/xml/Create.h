
#ifndef CREATE_H_INCLUDED
#define CREATE_H_INCLUDED

#include <string>
#include <vector>

class Create{

public:
	Create(std::string file);
	Create(std::string file, std::string templet);
	virtual ~Create();
	std::string getFile() const;
	std::string getTemplet() const;
	bool hasTemplet() const;

private:
	std::string file;
	std::string templet;
};

#endif
