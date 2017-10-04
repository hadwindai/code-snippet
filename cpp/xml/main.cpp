#include <iostream>
#include <stdio.h>
#include <vector>

#include "Config.h"
#include "Create.h"
#include "Component.h"
#include "XMLUtils.h"

int main(int argc, char *argv[]){

	std::vector<Config*> configs;
	processStart("E:\\vc\\xml\\start.xml", &configs);

	for(std::vector<Config*>::iterator it = configs.begin(); it != configs.end(); ++it){
		std::cout << "\n====================================" << std::endl;
		std::cout << "NAME: " << (*it)->getName() << std::endl;
		std::cout << "PATH: " << (*it)->getPath() << std::endl;
		std::cout << "NUMBER: " << (*it)->getNumber() << std::endl;
		std::cout << "TEMPLET: " << (*it)->getTemplet() << std::endl;
		std::vector<std::string> opens = (*it)->getOpens();
		for(std::vector<std::string>::iterator open_it = opens.begin(); open_it != opens.end(); ++open_it){
			std::cout << "OPEN: " << *open_it << std::endl;
		}
		std::vector<Create*> creates = (*it)->getCreates();
		for(std::vector<Create*>::iterator create_it = creates.begin(); create_it != creates.end(); ++create_it){
			std::cout << "Create: " << (*create_it)->getFile() << "\t -TEMPLET " << (*create_it)->getTemplet() << std::endl;
		}
		std::vector<Component*> assembles = (*it)->getAssembles();
		for(std::vector<Component*>::iterator assemble_it = assembles.begin(); assemble_it != assembles.end(); ++assemble_it){
			std::cout << "Assemble: " << (*assemble_it)->getFile();
			std::vector<std::string> children = (*assemble_it)->getChildren();
			for(std::vector<std::string>::iterator children_it = children.begin(); children_it != children.end(); ++children_it){
				std::cout << "\n\tChild: " << (*children_it);
			}
			std::cout << std::endl;
		}
		std::vector<Component*> imports = (*it)->getImports();
		for(std::vector<Component*>::iterator import_it = imports.begin(); import_it != imports.end(); ++import_it){
			std::cout << "Import: " << (*import_it)->getFile();
			std::vector<std::string> children = (*import_it)->getChildren();
			for(std::vector<std::string>::iterator children_it = children.begin(); children_it != children.end(); ++children_it){
				std::cout << "\n\tChild: " << (*children_it);
			}
			std::cout << std::endl;
		}
	}
	releaseConfigs(&configs);
	return 0;
}
