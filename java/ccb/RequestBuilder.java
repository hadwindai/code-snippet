package com.hadwin.ccb;

import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

public class RequestBuilder {

	public static String build6W1303(String requestSn, String accNo2, String otherName, String amount) {
		Request6W1303Xml xml = new Request6W1303Xml();
		xml.setRequestSn(requestSn);
		xml.setAccNo2(accNo2);
		xml.setOtherName(otherName);
		xml.setAmount(amount);
		xml.execute();
		return xml.toString();
	}

	public static void main(String[] args) throws ParserConfigurationException, FileNotFoundException, TransformerException, UnsupportedEncodingException {
		String input = RequestBuilder.build6W1303("xxxxxxxx", "xxxxxxxxxxxxxxxxxx", "XX", "10.00");
		System.out.println(input);
		String input2 = URLEncoder.encode(input, "GB18030");
		System.out.println(input2);

	}

}
