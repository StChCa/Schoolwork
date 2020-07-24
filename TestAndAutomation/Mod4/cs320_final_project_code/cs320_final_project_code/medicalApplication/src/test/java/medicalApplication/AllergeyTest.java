package medicalApplication;

import static org.junit.Assert.*;
import medical.com.medicalApplication.*;
import medical.com.medicalApplication.model.Allergey;

import org.junit.Before;
import org.junit.Test;

public class AllergeyTest {
	
	Allergey testAllergey;
	
	// Set up[ the Allergey objects for testing.
	@Before
	public void setUp() {
		testAllergey = new Allergey("NameString");
	}

	// Test Allergey::getName() returns the correct value
	@Test
	public void testGetName() {
		String expectedValue = "NameString";
		String actualValue = testAllergey.getName();
		assertEquals(expectedValue, actualValue);
	}
	
	// Test Allgergey::setName()
	@Test
	public void testSetName() {
		Allergey setNameAllergey = new Allergey("original");
		setNameAllergey.setName("NewNameValue");
		
		String expectedValue = "NewNameValue";
		String actualValue = setNameAllergey.getName();
		
		assertEquals(expectedValue, actualValue);
	}
	
	// Test Allergey::toString() function
	@Test
	public void testToString() {
		String expectedValue = "Allergy NameString";
		String actualValue = testAllergey.toString();
		
		assertEquals(expectedValue, actualValue);		
	}

}
