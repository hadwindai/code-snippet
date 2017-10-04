
public Map<String,Object> getCodisInfo(){
	Map<String, Object> result = ServiceUtil.returnSuccess();
	def key = (String) context.get("key");
	def value = [:];
	RedissonClient syncClient = CodisClient.getSyncInstance();
	RBinaryStream stream = syncClient.getBinaryStream(key);
	String password = UtilProperties.getPropertyValue("codis", "codis.encrypt.password");
	try {
		if(UtilValidate.isNotEmpty(stream)){
			def bytes = stream.get();
			if(bytes != null){
				Key decryptKey = DesCrypt.getDesKey(password.getBytes("UTF-8"));
				byte[] decrypts = DesCrypt.decrypt(decryptKey, bytes);
				String content = new String(decrypts, "UTF-8");
				if(UtilValidate.isNotEmpty(content)){
					value = JSON.from(content).toObject(Map.class);
				}
			}
		}
	} catch (Exception e) {
		Debug.logError(e, module);
		return ServiceUtil.returnError(e.getMessage());
	}
	result.put("value", value);
	return result;
}

public Map<String,Object> setCodisInfo(){
	Map<String, Object> result = ServiceUtil.returnSuccess();
	def key = (String) context.get("key");
	def value = (Map) context.get("value");
	if(UtilValidate.isNotEmpty(value)){
		Iterator iterator = value.keySet().iterator();
		while(iterator.hasNext()){
			String k = iterator.next();
			if(UtilValidate.isEmpty(value[k])){
				iterator.remove();
			}
		}
	}
	RedissonClient syncClient = CodisClient.getSyncInstance();
	RBinaryStream stream = syncClient.getBinaryStream(key);
	String password = UtilProperties.getPropertyValue("codis", "codis.encrypt.password");
	try {
		if(UtilValidate.isNotEmpty(stream)){
			def content = JSON.from(value?:"").toString();
			Key encryptKey = DesCrypt.getDesKey(password.getBytes("UTF-8"));
			byte[] encryptContent = DesCrypt.encrypt(encryptKey, content.getBytes("UTF-8"));
			stream.set(encryptContent);
		}
	} catch (Exception e) {
		Debug.logError(e, module);
		return ServiceUtil.returnError(e.getMessage());
	}
	return result;
}

public Map<String,Object> deleteCodisInfo(){
	Map<String, Object> result = ServiceUtil.returnSuccess();
	def key = (String) context.get("key");
	RedissonClient syncClient = CodisClient.getSyncInstance();
	RBinaryStream stream = syncClient.getBinaryStream(key);
	try {
		if(UtilValidate.isNotEmpty(stream)){
			stream.delete();
		}
	} catch (Exception e) {
		Debug.logError(e, module);
		return ServiceUtil.returnError(e.getMessage());
	}
	return result;
}
