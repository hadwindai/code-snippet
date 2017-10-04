#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLException.hpp>

#include "Config.h"
#include "Create.h"
#include "Component.h"
#include "XMLUtils.h"

class StrX{
	private :
		char *fLocalForm;

	public:
		StrX(const XMLCh* const toTranscode){
			fLocalForm = XERCES_CPP_NAMESPACE::XMLString::transcode(toTranscode);
		}
		~StrX(){
			XERCES_CPP_NAMESPACE::XMLString::release(&fLocalForm);
		} 
		const char* localForm()const{
			return fLocalForm;
		}
};

inline std::ostream& operator<<(std::ostream &target, const StrX &toDump){
	target << toDump.localForm();
	return target;
}

class StrC{
	private :
		XMLCh *fLocalForm;

	public:
		StrC(const char *toTranscode){
			fLocalForm = XERCES_CPP_NAMESPACE::XMLString::transcode(toTranscode);
		}
		~StrC(){
			XERCES_CPP_NAMESPACE::XMLString::release(&fLocalForm);
		} 
		const XMLCh* localForm()const{
			return fLocalForm;
		}
};

void processStart(const char *fileName, std::vector<Config*> *configs){
	try{
		XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize();
	}catch(const XERCES_CPP_NAMESPACE::XMLException &e){
		throw std::exception(StrX(e.getMessage()).localForm());
	}
	XERCES_CPP_NAMESPACE::XercesDOMParser *parser = new XERCES_CPP_NAMESPACE::XercesDOMParser();
	parser->setValidationScheme(XERCES_CPP_NAMESPACE::XercesDOMParser::Val_Never);
	parser->setDoNamespaces(false);
	parser->setDoSchema(false);
	parser->setHandleMultipleImports (false);
	parser->setValidationSchemaFullChecking(false);
	parser->setCreateEntityReferenceNodes(true);
	parser->parse(fileName);
	XERCES_CPP_NAMESPACE::DOMElement* root = parser->getDocument()->getDocumentElement();
	processConfigs(configs, root);
	delete parser;
	XERCES_CPP_NAMESPACE::XMLPlatformUtils::Terminate();
}

void processConfigs(std::vector<Config*> *configs, XERCES_CPP_NAMESPACE::DOMElement *root){
	XERCES_CPP_NAMESPACE::DOMNodeList *list = root->getChildNodes();
	for(XMLSize_t i = 0; i < list->getLength(); ++i){
		XERCES_CPP_NAMESPACE::DOMNode *child = list->item(i);
		if(child->getNodeType() == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE){
			XERCES_CPP_NAMESPACE::DOMElement *element = static_cast<XERCES_CPP_NAMESPACE::DOMElement*>(child);
			Config *config = new Config(std::string(StrX( element->getAttribute(StrC("name").localForm())).localForm()));
			config->setPath(std::string(StrX(element->getAttribute(StrC("path").localForm())).localForm()));
			config->setNumber(std::string(StrX(element->getAttribute(StrC("number").localForm())).localForm()));
			config->setTemplet(std::string(StrX(element->getAttribute(StrC("template").localForm())).localForm()));
			processAction(config, element);
			configs->push_back(config);
		}
	}
}

void processAction(Config *config, XERCES_CPP_NAMESPACE::DOMElement *configElement){
	XERCES_CPP_NAMESPACE::DOMNodeList *list = configElement->getChildNodes();
	for(XMLSize_t i = 0; i < list->getLength(); ++i){
		XERCES_CPP_NAMESPACE::DOMNode *child = list->item(i);
		if(child->getNodeType() == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE){
			XERCES_CPP_NAMESPACE::DOMElement *element = static_cast<XERCES_CPP_NAMESPACE::DOMElement*>(child);
			const XMLCh *action = element->getNodeName();
			if(XERCES_CPP_NAMESPACE::XMLString::equals(action, StrC("create").localForm())){
				config->addCreate(std::string(StrX(element->getAttribute(StrC("file").localForm())).localForm()), std::string(StrX(element->getAttribute(StrC("template").localForm())).localForm()));
			}
			if(XERCES_CPP_NAMESPACE::XMLString::equals(action, StrC("open").localForm())){
				config->addOpen(std::string(StrX(element->getAttribute(StrC("file").localForm())).localForm()));
			}
			if(XERCES_CPP_NAMESPACE::XMLString::equals(action, StrC("assemble").localForm())){
				Component *assemble = config->addAssemble(std::string(StrX(element->getAttribute(StrC("file").localForm())).localForm()));
				processChildren(assemble, element);
			}
			if(XERCES_CPP_NAMESPACE::XMLString::equals(action, StrC("import").localForm())){
				Component *import = config->addImport(std::string(StrX(element->getAttribute(StrC("file").localForm())).localForm()));
				processChildren(import, element);
			}
		}
	}
}

void processChildren(Component *component, XERCES_CPP_NAMESPACE::DOMElement *childrenElement){
	XERCES_CPP_NAMESPACE::DOMNodeList *list = childrenElement->getChildNodes();
	for(XMLSize_t i = 0; i < list->getLength(); ++i){
		XERCES_CPP_NAMESPACE::DOMNode *child = list->item(i);
		if(child->getNodeType() == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE){
			XERCES_CPP_NAMESPACE::DOMElement *element = static_cast<XERCES_CPP_NAMESPACE::DOMElement*>(child);
			component->addChild(std::string(StrX(element->getAttribute(StrC("file").localForm())).localForm()));
		}
	}
}

void releaseConfigs(std::vector<Config*> *configs){
	for(std::vector<Config*>::iterator it = configs->begin(); it != configs->end(); ++it){
		delete *it;
	}
}
