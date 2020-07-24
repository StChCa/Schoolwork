package medicalApplication.models;

import static org.junit.Assert.*;

import java.util.List;

import org.junit.Before;
import org.junit.Test;

import medical.com.medicalApplication.model.Allergey;
import medical.com.medicalApplication.model.MedicalRecord;
import medical.com.medicalApplication.model.Patient;
import medical.com.medicalApplication.model.PatientHistory;

public class MedicalRecordTest {

	Patient testPatient;
	MedicalRecord testMedicalRecord;
	
	@Before
	public void setUp() {
		testPatient = new Patient("PatientName", "PatientID");
		testMedicalRecord = new MedicalRecord(testPatient);
	}
	
	// Test MedicalRecord::getPatient()
	@Test
	public void testGetPatient() {
		// We expect to receive the class level patient objecet back
		Patient actual = testMedicalRecord.getPatient();
		
		assertEquals(testPatient, actual);
	}
	
	// Test MedicalRecord::getHistory()
	@Test
	public void testGetHistory() {
		// Verify that the getHistory function is returning an instance of PatientHistory.
		PatientHistory actual = testMedicalRecord.getHistory();
		
		assertTrue(actual instanceof PatientHistory);
	}
	
	// Test the ability to add many allergies to a patient.
	@Test
	public void testManyAllergies() {
		// Add a patient to the test service singleton.
		Patient patientA = new Patient("patientA", "patientAID");
		MedicalRecord mr = new MedicalRecord(patientA);
		
		Allergey a = new Allergey("A");
		Allergey b = new Allergey("B");
		Allergey c = new Allergey("C");
		
		// Attempt to add many allergies to patientA
		mr.getHistory().addAllergy(a);
		mr.getHistory().addAllergy(b);
		mr.getHistory().addAllergy(c);
		
		List<Allergey> allergies = mr.getHistory().getAlergies();
		
		Allergey[] expectedAllergies = {a, b, c};
		
		for( Allergey x : expectedAllergies ) {
			if( allergies.contains(x) ) {
				continue;
			} else {
				fail("Issue with attributing multiple allergies to one patient.");
			}
		}
	}

}
