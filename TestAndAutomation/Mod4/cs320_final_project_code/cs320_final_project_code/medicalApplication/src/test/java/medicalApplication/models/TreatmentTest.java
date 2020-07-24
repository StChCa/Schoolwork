package medicalApplication.models;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.Treatment;

public class TreatmentTest {
	
	Treatment unchangingTreatment;
	Treatment changingTreatment;
	
	@Before
	public void setUp() {
		unchangingTreatment = new Treatment("TestDate", "TestDiagnose", "TestDescription");
		changingTreatment = new Treatment("initialDate", "initialDiagnose", "initialDescription");
	}

	// Test Treatment::getTreatmentDate
	@Test
	public void testGetTreatmentDate() {
		String expected = "TestDate";
		String actual = unchangingTreatment.getTreatmentDate();
		
		assertEquals(expected, actual);
	}
	
	// Test Treatment::setTreatmentDate()
	@Test
	public void testSetTreatmentDate() {
		changingTreatment.setTreatmentDate("1/1/11");
		
		String expected = "1/1/11";
		String actual = changingTreatment.getTreatmentDate();
		
		assertEquals(expected,actual);
	}
	
	// Test Treatment::getDiagnosis()
	@Test
	public void testGetDiagnosis() {
		String expected = "TestDiagnose";
		String actual = unchangingTreatment.getDiagnose();
		
		assertEquals(expected,actual);
	}
	
	// Test Treatment::setDiagnose()
	@Test
	public void testSetDiagnose() {
		changingTreatment.setDiagnose("newDiagnose");
		
		String expected = "newDiagnose";
		String actual = changingTreatment.getDiagnose();
		
		assertEquals(expected,actual);
	}
	
	// Test Treatment::getDescription()
	@Test
	public void testGetDescription() {
		String expected = "TestDescription";
		String actual = unchangingTreatment.getDescription();
		
		assertEquals(expected,actual);
	}
	
	// Test Treatment::setDescription()
	@Test
	public void testSetDescription() {
		changingTreatment.setDescription("newDescription");
		
		String expected = "newDescription";
		String actual = changingTreatment.getDescription();
		
		assertEquals(expected,actual);
	}
	
	// Test Treatment::toString()
	@Test
	public void testToString() {
		String expected = "Treatment:  Date: TestDate Diagnose: TestDiagnose";
		String actual = unchangingTreatment.toString();
		
		assertEquals(expected,actual);
	}
}
