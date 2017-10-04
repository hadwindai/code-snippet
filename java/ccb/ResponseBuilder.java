package com.hadwin.ccb;

import java.io.FileNotFoundException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

public class ResponseBuilder {

	public static Response6W1303Xml build6W1303(String xmlString) {
		Response6W1303Xml xml = new Response6W1303Xml(xmlString);
		return xml;
	}

	public static void main(String[] args) throws ParserConfigurationException, FileNotFoundException, TransformerException {
		String xml = "<?xml version=\"1.0\" encoding=\"GB18030\" ?>"
				+ "<TX>"
				+ "<REQUEST_SN>xxxxxxxxxxx</REQUEST_SN>"
				+ "<CUST_ID>xxxxxxxxxxxxx</CUST_ID>"
				+ "<TX_CODE>6W1303</TX_CODE>"
				+ "<RETURN_CODE>xxxxxxxx</RETURN_CODE>"
				+ "<RETURN_MSG>帐户户名校验失败</RETURN_MSG>"
				+ "<LANGUAGE>CN</LANGUAGE>"
				+ "<TX_INFO>"
				+ "  <CREDIT_NO>xxxxxxxxx</CREDIT_NO>"
				+ "  <INDIVIDUAL_NAME1></INDIVIDUAL_NAME1>"
				+ "  <INDIVIDUAL1></INDIVIDUAL1>"
				+ "  <INDIVIDUAL_NAME2></INDIVIDUAL_NAME2>"
				+ "  <INDIVIDUAL2></INDIVIDUAL2>"
				+ "  <REM1>REM1</REM1>"
				+ "  <REM2>REM2</REM2>"
				+ "</TX_INFO>"
				+ "</TX> ";
		Response6W1303Xml response = ResponseBuilder.build6W1303(xml);

		System.out.println("请求序列号: " + response.getRequestSn());
		System.out.println("响应码: " + response.getReturnCode());
		System.out.println("响应信息: " + response.getReturnMsg());
		System.out.println("凭证号: " + response.getCreditNo());
	}

}
