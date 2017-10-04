package com.hadwin.ccb;

public class Request6W1303Xml extends RequestXml {

	private String requestSn;
	private String accNo2;
	private String otherName;
	private String amount;

	public Request6W1303Xml() {
	}

	@Override
	protected String getRequestSn() {
		return requestSn;
	}

	@Override
	protected String getTxCode() {
		return "6W1303";
	}

	public void setRequestSn(String requestSn) {
		this.requestSn = requestSn;
	}

	public void setAccNo2(String accNo2) {
		this.accNo2 = accNo2;
	}

	public void setOtherName(String otherName) {
		this.otherName = otherName;
	}

	public void setAmount(String amount) {
		this.amount = amount;
	}

	public void execute() {
		super.execute();
		appendChild(txInfo, "ACC_NO1", "xxxxxxxxxxxxxxxxxx");
		appendChild(txInfo, "BILL_CODE", "xxxxxxxxxxxxxxxxxx");
		appendChild(txInfo, "ACC_NO2", accNo2);
		appendChild(txInfo, "OTHER_NAME", otherName);
		appendChild(txInfo, "AMOUNT", amount);
		appendChild(txInfo, "USEOF_CODE", "xxxxxxx");
		appendChild(txInfo, "FLOW_FLAG", "0");
		appendChild(txInfo, "REM1", "REM1");
		appendChild(txInfo, "REM2", "REM2");
	}

}
