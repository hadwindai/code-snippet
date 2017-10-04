package com.hadwin.ccb;

import java.io.StringWriter;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

public abstract class RequestXml {

	protected Document document = null;
	protected Element root = null;
	protected Element txInfo = null;

	public RequestXml() {
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = null;
		try {
			builder = factory.newDocumentBuilder();
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		}
		document = builder.newDocument();
		document.setXmlStandalone(true);
		root = document.createElement("TX");
		document.appendChild(root);
	}

	protected Element appendChild(Element parent, String tag) {
		return appendChild(parent, tag, null);
	}

	protected Element appendChild(Element parent, String tag, String text) {
		Element child = document.createElement(tag);
		if (text != null) {
			child.setTextContent(text);
		}
		parent.appendChild(child);
		return child;
	}

	protected String getCustId() {
		return "XXXXXXXXXXXXXXX";
	}

	protected String getUserId() {
		return "WLPT01";
	}

	protected String getPassword() {
		return "XXXXX";
	}

	protected abstract String getRequestSn();

	protected abstract String getTxCode();

	public void execute() {
		appendChild(root, "REQUEST_SN", getRequestSn());
		appendChild(root, "CUST_ID", getCustId());
		appendChild(root, "USER_ID", getUserId());
		appendChild(root, "PASSWORD", getPassword());
		appendChild(root, "TX_CODE", getTxCode());
		appendChild(root, "LANGUAGE", "CN");
		txInfo = appendChild(root, "TX_INFO");
	}

	public String toString() {
		TransformerFactory tf = TransformerFactory.newInstance();
		Transformer transformer = null;
		try {
			transformer = tf.newTransformer();
		} catch (TransformerConfigurationException e) {
			e.printStackTrace();
		}
		DOMSource source = new DOMSource(document);
		transformer.setOutputProperty(OutputKeys.ENCODING, "gb2312");
		transformer.setOutputProperty(OutputKeys.STANDALONE, "yes");
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");

		StringWriter sw = new StringWriter();
		StreamResult result = new StreamResult(sw);
		try {
			transformer.transform(source, result);
		} catch (TransformerException e) {
			e.printStackTrace();
		}
		return sw.toString();
	}

}
