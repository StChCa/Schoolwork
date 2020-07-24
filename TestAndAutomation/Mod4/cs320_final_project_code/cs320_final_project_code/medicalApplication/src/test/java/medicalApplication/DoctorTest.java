package medicalApplication;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.Doctor;

public class DoctorTest {

	Doctor testDoctor;
	
	@Before
	public void setUp() {
		testDoctor = new Doctor("TestName", "TestID");
	}
	
	// Test Doctor::getName()
	@Test
	public void testGetName() {
		String expectedValue = "TestName";
		String actualValue = testDoctor.getName();
		
		assertEquals(expectedValue, actualValue);
	}
	
	// Test Doctor::setName
	@Test
	public void testSetName() {
		Doctor setNameDoctor = new Doctor("initial", "ID");
		setNameDoctor.setName("NewName");
		
		String expectedValue = "NewName";
		String actualValue = setNameDoctor.getName();
		
		assertEquals(expectedValue, actualValue);
	}
	
	// Test Doctor::getId()
	@Test
	public void testGetId() {
		String expectedValue = "TestID";
		String actualValue = testDoctor.getId();
		
		assertEquals(expectedValue, actualValue);
	}
	
	// Test Doctor::setId
	@Test
	public void testSetId() {
		Doctor setIdDoctor = new Doctor("name", "OriginalID");
		setIdDoctor.setId("NewID");
		
		String expected = "NewID";
		String actual = setIdDoctor.getId();
		
		assertEquals(expected, actual);
	}
	
	// Test Doctor::toString()
	@Test
	public void testToString() {
		String expected = "Doctor Name:TestName ID: TestID";
		String actual = testDoctor.toString();
		
		assertEquals(expected, actual);
	}
}
