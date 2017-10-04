package com.hadwin.ccb;

public class Response6W1303Xml extends ResponseXml {

	public Response6W1303Xml(String xml) {
		super(xml);
	}

	public String getCreditNo() {
		return getElementTextByTag(txInfo, "CREDIT_NO");
	}

	public String getIndividualName1() {
		return getElementTextByTag(txInfo, "INDIVIDUAL_NAME1");
	}

	public String getIndividual1() {
		return getElementTextByTag(txInfo, "INDIVIDUAL1");
	}

	public String getIndividualName2() {
		return getElementTextByTag(txInfo, "INDIVIDUAL_NAME2");
	}

	public String getIndividual2() {
		return getElementTextByTag(txInfo, "INDIVIDUAL2");
	}

	public String getRem1() {
		return getElementTextByTag(txInfo, "REM1");
	}

	public String getRem2() {
		return getElementTextByTag(txInfo, "REM2");
	}

}
