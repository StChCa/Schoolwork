package medicalApplication.models;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.Patient;

public class PatientTest {
	
	Patient unchangingPatient;
	Patient changingPatient;
	
	@Before
	public void setUp() {
		unchangingPatient = new Patient("TestName", "TestID");
		changingPatient = new Patient("TestName", "TestID");
	}

	// Test Patient::getName
	@Test
	public void testGetName() {
		String expected = "TestName";
		String actual = unchangingPatient.getName();
		
		assertEquals(actual, expected);
	}
	
	// Test Patient::setName
	@Test
	public void testSetName() {
		changingPatient.setName("NewName");
		
		String expected = "NewName";
		String actual = changingPatient.getName();
		
		assertEquals(actual, expected);
	}

	// Test Patient::getId
	@Test
	public void testGetId() {
		String expected = "TestID";
		String actual = unchangingPatient.getId();
		
		assertEquals(actual, expected);
	}
	
	// Test Patient::setId
	@Test
	public void testSetId() {
		changingPatient.setId("NewID");
		
		String expected = "NewID";
		String actual = changingPatient.getId();
		
		assertEquals(actual, expected);
	}
	
	// Test Patieent::toString
	@Test
	public void testToString() {
		String expected = "Patient Name: TestName ID: TestID";
		String actual = unchangingPatient.toString();
		
		assertEquals(expected,actual);
	}

}
