package schoolApplication.model;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import schoolApplication.app.model.Class;

public class TestClass {
	private Class clazz;

	@Before
	public void before() {
		this.clazz = new Class("IT 320", "1234");
		this.clazz.setGrade("A");
	}

	@Test
	public void testSetName() {
		assertTrue(clazz.getName().equals("IT 320"));
	}

	@Test
	public void testSetSection() {
		assertTrue(clazz.getSection().equals("1234"));
	}
	
	@Test
	public void testSetGrade() {
		assertTrue(clazz.getGrade().equals("A"));
	}
}
