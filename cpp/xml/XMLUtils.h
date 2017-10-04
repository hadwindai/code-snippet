
#ifndef XML_UTILS_H_INCLUDED
#define XML_UTILS_H_INCLUDED

#include <vector>

class Config;
class Component;

namespace XERCES_CPP_NAMESPACE{
	class DOMElement;
}

void processStart(const char *fileName, std::vector<Config*> *configs);

void processConfigs(std::vector<Config*> *configs, XERCES_CPP_NAMESPACE::DOMElement *root);

void processAction(Config *config, XERCES_CPP_NAMESPACE::DOMElement *configElement);

void processChildren(Component *component, XERCES_CPP_NAMESPACE::DOMElement *childrenElement);

void releaseConfigs(std::vector<Config*> *configs);

#endif



