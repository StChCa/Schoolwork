package medicalApplication.models;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.Medication;

public class MedicationTest {
	
	Medication unchangingMedication;
	Medication changingMedication;
	
	@Before
	public void setUp() {
		unchangingMedication = new Medication("TestName", "TestStartDate", "TestEndDate", "TestDose");
		changingMedication = new Medication("TestName", "TestStartDate", "TestEndDate", "TestDose");
	}
	
	// Test Medication::getName()
	@Test
	public void testGetName() {
		String expected = "TestName";
		String actual = unchangingMedication.getName();
		
		assertEquals(expected, actual);
	}
	
	// Test Medication::setName()
	@Test
	public void testSetName() {
		changingMedication.setName("NewName");
		
		String expected = "NewName";
		String actual = changingMedication.getName();
		
		assertEquals(expected, actual);
	}
	
	// Test Medication::getStartDate()
	@Test
	public void testGetStartDate() {
		String expected = "TestStartDate";
		String actual = unchangingMedication.getStartDate();
		
		assertEquals(expected, actual);
	}
	
	// Test Medication::setStartDate()
	@Test
	public void testSetStartDate() {
		changingMedication.setStartDate("NewStartDate");
		
		String expected = "NewStartDate";
		String actual = changingMedication.getStartDate();
		
		assertEquals(expected, actual);
	}
	
	// Test Medication::getEndDate()
	@Test
	public void testGetEndDate() {
		String expected = "TestEndDate";
		String actual = unchangingMedication.getEndDate();
		
		assertEquals(expected, actual);
	}
	
	// Test Medication::setEndDate()
	@Test
	public void testSetEndDate() {
		changingMedication.setEndDate("NewEndDate");
		
		String expected = "NewEndDate";
		String actual = changingMedication.getEndDate();
		
		assertEquals(expected, actual);
	}
	
	// Test Medication::getDose()
	@Test
	public void testGetDose() {
		String expected = "TestDose";
		String actual = unchangingMedication.getDose();
		
		assertEquals(expected, actual);
	}
	
	// Test Medication::SetDose()
	@Test
	public void testSetDose() {
		changingMedication.setDose("NewDose");
		
		String expected = "NewDose";
		String actual = changingMedication.getDose();
		
		assertEquals(expected, actual);
	}
	
	// Test Medication::toString()
	@Test
	public void testToString() {
		String expected = "Medication:TestName Start Date: TestStartDate End Date: TestEndDate Dose: TestDose";
		String actual = unchangingMedication.toString();
		
		assertEquals(expected, actual);
	}

}
