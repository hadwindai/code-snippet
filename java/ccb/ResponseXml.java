package com.hadwin.ccb;

import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

public abstract class ResponseXml {

	protected Document document = null;
	protected Element root = null;
	protected Element txInfo = null;

	public ResponseXml(String xml) {
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		try {
			DocumentBuilder builder = factory.newDocumentBuilder();
			Reader reader = new StringReader(xml);
			InputSource source = new InputSource(reader);
			document = builder.parse(source);
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (document != null) {
			root = document.getDocumentElement();
		}
		if (root != null) {
			NodeList notes = root.getElementsByTagName("TX_INFO");
			if (notes.getLength() > 0) {
				txInfo = (Element) notes.item(0);
			}
		}
	}

	protected String getElementTextByTag(Element element, String name) {
		if (element == null) {
			return null;
		}
		NodeList notes = element.getElementsByTagName(name);
		if (notes.getLength() == 0) {
			return null;
		}
		return notes.item(0).getTextContent();
	}

	public String getRequestSn() {
		return getElementTextByTag(root, "REQUEST_SN");
	}

	public String getCustId() {
		return getElementTextByTag(root, "CUST_ID");
	}

	public String getReturnCode() {
		return getElementTextByTag(root, "RETURN_CODE");
	}

	public String getReturnMsg() {
		return getElementTextByTag(root, "RETURN_MSG");
	}

	public String getTxCode() {
		return getElementTextByTag(root, "TX_CODE");
	}

	public String getLanguage() {
		return getElementTextByTag(root, "LANGUAGE");
	}

}
