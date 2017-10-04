import org.apache.commons.io.FileUtils
import org.apache.commons.lang.RandomStringUtils
import org.apache.http.HttpEntity
import org.apache.http.client.methods.CloseableHttpResponse
import org.apache.http.client.methods.HttpGet
import org.apache.http.impl.client.CloseableHttpClient
import org.apache.http.impl.client.HttpClients
import org.apache.http.util.EntityUtils
import org.apache.ofbiz.base.lang.JSON
import org.apache.ofbiz.base.util.UtilValidate
import org.apache.ofbiz.entity.condition.EntityCondition
import org.apache.ofbiz.entity.condition.EntityOperator
import org.apache.ofbiz.entity.util.EntityQuery
import org.apache.ofbiz.service.ServiceUtil
import org.jsoup.Jsoup
import org.jsoup.nodes.Document
import org.jsoup.nodes.Element
import org.jsoup.select.Elements
import org.w3c.css.sac.CSSException
import org.w3c.css.sac.CSSParseException
import org.w3c.css.sac.ErrorHandler
import org.w3c.css.sac.InputSource
import org.w3c.dom.css.CSSRuleList
import org.w3c.dom.css.CSSStyleDeclaration
import org.w3c.dom.css.CSSStyleRule
import org.w3c.dom.css.CSSStyleSheet

import com.steadystate.css.parser.CSSOMParser
import com.steadystate.css.parser.SACParserCSS3

import groovy.json.JsonSlurper

public Map<String, Object> crawlJDCategory(){
	CloseableHttpClient httpclient = HttpClients.createDefault();
	HttpGet httpGet = new HttpGet("https://dc.3.cn/category/get?callback=getCategoryCallback");
	httpGet.addHeader("user-agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36");
	CloseableHttpResponse response1 = httpclient.execute(httpGet);
	
	try {
		logInfo("StatusCode: " + response1.getStatusLine().getStatusCode());
		HttpEntity entity1 = response1.getEntity();
		String str = EntityUtils.toString(entity1, "GBK");
		str = str.substring("getCategoryCallback(".length(), str.length() - 1);
		def jsonSlurper = new JsonSlurper();
		def object = jsonSlurper.parseText(str)
		object.data.each{ item ->
			def firstId = storeJDCategory("CATEGORY_ROOT", item.s[0].n, 1);
			def list = item.s[0].s;
			list.each{ category ->
				def secondId = storeJDCategory(firstId, category.n, 2);
				def list2 = category.s;
				list2.each{ third ->
					storeJDCategory(secondId, third.n, 3);
				}
			}
		}
	} finally {
		response1.close();
		httpclient.close();
	}
	return ServiceUtil.returnSuccess();
}

private String storeJDCategory(String parentId, String cat, Long level){
	def res = cat.split("\\|");
	String name = res[1];
	String url = res[0];
	if(url.contains("jd.com")){
		if (url.indexOf("&") > 0){
			url = "http://" + url.substring(0, url.indexOf("&"));
		}else{
			url = "http://" + url;
		}
	}else{
		url = "http://list.jd.com/list.html?cat=" + url;
	}
	println "list: " + name + " " + url;
	def category = EntityQuery.use(delegator).from("ImportCategory").where(["url": url, "level": level]).queryFirst();
	if(UtilValidate.isEmpty(category)){
		category = delegator.makeValue("ImportCategory");
		def categoryId = delegator.getNextSeqId("ImportCategory");
		category.setString("categoryId", categoryId);
		category.setString("name", name);
		category.setString("url", url);
		category.setString("parentId", parentId);
		category.set("level", level);
		category.setString("isVisit", "N");
		category.create();
		return categoryId;
	}else{
		category.setString("parentId", parentId);
		category.set("level", level);
		category.store();
		return category.categoryId;
	}
}

public Map<String, Object> crawlJDList(){
	def params = ["isVisit" : "N"];
	if(context.categoryId){
		params.categoryId = context.categoryId;
	}
	def list = EntityQuery.use(delegator).from("ImportCategory").where(params).queryList();
	CloseableHttpClient httpclient = HttpClients.createDefault();
	list.each{ category ->
		int page = 1;
		def url = category.url.trim();
		if(url.contains("?")){
			url += "&";
		}else{
			url += "?";
		}
		url += "delivery=1&delivery_daofu=3&page=";
		//if(!category.url.contains("?cat=")){
		//	return true;
		//}
		for(int i = 1; i<= page; i ++){
			def currentUrl = url + i;
			logInfo("Get URL: " + currentUrl);
			long count = EntityQuery.use(delegator).from("ImportTemp").where(["url" : currentUrl]).queryCount();
			if(count > 0){
				continue;
			}
			HttpGet httpGet = new HttpGet(currentUrl);
			httpGet.addHeader("user-agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36");
			CloseableHttpResponse response1 = httpclient.execute(httpGet);
			HttpEntity entity1 = response1.getEntity();
			String str = EntityUtils.toString(entity1);
			response1.close();
			
			Document doc = Jsoup.parse(str);
			String pageStr = doc.select(".p-skip em b").text();
			if(pageStr != null && pageStr.length() > 0){
				page = Integer.parseInt(pageStr);
				logInfo("New page: " + page);
			}
			
			Elements itemList = doc.select(".gl-item .j-sku-item");
			for (Element element : itemList) {
				def skuId = element.attr("data-sku");
				long productCount = EntityQuery.use(delegator).from("ImportProduct").where(["skuId" : skuId]).queryCount();
				if(productCount > 0){
					logInfo("Exist Product: skuId=" + skuId);
					continue;
				}
				def name = element.select(".p-name em").text();
				def productUrl = element.select(".p-name a").attr("href");
				def imageElement = element.select(".p-img img").last();
				def image = imageElement.attr("src");
				if(UtilValidate.isEmpty(image)){
					image = imageElement.attr("data-lazy-img");
				}
				def price = element.select(".p-price .J_price:eq(0) i").text();
				
				def product = delegator.makeValue("ImportProduct");
				product.setString("productId", delegator.getNextSeqId("ImportProduct"));
				product.setString("categoryId", category.categoryId);
				product.setString("skuId", skuId);
				product.setString("name", name);
				product.setString("url", productUrl);
				product.setString("price", price);
				product.setString("image", image);
				product.setString("isPrice", "N");
				product.setString("isDetail", "N");
				product.setString("isDownload", "N");
				product.create();
				
				logInfo("Store Product: skuId=" + skuId+ ", name=" + name + ", url=" + productUrl);
			}
			
			def temp = delegator.makeValue("ImportTemp");
			temp.setString("tempId", delegator.getNextSeqId("ImportTemp"));
			temp.setString("url", currentUrl);
			temp.create();
		}
		category.setString("isVisit", "Y");
		category.store();
	}
	httpclient.close();
	return ServiceUtil.returnSuccess();
}

public Map<String, Object> crawlJDPrice(){
	CloseableHttpClient httpclient = HttpClients.createDefault();
	crawlJDPriceInner(httpclient);
	httpclient.close();
	return ServiceUtil.returnSuccess();
}

private void crawlJDPriceInner(CloseableHttpClient httpclient){
	def params = ["isPrice" : "N"];
	if(context.categoryId){
		params.categoryId = context.categoryId;
	}
	def list = EntityQuery.use(delegator).from("ImportProduct").where(params).maxRows(50).queryList();
	if(UtilValidate.isEmpty(list)){
		logInfo("执行完成crawlJDPriceInner.");
		return;
	}
	def skuIds = "";
	list.each{ product ->
		if(UtilValidate.isNotEmpty(skuIds)){
			skuIds +=  ",";
		}
		skuIds += "J_" + product.skuId;
	}
	def pduid = RandomStringUtils.randomNumeric(22);
	def currentUrl = "http://p.3.cn/prices/mgets?pduid=" + pduid + "&skuIds=" + skuIds;
	logInfo("Get URL: " + currentUrl);
	long count = EntityQuery.use(delegator).from("ImportTemp").where(["url" : currentUrl]).queryCount();
	if(count > 0){
		return;
	}
	HttpGet httpGet = new HttpGet(currentUrl);
	httpGet.addHeader("user-agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36");
	CloseableHttpResponse response1 = httpclient.execute(httpGet);
	HttpEntity entity1 = response1.getEntity();
	String str = EntityUtils.toString(entity1);
	response1.close();
	
	logInfo("Get Result: " + str);
	
	def priceList = null;
	try{
		priceList = JSON.from(str).toObject(List.class);
	}catch(Exception e){
		logInfo(e.getMessage());
	}
	
	if(UtilValidate.isNotEmpty(priceList)){
		list.each{ product ->
			def skuId = "J_" + product.skuId;
			priceList.each{ price ->
				if(price.id == skuId){
					product.setString("price", price.p);
					product.setString("oprice", price.op);
					product.setString("mprice", price.m);
					product.setString("isPrice", "Y");
					product.store();
					return true;
				}
			}
		}
	}else{
		def priceMap = JSON.from(str).toObject(Map.class);
		if(priceMap.containsKey("error")){
			return;
		}
	}
	def temp = delegator.makeValue("ImportTemp");
	temp.setString("tempId", delegator.getNextSeqId("ImportTemp"));
	temp.setString("url", currentUrl);
	temp.create();
	
	crawlJDPriceInner(httpclient);
}

public Map<String, Object> crawlJDDetail(){
	CloseableHttpClient httpclient = HttpClients.createDefault();
	crawlJDDetailInner(httpclient);
	httpclient.close();
	return ServiceUtil.returnSuccess();
}

private void crawlJDDetailInner(CloseableHttpClient httpclient){
	def params = ["isDetail" : "N"];
	if(context.categoryId){
		params.categoryId = context.categoryId;
	}
	def list = EntityQuery.use(delegator).from("ImportProduct").where(params).maxRows(30).queryList();
	if(UtilValidate.isEmpty(list)){
		logInfo("执行完成crawlJDDetailInner.");
		return;
	}
	list.each{ product ->
		def currentUrl = "http:" + product.url;
		logInfo("Get URL: " + currentUrl);
		long count = EntityQuery.use(delegator).from("ImportTemp").where(["url" : currentUrl]).queryCount();
		if(count > 0){
			return true;
		}
		HttpGet httpGet = new HttpGet(currentUrl);
		httpGet.addHeader("user-agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36");
		CloseableHttpResponse response1 = httpclient.execute(httpGet);
		HttpEntity entity1 = response1.getEntity();
		String str = EntityUtils.toString(entity1);
		response1.close();
		
		Document doc = Jsoup.parse(str);
		Elements imageList = doc.select("#spec-list ul li img");
		if(UtilValidate.isNotEmpty(imageList)){
			def image1 = imageList[0].attr("data-url");
			if(UtilValidate.isNotEmpty(image1)){
				product.setString("image1", "//img14.360buyimg.com/n0/" + image1);
			}
			if(imageList.size() > 1){
				def image2 = imageList[1].attr("data-url");
				if(UtilValidate.isNotEmpty(image2)){
					product.setString("image2", "//img14.360buyimg.com/n0/" + image2);
				}
			}
			if(imageList.size() > 2){
				def image3 = imageList[2].attr("data-url");
				if(UtilValidate.isNotEmpty(image3)){
					product.setString("image3", "//img14.360buyimg.com/n0/" + image3);
				}
			}
			if(imageList.size() > 3){
				def image4 = imageList[3].attr("data-url");
				if(UtilValidate.isNotEmpty(image4)){
					product.setString("image4", "//img14.360buyimg.com/n0/" + image4);
				}
			}
			if(imageList.size() > 4){
				def image5 = imageList[4].attr("data-url");
				if(UtilValidate.isNotEmpty(image5)){
					product.setString("image5", "//img14.360buyimg.com/n0/" + image5);
				}
			}
			if(imageList.size() > 5){
				def image6 = imageList[5].attr("data-url");
				if(UtilValidate.isNotEmpty(image6)){
					product.setString("image6", "//img14.360buyimg.com/n0/" + image6);
				}
			}
		}
		
		def contentDetail = doc.select("#J-detail-content");
		def cssText = contentDetail.select("style").html();
		cssText = cssText.replaceAll("[\\w\\-]+\\:\\;", "");
		InputSource source = new InputSource(new StringReader(cssText));
		CSSOMParser parser = new CSSOMParser(new SACParserCSS3());
		parser.setErrorHandler(new ErrorHandler() {
					
					@Override
					public void warning(CSSParseException e) throws CSSException {
						logInfo("URI: " + e.getURI());
						logInfo("Line: " + e.getLineNumber());
						logInfo("Column: " + e.getColumnNumber());
						logInfo("Code: " + e.getCode());
						logInfo("Message: " + e.getMessage());
						e.printStackTrace();
					}
					
					@Override
					public void fatalError(CSSParseException e) throws CSSException {
						logInfo("URI: " + e.getURI());
						logInfo("Line: " + e.getLineNumber());
						logInfo("Column: " + e.getColumnNumber());
						logInfo("Code: " + e.getCode());
						logInfo("Message: " + e.getMessage());
						e.printStackTrace();
					}
					
					@Override
					public void error(CSSParseException e) throws CSSException {
						logInfo("URI: " + e.getURI());
						logInfo("Line: " + e.getLineNumber());
						logInfo("Column: " + e.getColumnNumber());
						logInfo("Code: " + e.getCode());
						logInfo("Message: " + e.getMessage());
						e.printStackTrace();
					}
				});
		CSSStyleSheet sheet = parser.parseStyleSheet(source, null, null);
		
		Elements contentList = contentDetail.select(".ssd-module-wrap .ssd-module:not(.ssd-module-heading)");
		for (Element element : contentList) {
			def clazz = element.attr("class");
			if(UtilValidate.isEmpty(clazz) || clazz.length() < 12){
				continue;
			}
			def position = clazz.indexOf(" animate-");
			if(position < 0){
				position = clazz.length();
			}
			def clazz2 = clazz.substring(11, position);
			String propertyValue = getCssPropertyValue(sheet, ".ssd-module-wrap ." + clazz2, "background-image");
			if(UtilValidate.isNotEmpty(propertyValue)){
				def imageUrl = propertyValue;
				if(propertyValue.startsWith("url(//")){
					imageUrl = propertyValue.substring(4, propertyValue.length() - 1);
				}
				if(propertyValue.startsWith("url(https://")){
					imageUrl = propertyValue.substring(10, propertyValue.length() - 1);
				}
				long imageCount = EntityQuery.use(delegator).from("ImportProductImage").where([
					"productId" : product.productId,
					"image" : imageUrl
				]).queryCount();
				if(imageCount > 0){
					continue;
				}
				def productImage = delegator.makeValue("ImportProductImage");
				productImage.setString("productId", product.productId);
				delegator.setNextSubSeqId(productImage, "imageSeqId", 5, 1);
				productImage.setString("image", imageUrl);
				productImage.create();
			}else{
				logInfo("no exist class: " + clazz2 + ", background-image, url: " + currentUrl);
			}
		}
		
		Elements imageList2 = contentDetail.select("img");
		for (Element element : imageList2) {
			def imageUrl = element.attr("src");
			if(UtilValidate.isEmpty(imageUrl) || imageUrl.contains("blank.gif")){
				imageUrl = element.attr("data-lazyload");
			}
			if(UtilValidate.isEmpty(imageUrl)){
				continue;
			}
			if(imageUrl.startsWith("data:image/")){
				continue;
			}
			long imageCount = EntityQuery.use(delegator).from("ImportProductImage").where([
				"productId" : product.productId,
				"image" : imageUrl
			]).queryCount();
			if(imageCount > 0){
				continue;
			}
			def productImage = delegator.makeValue("ImportProductImage");
			productImage.setString("productId", product.productId);
			delegator.setNextSubSeqId(productImage, "imageSeqId", 5, 1);
			productImage.setString("image", imageUrl);
			productImage.create();
		}
		
		product.setString("isDetail", "Y");
		product.store();
		
		long contentCount = EntityQuery.use(delegator).from("ImportProductContent").where([
			"productId" : product.productId,
		]).queryCount();
		if(contentCount == 0){
			def productContent = delegator.makeValue("ImportProductContent");
			productContent.setString("productId", product.productId);
			productContent.setString("content", contentDetail.html());
			productContent.create();
		}
		
		def temp = delegator.makeValue("ImportTemp");
		temp.setString("tempId", delegator.getNextSeqId("ImportTemp"));
		temp.setString("url", currentUrl);
		temp.create();
	}
	crawlJDDetailInner(httpclient);
}

private String getCssPropertyValue(CSSStyleSheet sheet, String selectorText, String propertyName) {
	CSSRuleList rules = sheet.getCssRules();
	for (int i = 0; i < rules.getLength(); i++) {
		if(rules.item(i) instanceof CSSStyleRule){
			CSSStyleRule rule = (CSSStyleRule) rules.item(i);
			if (rule.getSelectorText().equals(selectorText)) {
				CSSStyleDeclaration ss = rule.getStyle();
				return ss.getPropertyValue(propertyName);
			}
		}
	}
	return null;
}

public Map<String, Object> downloadProductImage(){
	CloseableHttpClient httpclient = HttpClients.createDefault();
	downloadProductImageInner(httpclient);
	httpclient.close();
	return ServiceUtil.returnSuccess();
}

private void downloadProductImageInner(CloseableHttpClient httpclient){
	def params = ["isDownload" : "N"];
	if(context.categoryId){
		params.categoryId = context.categoryId;
	}
	def list = EntityQuery.use(delegator).from("ImportProduct").where(params).maxRows(30).queryList();
	if(UtilValidate.isEmpty(list)){
		logInfo("执行完成downloadProductImageInner.");
		return;
	}
	list.each{ product ->
		def location = downloadImage(httpclient, product.productId, product.image);
		product.set("location", location, false);
		
		def location1 = downloadImage(httpclient, product.productId, product.image1);
		product.set("location1", location1, false);
		
		def location2 = downloadImage(httpclient, product.productId, product.image2);
		product.set("location2", location2, false);
		
		def location3 = downloadImage(httpclient, product.productId, product.image3);
		product.set("location3", location3, false);
		
		def location4 = downloadImage(httpclient, product.productId, product.image4);
		product.set("location4", location4, false);
		
		def location5 = downloadImage(httpclient, product.productId, product.image5);
		product.set("location5", location5, false);
		
		def location6 = downloadImage(httpclient, product.productId, product.image6);
		product.set("location6", location6, false);
		
		product.setString("isDownload", "Y");
		product.store();
	}
	downloadProductImageInner(httpclient);
}

private String downloadImage(CloseableHttpClient httpclient, String productId, String imageUrl){
	if(UtilValidate.isEmpty(imageUrl)){
		return null;
	}
	if(imageUrl.startsWith("//")){
		imageUrl = "http:" + imageUrl;
	}
	if(!imageUrl.startsWith("http") ){
		logInfo("No Download: " + imageUrl);
		return null;
	}
	logInfo("Download URL: " + imageUrl);
	HttpGet httpGet = new HttpGet(imageUrl);
	httpGet.addHeader("user-agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36");
	CloseableHttpResponse response = httpclient.execute(httpGet);
	HttpEntity entity = response.getEntity();
	byte[] bytes = EntityUtils.toByteArray(entity);
	response.close();
	String fileSuffix = imageUrl.substring(imageUrl.lastIndexOf("."));
	String location =  productId + "/" + RandomStringUtils.randomAlphanumeric(20).toUpperCase() + fileSuffix;
	String filePath = "download/" + location;
	FileUtils.writeByteArrayToFile(new File(filePath), bytes);
	return location;
}

public Map<String, Object> downloadProductImage2(){
	CloseableHttpClient httpclient = HttpClients.createDefault();
	downloadProductImageInner2(httpclient);
	httpclient.close();
	return ServiceUtil.returnSuccess();
}

private void downloadProductImageInner2(CloseableHttpClient httpclient){
	def params = ["isDownload" : "N"];
	if(context.categoryId){
		params.categoryId = context.categoryId;
	}
	def list = EntityQuery.use(delegator).from("ImportProductImage").where(params).maxRows(30).queryList();
	if(UtilValidate.isEmpty(list)){
		logInfo("执行完成downloadProductImageInner2.");
		return;
	}
	list.each{ productImage ->
		def location = downloadImage(httpclient, productImage.productId, productImage.image);
		productImage.set("location", location, false);
		productImage.setString("isDownload", "Y");
		productImage.store();
	}
	downloadProductImageInner2(httpclient);
}

public Map<String, Object> runJDService(){
	def list = EntityQuery.use(delegator).from("ImportCategory").where("level", 3).queryList();
	list.each{ category ->
		dispatcher.runAsync(context.serviceName, ["categoryId": category.categoryId, "userLogin": userLogin]);
	}
	return ServiceUtil.returnSuccess();
}

public Map<String, Object>  deletedProductSamllImage(){
	deletedProductSamllImageInner();
	return ServiceUtil.returnSuccess();
}

private void deletedProductSamllImageInner(){
	def list = EntityQuery.use(delegator).from("ImportProduct").where(EntityCondition.makeCondition("image", EntityOperator.NOT_EQUAL, "S")).maxRows(30).queryList();
	if(UtilValidate.isEmpty(list)){
		logInfo("执行完成deletedProductSamllImageInner.");
		return;
	}
	list.each{ product ->
		File file = new File("D:/download/" + product.location);
		if(file.exists()){
			if (file.delete()){
				logInfo("Delete File: " + file.getAbsolutePath());
				product.set("location", "D");
			}else{
				logInfo("Delete Error: " + file.getAbsolutePath());
			}
		}else{
			logInfo("No Exist: " + file.getAbsolutePath());
		}
		//product.set("image", "S");
		product.store();
	}
	deletedProductSamllImageInner();
}
