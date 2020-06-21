package com.snhu.sslserver;

import java.util.concurrent.atomic.AtomicLong;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HashController {
	
	private final String returnFormat = "message: %s \n Name of Cipher Algorithm: SHA3-512 CheckSum Value: %s";

	// On the web request, return the hex value of the requested object
	@RequestMapping("/hash")
	public String hash(@RequestParam(value = "name", defaultValue = "World") String name) {
		
		Hash hash = new Hash();

		// Debugging code
//		System.out.println("2a5a3c33fb262e3418295da1734bc8a15f9e8b45fa4c4f56ffeee35997eb1294aa0eb1d0f3720515d57dce4384b92b4064bdf07fe0f82a0ae95e3f9cba9b97d7");
//		System.out.println(encodeHexString(hash.getStringDigest("Stephen")));
//		System.out.println(encodeHexString(hash.getStringDigest(name)));
//		
//		if("2a5a3c33fb262e3418295da1734bc8a15f9e8b45fa4c4f56ffeee35997eb1294aa0eb1d0f3720515d57dce4384b92b4064bdf07fe0f82a0ae95e3f9cba9b97d7".equals(encodeHexString(hash.getStringDigest(name)))) {
//			System.out.println("MATCH!@#!@#");
//		} else {
//			System.out.println("NO MATCH");
//		}
		return String.format(returnFormat, name, encodeHexString( hash.getStringDigest(name)) );
	}

	public String encodeHexString(byte[] byteArray) {
	    StringBuffer hexStringBuffer = new StringBuffer();
	    for (int i = 0; i < byteArray.length; i++) {
	        hexStringBuffer.append(byteToHex(byteArray[i]));
	    }
	    return hexStringBuffer.toString();
	}
	
	public String byteToHex(byte num) {
	    char[] hexDigits = new char[2];
	    hexDigits[0] = Character.forDigit((num >> 4) & 0xF, 16);
	    hexDigits[1] = Character.forDigit((num & 0xF), 16);
	    return new String(hexDigits);
	}

}