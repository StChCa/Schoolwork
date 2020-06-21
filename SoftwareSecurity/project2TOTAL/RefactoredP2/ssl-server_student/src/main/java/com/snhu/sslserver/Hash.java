package com.snhu.sslserver;

import java.security.*;

public class Hash{
	
	// Constructor
	public Hash() {
		super();
	}
	
	// Return Byte array of digested name
	public byte[] getStringDigest(String message) {
		byte[] outMessage = null;
		try {
			MessageDigest md = MessageDigest.getInstance("SHA3-512");
			
			outMessage = md.digest(message.getBytes());
			
		} catch(Exception e) {
			e.printStackTrace();
		}
		
		return outMessage;
	}

}
