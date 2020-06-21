package com.snhu.sslserver;

import java.util.concurrent.atomic.AtomicLong;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HashController {
	private static final String template = "Hello, %s!";
	private final AtomicLong counter = new AtomicLong();

	@GetMapping("/greeting")
	public Hash Hash(@RequestParam(value = "name", defaultValue = "World") String name) {
		System.out.println("greeting out");
		return new Hash(counter.incrementAndGet(), String.format(template, name));
	}
}

class Hash {

	public Hash(long incrementAndGet, String format) {
		// TODO Auto-generated constructor stub
	}
	
}